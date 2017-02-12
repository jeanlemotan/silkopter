#include "QStdAfx.h"
#include "video/GLES/GLES_Command_Stream.h"
#include "video/GLES/GLES_Interface.h"
#include "video/GLES/GLES_Fbo_Wrapper.h"
#include "video/GLES/GLES_Shader.h"
#include "video/GLES/GLES_Index_Buffer.h"
#include "video/GLES/GLES_Vertex_Buffer.h"
#include "video/GLES/GLES_Renderer.h"
#include "video/Render_Job.h"
#include "system/System.h"
#include "Profiler.h"

using namespace q;
using namespace video;

static const int GLTopology[] =
{
	gles::iGL_LINES,
	gles::iGL_LINE_LOOP,
	gles::iGL_LINE_STRIP,
	gles::iGL_TRIANGLES,
	gles::iGL_TRIANGLE_STRIP,
	gles::iGL_TRIANGLE_FAN
};

static const gles::iGLuint GLCmpFuncs[] =
{
	gles::iGL_LESS,
	gles::iGL_LEQUAL,
	gles::iGL_EQUAL,
	gles::iGL_GEQUAL,
	gles::iGL_GREATER,
	gles::iGL_NOTEQUAL,
	gles::iGL_ALWAYS,
	gles::iGL_NEVER
};

static const gles::iGLuint GLStencilFuncs[] =
{
	gles::iGL_KEEP,
	gles::iGL_ZERO,
	gles::iGL_REPLACE,
	gles::iGL_INVERT,
	gles::iGL_INCR,
	gles::iGL_DECR,
	gles::iGL_INCR_WRAP,
	gles::iGL_DECR_WRAP
};

static const gles::iGLuint GLBlendFactors[] =
{
	gles::iGL_ZERO,
	gles::iGL_ONE,
	gles::iGL_SRC_COLOR,
	gles::iGL_ONE_MINUS_SRC_COLOR,
	gles::iGL_DST_COLOR,
	gles::iGL_ONE_MINUS_DST_COLOR,
	gles::iGL_SRC_ALPHA,
	gles::iGL_ONE_MINUS_SRC_ALPHA,
	gles::iGL_DST_ALPHA,
	gles::iGL_ONE_MINUS_DST_ALPHA,
	gles::iGL_SRC_ALPHA_SATURATE
};
static const gles::iGLuint GLBlendEquations[] =
{
	gles::iGL_FUNC_ADD,
	gles::iGL_FUNC_SUBTRACT
};
static const gles::iGLuint GLCullFaces[] =
{
	gles::iGL_BACK,
	gles::iGL_FRONT
};
static const gles::iGLuint GLWindings[] =
{
	gles::iGL_CW,
	gles::iGL_CCW
};

Command_Stream::Command_Stream()
	: m_sink(512000)
{
	m_sink.seek(0);

	m_last_timestamp = Clock::now();
	m_time = 0.f;

	m_material_count = 0;
}

//////////////////////////////////////////////////////////////////////////

