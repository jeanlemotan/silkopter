#include "QStdAfx.h"
#include "video/GLES/GLES_Shader.h"
#include "video/GLES/GLES_Texture.h"
#include "video/GLES/GLES_Interface.h"

using namespace q;
using namespace video;

GLES_Shader::GLES_Shader(Type type, Shader_Source const& vertex, Shader_Source const& fragment)
	: m_type(type)
	, m_vertex_shader_src(vertex)
	, m_fragment_shader_src(fragment)
{
	int maxu = 0;
	int maxa = 0;
	int maxs = 0;

	gles::Interface interf;
	interf.iglGetIntegerv(gles::iGL_MAX_VERTEX_UNIFORM_VECTORS, &maxu);
	interf.iglGetIntegerv(gles::iGL_MAX_VERTEX_ATTRIBS, &maxa);
	interf.iglGetIntegerv(gles::iGL_MAX_TEXTURE_IMAGE_UNITS, &maxs);

	m_max_uniforms = (size_t)maxu;
	m_max_attributes = (size_t)maxa;
	m_max_samplers = (size_t)maxs;
}

GLES_Shader::~GLES_Shader()
{
	gles::Interface interf;
	if (m_vertex_shader_id)
	{
		interf.iglDeleteShader(m_vertex_shader_id);
	}
	if (m_fragment_shader_id)
	{
		interf.iglDeleteShader(m_fragment_shader_id);
	}
	if (m_shader_id)
	{
		interf.iglDeleteProgram(m_shader_id);
	}
}

Shader::Type GLES_Shader::get_type() const
{
	return m_type;
}
Shader_Source const& GLES_Shader::get_vertex_shader_source() const
{
	return m_vertex_shader_src;
}
Shader_Source const& GLES_Shader::get_fragment_shader_source() const
{
	return m_fragment_shader_src;
}

std::string GLES_Shader::get_shader_type_str(Uniform_Def const& def) const
{
	const char* t = 0;
	switch (def.get_default_value().get_type())
	{
	case Uniform::Type::BOOL: 	t = "bool"; break;
	case Uniform::Type::INT: 	t = "int"; break;
	case Uniform::Type::FLOAT: 	t = "float";break;
	case Uniform::Type::VEC2: 	t = "vec2"; break;
	case Uniform::Type::VEC3: 	t = "vec3"; break;
	case Uniform::Type::VEC4: 	t = "vec4"; break;
	case Uniform::Type::MAT3: 	t = "mat3"; break;
	case Uniform::Type::MAT4: 	t = "mat4"; break;
	default: QASSERT(0); return "";
	}

	return t;
}
std::string GLES_Shader::get_shader_type_precision_str(Uniform_Def const& def) const
{
	switch (def.get_precision())
	{
	case Uniform_Def::Precision::LOW: 		return "lowp";
	case Uniform_Def::Precision::MEDIUM: 	return "mediump";
	case Uniform_Def::Precision::HIGH: 		return "highp";
	}
	return std::string();
}

template<class T>
class Match_Def_By_Name
{
public:
	Match_Def_By_Name(String const& name) : m_name(name) {}
	bool operator()(T const& def)
	{
		return def.get_name() == m_name;
	}
	String m_name;
};


