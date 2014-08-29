#include "QStdAfx.h"

using namespace q;
using namespace video;

void Render_Job::set_shader_type(Shader::Type type)
{
	m_shader_type = type;
}
auto Render_Job::get_shader_type() const -> Shader::Type
{
	return m_shader_type;
}

void Render_Job::set_bounds(q::util::Bounds const& b)
{
	m_bounds = b;
}
auto Render_Job::get_bounds() const -> q::util::Bounds const&
{
	return m_bounds;
}

void Render_Job::set_world_transform(math::trans3df const& transform)
{
	m_world_transform = transform;
	m_has_world_transform = math::is_identity(transform);
}
auto Render_Job::get_world_transform() const -> math::trans3df const&
{
	return m_world_transform;
}
bool Render_Job::has_world_transform() const
{
	return m_has_world_transform;
}

void Render_Job::set_node_transform_count(size_t count)
{
	m_node_transforms.resize(count);
}
auto Render_Job::get_node_transform_count() const -> size_t
{
	return m_node_transforms.size();
}

void Render_Job::set_node_transform(size_t idx, math::trans3df const& transform)
{
	m_node_transforms[idx] = transform;
}
auto Render_Job::get_node_transform(size_t idx) const -> math::trans3df const&
{
	return m_node_transforms[idx];
}

void Render_Job::set_material(Material const& material)
{
	m_material = material;
}
auto Render_Job::get_material() -> Material&
{
	return m_material;
}
auto Render_Job::get_material() const -> Material const&
{
	return m_material;
}

void Render_Job::set_vertex_declaration(Vertex_Declaration const& vertex)
{
	m_vertex_declaration = vertex;
}

auto Render_Job::get_vertex_declaration() const -> Vertex_Declaration const&
{
	return m_vertex_declaration;
}

auto Render_Job::get_vertex_declaration() -> Vertex_Declaration&
{
	return m_vertex_declaration;
}
