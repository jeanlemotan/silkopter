#pragma once

namespace q
{
namespace scene
{

	class Skeleton
	{
	public:
		Skeleton();
		Skeleton(Skeleton&& other);

#if !defined(_WIN32)
		Skeleton(Skeleton const& other) = default;
		Skeleton& operator=(Skeleton const& other) = default;
#endif

		bool operator==(Skeleton const& other);
		bool operator!=(Skeleton const& other);

		//////////////////////////////////////////////////////////////////////////
		// Structure

		size_t				get_node_count() const;
		String				get_node_name(size_t idx) const;
		int					find_node_idx_by_name(String const& name) const;

		int					add_node(String const& name, size_t parent_idx);
		void				remove_node(size_t idx);
		void				remove_all_nodes();

		int					get_node_parent_idx(size_t idx) const;

		//////////////////////////////////////////////////////////////////////////
		// Local transform

		math::vec3f const&	get_node_position(size_t idx) const;
		math::quatf const&	get_node_rotation(size_t idx) const;
		math::vec3f const&	get_node_scale(size_t idx) const;
		math::trans3df const&	get_node_transform(size_t idx) const;

		void				set_node_position(size_t idx, math::vec3f const& pos);
		void				set_node_rotation(size_t idx, math::quatf const& rot);
		void				set_node_scale(size_t idx, math::vec3f const& scale);
		void				set_node_transform(size_t idx, math::vec3f const& pos, math::quatf const& rot, math::vec3f const& scale);
		void				set_node_transform(size_t idx, math::vec3f const& pos, math::quatf const& rot);

		//////////////////////////////////////////////////////////////////////////
		//World transform

		math::vec3f const&	get_node_world_position(size_t idx) const;
		math::quatf const&	get_node_world_rotation(size_t idx) const;
		math::vec3f const&	get_node_world_scale(size_t idx) const;
		math::trans3df const&	get_node_world_transform(size_t idx) const;

		void				set_node_world_position(size_t idx, math::vec3f const& pos);
		void				set_node_world_rotation(size_t idx, math::quatf const& rot);

		uint32_t			get_node_transform_version(size_t idx) const;
		uint32_t			get_node_world_transform_version(size_t idx) const;
		uint32_t			get_version() const;

	protected:

		void				update_node_transform(size_t idx) const;
		void				_update_node_transform(size_t idx) const;

		void				update_node_world_transform(size_t idx) const;
		void				_update_node_world_transform(size_t idx) const;


	private:
		struct Node
		{
			Node() : scale(1), version(1), transform_version(21), world_transform_version(321), skeleton_version(4321), parent_idx(-1) 
			{}

			bool operator==(Node const& other) const
			{
				return transform.get_translation() == other.transform.get_translation() &&
					rotation == other.rotation && scale == other.scale && parent_idx == other.parent_idx;
			}

			math::quatf rotation;
			math::vec3f scale;
			uint32_t			version;

			mutable math::trans3df transform;
			mutable uint32_t			transform_version;

			mutable math::trans3df world_transform;
			mutable math::quatf world_rotation;
			mutable math::vec3f	world_scale;
			mutable uint32_t			world_transform_version;
			mutable uint32_t			skeleton_version;

			int			parent_idx;
		};

		std::vector<String> m_names;
		std::vector<Node> m_nodes;

		uint32_t		m_version;
	};


	inline Skeleton::Skeleton(Skeleton&& other)
		: m_names(std::move(other.m_names))
		, m_nodes(std::move(other.m_nodes))
		, m_version(other.m_version)
	{
	}
	inline size_t Skeleton::get_node_count() const
	{
		return m_nodes.size();
	}
	inline String Skeleton::get_node_name(size_t idx) const
	{
		return m_names[idx];
	}
	inline int Skeleton::get_node_parent_idx(size_t idx) const
	{
		return m_nodes[idx].parent_idx;
	}
	inline math::vec3f const& Skeleton::get_node_position(size_t idx) const
	{
		return m_nodes[idx].transform.get_translation();
	}
	inline math::quatf const& Skeleton::get_node_rotation(size_t idx) const
	{
		return m_nodes[idx].rotation;
	}
	inline math::vec3f const& Skeleton::get_node_scale(size_t idx) const
	{
		return m_nodes[idx].scale;
	}
	inline void Skeleton::update_node_transform(size_t idx) const
	{
		const Node& node = m_nodes[idx];
		if (node.transform_version != node.version)
		{
			_update_node_transform(idx);
		}
	}
	inline void Skeleton::update_node_world_transform(size_t idx) const
	{
		//if the Skeleton didn't change - then the transform is the same for sure
		if (m_nodes[idx].skeleton_version != m_version) 
		{
			_update_node_world_transform(idx);
		}
	}
	inline uint32_t Skeleton::get_node_transform_version(size_t idx) const
	{
		return m_nodes[idx].transform_version;
	}
	inline uint32_t Skeleton::get_node_world_transform_version(size_t idx) const
	{
		return m_nodes[idx].world_transform_version;
	}
	inline uint32_t Skeleton::get_version() const
	{
		return m_version;
	}


}
}
