#pragma once



namespace q
{
namespace anim
{
	class Pose
	{
	public:

		struct Data
		{
			math::vec4f data;
			//float f[4];
			//add more if needed
		};

	public:
		Pose();
		Pose(Pose&& other);

        Pose(Pose const&) = default;
        Pose& operator=(Pose const&) = default;

		bool operator==(Pose const& other) const;
		bool operator!=(Pose const& other) const;

		//Returns the number of channels
		size_t	get_count() const;
		bool	is_empty() const;

		//Rethrns the path for the given channel
		Path const& get_path(size_t idx) const;

		//Returns the data in various formats
		int32_t		get_as_int32(size_t idx) const;
		uint32_t	get_as_uint32(size_t idx) const;
		float		get_as_float(size_t idx) const;
		math::vec2f const& get_as_vec2(size_t idx) const;
		math::vec3f const& get_as_vec3(size_t idx) const;
		math::vec4f const& get_as_vec4(size_t idx) const;
		math::quatf const& get_as_quat(size_t idx) const;

		//The cache is used by the node only.
		typedef uint32_t Cache;
		Cache		get_cache(size_t idx) const;
		void		set_cache(size_t idx, Cache cache);

		//The ID is to avoid searches by path in the node
		//USed by the node
		typedef uint32_t Id;
		Id			get_id(size_t idx) const;
		void		set_id(size_t idx, Id id);

		//Adds a channel. Returns the idx.
		//If the path already exists, it asserts and returns the path of the existing channel
		size_t	add_channel(Path const& path);

		//Removes a channel from the list. Does not changes IDs or anything
		void	remove_channel(size_t idx);

		//Returns the index of the channel with the given path, or -1 if not found
		int		find_idx_by_path(Path const& path) const;

		//Clears all the channels fromthe pose
		void	clear();

		//Raw getter/setter for the data
		//Node the data is MAX 16 bytes. More than this is clamped
		void	set_data(size_t idx, Data const& data);
		Data const& get_data(size_t idx) const;

	private:
		struct Channel_Data
		{
			Channel_Data() : id(0), cache(0) {}
			Channel_Data(Path const& path) : path(path), id(0), cache(0) {}
			bool operator==(Channel_Data const& other) const
			{
				return path == other.path && id == other.id && memcmp(&data.data, &other.data.data, sizeof(Data)) == 0;
			}
			Path	path;
			uint32_t	id;
			Data	data;
			mutable uint32_t cache;
		};
		std::vector<Channel_Data> m_channels;
	};

	inline Pose::Pose()
	{

	}
	inline Pose::Pose(Pose&& other)
		: m_channels(std::move(other.m_channels))
	{
	}

	inline bool Pose::operator==(Pose const& other) const
	{
		return m_channels == other.m_channels;
	}
	inline bool Pose::operator!=(Pose const& other) const
	{
		return !operator==(other);
	}

	inline size_t Pose::get_count() const
	{
		return m_channels.size();
	}
	inline bool Pose::is_empty() const
	{
		return m_channels.empty();
	}
	inline Path const& Pose::get_path(size_t idx) const
	{
		return m_channels[idx].path;
	}

	inline int Pose::get_as_int32(size_t idx) const
	{
		return *(int*)&get_data(idx).data.x;
	}
	inline uint32_t Pose::get_as_uint32(size_t idx) const
	{
		return *(uint32_t*)&get_data(idx).data.x;
	}
	inline float Pose::get_as_float(size_t idx) const
	{
		return *(float*)&get_data(idx).data.x;
	}
	inline math::vec2f const& Pose::get_as_vec2(size_t idx) const
	{
		return *(math::vec2f*)&get_data(idx).data;
	}
	inline math::vec3f const& Pose::get_as_vec3(size_t idx) const
	{
		return *(math::vec3f*)&get_data(idx).data;
	}
	inline math::vec4f const& Pose::get_as_vec4(size_t idx) const
	{
		return *(math::vec4f*)&get_data(idx).data;
	}
	inline math::quatf const& Pose::get_as_quat(size_t idx) const
	{
		return *(math::quatf*)&get_data(idx).data;
	}

	inline auto Pose::get_cache(size_t idx) const -> Cache
	{
		return m_channels[idx].cache;
	}
	inline void Pose::set_cache(size_t idx, Cache cache)
	{
		m_channels[idx].cache = cache;
	}

	inline auto Pose::get_id(size_t idx) const -> Id
	{
		return m_channels[idx].id;
	}
	inline void Pose::set_id(size_t idx, Id id)
	{
		m_channels[idx].id = id;
	}

	inline size_t Pose::add_channel(Path const& path)
	{
		int idx = find_idx_by_path(path);
		if (idx >= 0)
		{
			QLOG_ERR("Q", "Channel '{}' already exists. Ignoring.", path);
			return idx;
		}

		m_channels.emplace_back(path);
		return m_channels.size() - 1;
	}
	inline void Pose::remove_channel(size_t idx)
	{
		m_channels.erase(m_channels.begin() + idx);
	}

	inline void Pose::set_data(size_t idx, Data const& data)
	{
		m_channels[idx].data = data;
	}
	inline Pose::Data const& Pose::get_data(size_t idx) const
	{
		return m_channels[idx].data;
	}

	inline int Pose::find_idx_by_path(Path const& path) const
	{
		auto it = std::find_if(m_channels.begin(), m_channels.end(), [&] (Channel_Data const& ch) { return ch.path == path; });
		if (it != m_channels.end())
		{
			return std::distance(m_channels.begin(), it);;
		}
		return -1;
	}
	inline void Pose::clear()
	{
		m_channels.clear();
	}
}
}
