#include "QStdAfx.h"
#include "scene/Skeleton.h"

using namespace q;
using namespace scene;


Skeleton::Skeleton()
{
	m_nodes.emplace_back(Node());
	m_nodes[0].parent_idx = -1;
	m_names.emplace_back("<root>");
	m_version = 54321;
}

bool Skeleton::operator==(Skeleton const& other)
{
	return m_nodes == other.m_nodes && m_names == other.m_names;
}
bool Skeleton::operator!=(Skeleton const& other)
{
	return !operator==(other);
}


int Skeleton::find_node_idx_by_name(std::string const& name) const
{
	auto it = std::find(m_names.begin(), m_names.end(), name);
	return (it != m_names.end()) ? std::distance(m_names.begin(), it) : -1;
}
int Skeleton::add_node(std::string const& name, size_t parent_idx)
{
	int idx = find_node_idx_by_name(name);
	if (idx >= 0)
	{
        QLOGE("Duplicated node name '{}'. Ignoring new node", name);
		return -1;
	}

	m_names.emplace_back(name);
	Node node;
	node.parent_idx = parent_idx;
	m_nodes.emplace_back(node);
	return m_nodes.size() - 1;
}
void Skeleton::remove_node(size_t idx)
{
	QASSERT(idx > 0);
	if (idx == 0)
	{
		return;
	}

	//take care of the indices
	for(auto& n: m_nodes)
	{
		if (n.parent_idx >= (int)idx)
		{
			n.parent_idx--;
		}
	}
	m_nodes.erase(m_nodes.begin() + idx);
	m_names.erase(m_names.begin() + idx);
}

void Skeleton::remove_all_nodes()
{
	m_nodes.resize(1);
	m_names.resize(1);
}

void Skeleton::_update_node_world_transform(size_t idx) const
{
	auto const& node = m_nodes[idx];
	node.skeleton_version = m_version;
	if (node.parent_idx >= 0)
	{
		auto const& pt = get_node_world_transform(node.parent_idx);
		auto const& t = get_node_transform(idx);
		uint32_t atv = get_node_world_transform_version(node.parent_idx) + node.transform_version;
		//if (node.worldTransformVersion != atv)
		{
			node.world_transform_version = atv;
			math::multiply(node.world_transform, pt, t);
			node.world_rotation = get_node_world_rotation(node.parent_idx) * node.rotation;
		}
	}
	else
	{
		auto const& t = get_node_transform(idx);
		//if (node.worldTransformVersion != node.transformVersion)
		{
			node.world_transform_version = node.transform_version;
			node.world_transform = t;
			node.world_rotation = node.rotation;
		}
	}
}
math::trans3df const& Skeleton::get_node_world_transform(size_t idx) const
{
	update_node_world_transform(idx);
	return m_nodes[idx].world_transform;
}
math::vec3f const& Skeleton::get_node_world_position(size_t idx) const
{
	return get_node_world_transform(idx).get_translation();
}
math::quatf const& Skeleton::get_node_world_rotation(size_t idx) const
{
	update_node_world_transform(idx);
	return m_nodes[idx].world_rotation;
}
math::vec3f const& Skeleton::get_node_world_scale(size_t idx) const
{
	m_nodes[idx].world_scale = get_node_world_transform(idx).get_scale();
	return m_nodes[idx].world_scale;
}
void Skeleton::set_node_world_position(size_t idx, math::vec3f const& pos)
{
	QASSERT(0 && "test it!!!");
	auto& node = m_nodes[idx];
	if (node.parent_idx >= 0)
	{
		auto const& pt = get_node_world_transform(node.parent_idx);
		set_node_position(idx, math::transform(pt, pos));
	}
	else
	{
		set_node_position(idx, pos);
	}
}
void Skeleton::set_node_world_rotation(size_t idx, math::quatf const& rot)
{
	QASSERT(0 && "implement it!!!");
	auto& node = m_nodes[idx];
	if (node.parent_idx >= 0)
	{
		//auto const& pt = get_node_world_rotation(node.parent_idx);
		set_node_rotation(idx, rot);
	}
	else
	{
		set_node_rotation(idx, rot);
	}
}


void Skeleton::_update_node_transform(size_t idx) const
{
	auto const& node = m_nodes[idx];
	node.transform_version = node.version;
	
    math::mat3f rot(math::uninitialized);
	node.rotation.get_as_mat3(rot);
	node.transform.set_rotation(rot);
	node.transform.post_scale(node.scale);
}
math::trans3df const& Skeleton::get_node_transform(size_t idx) const
{
	update_node_transform(idx);
	return m_nodes[idx].transform;
}
void Skeleton::set_node_position(size_t idx, math::vec3f const& pos)
{
	if (*(math::vec2s32*)&get_node_position(idx) != *(math::vec2s32*)&pos)
	{
		auto& node = m_nodes[idx];
		node.transform.set_translation(pos);
		node.version++;
		m_version++;
	}
}
void Skeleton::set_node_rotation(size_t idx, math::quatf const& rot)
{
	if (*(math::vec4s32*)&get_node_rotation(idx) != *(math::vec4s32*)&rot)
	{
		auto& node = m_nodes[idx];
		node.rotation = rot;
		node.version++;
		m_version++;
	}
}
void Skeleton::set_node_scale(size_t idx, math::vec3f const& scale)
{
	if (*(math::vec2s32*)&get_node_scale(idx) != *(math::vec2s32*)&scale)
	{
		auto& node = m_nodes[idx];
		node.scale = scale;
		node.version++;
		m_version++;
	}
}
void Skeleton::set_node_transform(size_t idx, math::vec3f const& pos, math::quatf const& rot, math::vec3f const& scale)
{
	auto& node = m_nodes[idx];

	uint32_t changed = 0;
	if (*(math::vec2s32*)&get_node_position(idx) != *(math::vec2s32*)&pos)
	{
		node.transform.set_translation(pos);
		changed = 1;
	}
	if (*(math::vec4s32*)&get_node_rotation(idx) != *(math::vec4s32*)&rot)
	{
		node.rotation = rot;
		changed = 1;
	}
	if (*(math::vec2s32*)&get_node_scale(idx) != *(math::vec2s32*)&scale)
	{
		node.scale = scale;
		changed = 1;
	}
	node.version += changed;
	m_version += changed;
}
void Skeleton::set_node_transform(size_t idx, math::vec3f const& pos, math::quatf const& rot)
{
	auto& node = m_nodes[idx];

	uint32_t changed = 0;
	if (*(math::vec2s32*)&get_node_position(idx) != *(math::vec2s32*)&pos)
	{
		node.transform.set_translation(pos);
		changed = 1;
	}
	if (*(math::vec4s32*)&get_node_rotation(idx) != *(math::vec4s32*)&rot)
	{
		node.rotation = rot;
		changed = 1;
	}
	node.version += changed;
	m_version += changed;
}
