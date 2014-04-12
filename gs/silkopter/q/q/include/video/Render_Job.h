#pragma once

namespace q
{
namespace video
{

	class Render_Job
	{
	public:
		Render_Job();
		~Render_Job();


		//////////////////////////////////////////////////////////////////////////
		//what shader to ask from the ubershader?

		void set_shader_type(Shader::Type type);
		auto get_shader_type() const -> Shader::Type;

		//////////////////////////////////////////////////////////////////////////

		void set_bounds(q::util::Bounds const& b);
		auto get_bounds() const->q::util::Bounds const&;

		//////////////////////////////////////////////////////////////////////////

		void set_world_transform(math::trans3df const& transform);
		auto get_world_transform() const -> math::trans3df const&;
		bool has_world_transform() const;

		//////////////////////////////////////////////////////////////////////////
		//skinning/batching
		void set_node_transform_count(size_t count);
		auto get_node_transform_count() const -> size_t;

		void set_node_transform(size_t idx, math::trans3df const& transform);
		auto get_node_transform(size_t idx) const -> math::trans3df const&;

		//////////////////////////////////////////////////////////////////////////

		void set_material(Material const& material);
		auto get_material() -> Material&;
		auto get_material() const -> Material const&;

		//////////////////////////////////////////////////////////////////////////

		void set_vertex_declaration(Vertex_Declaration const& vertex);
		auto get_vertex_declaration() -> Vertex_Declaration&;
		auto get_vertex_declaration() const -> Vertex_Declaration const&;

		//////////////////////////////////////////////////////////////////////////

	private:
		Shader::Type		m_shader_type;
		util::Bounds		m_bounds;
		math::trans3df		m_world_transform;
		bool				m_has_world_transform;
		std::vector<math::trans3df>	m_node_transforms;
		Material			m_material;
		Vertex_Declaration	m_vertex_declaration;
	};


}
}