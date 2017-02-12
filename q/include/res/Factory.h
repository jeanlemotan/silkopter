#pragma once

#include "Loader.h"
#include "Streamer.h"
#include "video/Texture.h"

namespace q
{
namespace res
{

class Factory : util::Noncopyable
{
public:
    Factory(data::File_System_ptr const& file_system);
    ~Factory();

    //This registers a loader capable of loading resources of type T.
    template<class T>	void register_loader(std::unique_ptr<Loader<T>> loader);

    //Returns a (non-owning) pointer to a loader capable of loading resources of type T from the source
    //	or null if nonw was found (either none registered or the source is not recognized by none)
    template<class T>	auto find_loader_for_source(data::Source& source) const -> Loader<T>*;

    //Sets the streamer that will handle resources of type T
    //If none is specified for a certain T, that resource type cannot be loaded
    template<class T> 	void set_streamer(std::unique_ptr<Streamer<T>> streamer);

    //Loads a resource of type T from the path
    template<class T>	auto load(Path const& path) const -> std::shared_ptr<T>;

    //Searches for a source names 'name' in the internal filesystem and loads it using load<T>(path)
    template<class T>	auto load_by_name(std::string const& name) const -> std::shared_ptr<T>;

    //reloads all resources of a certain type
    template<class T>	void reload_all() const;

    //returns the internal filesystem
    auto get_file_system() const -> data::File_System&;

private:
    //Technique from this video: https://www.youtube.com/watch?v=_BpMYeUFXv8
    //It allows internal polymorphism of templated classes (Loader and Streamer in this case)

    struct Loader_Wrapper_Base { };
    template<class T> struct Loader_Wrapper : public Loader_Wrapper_Base
    {
        Loader_Wrapper(std::unique_ptr<Loader<T>> loader) : loader_ptr(std::move(loader)) {}
        std::shared_ptr<Loader<T>> loader_ptr;
    };

    typedef std::map<int, std::vector<std::unique_ptr<Loader_Wrapper_Base>>> Class_Id_To_Loader;

    struct Streamer_Wrapper_Base { };
    template<class T> struct Streamer_Wrapper : public Streamer_Wrapper_Base
    {
        Streamer_Wrapper(std::unique_ptr<Streamer<T>> streamer) : streamer_ptr(std::move(streamer)) {}
        std::shared_ptr<Streamer<T>> streamer_ptr;
    };

    typedef std::map<size_t, std::unique_ptr<Streamer_Wrapper_Base>> Class_Id_To_Streamer;

    data::File_System_ptr	m_file_system;

    Class_Id_To_Loader		m_registered_loaders;
    Class_Id_To_Streamer	m_registered_streamers;

    mutable std::mutex	m_mutex;
};

inline auto Factory::get_file_system() const -> data::File_System&
{
    QASSERT(m_file_system); //it will crash after this line.
    return *m_file_system;
}

template<class T> void Factory::register_loader(std::unique_ptr<Loader<T>> loader)
{
    QASSERT(loader);
    if (!loader)
    {
        return;
    }
    std::lock_guard<std::mutex> sm(m_mutex);

    size_t class_id = typeid(T).hash_code();
    auto& loaders = m_registered_loaders[class_id];

    loaders.emplace_back(new Loader_Wrapper<T>(std::move(loader)));
}

template<class T> auto Factory::find_loader_for_source(data::Source& source) const -> Loader<T>*
{
    std::lock_guard<std::mutex>sm(m_mutex);

    size_t class_id = typeid(T).hash_code();
    auto cit = m_registered_loaders.find(class_id);
    if (cit == m_registered_loaders.end())
    {
        return nullptr;
    }

    auto& loaders = cit->second;
    for (auto it = loaders.begin(); it != loaders.end(); ++it)
    {
        std::unique_ptr<Loader_Wrapper_Base> const& c = *it;
        auto m = static_cast<Loader_Wrapper<T> const*>(c.get());
        auto const& loader = m->loader_ptr;
        if (loader->can_load_from_source(source))
        {
            return loader.get();
        }
    }
    return nullptr;
}

template<class T> void Factory::set_streamer(std::unique_ptr<Streamer<T>> streamer)
{
    QASSERT(streamer);
    if (!streamer)
    {
        return;
    }
    std::lock_guard<std::mutex> sm(m_mutex);

    size_t class_id = typeid(T).hash_code();
    m_registered_streamers[class_id].reset(new Streamer_Wrapper<T>(std::move(streamer)));
}

template <class T> auto Factory::load(Path const& path) const -> std::shared_ptr<T>
{
    std::shared_ptr<Streamer<T>> streamer;

    {
        std::lock_guard<std::mutex> sm(m_mutex);

        size_t class_id = typeid(T).hash_code();
        auto it = m_registered_streamers.find(class_id);
        if (it == m_registered_streamers.end())
        {
            return std::shared_ptr<T>();
        }

        streamer = static_cast<Streamer_Wrapper<T> const*>(it->second.get())->streamer_ptr;
    }
    QASSERT(streamer);

    auto res = streamer->load(*this, path);
    std::shared_ptr<T> tres = std::dynamic_pointer_cast<T>(res);
    QASSERT(!res || tres);
    return tres;
}
template <class T> auto Factory::load_by_name(std::string const& name) const -> std::shared_ptr<T>
{
    std::shared_ptr<Streamer<T>> streamer;

    //only need to lock to access members
    {
        std::lock_guard<std::mutex> sm(m_mutex);

        size_t class_id = typeid(T).hash_code();
        auto it = m_registered_streamers.find(class_id);
        if (it == m_registered_streamers.end())
        {
            return std::shared_ptr<T>();
        }

        streamer = static_cast<Streamer_Wrapper<T> const*>(it->second.get())->streamer_ptr;
    }
    QASSERT(streamer);

    auto path = m_file_system->find_path_by_name(name);
    if (path.is_empty())
    {
        return std::shared_ptr<T>();
    }

    auto res = streamer->load(*this, path);
    std::shared_ptr<T> tres = std::dynamic_pointer_cast<T>(res);
    QASSERT(tres);
    return tres;
}
template <class T> void Factory::reload_all() const
{
    std::shared_ptr<Streamer<T>> streamer;

    {
        std::lock_guard<std::mutex> sm(m_mutex);

        size_t class_id = typeid(T).hash_code();
        auto it = m_registered_streamers.find(class_id);
        if (it != m_registered_streamers.end())
        {
            streamer = static_cast<Streamer_Wrapper<T> const*>(it->second.get())->streamer_ptr;
        }
    }

    if (streamer)
    {
        streamer->reload_all(*this);
    }
}

}
}
