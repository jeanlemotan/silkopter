#include "QStdAfx.h"
#include "scene/Model.h"

using namespace q;
using namespace scene;

Model::Model(Model_Base_ptr const& base)
	: m_base(base)
{
	m_this_ptr = Model_ptr(this, noop_deleter<Model>);

	m_skeleton = m_base->get_skeleton();

	m_material_idx_to_mesh_idx.resize(m_base->get_material_count());
// 	for (size_t i = 0; i < m_material_idx_to_mesh_idx.size(); i++)
// 	{
// 		ModelBase::MaterialData const& data = m_base->getMaterial(i);
// 		mMaterials[i] = data.material;
// 	}

	m_meshes.resize(m_base->get_mesh_count());
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		auto const& data = m_base->get_mesh(i);
		m_meshes[i].node_idx = data.node_idx;
		m_meshes[i].aabb = data.aabb;
		m_meshes[i].render_job.set_material(m_base->get_material(data.material_idx).material);
		m_material_idx_to_mesh_idx[data.material_idx].push_back(i);
		m_meshes[i].render_job.set_vertex_declaration(data.vertex_declaration);
		if (data.skin_npv > 0)
		{
			m_meshes[i].render_job.set_shader_type(video::Shader::Type((int)video::Shader::Type::SKINNING_1NPV + data.skin_npv - 1));
			m_meshes[i].render_job.set_node_transform_count(data.indices.size());
		}
		else
		{
			m_meshes[i].render_job.set_shader_type(video::Shader::Type::NORMAL);
		}
	}

	update_aabb();
}
Model::~Model()
{
}

math::aabb3f const& Model::get_local_aabb() const
{
	auto root_matrix_inv = m_skeleton.get_node_world_transform(0);
	root_matrix_inv.invert();

	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		auto const& mesh = m_meshes[i];
		auto trans = m_skeleton.get_node_world_transform(mesh.node_idx);
		math::multiply(trans, root_matrix_inv, trans);
		math::aabb3f m = math::transform(trans, m_base->get_mesh(i).aabb);
		if (i == 0)
		{
			m_local_aabb.reset(m);
		}
		else
		{
			m_local_aabb.add_internal_box(m);
		}
	}
	return m_local_aabb;
}

void Model::update_aabb() const
{
	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		update_mesh_aabb(i);
	}
	if (m_aabb_cached_version != m_aabb_version)
	{
		m_aabb.reset(m_meshes[0].aabb);
		for (size_t i = 1; i < m_meshes.size(); i++)
		{
			m_aabb.add_internal_box(m_meshes[i].aabb);
		}
		m_aabb_cached_version = m_aabb_version;
	}
}

void Model::update_mesh_aabb(size_t idx) const
{
	auto const& mesh = m_meshes[idx];
	auto const& trans = m_skeleton.get_node_world_transform(mesh.node_idx);
	if (mesh.aabb_cached_skeleton_version != m_skeleton.get_node_world_transform_version(mesh.node_idx))
	{
		mesh.aabb_cached_skeleton_version = m_skeleton.get_node_world_transform_version(mesh.node_idx);
		mesh.aabb = math::transform(trans, m_base->get_mesh(idx).aabb);
		m_aabb_version++;
	}
}

extern draw::Painter* s_painter;

void Model::cull(Cull_Result& result, Cull_Params const& params) const
{
	update_aabb();
	result.status = params.camera.get_frustum().cull_aabb(m_aabb);
}

