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
    auto get_params(size_t idx) -> decltype(std::make_tuple())
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
            auto crop = math::min(parent_count, m_samples.size());
            QLOGW("Stream is out of sync: {} samples pending. Cropping {} samples", m_samples.size(), crop);
            m_samples.erase(m_samples.begin(), m_samples.end() - crop);
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

    template<size_t N, class T>
    void set_stream(std::shared_ptr<T> stream)
    {
        m_storage.set_stream<N>(stream);
    }

    auto lock() -> bool
    {
        return m_storage.lock();
    }
    void unlock()
    {
        m_storage.unlock();
    }
    auto process(std::function<void(size_t idx, typename Streams::Sample const&...)> const& func) -> bool
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
            auto i_params = std::tuple_cat(std::make_tuple(i), params);
            detail::call(func, i_params);
        }

        m_storage.consume(count);
        m_storage.unlock();
        return true;
    }
};


}