Uniform const& Command_Stream::get_uniform_matrix_w(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::MAT4, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::MAT4, 1, (uint8_t*)&ctx->render_job_data->world_transform);
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_matrix_v(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::MAT4, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::MAT4, 1, (uint8_t*)&ctx->camera->get_view_matrix());
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_matrix_p(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::MAT4, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::MAT4, 1, (uint8_t*)&ctx->camera->get_projection_matrix());
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_matrix_wvp(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::MAT4, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::MAT4, 1);
	math::mat4f trans;
	math::multiply(trans, ctx->camera->get_view_projection_matrix(), ctx->render_job_data->world_transform.mat);
	ctx->_this->m_uniform = trans;
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_matrix_vp(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::MAT4, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::MAT4, 1, (uint8_t*)&ctx->camera->get_view_projection_matrix());
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_matrix_wvp_i(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::MAT4, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::MAT4, 1);
	math::mat4f mvp;
	math::multiply(mvp, ctx->camera->get_view_projection_matrix(), ctx->render_job_data->world_transform.mat);
	ctx->_this->m_uniform = math::inverse(mvp);
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_viewport_size(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::VEC2, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::VEC2, 1);
	Viewport vp = ctx->camera->get_viewport();
	ctx->_this->m_uniform = math::vec2f(vp.size);
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_camera_near(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::FLOAT, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::FLOAT, 1);
	ctx->_this->m_uniform = ctx->camera->get_near_distance();
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_camera_far(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::FLOAT, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::FLOAT, 1);
	ctx->_this->m_uniform = ctx->camera->get_far_distance();
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_camera_range(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::FLOAT, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::FLOAT, 1);
	ctx->_this->m_uniform = ctx->camera->get_far_distance() - ctx->camera->get_near_distance();
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_camera_range_packed(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::VEC4, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	float cnear = ctx->camera->get_near_distance();
	float cfar = ctx->camera->get_far_distance();
	float crange = cfar - cnear;
	float cinvrange = math::is_zero(crange) ? 1.f : 1.f / crange;

	ctx->_this->m_uniform = Uniform(Uniform::Type::VEC4, 1);
	ctx->_this->m_uniform = math::vec4f(cnear, cfar, crange, cinvrange);
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_camera_position(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::VEC3, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::VEC3, 1);
	ctx->_this->m_uniform = ctx->camera->get_position();
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_camera_front(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::VEC3, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::VEC3, 1);
	ctx->_this->m_uniform = ctx->camera->get_front_vector();
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_camera_right(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::VEC3, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::VEC3, 1);
	ctx->_this->m_uniform = ctx->camera->get_right_vector();
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_camera_up(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::VEC3, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::VEC3, 1);
	ctx->_this->m_uniform = ctx->camera->get_up_vector();
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_time(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::FLOAT, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	auto time = Clock::now();
	auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(time - ctx->_this->m_last_timestamp).count();
	ctx->_this->m_last_timestamp = time;
	ctx->_this->m_time += (float)diff * 0.001f;

	ctx->_this->m_uniform = Uniform(Uniform::Type::FLOAT, 1);
	ctx->_this->m_uniform = ctx->_this->m_time;
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_fract_time(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::FLOAT, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::FLOAT, 1);
	ctx->_this->m_uniform = math::fract(get_uniform_time(data).get_as_float());
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_screen_size(void* data)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::VEC2, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;

	ctx->_this->m_uniform = Uniform(Uniform::Type::VEC2, 1);
	ctx->_this->m_uniform = math::vec2f(ctx->renderer->get_viewport_size());
	return ctx->_this->m_uniform;
}
Uniform const& Command_Stream::get_uniform_render_target_size(void* data, std::string const& name)
{
	if (!data)
	{
		static Uniform u(Uniform::Type::VEC2, 1);
		return u;
	}
	Render_Context* ctx = (Render_Context*)data;
	Render_Target_ptr rt = ctx->renderer->find_render_target_by_name(name);

	ctx->_this->m_uniform = Uniform(Uniform::Type::VEC2, 1);
	if (rt)
	{
		ctx->_this->m_uniform = math::vec2f(rt->get_size());
	}
	else
	{
		ctx->_this->m_uniform = math::vec2f(0.f, 0.f);
	}
	return ctx->_this->m_uniform;
}

//////////////////////////////////////////////////////////////////////////

Sampler const& Command_Stream::get_sampler_screen_color_buffer(void* data)
{
	if (!data)
	{
		static Sampler s;
		return s;
	}
	Render_Context* ctx = (Render_Context*)data;
 	Texture_cptr tex = ctx->screen_render_target->get_color_texture();
 	ctx->_this->m_sampler.set_texture(tex);
 	ctx->_this->m_sampler.set_wrapping(Sampler::Wrap::CLAMP, Sampler::Wrap::CLAMP);
 	ctx->_this->m_sampler.set_mipmapping(false);
	return ctx->_this->m_sampler;
}
Sampler const& Command_Stream::get_sampler_screen_depth_buffer(void* data)
{
	if (!data)
	{
		static Sampler s;
		return s;
	}
	Render_Context* ctx = (Render_Context*)data;
	QASSERT(0);
// 	Texture_cptr tex = ctx->display->getPostFxRenderTarget(0)->getDepthTexture();
// 	ctx->_this->mSampler.setTexture(tex);
// 	ctx->_this->mSampler.setWrapping(Sampler::Wrap::CLAMP, Sampler::Wrap::CLAMP);
// 	ctx->_this->mSampler.setFiltering(Sampler::Filter::NEAREST);
// 	ctx->_this->mSampler.setMipmapping(false);
	return ctx->_this->m_sampler;
}

Sampler const& Command_Stream::get_sampler_render_target_color_buffer(void* data, std::string const& name)
{
	if (!data)
	{
		static Sampler s;
		return s;
	}
	Render_Context* ctx = (Render_Context*)data;
	Render_Target_ptr rt = ctx->renderer->find_render_target_by_name(name);
	Texture_cptr tex = rt ? rt->get_color_texture() : Texture_cptr();
	ctx->_this->m_sampler.set_texture(tex);
	ctx->_this->m_sampler.set_wrapping(Sampler::Wrap::CLAMP, Sampler::Wrap::CLAMP);
	ctx->_this->m_sampler.set_mipmapping(false);
	return ctx->_this->m_sampler;
}

Sampler const& Command_Stream::get_sampler_render_target_depth_buffer(void* data, std::string const& name)
{
	if (!data)
	{
		static Sampler s;
		return s;
	}
	Render_Context* ctx = (Render_Context*)data;
	Render_Target_ptr rt = ctx->renderer->find_render_target_by_name(name);
	Texture_cptr tex = rt ? rt->get_color_texture() : Texture_cptr();
	ctx->_this->m_sampler.set_texture(tex);
	ctx->_this->m_sampler.set_wrapping(Sampler::Wrap::CLAMP, Sampler::Wrap::CLAMP);
	ctx->_this->m_sampler.set_filtering(Sampler::Filter::NEAREST);
	ctx->_this->m_sampler.set_mipmapping(false);
	return ctx->_this->m_sampler;
}

void Command_Stream::apply_render_state(gles::Interface& interf, Render_State const& rs)
{
	Render_State::Blend_Formula f = rs.get_blend_formula();

	if (f.src_color != Render_State::Blend_Formula::Factor::ONE || f.dst_color != Render_State::Blend_Formula::Factor::ZERO ||
		f.src_alpha != Render_State::Blend_Formula::Factor::ONE || f.dst_alpha != Render_State::Blend_Formula::Factor::ZERO)
	{
		interf.iglEnable(gles::iGL_BLEND);
		interf.iglBlendFuncSeparate(GLBlendFactors[(uint32_t)f.src_color]
		, GLBlendFactors[(uint32_t)f.dst_color]
		, GLBlendFactors[(uint32_t)f.src_alpha]
		, GLBlendFactors[(uint32_t)f.dst_alpha]);
		interf.iglBlendEquation(GLBlendEquations[(uint32_t)f.operation]);
	}
	else
	{
		interf.iglDisable(gles::iGL_BLEND);
	}

	bool cull = rs.get_culling();
	if (cull)
	{
		interf.iglEnable(gles::iGL_CULL_FACE);
		interf.iglCullFace(GLCullFaces[(uint32_t)rs.get_cull_face()]);
	}
	else
	{
		interf.iglDisable(gles::iGL_CULL_FACE);
	}

	interf.iglFrontFace(GLWindings[(uint32_t)rs.get_winding()]);

	bool depthTest = rs.get_depth_test();
	if (depthTest)
	{
		interf.iglEnable(gles::iGL_DEPTH_TEST);
		interf.iglDepthFunc(GLCmpFuncs[(uint32_t)rs.get_depth_func()]);
	}
	else
	{
		interf.iglDisable(gles::iGL_DEPTH_TEST);
	}

	bool depthWrite = rs.get_depth_write();
	if (depthWrite)
	{
		interf.iglDepthMask(gles::iGL_TRUE);
	}
	else
	{
		interf.iglDepthMask(gles::iGL_FALSE);
	}

	bool stencil = rs.get_stencil();
	if (stencil)
	{
		interf.iglEnable(gles::iGL_STENCIL_TEST);
		interf.iglStencilFunc(GLCmpFuncs[(uint32_t)rs.get_stencil_func()], rs.get_stencil_ref(), rs.get_stencil_mask());
		interf.iglStencilOpSeparate(gles::iGL_FRONT, GLStencilFuncs[(uint32_t)rs.get_stencil_front_s_fail_op()]
		, GLStencilFuncs[(uint32_t)rs.get_stencil_front_z_fail_op()]
		, GLStencilFuncs[(uint32_t)rs.get_stencil_front_pass_op()]);
		interf.iglStencilOpSeparate(gles::iGL_FRONT, GLStencilFuncs[(uint32_t)rs.get_stencil_back_s_fail_op()]
		, GLStencilFuncs[(uint32_t)rs.get_stencil_back_z_fail_op()]
		, GLStencilFuncs[(uint32_t)rs.get_stencil_back_pass_op()] );
	}
	else
	{
		interf.iglDisable(gles::iGL_STENCIL_TEST);
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

Command_Stream::Render_Target_Handle Command_Stream::add_render_target(std::shared_ptr<GLES_Fbo_Wrapper> const& renderTarget)
{
	PROFILE_SCOPED();

	Render_Target_Data rt;
	rt.render_target = renderTarget;
	rt.clear_color = renderTarget->get_color_clear_value();
	rt.clear_depth = renderTarget->get_depth_clear_value();
	m_render_targets.push_back(rt);
	//target->lock(this);
	return Render_Target_Handle(m_render_targets.size() - 1);
}
Command_Stream::Camera_Handle Command_Stream::add_camera(scene::Camera const& camera)
{
	PROFILE_SCOPED();

	m_cameras.push_back(camera);
	return Camera_Handle(m_cameras.size() - 1);
}
Command_Stream::Material_Handle Command_Stream::add_material(Material const& material)
{
	PROFILE_SCOPED();

	if (m_material_count >= m_materials.size())
	{
		m_materials.resize((m_material_count + 1) * 2);
	}
	m_materials[m_material_count] = material;
	m_material_count++;

	return Material_Handle(m_material_count - 1);
}
Material const& Command_Stream::get_material(Material_Handle mh) const
{
	QASSERT(mh.id < m_material_count);
	return m_materials[mh.id];
}
Command_Stream::Render_Job_Handle Command_Stream::add_render_job(Render_Job const& render_job)
{
	PROFILE_SCOPED();

	Render_Job_Data data;

	data.shader_type = render_job.get_shader_type();
	data.bounds = render_job.get_bounds();
	data.world_transform = render_job.get_world_transform();
	data.node_transform_count = render_job.get_node_transform_count();
	if (data.node_transform_count > 0)
	{
		data.node_transform_start_idx = m_node_transforms.size();
		for (size_t i = 0; i < data.node_transform_count; i++)
		{
			m_node_transforms.emplace_back(render_job.get_node_transform(i));
		}
	}

	m_render_jobs.push_back(std::move(data));
	return Render_Job_Handle(m_render_jobs.size() - 1);
}
Command_Stream::Vertex_Declaration_Handle Command_Stream::add_vertex_declaration(Vertex_Declaration const& declaration)
{
	PROFILE_SCOPED();

	//lock the buffers
	size_t frameIdx = System::inst().get_renderer()->get_frame_idx();
	for (size_t i = 0; i < declaration.get_vertex_buffer_count(); i++)
	{
		GLES_Vertex_Buffer* vb = (GLES_Vertex_Buffer*)declaration.get_vertex_buffer(i).get();
		if (vb)
		{
			vb->get_buffer().set_last_rendered_frame_idx(frameIdx);
		}
	}
	GLES_Index_Buffer* ib = (GLES_Index_Buffer*)declaration.get_index_buffer().get();
	if (ib)
	{
		ib->get_buffer().set_last_rendered_frame_idx(frameIdx);
	}

	//add it
	m_vertex_declarations.emplace_back(declaration);
	return Vertex_Declaration_Handle(m_vertex_declarations.size() - 1);
}

void Command_Stream::do_clear_fbo(Render_Target_Handle renderTarget, bool color, bool depth, bool stencil)
{
	m_sink << Command::CLEAR_FBO;
	m_sink << renderTarget.id;
	uint32_t bits = (color ? gles::iGL_COLOR_BUFFER_BIT : 0) | (depth ? gles::iGL_DEPTH_BUFFER_BIT : 0) | (stencil ? gles::iGL_STENCIL_BUFFER_BIT : 0);
	m_sink << bits;
}

void Command_Stream::do_render(Render_Target_Handle screenRenderTarget, Render_Target_Handle renderTarget, Vertex_Declaration_Handle declaration, Render_Job_Handle job, Material_Handle material, Camera_Handle camera, size_t passIdx)
{
	m_sink << Command::RENDER;
	m_sink << screenRenderTarget.id;
	m_sink << renderTarget.id;
	m_sink << declaration.id;
	m_sink << job.id;
	m_sink << material.id;
	m_sink << camera.id;
	m_sink << passIdx;
}

void Command_Stream::reset()
{
	PROFILE_SCOPED();

	//mNodeTransforms.resize(0);
	m_node_transforms.clear();

	//mCameras.resize(0);
	m_cameras.clear();

	for (size_t i = 0; i < m_material_count; i++)
	{
		m_materials[i].set_technique(Technique_ptr());
	}
	m_material_count = 0;

	//mMaterials.resize(0);
	//mRenderJobs.resize(0);
	m_render_jobs.clear();
//	for (auto const& rt: m_render_targets)
	{
		//		rt->unlock(this);
	}
	//mRenderTargets.resize(0);
	m_render_targets.clear();
	//mGeometries.resize(0);
	m_vertex_declarations.clear();

	m_sink.seek(0);
}

void Command_Stream::execute()
{
	PROFILE_SCOPED();

	m_source.swap_data(m_sink);
	m_source.seek(0);
	while (m_source.get_offset() < m_source.get_size())
	{
		uint32_t command;
		m_source >> command;
		switch ((Command)command)
		{
		case Command::RENDER: execute_render_command(m_source); break;
		case Command::CLEAR_FBO: execute_clear_fbo_command(m_source); break;
		default:
            QLOGE( "Invalid command: {}", command);
			m_source.seek(m_source.get_size() - 1);
			break;
		}
	}

	//int64_t start = System::inst().getUTime();

	m_sink.swap_data(m_source);
	m_sink.seek(0);

	reset();

	//int64_t end = System::inst().getUTime();
	//print("\n Time to execute: {}", (uint32_t)(end - start));

	//25856: 7300 - 7400
}

void Command_Stream::execute_render_command(data::Source& source)
{
	PROFILE_SCOPED();

	gles::Interface interf;
	Render_Context context;

	context._this = this;
	context.renderer = static_cast<GLES_Renderer*>(System::inst().get_renderer().get());

	uint32_t id;
	source >> id;
	Render_Target_Data& screenRenderTargetData = m_render_targets[id];
	context.screen_render_target = screenRenderTargetData.render_target.get();

	source >> id;
	Render_Target_Data& renderTargetData = m_render_targets[id];
	context.render_target = renderTargetData.render_target.get();

	source >> id;
	context.vertex_declaration = &m_vertex_declarations[id];

	source >> id;
	context.render_job_data = &m_render_jobs[id];

	source >> id;
	context.material = &m_materials[id];

	source >> id;
	context.camera = &m_cameras[id];

	source >> context.pass_idx;

	//Set the pointer to the node transforms here.
	//Don't do it earlier as the mNodeTransforms vector might resize itself and invalidate cached pointers.
	//At this point I know for sure the vector is good
	if (context.render_job_data->node_transform_count > 0)
	{
		context.render_job_data->node_transforms = &m_node_transforms[context.render_job_data->node_transform_start_idx];
	}
	else
	{
		context.render_job_data->node_transforms = nullptr;
	}

	Material const& material = *context.material;
	Technique const& technique = *material.get_technique();

	Pass const& pass = technique.get_pass(context.pass_idx);

	context.render_target->bind(interf, renderTargetData.clear_color, renderTargetData.clear_depth);

	Viewport const& vp = context.camera->get_viewport();
	interf.iglViewport(vp.position.x, vp.position.y, vp.size.x, vp.size.y);

	Uber_Shader const& uberShader = pass.get_uber_shader();
	context.shader = reinterpret_cast<GLES_Shader*>(uberShader.get_shader(material, context.render_job_data->shader_type).get());

	apply_render_state(interf, material.get_render_state(context.pass_idx));

	context.shader->upload_data(interf, context);
	render(interf, context);

	//size_t pc = context.renderJobData->primitiveCount;
// 	mPrimitiveCount += pc;
// 	if (job.getTopology() == RenderJob::Topology::LINE_LIST || job.getTopology() == RenderJob::Topology::LINE_LOOP || job.getTopology() == RenderJob::Topology::LINE_STRIP)
// 	{
// 		mLineCount += pc;
// 	}
// 	else
// 	{
// 		mTriangleCount += pc;
// 	}
// 	mVertexCount += geometry->getVertexCount();
// 	mDrawCalls++;
}

void Command_Stream::execute_clear_fbo_command(data::Source& source)
{
	PROFILE_SCOPED();

	gles::Interface interf;

	uint32_t id;
	source >> id;
	Render_Target_Data& renderTargetData = m_render_targets[id];
	uint32_t bits;
	source >> bits;

	renderTargetData.render_target->clear(interf, renderTargetData.clear_color, renderTargetData.clear_depth, bits);
}

void Command_Stream::render(gles::Interface& interf, Render_Context const& context) const
{
	PROFILE_SCOPED();

	if (!bind_attributes(interf, context))
	{
        QLOGE("No attributes binded.");
		return;
	}

	{
		auto t = context.vertex_declaration->get_topology();

		size_t start = context.vertex_declaration->get_range_start();
		size_t count = context.vertex_declaration->get_range_count();
		size_t index_count = context.vertex_declaration->get_index_count();
		size_t max_count = index_count ? index_count : context.vertex_declaration->get_vertex_count();
		if (start == 0 && count == 0)
		{
			count = max_count;
		}
		if (start + count > max_count)
		{
            QLOGE("RenderJob range ({} - {}) is out of range ({})", start, count, max_count);
			return;
		}

		int glt = GLTopology[(int)t];

		if (index_count)
		{
			GLES_Index_Buffer* buffer = (GLES_Index_Buffer*)(context.vertex_declaration->get_index_buffer().get());
			QASSERT(buffer);
			QASSERT(!buffer->is_locked_for_reading() && !buffer->is_locked_for_writing());

			QASSERT(buffer->get_buffer().get_gl_id());
			interf.iglBindBuffer(gles::iGL_ELEMENT_ARRAY_BUFFER, buffer->get_buffer().get_gl_id());
			interf.iglDrawElements(glt, count, gles::iGL_UNSIGNED_SHORT, (void*)(size_t)(start*2));
		}
		else
		{
			interf.iglDrawArrays(glt, start, count);
		}
	}

	unbind_attributes(interf);
}

bool Command_Stream::bind_attributes(gles::Interface& interf, Command_Stream::Render_Context const& context) const
{
	PROFILE_SCOPED();

	Material const& material = *context.material;
	Technique const& technique = *material.get_technique();
	GLES_Shader const& shader = *context.shader;
	Vertex_Declaration const& declaration = *context.vertex_declaration;

	//GLESRenderer* renderer = static_cast<GLESRenderer*>(System::inst().getRenderer().get());

	size_t ac = shader.get_attributes().size();//pass.getAttributeCount();
	m_active_attributes.clear();
	for (size_t i = 0; i < ac; i++)
	{
		Attribute_Def const& def = shader.get_attributes()[i];//pass.getAttribute(i);
		if (!def.is_enabled())
		{
			continue;
		}

		Vertex_Declaration::Semantic semantic = def.get_semantic();

		//TODO cache the index to avoid the find
		int attIdx = -1;
		if (semantic == Vertex_Declaration::Semantic::USER)
		{
            std::string attName = def.get_attribute_name();
			attIdx = declaration.find_attribute_idx_by_name(attName);
		}
		else
		{
			attIdx = declaration.find_attribute_idx_by_semantic(semantic);
			if (semantic > Vertex_Declaration::Semantic::TEX_COORDS0 && semantic <= Vertex_Declaration::Semantic::TEX_COORDS3)
			{
				if (attIdx < 0)
				{
					attIdx = declaration.find_attribute_idx_by_semantic(Vertex_Declaration::Semantic::TEX_COORDS0);
				}
			}
		}

		if (attIdx >= 0)
		{
			Vertex_Declaration::Attribute const& a = declaration.get_attribute(attIdx);
			GLES_Vertex_Buffer* buffer = (GLES_Vertex_Buffer*)(declaration.get_vertex_buffer(a.buffer_idx).get());
			QASSERT(buffer);
			QASSERT(!buffer->is_locked_for_reading() && !buffer->is_locked_for_writing());

			QASSERT(a.count == def.get_component_count());

			QASSERT(buffer->get_buffer().get_gl_id());
			interf.iglBindBuffer(gles::iGL_ARRAY_BUFFER, buffer->get_buffer().get_gl_id());

			switch (a.type)
			{
			case Vertex_Declaration::Type::FLOAT:
				interf.iglVertexAttribPointer(i, a.count, gles::iGL_FLOAT, gles::iGL_FALSE, a.stride, (void*)(size_t)a.offset);
				break;
			case Vertex_Declaration::Type::FP_U8:
				interf.iglVertexAttribPointer(i, a.count, gles::iGL_UNSIGNED_BYTE, gles::iGL_TRUE, a.stride, (void*)(size_t)a.offset);
				break;
			case Vertex_Declaration::Type::FP_S8:
				interf.iglVertexAttribPointer(i, a.count, gles::iGL_BYTE, gles::iGL_TRUE, a.stride, (void*)(size_t)a.offset);
				break;
			case Vertex_Declaration::Type::FP_S16:
				interf.iglVertexAttribPointer(i, a.count, gles::iGL_SHORT, gles::iGL_TRUE, a.stride, (void*)(size_t)a.offset);
				break;
			case Vertex_Declaration::Type::FP_U16:
				interf.iglVertexAttribPointer(i, a.count, gles::iGL_UNSIGNED_SHORT, gles::iGL_TRUE, a.stride, (void*)(size_t)a.offset);
				break;
			case Vertex_Declaration::Type::U8:
				interf.iglVertexAttribPointer(i, a.count, gles::iGL_UNSIGNED_BYTE, gles::iGL_FALSE, a.stride, (void*)(size_t)a.offset);
				break;
			case Vertex_Declaration::Type::S8:
				interf.iglVertexAttribPointer(i, a.count, gles::iGL_BYTE, gles::iGL_FALSE, a.stride, (void*)(size_t)a.offset);
				break;
			case Vertex_Declaration::Type::S16:
				interf.iglVertexAttribPointer(i, a.count, gles::iGL_SHORT, gles::iGL_FALSE, a.stride, (void*)(size_t)a.offset);
				break;
			case Vertex_Declaration::Type::U16:
				interf.iglVertexAttribPointer(i, a.count, gles::iGL_UNSIGNED_SHORT, gles::iGL_FALSE, a.stride, (void*)(size_t)a.offset);
				break;
			}
			m_active_attributes.push_back(i);
		}
		else
		{
            QLOGW("Shader '{}' is using attribute '{}' that doesn't exist in the vertex declaration!!!",
				technique.get_path(), def.get_name());
		}
	}

	for(auto aa: m_active_attributes)
	{
		interf.iglEnableVertexAttribArray(aa);
	}
	return !m_active_attributes.empty();
}

void Command_Stream::unbind_attributes(gles::Interface& interf) const
{
	PROFILE_SCOPED();

	for(auto aa: m_active_attributes)
	{
		interf.iglDisableVertexAttribArray(aa);
	}
	m_active_attributes.clear();
}