void Model::render(video::Renderer& renderer, Scene& /*scene*/)
{
	static Duration time;
	//if (time == 0)
	{
		animate(time);
		time = time + std::chrono::milliseconds(26);
	}
	if (m_animation_node && time >= m_animation_node->get_duration())
	{
		time = Duration(0);
	}

// 	int iidx = m_skeleton.find_node_idx_by_name("Bone003");
// 	if (iidx >= 0)
// 	{
// 		m_skeleton.set_node_rotation(iidx, m_skeleton.get_node_rotation(iidx) * math::quatAxisZ(math::radians(1.f)));
// 	}

// 	for (size_t i = 0; i < m_skeleton.get_node_count(); i++)
// 	{
// 		int parentIdx = m_skeleton.getNodeParentIdx(i);
// 		if (parentIdx >= 0)
// 		{
// 			math::vec3f pos = m_skeleton.get_node_world_position(i);
// 			math::vec3f ppos = m_skeleton.get_node_world_position(parentIdx);
// 			s_painter->drawLine(draw::Vertex(pos), draw::Vertex(ppos));
// 		}
// 	}

	for (size_t i = 0; i < m_meshes.size(); i++)
	{
		auto& mesh = m_meshes[i];

		//for skinned meshes set the node transforms
		auto const& base_data = m_base->get_mesh(i);
		if (!base_data.indices.empty())
		{
			for (size_t k = 0; k < base_data.indices.size(); k++)
			{
				uint32_t idx = base_data.indices[k];
				//math::mat4f trans = m_skeleton.get_node_world_transform(m_skeleton.getNodeParentIdx(idx));
				//trans = trans * (baseData.bindPoseTransforms[k] * m_skeleton.get_node_transform(idx));

				auto const& ntrans = m_skeleton.get_node_world_transform(idx);
				math::trans3df trans;
				math::multiply(trans, ntrans, base_data.bind_pose_transforms[k]);
				//trans.setTranslation(math::vec3f(0, 0, 2));
				mesh.render_job.set_node_transform(k, trans);

				//s_painter->setTransform(trans);
				//s_painter->drawAxis(math::vec3f(0, 0, 0), 1.f);
			}
		}
		else
		{
			auto const& trans = m_skeleton.get_node_world_transform(mesh.node_idx);

			//s_painter->setTransform(trans);
			//s_painter->drawAxis(math::vec3f(0, 0, 0), 1.f);

			mesh.render_job.set_world_transform(trans);
		}

		renderer.render(mesh.render_job);
	}

	//s_painter->setTransform(math::mat4f());
}

// video::MaterialPtr Model::addMaterial(String localName, String name)
// {
// 	video::MaterialPtr m = findMaterialByName(localName);
// 	if (m)
// 		return m;
// 
// 	video::Technique_ptr technique = System::inst().getFactory().load<video::Technique>(Path(name));
// 	if (!technique || technique->getStatus() != Resource::Status::READY)
// 	{
// 		static String def("default.technique");
// 		technique = System::inst().getFactory().load<video::Technique>(def);
// 	}
// 	video::MaterialPtr material = boost::make_shared<video::Material>(technique);
// 	mMaterials.push_back(std::make_pair(localName, material));
// 	mNameToMaterialMap[localName] = material;
// 	return material;
// }

//////////////////////////////////////////////////////////////////////////
// LUA bindings
//////////////////////////////////////////////////////////////////////////

void Model::set_animation_node(anim::Animation_Node_ptr const& node)
{
	m_animation_node = node;
	link_animation_node();
}
anim::Animation_Node_ptr const& Model::get_animation_node()
{
	return m_animation_node;
}

static String k_position("position");
static String k_rotation("rotation");
static String k_scale("scale");

void Model::link_animation_node()
{
	if (m_animation_node)
	{
		m_pose = m_animation_node->get_pose();
	}
	else
	{
		m_pose.clear();
	}

	//first eliminate the channels that we don't need
	for (int i = (int)m_pose.get_count() - 1; i >= 0; i--)
	{
		Path const& path = m_pose.get_path(i);
		if (!path.empty() && path[0] == "skeleton" && path.get_count() == 3)
		{
			auto const& nodeName = path[1];
			int node_idx = m_skeleton.find_node_idx_by_name(nodeName);
			if (node_idx <= 0) //node zero cannot be animated - it's the root
			{
				m_pose.remove_channel(i);
				continue;
			}
		}
		m_pose.remove_channel(i);
	}

	m_binded_skeleton_positions.clear();
	m_binded_skeleton_positions.reserve(m_pose.get_count());

	m_binded_skeleton_rotations.clear();
	m_binded_skeleton_rotations.reserve(m_pose.get_count());

	m_binded_skeleton_scales.clear();
	m_binded_skeleton_scales.reserve(m_pose.get_count());

	for (size_t i = 0; i < m_pose.get_count(); i++)
	{
		Path const& path = m_pose.get_path(i);
		QASSERT(path.get_count() == 3);
		auto const& node_name = path[1];
		int node_idx = m_skeleton.find_node_idx_by_name(node_name);
		QASSERT(node_idx >= 0);
		if (path[2] == k_position)
		{
			m_binded_skeleton_positions.emplace_back(i, node_idx);
		}
		else if (path[2] == k_rotation)
		{
			m_binded_skeleton_rotations.emplace_back(i, node_idx);
		}
		else if (path[2] == k_scale)
		{
			m_binded_skeleton_scales.emplace_back(i, node_idx);
		}
		else
		{
			QASSERT(0);
		}
	}
	//QASSERT(m_pose.getCount() == mBindedSkeletonNodes.size());
}

