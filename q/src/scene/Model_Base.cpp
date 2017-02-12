#include "QStdAfx.h"
#include "scene/Model_Base.h"
#include "scene/Model.h"

using namespace q;
using namespace scene;

Model_Base::Model_Base(Path const& path)
	: Resource(path)
{
}
Model_Base::~Model_Base()
{
}

Model_ptr Model_Base::create_instance()
{
	return is_valid() ? Model_ptr(new Model(shared_from_this())) : Model_ptr();
}

void Model_Base::unload()
{
	QASSERT(0);
}
bool Model_Base::is_valid() const
{
	return !m_meshes.empty();
}

Skeleton const& Model_Base::get_skeleton() const
{
	return m_skeleton;
}
void Model_Base::set_skeleton(Skeleton const& skeleton)
{
	m_skeleton = skeleton;
}
size_t Model_Base::add_mesh(std::string const& name, size_t node_idx, size_t material_idx, video::Vertex_Declaration const& declaration, math::aabb3f const& aabb)
{
	QASSERT(node_idx < m_skeleton.get_node_count());
	QASSERT(material_idx < m_materials.size());
	Mesh_Data data;
	data.name = name;
	data.node_idx = node_idx;
	data.material_idx = material_idx;
	data.vertex_declaration = declaration;
	data.aabb = aabb;
	m_meshes.push_back(data);
	return m_meshes.size() - 1;
}
void Model_Base::set_mesh_skin_data(size_t mesh_idx, size_t skin_npv, std::vector<uint32_t> const& indices, std::vector<math::trans3df> const& bind_pose_transforms)
{
	QASSERT(mesh_idx < m_meshes.size());
	QASSERT(skin_npv <= 4);

	auto& sm = m_meshes[mesh_idx];
	sm.skin_npv = skin_npv;
	sm.indices = indices;
	sm.bind_pose_transforms = bind_pose_transforms;
}
size_t Model_Base::get_mesh_count() const
{
	return m_meshes.size();
}
Model_Base::Mesh_Data const& Model_Base::get_mesh(size_t idx) const
{
	return m_meshes[idx];
}
size_t Model_Base::add_material(std::string const& name, video::Material const& material)
{
	QASSERT(find_material_idx_by_name(name) < 0);
	Material_Data data;
	data.name = name;
	data.material = material;
	m_materials.push_back(data);
	return m_materials.size() - 1;
}
size_t Model_Base::get_material_count() const
{
	return m_materials.size();
}
Model_Base::Material_Data const& Model_Base::get_material(size_t idx)
{
	return m_materials[idx];
}
int Model_Base::find_material_idx_by_name(std::string const& name)
{
	auto it = std::find_if(m_materials.begin(), m_materials.end(), [&](Material_Data const& m) { return m.name == name; });
	if (it != m_materials.end())
	{
		return std::distance(m_materials.begin(), it);
	}
	return -1;
}

size_t Model_Base::add_texture_channel(std::string const& name, std::string const& textureName, video::Sampler::Wrap wrapu, video::Sampler::Wrap wrapv)
{
	Texture_Channel_Data data;
	data.name = name;
	data.texture_name = textureName;
	data.wrap_u = wrapu;
	data.wrap_v = wrapv;
	m_texture_channels.push_back(data);
	return m_texture_channels.size() - 1;
}
size_t Model_Base::get_texture_channel_count() const
{
	return m_texture_channels.size();
}
Model_Base::Texture_Channel_Data const& Model_Base::get_texture_channel(size_t idx) const
{
	return m_texture_channels[idx];
}