String GLES_Shader::patch_shader(
	std::vector<Uniform_Def>& uniforms,
	std::vector<Sampler_Def>& samplers,
	std::vector<Attribute_Def>& attributes,
	Shader_Source const& source)
{
	String src = source.get_source();
	size_t tok_count = source.get_token_count();

	std::map<String, std::vector<Shader_Source::Expression>> uniform_expressions; //uniforms
	std::map<String, std::vector<Shader_Source::Expression>> attribute_expressions; //attributes
	std::map<String, std::vector<Shader_Source::Expression>> sampler_expressions; //samplers

	//first collect them
	for (size_t i = 0; i < tok_count; i++)
	{
		Shader_Source::Token const& t = source.get_token(i);
		switch (t.type)
		{
		case Shader_Source::Token::Type::CONDITIONAL:
			{
				auto pos = util::find_line_char_by_offset(src, t.value.start);
                QLOGE("Shader contains unfiltered conditionals at {0}", pos);
				return String::null;
				break;
			}
		case Shader_Source::Token::Type::EXPRESSION:
			{
				Shader_Source::Expression const& a = reinterpret_cast<Shader_Source::Expression const&>(t);
				auto uit = std::find_if(uniforms.begin(), uniforms.end(), Match_Def_By_Name<Uniform_Def>(a.name.value));
				if (uit != uniforms.end())
				{
					uniform_expressions[a.name.value].push_back(a);
					break;
				}
				auto sit = std::find_if(samplers.begin(), samplers.end(), Match_Def_By_Name<Sampler_Def>(a.name.value));
				if (sit != samplers.end())
				{
					sampler_expressions[a.name.value].push_back(a);
					break;
				}
				auto ait = std::find_if(attributes.begin(), attributes.end(), Match_Def_By_Name<Attribute_Def>(a.name.value));
				if (ait != attributes.end())
				{
					attribute_expressions[a.name.value].push_back(a);
					break;
				}

				{
					auto pos = util::find_line_char_by_offset(src, t.value.start);
                    QLOGE("Expression '{}' cannot be found at {}", a.name.value, pos);
					return String::null;
				}
				break;
			}
		default:
			{
				auto pos = util::find_line_char_by_offset(src, t.value.start);
                QLOGE("Shader contains unrecognized token at {}", pos);
				return String::null;
			}
		}
	}

	//Renderer& renderer = *System::inst().get_renderer();

	String patched;
	//size_t lastPos = 0;

	std::string declaration;
	std::string functions;
	std::vector<std::pair<Shader_Source::Literal, std::string>> patch_items;

	functions += "// --- helper functions ---\n";
	declaration += "// --- uniforms, samplers and attributes ---\n";

	//how many nodes per vertex.
	//0 for no skinning
	size_t skin_npv = get_type() >= Shader::Type::SKINNING_1NPV && get_type() <= Shader::Type::SKINNING_4NPV ? ((size_t)get_type() - (size_t)Shader::Type::SKINNING_1NPV + 1) : 0;

	bool has_world_transform = false;
	size_t uniform_floats = 0;
	for (auto const& it: uniform_expressions)
	{
		String name = it.first;

		auto uit = std::find_if(uniforms.begin(), uniforms.end(), Match_Def_By_Name<Uniform_Def>(name));
		QASSERT(uit != uniforms.end());
		Uniform_Def& udef = *uit;

        uniform_floats += math::max<size_t>(udef.get_default_value().get_data_size(), 4) / sizeof(float);

		//char final_name[512];
		std::string final_name;
		if (udef.get_link() == Path("transform/world") && skin_npv > 0)
		{
			final_name = "(getSkinTransform())";
			has_world_transform = true;
		}
		else if (udef.get_link() == Path("transform/worldviewprojection") && skin_npv > 0)
		{
			util::format(final_name, "_u_{}_", name);
			declaration += "uniform ";
			declaration += get_shader_type_precision_str(udef);
			declaration += " ";
			declaration += get_shader_type_str(udef);
			declaration += " ";
			declaration += final_name;
			declaration += "; // auto declaration\n";

			util::format(final_name, "({} * getSkinTransform())", final_name);
			has_world_transform = true;
		}
		else
		{
			util::format(final_name, "_u_{}_", name);
			declaration += "uniform ";
			declaration += get_shader_type_precision_str(udef);
			declaration += " ";
			declaration += get_shader_type_str(udef);
			declaration += " ";
			declaration += final_name;
			if (udef.get_default_value().get_count() > 1)
			{
				std::string _s;
				util::format(_s, "[{}]", udef.get_default_value().get_count());
				declaration += _s;
			}
			declaration += "; // auto declaration\n";
		}

		//check for array/scalar compatibility
		bool expencting_array = udef.get_default_value().get_count() > 1;

		std::vector<Shader_Source::Expression> const& all_expressions = it.second;
		for (auto const& exp: all_expressions)
		{
			bool is_array = !exp.index.value.empty();
			if (expencting_array != is_array) //have to be the same
			{
				const char* x[] = {"array", "scalar"};
				auto pos = util::find_line_char_by_offset(src, exp.value.start);
                QLOGE("Uniform {} used as a {} but it's actually a {} at {}", exp.name.value, x[is_array ? 0 : 1], x[expencting_array ? 0 : 1], pos);
				return String::null;
			}

			if (is_array)
			{
				std::string final_name_array;
				util::format(final_name_array, "{}[{}]", final_name, exp.index.value);
				patch_items.emplace_back(exp.value, final_name_array);
			}
			else
			{
				patch_items.emplace_back(exp.value, final_name);
			}
		}
	}
	for (auto const& it: sampler_expressions)
	{
		String name = it.first;

		auto sit = std::find_if(samplers.begin(), samplers.end(), Match_Def_By_Name<Sampler_Def>(name));
		QASSERT(sit != samplers.end());
		Sampler_Def& sdef = *sit;

		std::string final_name;
		util::format(final_name, "_s_{}_", name);

		std::vector<Shader_Source::Expression> const& all_expressions = it.second;
		for (auto const& exp: all_expressions)
		{
			patch_items.emplace_back(exp.value, final_name);
		}

		declaration += "uniform ";
		if (sdef.get_default_value().get_type() == Sampler::Type::TEXTURE_2D)
		{
			declaration += "sampler2D ";
		}
		else
		{
			declaration += "samplerCube ";
		}

		declaration += final_name;
		declaration += "; // auto declaration\n";
	}
	for (auto const& it : attribute_expressions)
	{
		String name = it.first;

		auto ait = std::find_if(attributes.begin(), attributes.end(), Match_Def_By_Name<Attribute_Def>(name));
		QASSERT(ait != attributes.end());
		Attribute_Def& adef = *ait;

		std::string final_name;
		util::format(final_name, "_a_{}_", name);

		std::vector<Shader_Source::Expression> const& allExpressions = it.second;
		for (auto const& exp: allExpressions)
		{
			patch_items.emplace_back(exp.value, final_name);
		}

		declaration += "attribute ";
		if (adef.get_component_count() == 1)
		{
			declaration += "float ";
		}
		else if (adef.get_component_count() == 2)
		{
			declaration += "vec2 ";
		}
		else if (adef.get_component_count() == 3)
		{
			declaration += "vec3 ";
		}
		else if (adef.get_component_count() == 4)
		{
			declaration += "vec4 ";
		}
		declaration += final_name;
		declaration += "; //auto declaration\n";
	}


	//add skinning declarations
	if (skin_npv > 0 && has_world_transform)
	{
		declaration += "// skinning data\n";
		if (skin_npv == 1)
		{
			declaration += "attribute highp vec4 _a_skin_indices_;\n";
			functions +=
				"highp mat4 getSkinTransform()\n"\
				"{\n"\
				"	ivec4 indices = ivec4(3.0*_a_skin_indices_);\n"\
				"	highp vec4 m0, m1, m2;\n"\
				"	m0 = _u_skin_transforms_[indices.x ];\n"\
				"	m1 = _u_skin_transforms_[indices.x+1];\n"\
				"	m2 = _u_skin_transforms_[indices.x+2];\n"\
				"	return mat4(vec4(m0.x, m1.x, m2.x, 0.0), vec4(m0.y, m1.y, m2.y, 0.0), vec4(m0.z, m1.z, m2.z, 0.0), vec4(m0.w, m1.w, m2.w, 1.0));\n"\
				"}\n";
		}
		else if (skin_npv == 2)
		{
			declaration += "attribute highp vec4 _a_skin_indices_;\n";
			declaration += "attribute highp vec2 _a_skin_weights_;\n";
			functions +=
				"highp mat4 getSkinTransform()\n"\
				"{\n"\
				"	ivec4 indices = ivec4(3.0*_a_skin_indices_);\n"\
				"	highp vec4 m0, m1, m2;\n"\
				"	m0 = _u_skin_transforms_[indices.x ]*_a_skin_weights_.x;\n"\
				"	m1 = _u_skin_transforms_[indices.x+1]*_a_skin_weights_.x;\n"\
				"	m2 = _u_skin_transforms_[indices.x+2]*_a_skin_weights_.x;\n"\
				"	m0 += _u_skin_transforms_[indices.y ]*_a_skin_weights_.y;\n"\
				"	m1 += _u_skin_transforms_[indices.y+1]*_a_skin_weights_.y;\n"\
				"	m2 += _u_skin_transforms_[indices.y+2]*_a_skin_weights_.y;\n"\
				"	return mat4(vec4(m0.x, m1.x, m2.x, 0.0), vec4(m0.y, m1.y, m2.y, 0.0), vec4(m0.z, m1.z, m2.z, 0.0), vec4(m0.w, m1.w, m2.w, 1.0));\n"\
				"}\n";
		}
		else if (skin_npv == 3)
		{
			declaration += "attribute highp vec4 _a_skin_indices_;\n";
			declaration += "attribute highp vec3 _a_skin_weights_;\n";
			functions +=
				"highp mat4 getSkinTransform()\n"\
				"{\n"\
				"	ivec4 indices = ivec4(3.0*_a_skin_indices_);\n"\
				"	highp vec4 m0, m1, m2;\n"\
				"	m0 = _u_skin_transforms_[indices.x ]*_a_skin_weights_.x;\n"\
				"	m1 = _u_skin_transforms_[indices.x+1]*_a_skin_weights_.x;\n"\
				"	m2 = _u_skin_transforms_[indices.x+2]*_a_skin_weights_.x;\n"\
				"	m0 += _u_skin_transforms_[indices.y ]*_a_skin_weights_.y;\n"\
				"	m1 += _u_skin_transforms_[indices.y+1]*_a_skin_weights_.y;\n"\
				"	m2 += _u_skin_transforms_[indices.y+2]*_a_skin_weights_.y;\n"\
				"	m0 += _u_skin_transforms_[indices.z ]*_a_skin_weights_.z;\n"\
				"	m1 += _u_skin_transforms_[indices.z+1]*_a_skin_weights_.z;\n"\
				"	m2 += _u_skin_transforms_[indices.z+2]*_a_skin_weights_.z;\n"\
				"	return mat4(vec4(m0.x, m1.x, m2.x, 0.0), vec4(m0.y, m1.y, m2.y, 0.0), vec4(m0.z, m1.z, m2.z, 0.0), vec4(m0.w, m1.w, m2.w, 1.0));\n"\
				"}\n";

		}
		else if (skin_npv == 4)
		{
			declaration += "attribute highp vec4 _a_skin_indices_;\n";
			declaration += "attribute highp vec4 _a_skin_weights_;\n";
			functions +=
				"highp mat4 getSkinTransform()\n"\
				"{\n"\
				"	ivec4 indices = ivec4(3.0*_a_skin_indices_);\n"\
				"	highp vec4 m0, m1, m2;\n"\
				"	m0 = _u_skin_transforms_[indices.x ]*_a_skin_weights_.x;\n"\
				"	m1 = _u_skin_transforms_[indices.x+1]*_a_skin_weights_.x;\n"\
				"	m2 = _u_skin_transforms_[indices.x+2]*_a_skin_weights_.x;\n"\
				"	m0 += _u_skin_transforms_[indices.y ]*_a_skin_weights_.y;\n"\
				"	m1 += _u_skin_transforms_[indices.y+1]*_a_skin_weights_.y;\n"\
				"	m2 += _u_skin_transforms_[indices.y+2]*_a_skin_weights_.y;\n"\
				"	m0 += _u_skin_transforms_[indices.z ]*_a_skin_weights_.z;\n"\
				"	m1 += _u_skin_transforms_[indices.z+1]*_a_skin_weights_.z;\n"\
				"	m2 += _u_skin_transforms_[indices.z+2]*_a_skin_weights_.z;\n"\
				"	m0 += _u_skin_transforms_[indices.w ]*_a_skin_weights_.w;\n"\
				"	m1 += _u_skin_transforms_[indices.w+1]*_a_skin_weights_.w;\n"\
				"	m2 += _u_skin_transforms_[indices.w+2]*_a_skin_weights_.w;\n"\
				"	return mat4(vec4(m0.x, m1.x, m2.x, 0.0), vec4(m0.y, m1.y, m2.y, 0.0), vec4(m0.z, m1.z, m2.z, 0.0), vec4(m0.w, m1.w, m2.w, 1.0));\n"\
				"}\n";
		}

		m_max_skin_nodes = (m_max_uniforms - uniform_floats) / 16;//(9 + 3 + 9 + 9);
		//declaration += String::format("uniform highp mat3 _u_skin_rotations_[%d];\n", mMaxSkinNodes).c_str();
		std::string _s;
		util::format(_s, "uniform highp vec4 _u_skin_transforms_[{}];\n", m_max_skin_nodes);
		declaration += _s;

		{
			Attribute_Def adef("skin_indices");
			adef.set_component_count(4);
			adef.set_semantic(Vertex_Declaration::Semantic::NODE_INDICES);
			adef.set_attribute_name("nodeIndices");
			attributes.push_back(adef);
		}
		if (skin_npv > 1)
		{
			Attribute_Def adef("skin_weights");
			adef.set_component_count(skin_npv);
			adef.set_semantic(Vertex_Declaration::Semantic::NODE_WEIGHTS);
			adef.set_attribute_name("nodeWeights");
			attributes.push_back(adef);
		}

		{
			Uniform_Def udef("skin_transforms", String::null);
			udef.set_link(Path("skinning/transforms"));
			udef.set_getter(std::bind(&GLES_Shader::get_uniform_skin_matrix, this, std::placeholders::_1));
			uniforms.push_back(udef);
		}
	}

	//do the actual patching
	struct Patch_Item_Pred
	{
		bool operator()(std::pair<Shader_Source::Literal, std::string> const& p0, std::pair<Shader_Source::Literal, std::string> const& p1)
		{
			//sort inverse by position in file
			return p0.first.start > p1.first.start;
		}
	};

	std::sort(patch_items.begin(), patch_items.end(), Patch_Item_Pred());
	std::string patched_source = src.c_str();
	for (auto const& pi: patch_items)
	{
		size_t start = pi.first.start;
		size_t length = pi.first.length;
		std::string const& str = pi.second;

		patched_source = patched_source.substr(0, start) + str + patched_source.substr(start + length);
	}

	return String(declaration + functions + patched_source);
}