void Model::apply_pose()
{
	for (auto const& b: m_binded_skeleton_positions)
	{
		auto channel_idx = b.first;
		auto node_idx = b.second;
		auto const& d = m_pose.get_as_vec3(channel_idx);
		m_skeleton.set_node_position(node_idx, d);
	}
	for (auto const& b: m_binded_skeleton_rotations)
	{
		auto channel_idx = b.first;
		auto node_idx = b.second;
		auto const& d = m_pose.get_as_quat(channel_idx);
		m_skeleton.set_node_rotation(node_idx, d);
	}
	for (auto const& b: m_binded_skeleton_scales)
	{
		auto channel_idx = b.first;
		auto node_idx = b.second;
		auto const& d = m_pose.get_as_vec3(channel_idx);
		m_skeleton.set_node_scale(node_idx, d);
	}
}

void Model::animate(Duration time)
{
	if (m_animation_node && m_pose.get_count() != 0)
	{
		bool res = m_animation_node->update_pose(m_pose, time);
		if (res)
		{
			apply_pose();
		}
	}
}

//////////////////////////////////////////////////////////////////////////

// math::vec3f const* Model::raycast(const math::line3f& ray) const
// {
// 	float t0, t1;
//  	if (!math::rayIntersectSphere(ray.start, ray.getVectorNormalized(), m_aabb.getCenter(), m_aabb.getRadius(), t0, t1)) 
//  	{
//  		return false;
//  	}
// 
// 	QASSERT(0);
// 	return false;

