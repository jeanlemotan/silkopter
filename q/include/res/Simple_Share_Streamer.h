#pragma once

#include "video/Texture.h"
#include "Factory.h"

namespace q
{
namespace res
{

	template<class T>
	class Simple_Share_Streamer : public Streamer<T>
	{
	public:
		typedef std::shared_ptr<T> T_ptr;

        virtual ~Simple_Share_Streamer<T>() {}
        
		T_ptr		load(Factory const& factory, Path const& path);
		T_ptr		find_resource_by_path(Path const& path);

		void		update(Factory const& factory, Clock::duration dt);

		void		reload_all(Factory const& factory);

	protected:
		T_ptr		find_resource_by_path_locked(Path const& path);

		T_ptr		create(Path const& path);

		void		add(Path const& path, const T_ptr& r);
		void		add_locked(Path const& path, const T_ptr& r);

	private:
		typedef std::unordered_map<Path, std::weak_ptr<T>> ResourceMap;

		std::vector<Path> m_resources_being_loaded;
		bool is_being_loaded(Path const& path) const;
		bool is_being_loaded_locked(Path const& path) const;

		mutable std::mutex m_mutex;
		mutable ResourceMap	m_resource_map;
	};

	template<class T> typename Simple_Share_Streamer<T>::T_ptr Simple_Share_Streamer<T>::find_resource_by_path(Path const& path)
	{
		std::lock_guard<std::mutex>sm(m_mutex);
		return find_resource_by_path_locked(path);
	}
	template<class T> typename Simple_Share_Streamer<T>::T_ptr Simple_Share_Streamer<T>::find_resource_by_path_locked(Path const& path)
	{
		auto it = m_resource_map.find(path);
		if (it != m_resource_map.end())
		{
			auto r = it->second.lock();
			if (!r)
			{
				m_resource_map.erase(it);
			}
			return r;
		}
		return T_ptr();
	}
	template<class T> void Simple_Share_Streamer<T>::add(Path const& path, const T_ptr& r)
	{
		std::lock_guard<std::mutex>sm(m_mutex);
		QASSERT(!find_resource_by_path_locked(path));
		add_locked(path, r);
	}
	template<class T> void Simple_Share_Streamer<T>::add_locked(Path const& path, const T_ptr& r)
	{
		QASSERT(!find_resource_by_path_locked(path));
		m_resource_map[path] = r;
	}
	template<class T> std::shared_ptr<T> Simple_Share_Streamer<T>::create(Path const& path)
	{
		return std::make_shared<T>(path);
	}
	template <>
	inline video::Texture_ptr Simple_Share_Streamer<video::Texture>::create(Path const& path)
	{
		return video::Texture::create(path);
	}

	template<class T> bool Simple_Share_Streamer<T>::is_being_loaded(Path const& path) const
	{
		std::lock_guard<std::mutex>sm(m_mutex);
		return is_being_loaded_locked(path);
	}
	template<class T> bool Simple_Share_Streamer<T>::is_being_loaded_locked(Path const& path) const
	{
		return std::find(m_resources_being_loaded.begin(), m_resources_being_loaded.end(), path) != m_resources_being_loaded.end();
	}

	template<class T> typename Simple_Share_Streamer<T>::T_ptr Simple_Share_Streamer<T>::load(Factory const& factory, Path const& path)
	{
		std::unique_lock<std::mutex>sm(m_mutex);

		if (path.is_empty())
		{
			return T_ptr();
		}
		auto r = find_resource_by_path_locked(path);
		if (r)
		{
			return r;
		}

		//check if it's being loaded by another thread
		if (is_being_loaded_locked(path))
		{
			sm.unlock();
			while (is_being_loaded(path))
			{
				//wait for the other thread to load
				std::this_thread::sleep_for(std::chrono::microseconds(1));
			}
			r = find_resource_by_path_locked(path);
			QASSERT(r);
			return r;
		}

		//try to open the source
		auto source = factory.get_file_system().open(path);
		if (!source)
		{
            QLOGW("Failed to load {}, path '{}' not found", typeid(T).name(), path);
			return T_ptr();
		}

		auto* loader = factory.find_loader_for_source<T>(*source);
		if (!loader)
		{
            QLOGW("Failed to load {}, no loaders for '{}'", typeid(T).name(), path);
			return T_ptr();
		}

		r = create(path);
		m_resources_being_loaded.emplace_back(path);

		//unlock so the loader can be multithreaded
		sm.unlock();

		loader->load(path, *source, *r);
		if (!r->is_valid())
		{
            QLOGW("Failed to load {}, source '{}' is invalid", typeid(T).name(), path);
		}

		//finished loading, lock again and add
		{
			sm.lock();
			add_locked(path, r);
			m_resources_being_loaded.erase(std::remove(m_resources_being_loaded.begin(), m_resources_being_loaded.end(), path), m_resources_being_loaded.end());
		}

		return r;
	}

	template<class T> void Simple_Share_Streamer<T>::update(Factory const&, Clock::duration)
	{

	}

	template<class T> void Simple_Share_Streamer<T>::reload_all(Factory const& factory)
	{
		std::lock_guard<std::mutex> sm(m_mutex);

		auto it = m_resource_map.begin();
		for (; it != m_resource_map.end(); ++it)
		{
			auto r = it->second.lock();
			if (r)
			{
				auto const& path = r->get_path();
				//try to open the source
				auto source = factory.get_file_system().open(path);
				if (!source)
				{
					continue;
				}

				auto loader = factory.find_loader_for_source<T>(*source); //if fails try the file header
				if (!loader)
				{
					continue;
				}
				loader->load(path, *source, *r);
			}
		}
	}



}
}