void GLES_Shader::compile(
	std::vector<Uniform_Def>& uniforms,
	std::vector<Sampler_Def>& samplers,
	std::vector<Attribute_Def>& attributes)
{
	m_uniforms = uniforms;
	m_samplers = samplers;
	m_attributes = attributes;

	m_patched_vertex_shader_src = patch_shader(m_uniforms, m_samplers, m_attributes, m_vertex_shader_src);
	m_patched_fragment_shader_src = patch_shader(m_uniforms, m_samplers, m_attributes, m_fragment_shader_src);
	if (m_patched_vertex_shader_src.empty() || m_patched_fragment_shader_src.empty())
	{
		return;
	}

	gles::Interface interf;

	m_vertex_shader_id = compile_shader(gles::iGL_VERTEX_SHADER, m_patched_vertex_shader_src);
	//if (!mVShaderId)
	{
        QLOGI("{}", util::get_line_annotated_string(m_patched_vertex_shader_src));
	}
	QASSERT(m_vertex_shader_id);

	m_fragment_shader_id = compile_shader(gles::iGL_FRAGMENT_SHADER, m_patched_fragment_shader_src);
	//if (!mFShaderId)
	{
        QLOGI("{}", util::get_line_annotated_string(m_patched_fragment_shader_src));
	}
	QASSERT(m_fragment_shader_id);

	m_shader_id = interf.iglCreateProgram();
	interf.iglAttachShader(m_shader_id, m_vertex_shader_id);
	interf.iglAttachShader(m_shader_id, m_fragment_shader_id);

	//link attributes
	for (size_t i = 0; i < m_attributes.size(); i++)
	{
		Attribute_Def& d = m_attributes[i];
		char final_name[256];
		sprintf(final_name, "_a_%s_", d.get_name().c_str());
		interf.iglBindAttribLocation(m_shader_id, i, final_name);
	}

	//link the Shader object
	interf.iglLinkProgram(m_shader_id);
	interf.iglValidateProgram(m_shader_id);
	//interf.iglUseProgram(mShaderId);

	gles::iGLint link_status;
	interf.iglGetProgramiv(m_shader_id, gles::iGL_LINK_STATUS, &link_status);
	if (!link_status)
	{
		int info_log_length, chars_written;
		interf.iglGetProgramiv(m_shader_id, gles::iGL_INFO_LOG_LENGTH, &info_log_length);

		std::vector<char> info_log(info_log_length);
		interf.iglGetProgramInfoLog(m_shader_id, info_log_length, &chars_written, info_log.data());
        QLOGE("{}", info_log.data());
	}

	interf.iglUseProgram(m_shader_id);

	m_uniform_gl_ids.resize(m_uniforms.size());
	for (size_t i = 0; i < m_uniforms.size(); i++)
	{
		Uniform_Def& d = m_uniforms[i];
		char final_name[256];
		sprintf(final_name, "_u_%s_", d.get_name().c_str());
		int uniform_id = interf.iglGetUniformLocation(m_shader_id, final_name);
		if (uniform_id >= 0)
		{
			upload_uniform(uniform_id, d.get_default_value(), d.get_default_value().get_data());
		}
		d.set_enabled(uniform_id >= 0);
		if (i < uniforms.size())
		{
			uniforms[i].set_enabled(d.is_enabled());
		}
		m_uniform_gl_ids[i] = uniform_id;
	}

	for (size_t i = 0; i < m_samplers.size(); i++)
	{
		Sampler_Def& d = m_samplers[i];
		char final_name[256];
		sprintf(final_name, "_s_%s_", d.get_name().c_str());
		int sampler_id = interf.iglGetUniformLocation(m_shader_id, final_name);
		if (sampler_id >= 0)
		{
			interf.iglUniform1i(sampler_id, i);
		}
		d.set_enabled(sampler_id >= 0);
		if (i < samplers.size())
		{
			samplers[i].set_enabled(d.is_enabled());
		}
	}

	for (size_t i = 0; i < m_attributes.size(); i++)
	{
		Attribute_Def& d = m_attributes[i];
		char final_name[256];
		sprintf(final_name, "_a_%s_", d.get_name().c_str());
		int attribute_id = interf.iglGetAttribLocation(m_shader_id, final_name);
		d.set_enabled(attribute_id >= 0);
		if (i < attributes.size())
		{
			attributes[i].set_enabled(d.is_enabled());
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void GLES_Shader::upload_uniform(uint32_t id, Uniform const& u, uint8_t const* data) const
{
	gles::Interface interf;
	switch (u.get_type())
	{
	case Uniform::Type::BOOL:
		{
			boost::auto_buffer<uint8_t*, boost::store_n_bytes<64>> buffer;
			buffer.uninitialized_resize(u.get_data_size());
			int* ibuf = (int*)buffer.data();
			for (size_t i = 0, sz = u.get_count(); i < sz; i++)
			{
				ibuf[i] = ((bool*)data)[i] ? 1 : 0;
			}
			interf.iglUniform1iv(id, u.get_count(), ibuf);
		}
		break;
	case Uniform::Type::INT:
		interf.iglUniform1iv(id, u.get_count(), (int const*)data);
		break;
	case Uniform::Type::FLOAT:
		interf.iglUniform1fv(id, u.get_count(), (float const*)data);
		break;
	case Uniform::Type::MAT3:
		interf.iglUniformMatrix3fv(id, u.get_count(), gles::iGL_FALSE, (float const*)data);
		break;
	case Uniform::Type::MAT4:
		interf.iglUniformMatrix4fv(id, u.get_count(), gles::iGL_FALSE, (float const*)data);
		break;
	case Uniform::Type::VEC2:
		interf.iglUniform2fv(id, u.get_count(), (float const*)data);
		break;
	case Uniform::Type::VEC3:
		interf.iglUniform3fv(id, u.get_count(), (float const*)data);
		break;
	case Uniform::Type::VEC4:
		interf.iglUniform4fv(id, u.get_count(), (float const*)data);
		break;
	}
}


uint32_t GLES_Shader::compile_shader(uint32_t shaderType, String const& source)
{
	gles::Interface interf;
	gles::iGLuint shader_id = interf.iglCreateShader(shaderType);
	QASSERT(shader_id);

	const char* defines = "";
#if defined Q_WINDOWS || defined Q_LINUX
	defines = "#define highp \n#define mediump \n#define lowp \n";
#endif

	const char* buffers[2] = { defines, source.c_str() };
    interf.iglShaderSource(shader_id, 2, buffers, nullptr);
	interf.iglCompileShader(shader_id);

	gles::iGLint is_compiled;
	interf.iglGetShaderiv(shader_id, gles::iGL_COMPILE_STATUS, &is_compiled);
	if (!is_compiled)
	{
		int info_log_length, chars_written;
		interf.iglGetShaderiv(shader_id, gles::iGL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> info_log(info_log_length);
		interf.iglGetShaderInfoLog(shader_id, info_log_length, &chars_written, info_log.data());

        QLOGE("While compiling shader:\n{}\n\n{}", util::get_line_annotated_string(source), info_log.data());

		interf.iglDeleteShader(shader_id);

		return 0;
	}

	return shader_id;
}

void GLES_Shader::upload_data(gles::Interface& interf, const Command_Stream::Render_Context& context) const
{
	Material const& material = *context.material;
	Technique_cptr const& technique = material.get_technique();
	size_t pass_idx = context.pass_idx;

	QASSERT(technique && pass_idx < technique->get_pass_count());
	Pass const& pass = technique->get_pass(pass_idx);

	//GLESRenderer* renderer = static_cast<GLESRenderer*>(System::inst().getRenderer().get());

	interf.iglUseProgram(m_shader_id);

	//now the user uniforms
	auto count = m_uniforms.size();
	for (size_t i = 0; i < count; i++)
	{
		const Uniform_Def& def = m_uniforms[i];
		if (def.is_enabled())
		{
			if (def.has_getter())
			{
				const Uniform& u = def.get_value((void*)&context);
				upload_uniform(m_uniform_gl_ids[i], u, u.get_data());
			}
			else
			{
				upload_uniform(m_uniform_gl_ids[i], def.get_default_value(), material.get_uniform_data(pass_idx, pass.get_material_uniform_idx_from_uniform_idx(i)));
			}
		}
	}

	//user samplers
	count = m_samplers.size();
	for (size_t i = 0; i < count; i++)
	{
		const Sampler_Def& def = m_samplers[i];
		if (def.is_enabled())
		{
			if (def.has_getter())
			{
				Sampler const& s = def.get_value((void*)&context);
				GLES_Texture const* texture = static_cast<GLES_Texture const*>(s.get_texture().get());
				if (texture)
				{
					texture->bind(s, i/*pass.getSamplerCount() + i*/);
				}
			}
			else
			{
				Sampler const& sampler = material.get_sampler(pass_idx, i);
				GLES_Texture const* texture = static_cast<GLES_Texture const*>(sampler.get_texture().get());
				if (texture)
				{
					texture->bind(sampler, i);
				}
			}
		}
	}
}

Uniform const& GLES_Shader::get_uniform_skin_matrix(void* data) const
{
	Command_Stream::Render_Context* ctx = (Command_Stream::Render_Context*)data;
	static Uniform u(Uniform::Type::VEC4, 1);
	if (u.get_count() == 1)
	{
		u = Uniform(Uniform::Type::VEC4, m_max_skin_nodes * 3);
	}
	if (ctx)
	{
		QASSERT(ctx->render_job_data->node_transform_count < m_max_skin_nodes);
		size_t count = math::min(ctx->render_job_data->node_transform_count, m_max_skin_nodes);
		if (u.get_count() == count * 3)
		{
			u = Uniform(Uniform::Type::VEC4, count * 3);
		}
		for (size_t i = 0; i < count; i++)
		{
			auto const& mat = ctx->render_job_data->node_transforms[i].mat;
			u.set(i*3 + 0, mat.get_row(0));
			u.set(i*3 + 1, mat.get_row(1));
			u.set(i*3 + 2, mat.get_row(2));
		}
	}
	return u;
}


