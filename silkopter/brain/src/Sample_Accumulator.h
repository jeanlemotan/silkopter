#pragma once

#include "MPL_Helper.h"

namespace silk
{

namespace detail
{

constexpr bool Greater_Than_Zero(size_t N)
{
    return N > 0;
}

template<class... Streams>
class Storage
{
public:
    template<size_t N>
    ts::Result<void> set_stream_path(size_t, q::Path const&, uint32_t, HAL&)
    {
        return make_error("Invalid stream index");
    }
    template<size_t N>
    auto get_stream_path(size_t) const -> q::Path const&
    {
        return q::Path::null;
    }

    void clear_streams()
    {
    }

    auto lock() -> bool
    {
        return true;
    }
    void unlock()
    {
    }
    auto collect() -> size_t
    {
        return 0;
    }
    auto consume(size_t /*count*/) -> size_t
    {
        return 0;
    }
    auto get_sample_count() -> size_t
    {
        return std::numeric_limits<size_t>::max();
    }
    auto get_params(size_t) -> decltype(std::make_tuple())
    {
        return std::make_tuple();
    }
};

template<class Stream, class... Streams>
class Storage<Stream, Streams...> : public Storage<Streams...>
{
public:
    typedef Storage<Streams...> Parent_t;
    typedef typename Stream::Sample Sample_t;
    typedef std::tuple<Sample_t const&, typename Streams::Sample const&...> Params_t;

    void clear_streams()
    {
        QASSERT(!m_locked_stream);
        m_stream.reset();
        Parent_t::clear_streams();
    }

    template<size_t N, class T>
    typename std::enable_if<N != 0>::type set_stream(std::shared_ptr<T> stream)
    {
        Parent_t::template set_stream<N - 1, T>(stream);
    }

    template<size_t N, class T>
    typename std::enable_if<N == 0>::type set_stream(std::shared_ptr<T> stream)
    {
        QASSERT(!m_locked_stream);
        m_stream = stream;
    }

    template<size_t N, class T>
    typename std::enable_if<N != 0, std::shared_ptr<T>>::type get_stream()
    {
        return Parent_t::template get_stream<N - 1, T>();
    }

    template<size_t N, class T>
    typename std::enable_if<N == 0, std::shared_ptr<T>>::type get_stream()
    {
        return m_stream.lock();
    }

    template<size_t N>
    ts::Result<void> set_stream_path(size_t idx, q::Path const& path, uint32_t desired_rate, HAL& hal)
    {
        if (idx == N)
        {
            m_stream_path.clear();
            m_stream.reset();

            if (!path.empty())
            {
                auto stream = hal.get_stream_registry().find_by_name<Stream>(path.get_as<std::string>());
                if (stream)
                {
                    if (desired_rate > 0 && stream->get_rate() != desired_rate)
                    {
                        return make_error("Bad input stream '{}'. Expected rate {}Hz, got {}Hz", path, desired_rate, stream->get_rate());
                    }
                    m_stream_path = path;
                    m_stream = stream;
                }
                else
                {
                    return make_error("Cannot find stream '{}'", path);
                }
            }
            return ts::success;
        }
        else
        {
            return Parent_t::template set_stream_path<N + 1>(idx, path, desired_rate, hal);
        }
    }
    template<size_t N>
    auto get_stream_path(size_t idx) const -> q::Path const&
    {
        if (idx == N)
        {
            return m_stream_path;
        }
        else
        {
            return Parent_t::template get_stream_path<N + 1>(idx);
        }
    }

    auto lock() -> bool
    {
        m_locked_stream = m_stream.lock();
        return m_locked_stream && Parent_t::lock();
    }
    void unlock()
    {
        m_locked_stream.reset();
        Parent_t::unlock();
    }
    auto collect() -> size_t
    {
        QASSERT(m_locked_stream);
        auto const& samples = m_locked_stream->get_samples();
        m_samples.reserve(m_samples.size() + samples.size());
        std::copy(samples.begin(), samples.end(), std::back_inserter(m_samples));
        Parent_t::collect();
        return m_samples.size();
    }
    auto consume(size_t count) -> size_t
    {
        QASSERT(count <= m_samples.size());
        m_samples.erase(m_samples.begin(), m_samples.begin() + count);

        auto parent_count = Parent_t::consume(count);
        if (m_samples.size() > 30)
        {
            //crop to parent count
            size_t crop = math::min(parent_count, m_samples.size());
            auto begin = m_samples.begin();
            auto end = m_samples.end() - crop;
            QLOGW("Stream is out of sync: {} samples pending. Cropping {} samples", m_samples.size(), std::distance(begin, end));
            m_samples.erase(begin, end);
        }

        return m_samples.size();
    }
    auto get_sample_count() -> size_t
    {
        return math::min(m_samples.size(), Parent_t::get_sample_count());
    }

    auto get_params(size_t idx) -> Params_t
    {
        return std::tuple_cat(std::tuple<Sample_t const&>(m_samples[idx]), Parent_t::get_params(idx));
    }

private:
    std::shared_ptr<Stream> m_locked_stream;
    std::weak_ptr<Stream> m_stream;
    q::Path m_stream_path;
    std::vector<Sample_t> m_samples;

};

}



template<class ... Streams>
class Sample_Accumulator
{
    typedef detail::Storage<Streams...> Storage_t;
    Storage_t m_storage;
public:

    void clear_streams()
    {
        m_storage.clear_streams();
    }

    ts::Result<void> set_stream_path(size_t idx, q::Path const& path, uint32_t desired_rate, HAL& hal)
    {
        return m_storage.template set_stream_path<0>(idx, path, desired_rate, hal);
    }
    auto get_stream_path(size_t idx) const -> q::Path const&
    {
        return m_storage.template get_stream_path<0>(idx);
    }

    template<size_t N, class T>
    void set_stream(std::shared_ptr<T> stream)
    {
        m_storage.set_stream<N>(stream);
    }

    template<size_t N, class T>
    auto get_stream() -> std::shared_ptr<T>
    {
        return m_storage.get_stream<N>();
    }

    auto lock() -> bool
    {
        return m_storage.lock();
    }
    void unlock()
    {
        m_storage.unlock();
    }
    auto process(std::function<void(typename Streams::Sample const&...)> const& func) -> bool
    {
        if (!m_storage.lock())
        {
            m_storage.unlock();
            return false;
        }
        m_storage.collect();
        auto count = m_storage.get_sample_count();

        for (size_t i = 0; i < count; i++)
        {
            auto params = m_storage.get_params(i);
            detail::call(func, params);
        }

        m_storage.consume(count);
        m_storage.unlock();
        return true;
    }
};


}
