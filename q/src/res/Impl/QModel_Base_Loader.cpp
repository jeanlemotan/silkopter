#include "QStdAfx.h"
#include "QModel_Base_Loader.h"

using namespace q;
using namespace res;
using namespace impl;
using namespace data;
using namespace scene;
using namespace video;

bool QModel_Base_Loader::can_load_from_source(data::Source& source) const
{
	source.seek(0);

	uint8_t h[2];
	size_t s = source.read(h, sizeof(h));
	if (s != 2)
	{
		return false;
	}
	if (h[0] != 'q' || h[1] != 'm')
	{
		return false;
	}

	return true;
}

void QModel_Base_Loader::load(Path const& /*path*/, data::Source& source, Model_Base& model_base) const
{
	model_base.unload();
	source.seek(0);

	{
		uint8_t q, m;
		source >> q >> m;
		if (q != 'q' && m != 'm')
		{
			return;
		}
	}

	uint32_t version;
	source >> version;

	uint32_t material_count;
	source >> material_count;

	for (uint32_t i = 0; i < material_count; i++)
	{
		String name;
		source >> name;

		video::Material material;

		video::Technique_ptr technique = System::inst().get_factory().load<video::Technique>(Path(name));
		if (!technique || !technique->is_valid())
		{
			QLOG_WARNING("Q", "Cannot load technique '{}'. Trying the default one.", name);
			static String def("default.technique");
			technique = System::inst().get_factory().load_by_name<video::Technique>(def);
			if (!technique)
			{
				QLOG_ERR("Q", "Cannot default technique.");
				return;
			}
		}
		material.set_technique(technique);

		bool twoSided;
		float shininess, reflection, refraction;
		source >> twoSided >> shininess >> reflection >> refraction;

		material.get_render_state(0).set_culling(!twoSided);

		static String shininessStr("shininess");
		static String reflectionStr("reflection");
		static String refractionStr("refraction");
		static String ambientColorStr("ambientColor");
		static String diffuseColorStr("diffuseColor");
		static String specularColorStr("specularColor");
		static String emissiveColorStr("emissiveColor");

		int uidx = material.find_uniform_idx_by_name(0, shininessStr);
		if (uidx >= 0)
		{
			material.set_uniform(0, uidx, video::Uniform(shininess));
		}
		uidx = material.find_uniform_idx_by_name(0, reflectionStr);
		if (uidx >= 0)
		{
			material.set_uniform(0, uidx, video::Uniform(reflection));
		}
		uidx = material.find_uniform_idx_by_name(0, refractionStr);
		if (uidx >= 0)
		{
			material.set_uniform(0, uidx, video::Uniform(reflection));
		}

		math::vec4f ambient, diffuse, specular, emissive;
		source >> ambient >> diffuse >> specular >> emissive;

		uidx = material.find_uniform_idx_by_name(0, ambientColorStr);
		if (uidx >= 0)
		{
			Uniform u = material.get_uniform(0, uidx);
			if (u.get_type() == Uniform::Type::VEC4) 
			{
				u = ambient; 
			}
			if (u.get_type() == Uniform::Type::VEC3) 
			{
				u = math::vec3f(ambient);
			}
			else 
			{
				u = ambient.x;
			}
			material.set_uniform(0, uidx, u);
		}
		uidx = material.find_uniform_idx_by_name(0, diffuseColorStr);
		if (uidx >= 0)
		{
			Uniform u = material.get_uniform(0, uidx);
			if (u.get_type() == Uniform::Type::VEC4) 
			{
				u = diffuse; 
			}
			if (u.get_type() == Uniform::Type::VEC3) 
			{
				u = math::vec3f(diffuse);
			}
			else 
			{
				u = diffuse.x;
			}
			material.set_uniform(0, uidx, u);
		}
		uidx = material.find_uniform_idx_by_name(0, specularColorStr);
		if (uidx >= 0)
		{
			Uniform u = material.get_uniform(0, uidx);
			if (u.get_type() == Uniform::Type::VEC4) 
			{
				u = specular; 
			}
			if (u.get_type() == Uniform::Type::VEC3) 
			{
				u = math::vec3f(specular);
			}
			else 
			{
				u = specular.x;
			}
			material.set_uniform(0, uidx, u);
		}
		uidx = material.find_uniform_idx_by_name(0, emissiveColorStr);
		if (uidx >= 0)
		{
			Uniform u = material.get_uniform(0, uidx);
			if (u.get_type() == Uniform::Type::VEC4) 
			{
				u = emissive; 
			}
			if (u.get_type() == Uniform::Type::VEC3) 
			{
				u = math::vec3f(emissive);
			}
			else 
			{
				u = emissive.x;
			}
			material.set_uniform(0, uidx, u);
		}

		uint32_t tex_channel_count;
		source >> tex_channel_count;
		for (uint32_t tc = 0; tc < tex_channel_count; tc++)
		{
			String channelName, textureName;
			char wrapu, wrapv;
			source >> channelName >> textureName >> wrapu >> wrapv;

			int sidx = material.find_sampler_idx_by_name(0, channelName);
			if (sidx >= 0)
			{
				Sampler sampler = material.get_sampler(0, sidx);
				sampler.set_wrapping(wrapu == 'c' ? Sampler::Wrap::CLAMP : Sampler::Wrap::WRAP, wrapv == 'c' ? Sampler::Wrap::CLAMP : Sampler::Wrap::WRAP);
				sampler.set_texture(System::inst().get_factory().load_by_name<video::Texture>(textureName));
				material.set_sampler(0, sidx, sampler);
			}

			model_base.add_texture_channel(channelName, textureName, wrapu == 'c' ? Sampler::Wrap::CLAMP : Sampler::Wrap::WRAP, wrapv == 'c' ? Sampler::Wrap::CLAMP : Sampler::Wrap::WRAP);
		}

		model_base.add_material(name, material);
	}

	Skeleton skeleton;

	uint32_t node_count;
	source >> node_count;
	for (uint32_t i = 0; i < node_count; i++)
	{
		String name;
		int parentIdx;
		source >> name >> parentIdx;

		//NodePtr node = model_base.getNode(parentIdx + 1)->addChild();
		//node->setName(name);
		math::vec3f pos, scale;
		math::quatf rot;
		source >> pos >> rot >> scale;
		//node->setTransform(pos, rot, scale);

		int idx = skeleton.add_node(name, parentIdx + 1);
		if (idx >= 0)
		{
			skeleton.set_node_transform(idx, pos, rot, scale);
		}
	}

	model_base.set_skeleton(skeleton);

	uint32_t mesh_count;
	source >> mesh_count;
	for (uint32_t i = 0; i < mesh_count; i++)
	{
		String name;
		source >> name;

		uint32_t node_idx;
		source >> node_idx;

		uint32_t material_idx;
		source >> material_idx;

		//SubMeshPtr sm = mesh->addSubMesh(model_base.getMaterial(material_idx));

		math::aabb3f aabb;
		source >> aabb;

		//smodel_base.setBounds(util::Bounds(center, radius));

		Vertex_Buffer_ptr vertexBuffer(Vertex_Buffer::create());
		vertexBuffer->allocate(0, Buffer::Usage::STATIC);
		Index_Buffer_ptr indexBuffer(Index_Buffer::create());
		indexBuffer->allocate(0, Buffer::Usage::STATIC);
		Vertex_Declaration declaration;

		declaration.set_topology(video::Vertex_Declaration::Topology::TRIANGLE_LIST);

		uint32_t vertex_count;
		source >> vertex_count;

		uint32_t index_count;
		source >> index_count;
		QASSERT(index_count);

		uint32_t vertex_format;
		source >> vertex_format;

		uint32_t _skin_npv;
		source >> _skin_npv;
		uint8_t skin_npv = static_cast<uint8_t>(_skin_npv);

		std::vector<uint32_t> skin_node_indices;
		std::vector<math::trans3df> skin_node_bind_pose_transforms;
		if (skin_npv > 0)
		{
			uint32_t c;
			source >> c;
			QASSERT(c > 0);
			skin_node_indices.resize(c);
			skin_node_bind_pose_transforms.resize(c);
			for (uint32_t k = 0; k < c; k++)
			{
				source >> skin_node_indices[k];
				skin_node_indices[k]++;
				source >> skin_node_bind_pose_transforms[k];
			}
		}

		QASSERT(vertex_format & (1 << (int)Vertex_Declaration::Semantic::POSITIONS));

		uint16_t stride = (vertex_format & (1 << (int)Vertex_Declaration::Semantic::POSITIONS)) ? 12 : 0;
		stride += (vertex_format & (1 << (int)Vertex_Declaration::Semantic::NORMALS)) ? 12 : 0;
		stride += (vertex_format & (1 << (int)Vertex_Declaration::Semantic::TANGENTS)) ? 16 : 0;
		stride += (vertex_format & (1 << (int)Vertex_Declaration::Semantic::COLORS)) ? 4 : 0;
		stride += (vertex_format & (1 << (int)Vertex_Declaration::Semantic::NODE_INDICES)) ? 4 : 0;
		stride += (vertex_format & (1 << (int)Vertex_Declaration::Semantic::NODE_WEIGHTS)) ? (skin_npv * 4) : 0;
		stride += (vertex_format & (1 << (int)Vertex_Declaration::Semantic::TEX_COORDS0)) ? 8 : 0;
		stride += (vertex_format & (1 << (int)Vertex_Declaration::Semantic::TEX_COORDS1)) ? 8 : 0;
		stride += (vertex_format & (1 << (int)Vertex_Declaration::Semantic::TEX_COORDS2)) ? 8 : 0;
		stride += (vertex_format & (1 << (int)Vertex_Declaration::Semantic::TEX_COORDS3)) ? 8 : 0;

		uint16_t offset = 0;


		//all models have positions
		int idx = declaration.add_attribute(vertexBuffer, Vertex_Declaration::Semantic::POSITIONS, Vertex_Declaration::Type::FLOAT, 3, offset, stride);
		QASSERT(idx >= 0);
		offset += 12;
		if (vertex_format & (1 << (int)Vertex_Declaration::Semantic::NORMALS))
		{
			idx = declaration.add_attribute(vertexBuffer, Vertex_Declaration::Semantic::NORMALS, Vertex_Declaration::Type::FLOAT, 3, offset, stride);
			QASSERT(idx >= 0);
			offset += 12;
		}
		if (vertex_format & (1 << (int)Vertex_Declaration::Semantic::TANGENTS))
		{
			idx = declaration.add_attribute(vertexBuffer, Vertex_Declaration::Semantic::TANGENTS, Vertex_Declaration::Type::FLOAT, 4, offset, stride);
			QASSERT(idx >= 0);
			offset += 16;
		}
		if (vertex_format & (1 << (int)Vertex_Declaration::Semantic::COLORS))
		{
			idx = declaration.add_attribute(vertexBuffer, Vertex_Declaration::Semantic::COLORS, Vertex_Declaration::Type::FP_U8, 4, offset, stride);
			QASSERT(idx >= 0);
			offset += 4;
		}
		if (vertex_format & (1 << (int)Vertex_Declaration::Semantic::NODE_INDICES))
		{
			QASSERT(skin_npv > 0);
			idx = declaration.add_attribute(vertexBuffer, Vertex_Declaration::Semantic::NODE_INDICES, Vertex_Declaration::Type::U8, 4, offset, stride);
			QASSERT(idx >= 0);
			offset += 4;
		}
		if (vertex_format & (1 << (int)Vertex_Declaration::Semantic::NODE_WEIGHTS))
		{
			QASSERT(skin_npv > 0);
			idx = declaration.add_attribute(vertexBuffer, Vertex_Declaration::Semantic::NODE_WEIGHTS, Vertex_Declaration::Type::FLOAT, skin_npv, offset, stride);
			QASSERT(idx >= 0);
			offset += skin_npv * 4;
		}

		for (uint32_t t = 0; t < 4; t++)
		{
			if (vertex_format & (1 << ((int)Vertex_Declaration::Semantic::TEX_COORDS0 + t)))
			{
				auto semantic = (Vertex_Declaration::Semantic)((int)Vertex_Declaration::Semantic::TEX_COORDS0 + t);
				idx = declaration.add_attribute(vertexBuffer, semantic, Vertex_Declaration::Type::FLOAT, 2, offset, stride);
				QASSERT(idx >= 0);
				offset += 8;
			}
		}

		declaration.set_index_buffer(indexBuffer);
		declaration.set_index_count(index_count);
		declaration.set_vertex_count(vertex_count);

		//all models have positions
		idx = declaration.find_attribute_idx_by_semantic(Vertex_Declaration::Semantic::POSITIONS);
		if (idx >= 0)
		{
			auto ptr = declaration.lock_for_writing<math::vec3f>(idx);
			QASSERT(ptr.is_valid());
			if (!ptr.is_valid())
			{
				model_base.unload();
				return;
			}
			std::vector<uint8_t> data(sizeof(float) * 3 * vertex_count);
			source.read(data.data(), data.size());

			util::Const_Ptr_Iterator<math::vec3f> src(data.data(), vertex_count);
			std::copy(src, src + vertex_count, ptr);
			//util::memcpy(ptr.get(), stride, data.data(), 0, sizeof(float) * 3, vertexCount);
		}

		idx = declaration.find_attribute_idx_by_semantic(Vertex_Declaration::Semantic::NORMALS);
		if (idx >= 0)
		{
			auto ptr = declaration.lock_for_writing<math::vec3f>(idx);
			QASSERT(ptr.is_valid());
			if (!ptr.is_valid())
			{
				model_base.unload();
				return;
			}
			std::vector<uint8_t> data(sizeof(float) * 3 * vertex_count);
			source.read(data.data(), data.size());

			util::Const_Ptr_Iterator<math::vec3f> src(data.data(), vertex_count);
			std::copy(src, src + vertex_count, ptr);
//			util::memcpy((uint8_t*)ptr, stride, data.data(), 0, sizeof(float) * 3, vertexCount);
		}
		idx = declaration.find_attribute_idx_by_semantic(Vertex_Declaration::Semantic::TANGENTS);
		if (idx >= 0)
		{
			auto ptr = declaration.lock_for_writing<math::vec4f>(idx);
			QASSERT(ptr.is_valid());
			if (!ptr.is_valid())
			{
				model_base.unload();
				return;
			}
			std::vector<uint8_t> data(sizeof(math::vec4f) * vertex_count);
			source.read(data.data(), data.size());

			util::Const_Ptr_Iterator<math::vec4f> src(data.data(), vertex_count);
			std::copy(src, src + vertex_count, ptr);
			//util::memcpy((uint8_t*)ptr, stride, data.data(), 0, sizeof(float) * 4, vertexCount);
		}
		idx = declaration.find_attribute_idx_by_semantic(Vertex_Declaration::Semantic::COLORS);
		if (idx >= 0)
		{
			auto ptr = declaration.lock_for_writing<math::vec4u8>(idx);
			QASSERT(ptr.is_valid());
			if (!ptr.is_valid())
			{
				model_base.unload();
				return;
			}
			std::vector<uint8_t> data(sizeof(uint8_t) * 4 * vertex_count);
			source.read(data.data(), data.size());

			util::Const_Ptr_Iterator<math::vec4u8> src(data.data(), vertex_count);
			std::copy(src, src + vertex_count, ptr);

			//util::memcpy((uint8_t*)ptr, stride, data.data(), 0, sizeof(uint8_t) * 4, vertexCount);
		}
		idx = declaration.find_attribute_idx_by_semantic(Vertex_Declaration::Semantic::NODE_INDICES);
		if (idx >= 0)
		{
			auto ptr = declaration.lock_for_writing<math::vec4u8>(idx);
			QASSERT(ptr.is_valid());
			if (!ptr.is_valid())
			{
				model_base.unload();
				return;
			}
			std::vector<uint8_t> data(sizeof(uint8_t) * 4 * vertex_count);
			source.read(data.data(), data.size());

			util::Const_Ptr_Iterator<math::vec4u8> src(data.data(), vertex_count);
			std::copy(src, src + vertex_count, ptr);

			//util::memcpy((uint8_t*)ptr, stride, data.data(), 0, sizeof(uint8_t) * 4, vertexCount);
		}
		idx = declaration.find_attribute_idx_by_semantic(Vertex_Declaration::Semantic::NODE_WEIGHTS);
		if (idx >= 0)
		{
			auto ptr = declaration.lock_for_writing<float>(idx);
			QASSERT(ptr.is_valid());
			if (!ptr.is_valid())
			{
				model_base.unload();
				return;
			}
			std::vector<uint8_t> data(sizeof(float) * skin_npv * vertex_count);
			source.read(data.data(), data.size());

			util::Const_Ptr_Iterator<float> src(data.data(), vertex_count * skin_npv);
			std::copy(src, src + vertex_count, ptr);

			//util::memcpy((uint8_t*)ptr, stride, data.data(), 0, sizeof(float) * skinNPV, vertexCount);
		}

		for (uint32_t t = 0; t < 4; t++)
		{
			idx = declaration.find_attribute_idx_by_semantic(Vertex_Declaration::Semantic((int)Vertex_Declaration::Semantic::TEX_COORDS0 + t));
			if (idx >= 0)
			{
				auto ptr = declaration.lock_for_writing<math::vec2f>(idx);
				QASSERT(ptr.is_valid());
				if (!ptr.is_valid())
				{
					model_base.unload();
					return;
				}
				std::vector<uint8_t> data(sizeof(math::vec2f) * vertex_count);
				source.read(data.data(), data.size());

				util::Const_Ptr_Iterator<math::vec2f> src(data.data(), vertex_count);
				std::copy(src, src + vertex_count, ptr);

				//util::memcpy((uint8_t*)ptr, stride, data.data(), 0, sizeof(float) * 2, vertexCount);
			}
		}

		auto ptr = declaration.lock_indices_for_writing();
		QASSERT(ptr.is_valid());
		if (!ptr.is_valid())
		{
			model_base.unload();
			return;
		}
		source.read((uint8_t*)ptr.get(), sizeof(uint16_t) * index_count);

		declaration.unlock_all();

		int smidx = model_base.add_mesh(name, node_idx + 1, material_idx, declaration, aabb);
		if (skin_npv > 0)
		{
			model_base.set_mesh_skin_data(smidx, skin_npv, skin_node_indices, skin_node_bind_pose_transforms);
		}
	}

	//model_base.set_is_valid(true);
}