// 	bool foundCollPoint = false;
// 	math::vec3f collPoint;
// 
// 	size_t mCount = m_meshes.size();
// 	for (size_t g = 0; g < mCount; g++)
// 	{
// 		Mesh const& mesh = m_meshes[g];
//  		if (!math::rayIntersectSphere(ray.start, ray.getVectorNormalized(), mesh.aabb3.getCenter(), mesh.aabb3.getRadius(), t0, t1))
//  		{
//  			continue;
//  		}
// 
// 		for (size_t k = 0, sz = mesh.subMeshes.size(); k < sz; k++)
// 		{
// 			SubMesh const& sm = mesh.subMeshes[k];
// 			video::MaterialCPtr const& material = sm.render_job.getMaterial();
// 			if (sm.render_job.getGeometry())
// 			{				
// 				video::RenderJob const& rj = sm.render_job;
// 				if (rj.getShaderType() != video::Shader::Type::NORMAL || rj.getTopology() != video::RenderJob::Topology::TRIANGLE_LIST)
// 				{
// 					continue;
// 				}
// 
// 				video::GeometryCPtr const& geo = rj.getGeometry();
// 
// 				size_t triCount = rj.getPrimitiveCount();
// 				if (geo)
// 				{				
// 					int attIdx = geo->findAttributeIdxBySemantic(video::Geometry::Semantic::POSITIONS);
// 					QASSERT(attIdx >= 0);
// 					if (attIdx < 0)
// 					{
// 						continue;
// 					}
// 
// 					size_t stride = geo->getAttribute(attIdx).stride;
// 					uint16_t const* indices = geo->mapIndicesRead();
// 					uint8_t const* positions = (const uint8_t*)geo->mapAttributeFloatRead(attIdx);
// 					if (!indices || !positions)
// 					{
// 						if (indices || positions)
// 						{
// 							geo->unmapAll();
// 						}
// 						continue;
// 					}			
// 
// 					bool bfc = (material->getPassCount() > 0 && material->getRenderState(0).getCulling() == true);
// 
// 					math::line3f localRay = ray;
// 
// 					//transform ray into local space
// 					math::mat4f trans = m_skeleton.get_node_world_transform(mesh.node_idx);
// 					math::mat4f inv = math::inverse(trans);
// 					localRay = math::line3f(math::transform(inv, ray.start), math::transform(inv, ray.end));
// 
// 					const math::vec3f& start = localRay.start;
// 					math::vec3f dir = localRay.getVectorNormalized();
// 
// 					for (size_t t = 0; t < triCount; t++)
// 					{
// 						const math::vec3f& a = *(const math::vec3f*)(positions + indices[t*3 + 0] * stride);
// 						const math::vec3f& b = *(const math::vec3f*)(positions + indices[t*3 + 1] * stride);
// 						const math::vec3f& c = *(const math::vec3f*)(positions + indices[t*3 + 2] * stride);
// 
// 						math::triangle3f tri(a, b, c);
// 						if (math::isZero(tri.getAreaSq()))//skip collinear triangles to avoid normalize assertion
// 						{
// 							continue;
// 						}
// 
// 						float dist, u, v;
// 						if (!tri.intersectRay(start, dir, bfc, dist, u, v))
// 						{
// 							continue;
// 						}
// 						collPoint = start + dir*dist;
// 						if (!localRay.isPointBetweenStartAndEnd(collPoint))
// 						{
// 							continue;
// 						}
// 
// 						//bring the point back to world coords
// 						collPoint = math::transform(trans, collPoint);
// 						if (!foundCollPoint)
// 						{
// 							foundCollPoint = true;
// 							m_collision_point = collPoint;
// 						}
// 						else
// 						{
// 							if ((collPoint - ray.start).getLengthSq() < (m_collision_point - ray.start).getLengthSq())
// 							{
// 								m_collision_point = collPoint;
// 							}
// 						}
// 					}
// 
// 					geo->unmapAll();
// 				}
// 			}
// 		}
// 	}
// 
// 	return foundCollPoint ? &m_collision_point : nullptr;
//}

void Model::set_material(size_t idx, video::Material const& material)
{
	for (auto midx: m_material_idx_to_mesh_idx[idx])
	{
		m_meshes[midx].render_job.set_material(material);
	}
}

int Model::find_material_idx_by_name(String const& name) const
{
	return m_base->find_material_idx_by_name(name);
}
size_t Model::get_material_count() const
{
	return m_material_idx_to_mesh_idx.size();
}
video::Material const& Model::get_material(size_t idx) const
{
	return m_meshes[m_material_idx_to_mesh_idx[idx][0]].render_job.get_material();
}
String Model::get_material_name(size_t idx) const
{
	return m_base->get_material(idx).name;
}
math::aabb3f const& Model::get_aabb() const
{
	update_aabb();
	return m_aabb;
}
Skeleton const& Model::get_skeleton() const
{
	return m_skeleton;
}
Skeleton& Model::get_skeleton()
{
	return m_skeleton;
}
math::vec3f const& Model::get_position() const
{
	return m_skeleton.get_node_position(0);
}
math::quatf const& Model::get_rotation() const
{
	return m_skeleton.get_node_rotation(0);
}
math::vec3f const& Model::get_scale() const
{
	return m_skeleton.get_node_scale(0);
}
math::trans3df const& Model::get_transform() const
{
	return m_skeleton.get_node_transform(0);
}
void Model::set_position(math::vec3f const& pos)
{
	m_skeleton.set_node_position(0, pos);
}
void Model::set_rotation(math::quatf const& rot)
{
	m_skeleton.set_node_rotation(0, rot);
}
void Model::set_scale(math::vec3f const& scale)
{
	m_skeleton.set_node_scale(0, scale);
}
void Model::set_transform(math::vec3f const& pos, math::quatf const& rot, math::vec3f const& scale)
{
	m_skeleton.set_node_transform(0, pos, rot, scale);
}
void Model::set_transform(math::vec3f const& pos, math::quatf const& rot)
{
	m_skeleton.set_node_transform(0, pos, rot);
}

Model_Base_ptr const& Model::get_base() const
{
	return m_base;
}
