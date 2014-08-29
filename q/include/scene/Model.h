#pragma once

namespace q
{
namespace scene
{
	class Model : public Scene_Node
	{
		friend class Model_Base;
		DEFINE_RTTI_CLASS(q::scene::Model, Scene_Node);
	public:
		~Model();

		auto get_base() const -> Model_Base_ptr const&;

		auto get_skeleton() const -> Skeleton const&;
		auto get_skeleton() -> Skeleton&;

		auto get_position() const -> math::vec3f const&;
		auto get_rotation() const -> math::quatf const&;
		auto get_scale() const -> math::vec3f const&;
		auto get_transform() const -> math::trans3df const&;

		void set_position(math::vec3f const& pos);
		void set_rotation(math::quatf const& rot);
		void set_scale(math::vec3f const& scale);
		void set_transform(math::vec3f const& pos, math::quatf const& rot, math::vec3f const& scale);
		void set_transform(math::vec3f const& pos, math::quatf const& rot);

		auto get_aabb() const -> math::aabb3f const&;
		auto get_local_aabb() const -> math::aabb3f const&;

		auto find_material_idx_by_name(String const& name) const -> int;
		auto get_material_count() const -> size_t;
		auto get_material_name(size_t idx) const -> String;
		auto get_material(size_t idx) const -> video::Material const&;
		void set_material(size_t idx, video::Material const& material);

		void set_animation_node(anim::Animation_Node_ptr const& animationNode);
		auto get_animation_node() -> anim::Animation_Node_ptr const&;

		void animate(Duration time);
		void cull(Cull_Result& result, Cull_Params const& params) const;
		void render(video::Renderer& renderer, Scene& scene);

	protected:
		void update_mesh_aabb(size_t idx) const;
		void update_aabb() const;
		Model(Model_Base_ptr const& base);

	private:
		void link_animation_node();
		void apply_pose();

		anim::Animation_Node_ptr		m_animation_node;
		anim::Pose			m_pose;
		std::vector<std::pair<uint32_t, uint32_t>>	m_binded_skeleton_positions;
		std::vector<std::pair<uint32_t, uint32_t>>	m_binded_skeleton_rotations;
		std::vector<std::pair<uint32_t, uint32_t>>	m_binded_skeleton_scales;

		Model_Base_ptr			m_base;
		Model_ptr				m_this_ptr;
		Skeleton				m_skeleton;

		mutable math::vec3f		m_collision_point;

		struct Mesh
		{
            size_t node_idx = 0;
			video::Render_Job render_job;
			mutable math::aabb3f aabb;
            mutable uint32_t aabb_cached_skeleton_version = 0;
		};

		std::vector<Mesh>		m_meshes;
		
		mutable math::aabb3f	m_aabb;
        mutable uint32_t		m_aabb_version = 0;
        mutable uint32_t		m_aabb_cached_version = 0;

		mutable math::aabb3f	m_local_aabb;

		//for a given material idx the renderjobs using it
		std::vector<std::vector<size_t>> m_material_idx_to_mesh_idx;
	};

}
}

