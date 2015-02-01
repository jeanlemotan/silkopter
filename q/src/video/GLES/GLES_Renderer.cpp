#include "QStdAfx.h"
#include "video/GLES/GLES_Renderer.h"
#include "video/GLES/GLES_Vertex_Buffer.h"
#include "video/GLES/GLES_Index_Buffer.h"
#include "video/GLES/GLES_Uniform_Buffer.h"
#include "video/GLES/GLES_Render_Target.h"
#include "video/GLES/GLES_Texture.h"
#include "video/GLES/GLES_Shader.h"
#include "video/GLES/GLES_Command_Stream.h"

#include "video/GLES/GLES_Interface.h"
#include "Profiler.h"


using namespace q;
using namespace video;


GLES_Renderer::GLES_Renderer()
{
	m_render_buckets.resize((size_t)Bucket_Type::COUNT);

	m_last_timestamp = Clock::now();

	m_command_streams[0] = new Command_Stream;
	m_command_streams[1] = new Command_Stream;
}
GLES_Renderer::~GLES_Renderer()
{
	m_post_fx_data.effects.clear();
	m_post_fx_data.old_render_target.reset();
	for (size_t i = 0; i < m_post_fx_data.render_targets.size(); i++)
	{
		m_post_fx_data.render_targets[i].reset();
	}
	m_post_fx_data.declaration = Vertex_Declaration();
}

void GLES_Renderer::init(math::vec2u32 const& size,
						Render_Target::Color_Format color_format,
						Render_Target::Depth_Format depth_format,
						Render_Target::Stencil_Format stencil_format)
{
	QASSERT(!m_frame_begun);

	gles::set_context(this);
	init_capabilities();

	m_default_render_target = create_default_render_target(size, color_format, depth_format, stencil_format);
	if (!m_default_render_target)
	{
        QLOGE("Cannot create default render target. Probably bad format.");
		return;
	}

	if (m_post_fx_data.declaration.get_attribute_count() == 0)
	{
		auto vertexBuffer = Vertex_Buffer::create();
		vertexBuffer->allocate(0, Buffer::Usage::STATIC);

		//auto indexBuffer = IndexBuffer::create();
		//indexBuffer->allocate(0, Buffer::Usage::STATIC);

		Vertex_Declaration declaration;

		declaration.set_topology(Vertex_Declaration::Topology::TRIANGLE_STRIP);

		auto posIdx = declaration.add_attribute(vertexBuffer, Vertex_Declaration::Semantic::POSITIONS, Vertex_Declaration::Type::FLOAT, 2, 0, 16);
		QASSERT(posIdx >= 0);
		auto tcIdx = declaration.add_attribute(vertexBuffer, Vertex_Declaration::Semantic::TEX_COORDS0, Vertex_Declaration::Type::FLOAT, 2, 8, 16);
		QASSERT(tcIdx >= 0);
		declaration.set_vertex_count(4);

		auto pos_ptr = declaration.lock_for_writing<math::vec2f>(posIdx);
		QASSERT(pos_ptr.is_valid());

		*pos_ptr++ = math::vec2f(-1, -1);
		*pos_ptr++ = math::vec2f(1, -1);
		*pos_ptr++ = math::vec2f(-1, 1);
		*pos_ptr++ = math::vec2f(1, 1);

		auto tc_ptr = declaration.lock_for_writing<math::vec2f>(tcIdx);
		QASSERT(tc_ptr.is_valid());

		*tc_ptr++ = math::vec2f(0.f, 0.f);
		*tc_ptr++ = math::vec2f(1.f, 0.f);
		*tc_ptr++ = math::vec2f(0.f, 1.f);
		*tc_ptr++ = math::vec2f(1.f, 1.f);

		declaration.unlock_all();

		m_post_fx_data.declaration = declaration;
	}
}

Render_Target_ptr GLES_Renderer::create_default_render_target(math::vec2u32 const& size,
															Render_Target::Color_Format color_format,
															Render_Target::Depth_Format depth_format,
															Render_Target::Stencil_Format stencil_format)
{
	gles::Interface interf;

	int fbo_id = 0;
	interf.iglGetIntegerv(gles::iGL_FRAMEBUFFER_BINDING, &fbo_id);

	auto rt = std::make_shared<GLES_Fbo_Wrapper>();
	rt->set_gl_id(fbo_id);
	rt->allocate(math::vec2u32(size), color_format, depth_format, stencil_format, Render_Target::AA_Format::NONE, false);
	return rt;
}

void GLES_Renderer::init_capabilities()
{
	QASSERT(!m_frame_begun);

	if (m_capabilities_initialized)
	{
		return;
	}
	m_capabilities_initialized = true;

	m_capabilities = Capabilities();

	gles::Interface interf;

	interf.iglGetIntegerv(gles::iGL_MAX_TEXTURE_SIZE, reinterpret_cast<int*>(&m_capabilities.max_texture_size));
	m_capabilities.max_render_target_size = m_capabilities.max_texture_size;	 //for now

	bool hasAniso = interf.ihasExtensionPart("texture_filter_anisotropic");
	if (hasAniso)
	{
		float max_aniso = 0.f;
		interf.iglGetFloatv(gles::iGL_MAX_TEXTURE_MAX_ANISOTROPY, &max_aniso); // this gets the maximum supported anisotropy level for this implementation (hardware or driver max).
		m_capabilities.max_anisotropy = (size_t)max_aniso;
        QLOGI("\tMaximum anisotropy: {}", max_aniso);
	}
	{
		m_capabilities.occlusion_query_support = interf.ihasExtensionPart("_occlusion_query_boolean");
        QLOGI("\tOcclusion query support: {}", m_capabilities.occlusion_query_support);
	}

	{
		m_capabilities.float_texture_support =
			interf.ihasExtensionPart("_texture_float") ||
			interf.ihasExtensionPart("_texture_float");
        QLOGI("\tFloat texture support: {}", m_capabilities.float_texture_support);
	}
	{
		m_capabilities.half_float_texture_support =
			interf.ihasExtensionPart("_texture_half_float") ||
			interf.ihasExtensionPart("_texture_half_float") ||
			interf.ihasExtensionPart("_half_float_pixel");
        QLOGI("\tHalf float texture support: {}", m_capabilities.half_float_texture_support);
	}

	{
		m_capabilities.depth_texture_support =
			interf.ihasExtension("GL_OES_depth_texture") ||
			interf.ihasExtension("GL_ARB_depth_texture") ||
			interf.ihasExtension("GL_EXT_depth_texture") ||
			interf.ihasExtension("GL_IMG_depth_texture") ||
			interf.ihasExtension("GL_depth_texture");
        QLOGI("\tDepth texture support: {}", m_capabilities.depth_texture_support);
	}

	{
		m_capabilities.pvrtc_compression_support = interf.ihasExtensionPart("pvrtc");
        QLOGI("\tPVRTC compression support: {}", m_capabilities.pvrtc_compression_support);
	}

	{
		m_capabilities.etc1_compression_support = interf.ihasExtensionPart("etc");
        QLOGI("\tETC compression support: {}", m_capabilities.etc1_compression_support);
	}

	{
		m_capabilities.dxt_compression_support =
			interf.ihasExtension("GL_EXT_texture_compression_s3tc") ||
			interf.ihasExtension("GL_OES_texture_compression_S3TC") ||
			interf.ihasExtensionPart("dxt");
        QLOGI("\tDXT compression support: {}", m_capabilities.dxt_compression_support);
	}

	if (interf.ihasExtension("GL_ARB_multisample") || interf.ihasExtension("GL_OES_multisample"))
	{
#ifndef GL_MAX_SAMPLES
#	define GL_MAX_SAMPLES 0x8D57
#endif
		int maxSamples = 0;
		interf.iglGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
		m_capabilities.max_aa_samples = maxSamples;
        QLOGI("\tMax AA samples: {}", m_capabilities.max_aa_samples);
	}

	{
		m_capabilities.multiple_render_targets = 1;
        QLOGI("\tMultiple render targets supported: {}", m_capabilities.multiple_render_targets);
	}

	{
		gles::iGLint vtf;
		interf.iglGetIntegerv(gles::iGL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &vtf);
		m_capabilities.vertex_textures = vtf;
        QLOGI("\tVertex textures: {}", vtf);
	}

	{
		String vendor(reinterpret_cast<const char*>(interf.iglGetString(gles::iGL_VENDOR)));
		String renderer(reinterpret_cast<const char*>(interf.iglGetString(gles::iGL_RENDERER)));
		String version(reinterpret_cast<const char*>(interf.iglGetString(gles::iGL_VERSION)));
        QLOGI("Rendering info:");
        QLOGI("\t Vendor: {}", vendor);
        QLOGI("\t Renderer: {}", renderer);
        QLOGI("\t Version: {}", version);

		if (renderer.find("powervr") != String::npos)
		{
			//powervr needs this to avoid buffer resolves
			m_needs_full_clear_every_frame = true;
		}
		else
		{
			m_needs_full_clear_every_frame = false;
		}
	}
}

void GLES_Renderer::add_end_frame_callback(End_Frame_Callback const* callback)
{
	QASSERT(callback && !m_ending_frame);
	remove_end_frame_callback(callback);
	m_end_frame_callbacks.push_back(callback);
}
void GLES_Renderer::remove_end_frame_callback(End_Frame_Callback const* callback)
{
	QASSERT(callback && !m_ending_frame);
	m_end_frame_callbacks.erase(std::remove(m_end_frame_callbacks.begin(), m_end_frame_callbacks.end(), callback), m_end_frame_callbacks.end());
}
void GLES_Renderer::add_flush_callback(Flush_Callback const* callback)
{
	QASSERT(callback);
	remove_flush_callback(callback);
	m_flush_callbacks.push_back(callback);
}
void GLES_Renderer::remove_flush_callback(Flush_Callback const* callback)
{
	QASSERT(callback);
	m_flush_callbacks.erase(std::remove(m_flush_callbacks.begin(), m_flush_callbacks.end(), callback), m_flush_callbacks.end());
}

void GLES_Renderer::add_render_target(String const& name, Render_Target_ptr rt)
{
	QASSERT(!m_frame_begun);

	if (!rt)
	{
        QLOGE("Trying to add empty render target with name '{}'", name);
		return;
	}
	if (find_render_target_by_name(name))
	{
        QLOGE("Render target named '{}' already exists", name);
		return;
	}

	m_render_targets.emplace_back(name, rt);
}
Render_Target_ptr GLES_Renderer::find_render_target_by_name(String const& name)
{
	auto it = std::find_if(m_render_targets.begin(), m_render_targets.end(), [&](std::pair<String, Render_Target_ptr> const& rtd) { return rtd.first == name; });
	if (it != m_render_targets.end())
	{
		return it->second;
	}
	return Render_Target_ptr();
}
int GLES_Renderer::find_render_target_idx_by_name(String const& name) const
{
	auto it = std::find_if(m_render_targets.begin(), m_render_targets.end(), [&] (std::pair<String, Render_Target_ptr> p) { return p.first == name; });
	return it != m_render_targets.end() ? std::distance(m_render_targets.begin(), it) : -1;
}

Dynamic_Image_uptr	GLES_Renderer::read_color_pixels(math::vec2u32 const& position, math::vec2u32 const& size)
{
	QASSERT(!m_frame_begun);

	PROFILE_SCOPED();

	flush();

	gles::Interface interf;

	Dynamic_Image_uptr image(new Dynamic_Image);
	image->allocate(Dynamic_Image::Format::RGBA_8, size, nullptr);

	uint32_t x = position.x;
	uint32_t y = position.y;
	uint32_t sx = size.x;
	uint32_t sy = size.y;

	math::vec2u32 dsize = get_viewport_size();
	if (x >= dsize.x)
	{
		x = dsize.x - 1;
	}
	if (y >= dsize.y)
	{
		y = dsize.y - 1;
	}
	y = dsize.y - y;

	sx = math::min(sx, dsize.x - x);
	sy = math::min(sy, dsize.y - y);

	interf.iglReadPixels(x, y, sx, sy, gles::iGL_RGBA, gles::iGL_UNSIGNED_BYTE, image->get_mutable_data());

	return image;
}
Dynamic_Image_uptr	GLES_Renderer::read_depth_pixels(math::vec2u32 const& position, math::vec2u32 const& size)
{
	QASSERT(!m_frame_begun);

	PROFILE_SCOPED();

#if !defined(Q_WINDOWS)
	return Dynamic_Image_uptr();
#endif

	flush();

	gles::Interface interf;

	Dynamic_Image_uptr image(new Dynamic_Image);
	image->allocate(Dynamic_Image::Format::I_32, size, nullptr);

	uint32_t x = position.x;
	uint32_t y = position.y;
	uint32_t sx = size.x;
	uint32_t sy = size.y;

	math::vec2u32 dsize = get_viewport_size();
	if (x >= dsize.x)
	{
		x = dsize.x - 1;
	}
	if (y >= dsize.y)
	{
		y = dsize.y - 1;
	}
//	y = dsize.y - y;
//
// 	sx = math::min(sx, dsize.x - x);
// 	sy = math::min(sy, dsize.y - y);


	interf.iglPixelStorei(gles::iGL_UNPACK_ALIGNMENT, 1);
	interf.iglPixelStorei(gles::iGL_PACK_ALIGNMENT, 1);
	interf.iglReadPixels(x, y, sx, sy, gles::iGL_DEPTH_COMPONENT, gles::iGL_FLOAT, image->get_mutable_data());
	image->flip_v();

	return image;
}

void GLES_Renderer::set_user_uniform_getter(String const& name, Uniform_Def::Getter const& getter)
{
	m_user_uniform_getters[name] = getter;
}
void GLES_Renderer::set_user_sampler_getter(String const& name, Sampler_Def::Getter const& getter)
{
	m_user_sampler_getters[name] = getter;
}

void GLES_Renderer::begin_frame()
{
	QASSERT(!m_frame_begun);
	PROFILE_SCOPED();

	m_default_render_target_binded = false;
	m_triangle_count = m_line_count = m_primitive_count = m_draw_calls = m_vertex_count = 0;

	gles::set_context(this);

    //don't know if this is ok or not
    //if we have post fx the main target will be binded twice
    m_default_render_target_binded = true;
    //std::static_pointer_cast<GLESRenderTarget>(mDisplay->getRenderTarget())->bind();
	m_frame_begun = true;

	if (m_hdr.format != Hdr_Format::NONE && m_hdr.resolver.get_material().is_valid())
	{
		QASSERT(0); //cucu
		//beginPostFx(PostFx());
	}
}

void GLES_Renderer::end_frame()
{
	QASSERT(m_frame_begun);

	PROFILE_SCOPED();

    flush();

	m_ending_frame = true;
	for (auto const& cb: m_end_frame_callbacks)
	{
		(*cb)();
	}
	m_ending_frame = false;

	end_post_fx();

    flush();

	if (!m_default_render_target_binded)
	{
		//std::static_pointer_cast<GLESRenderTarget>(mDisplay->getRenderTarget())->bind();
	}

	m_frame_idx++;

    //QLOGI("dc:{}, tc:{}, lc:{}, vc:{}", m_draw_calls, m_triangle_count, m_line_count, m_vertex_count);
	//Profiler::dump();

	m_cameras.clear();

	m_frame_begun = false;
}

Render_Target_ptr GLES_Renderer::get_post_fx_render_target(Hdr_Format format)
{
	size_t rt_idx = static_cast<size_t>(format);
	QASSERT(rt_idx < m_post_fx_data.render_targets.size());
	auto& pfx_rt = m_post_fx_data.render_targets[rt_idx];
	auto const& rt = get_render_target();

	Render_Target::Color_Format desired_color_format = rt->get_color_format();
	if (format != Hdr_Format::NONE)
	{
		auto color_format = rt->get_color_format();
		if (color_format == Render_Target::Color_Format::RGBA_4 ||
			color_format == Render_Target::Color_Format::RGBA_8 ||
			color_format == Render_Target::Color_Format::RGBA_5551 ||
			color_format == Render_Target::Color_Format::RGBA_16 ||
			color_format == Render_Target::Color_Format::RGBA_32)
		{
			desired_color_format = format == Hdr_Format::HALF ? Render_Target::Color_Format::RGBA_16 : Render_Target::Color_Format::RGBA_32;
		}
		else if (color_format == Render_Target::Color_Format::RGB_8 ||
			color_format == Render_Target::Color_Format::RGB_565 ||
			color_format == Render_Target::Color_Format::RGB_16 ||
			color_format == Render_Target::Color_Format::RGB_32)
		{
			desired_color_format = format == Hdr_Format::HALF ? Render_Target::Color_Format::RGB_16 : Render_Target::Color_Format::RGB_32;
		}
		else if (color_format == Render_Target::Color_Format::A_8 ||
			color_format == Render_Target::Color_Format::A_16 ||
			color_format == Render_Target::Color_Format::A_32)
		{
			desired_color_format = format == Hdr_Format::HALF ? Render_Target::Color_Format::A_16 : Render_Target::Color_Format::A_32;
		}
		else if (color_format == Render_Target::Color_Format::AI_8 ||
			color_format == Render_Target::Color_Format::AI_16 ||
			color_format == Render_Target::Color_Format::AI_32)
		{
			desired_color_format = format == Hdr_Format::HALF ? Render_Target::Color_Format::AI_16 : Render_Target::Color_Format::AI_32;
		}
		else if (color_format == Render_Target::Color_Format::I_8 ||
			color_format == Render_Target::Color_Format::I_16 ||
			color_format == Render_Target::Color_Format::I_32)
		{
			desired_color_format = format == Hdr_Format::HALF ? Render_Target::Color_Format::I_16 : Render_Target::Color_Format::I_32;
		}
	}

	bool recreate = false;
	if (!pfx_rt)
	{
        QLOGI("Creating postfx render target for format {}", static_cast<int>(format));
		recreate = true;
	}
	else if (pfx_rt->get_size() != rt->get_size())
	{
        QLOGI("Creating postfx render target for format {} because of size change", static_cast<int>(format));
		recreate = true;
	}
	else if (format == Hdr_Format::NONE &&
		(rt->get_color_format() != pfx_rt->get_color_format() || rt->get_depth_format() != pfx_rt->get_depth_format() ||
		rt->get_stencil_format() != pfx_rt->get_stencil_format() || rt->get_aa_format() != pfx_rt->get_aa_format()))
	{
        QLOGI("Creating postfx render target for format {} because of mode change", static_cast<int>(format));
		recreate = true;
	}

	if (recreate)
	{
		pfx_rt = std::make_shared<GLES_Render_Target>();
		bool res = pfx_rt->allocate(rt->get_size(), desired_color_format, rt->get_depth_format(), rt->get_stencil_format(), rt->get_aa_format(), false);
		if (!res)
		{
            QLOGE("Cannot create postfx render target for format {}", static_cast<int>(format));
			return Render_Target_ptr();
		}
		m_post_fx_data.render_targets[rt_idx] = pfx_rt;
	}

	pfx_rt->set_color_clear_value(rt->get_color_clear_value());
	pfx_rt->set_depth_clear_value(rt->get_depth_clear_value());

	return pfx_rt;
}

void GLES_Renderer::begin_post_fx(Post_Fx const& pfx, Hdr_Stage stage)
{
	QASSERT(m_frame_begun);
	PROFILE_SCOPED();

	flush();

	if (stage == Hdr_Stage::BEFORE_RESOLVE)
	{
		m_post_fx_data.effects.emplace_back(pfx);
	}

	//hdr resolve, if any
	if (m_hdr.format != Hdr_Format::NONE)
	{
		apply_hdr_params();
		m_post_fx_data.effects.emplace_back(m_hdr.resolver);
		//format = mHdr.resolver.getColorFormat();
		QASSERT(0); //cucu
	}

	if (stage == Hdr_Stage::AFTER_RESOLVE)
	{
		m_post_fx_data.effects.emplace_back(pfx);
		//format = pfx->getColorFormat();
		QASSERT(0); //cucu
	}

	if (m_post_fx_data.effects.empty())
	{
		return;
	}

	m_post_fx_data.is_rendering = true;
	m_post_fx_data.is_flushing = false;
	m_post_fx_data.old_render_target = m_crt_render_target;

	set_render_target(get_post_fx_render_target(m_hdr.format));
}


void GLES_Renderer::end_post_fx()
{
	QASSERT(m_frame_begun);
	PROFILE_SCOPED();

	flush();

	if (m_post_fx_data.effects.empty())
	{
		return;
	}

	m_post_fx_data.is_flushing = true;
	m_post_fx_data.is_rendering = false;

	Render_Job job;
	job.set_material(m_post_fx_data.effects.back().get_material());
	job.set_vertex_declaration(m_post_fx_data.declaration);

	Command_Stream& stream = lock_command_stream();
	m_post_fx_data.screen_render_target_handle = stream.add_render_target(std::static_pointer_cast<GLES_Fbo_Wrapper>(get_render_target()));

	set_render_target(m_post_fx_data.old_render_target);
	render(job);

	flush();

	m_post_fx_data.is_flushing = false;
	m_post_fx_data.is_rendering = false;

	m_post_fx_data.effects.clear();
	if (m_hdr.format != Hdr_Format::NONE)
	{
		m_post_fx_data.effects.emplace_back(m_hdr.resolver);
	}
}

void GLES_Renderer::render(Render_Job const& job)
{
	QASSERT(m_frame_begun);
	PROFILE_SCOPED();
	QASSERT(job.get_vertex_declaration().get_attribute_count() > 0);

	if (!job.get_material().is_valid())
	{
		return;
	}

	if (m_cameras.empty())
	{
		set_camera(scene::Camera());
	}

	scene::Camera const& camera = m_cameras.back();
	math::vec3f const& cameraPosition = camera.get_position();

	float cameraDistance = 0.f;
	float radius = job.get_bounds().get_radius();
	if (radius <= 0.f)
	{
		auto pos = job.get_world_transform().get_translation();
		cameraDistance = math::distance(cameraPosition, pos);
	}
	else
	{
		math::vec3f const& pos = job.get_bounds().get_center();
		cameraDistance = math::max(math::distance(cameraPosition, pos) - job.get_bounds().get_radius(), 0.f);
	}

	Material const& material = job.get_material();
	Technique const& technique = *material.get_technique();
	auto const& defaultRT = get_render_target();

	Command_Stream& stream = lock_command_stream();

	Command_Stream::Material_Handle mh = stream.add_material(material);
	Command_Stream::Render_Job_Handle rjh = stream.add_render_job(job);
	Command_Stream::Vertex_Declaration_Handle vdh = stream.add_vertex_declaration(job.get_vertex_declaration());
	Command_Stream::Render_Target_Handle defaultRTH = stream.add_render_target(std::static_pointer_cast<GLES_Fbo_Wrapper>(defaultRT));
	Command_Stream::Camera_Handle defaultCH = stream.add_camera(m_cameras.back());

	size_t pc = job.get_vertex_declaration().get_primitive_count();
 	m_primitive_count += pc;
 	if (job.get_vertex_declaration().get_topology() == Vertex_Declaration::Topology::LINE_LIST || job.get_vertex_declaration().get_topology() == Vertex_Declaration::Topology::LINE_LOOP || job.get_vertex_declaration().get_topology() == Vertex_Declaration::Topology::LINE_STRIP)
 	{
 		m_line_count += pc;
 	}
 	else
 	{
 		m_triangle_count += pc;
 	}
 	m_draw_calls++;

	size_t pass_count = technique.get_pass_count();
	for (size_t i = 0; i < pass_count; i++)
	{
		Command_Stream::Render_Target_Handle rth = defaultRTH;
		Command_Stream::Camera_Handle ch = defaultCH;
		auto const& rt = technique.get_pass(i).get_render_target();
		if (rt && rt != defaultRT)
		{
			rth = stream.add_render_target(std::static_pointer_cast<GLES_Fbo_Wrapper>(rt));

			if (m_post_fx_data.is_flushing)
			{
				scene::Camera camera = m_cameras.back();
				camera.set_viewport(Viewport(math::vec2u32::zero, rt->get_size()));
				ch = stream.add_camera(camera);
			}
		}

		size_t bucket_idx = get_render_bucket_idx(job, i);
		Render_Context context;//(job, i, mCameras.size() - 1);
		context.command_stream = &stream;
		context.material_handle = mh;
		context.render_job_handle = rjh;
		context.vertex_declaration_handle = vdh;
		context.render_target_handle = rth;
		context.camera_handle = ch;
		context.pass_idx = i;

		//UberShader const& uberShader = technique.getPass(i).getUberShader();
		//GLESShader* shader = reinterpret_cast<GLESShader*>(uberShader.getShader(material, job.getShaderType()).get());
		//context.mShader = shader;
		//context.mJob.lock();

		m_render_buckets[bucket_idx].emplace_back(context, cameraDistance);
	}
}

void GLES_Renderer::flush()
{
	QASSERT(m_frame_begun);
	PROFILE_SCOPED();

	for (auto const& cb: m_flush_callbacks)
	{
		(*cb)();
	}

	if (!m_post_fx_data.is_flushing)
	{
		std::sort(m_render_buckets[(uint32_t)Bucket_Type::SOLID].begin(), m_render_buckets[(uint32_t)Bucket_Type::SOLID].end(), Distance_Technique_Sorter());
		std::sort(m_render_buckets[(uint32_t)Bucket_Type::TRANSPARENT_UNORDERED].begin(), m_render_buckets[(uint32_t)Bucket_Type::TRANSPARENT_UNORDERED].end(), Technique_Sorter());
		std::sort(m_render_buckets[(uint32_t)Bucket_Type::TRANSPARENT_ORDERED].begin(), m_render_buckets[(uint32_t)Bucket_Type::TRANSPARENT_ORDERED].end(), Inv_Distance_Technique_Sorter());
	}

	flush_bucket(m_render_buckets[(uint32_t)Bucket_Type::SOLID]);
	flush_bucket(m_render_buckets[(uint32_t)Bucket_Type::TRANSPARENT_UNORDERED]);
	flush_bucket(m_render_buckets[(uint32_t)Bucket_Type::TRANSPARENT_ORDERED]);

    lock_command_stream().execute();
}

void GLES_Renderer::wait_for_gpu()
{
	gles::Interface().iglFinish();
}

void GLES_Renderer::flush_bucket(Render_Pass_Bucket& bucket)
{
	if (bucket.empty())
	{
		return;
	}

	PROFILE_SCOPED();

	gles::Interface interf;
	for(auto const& rp: bucket)
	{
		Render_Context const& context = rp.context;
		Command_Stream& stream = lock_command_stream();
		stream.do_render(m_post_fx_data.screen_render_target_handle,
			context.render_target_handle,
			context.vertex_declaration_handle,
			context.render_job_handle,
			context.material_handle,
			context.camera_handle,
			context.pass_idx);
		m_draw_calls++;
	}

	bucket.clear();
}

size_t GLES_Renderer::get_render_bucket_idx(Render_Job const& job, size_t passIdx) const
{
	Material const& material = job.get_material();

	Render_State const& rs = material.get_render_state(passIdx);
	Render_State::Blend_Formula f = rs.get_blend_formula();

	if (f.src_color == Render_State::Blend_Formula::Factor::ONE && f.dst_color == Render_State::Blend_Formula::Factor::ZERO &&
		f.src_alpha == Render_State::Blend_Formula::Factor::ONE && f.dst_alpha == Render_State::Blend_Formula::Factor::ZERO)
	{
		return (size_t)Bucket_Type::SOLID;
	}

	if (f.src_color == f.dst_color && f.src_alpha == f.dst_alpha)
	{
		return (size_t)Bucket_Type::TRANSPARENT_UNORDERED;
	}

	return (size_t)Bucket_Type::TRANSPARENT_ORDERED;
}

Vertex_Buffer_ptr GLES_Renderer::create_vertex_buffer()
{
	return std::make_shared<GLES_Vertex_Buffer>();
}
Index_Buffer_ptr GLES_Renderer::create_index_buffer()
{
	return std::make_shared<GLES_Index_Buffer>();
}
Uniform_Buffer_ptr GLES_Renderer::create_uniform_buffer()
{
	return std::make_shared<GLES_Uniform_Buffer>();
}
Render_Target_ptr GLES_Renderer::create_render_target()
{
	return std::make_shared<GLES_Render_Target>();
}
Texture_ptr GLES_Renderer::create_texture(Path const& path)
{
	return std::make_shared<GLES_Texture>(path);
}
Shader_ptr GLES_Renderer::create_shader(Shader::Type type, Shader_Source const& vertex, Shader_Source const& fragment)
{
	return std::make_shared<GLES_Shader>(type, vertex, fragment);
}

bool GLES_Renderer::link_uniform(Uniform_Def& def) const
{
	Path link = def.get_link();
	if (link.is_empty())
	{
		return false;
	}

	using namespace data;

	if (link[0] == "user" && link.get_count() == 2)
	{
		String name = link[1];
		auto it = m_user_uniform_getters.find(name);
		if (it == m_user_uniform_getters.end())
		{
			return false;
		}

		def.set_getter(it->second);
		return true;
	}

	if (link == Path("transform/world"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_matrix_w, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}
	if (link == Path("transform/view"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_matrix_v, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}
	if (link == Path("transform/projection"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_matrix_p, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}
	if (link == Path("transform/world_view_projection"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_matrix_wvp, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}
	if (link == Path("transform/view_projection"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_matrix_vp, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}
	if (link == Path("transform/world_view_projection/inverse"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_matrix_wvp_i, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}

	if (link == Path("viewport/size"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_viewport_size, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}

	if (link == Path("camera/near"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_camera_near, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}
	if (link == Path("camera/far"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_camera_far, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}
	if (link == Path("camera/range"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_camera_range, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}
	if (link == Path("camera/range_packed"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_camera_range_packed, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}
	if (link == Path("camera/position"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_camera_position, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}
	if (link == Path("camera/front"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_camera_front, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}
	if (link == Path("camera/right"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_camera_right, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}
	if (link == Path("camera/up"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_camera_up, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}

	if (link == Path("time"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_time, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}
	if (link == Path("fract_time"))
	{
		def.set_getter(std::bind(&Command_Stream::get_uniform_fract_time, std::placeholders::_1));
		def.set_precision(Uniform_Def::Precision::HIGH);
		def.set_enabled(true);
		return true;
	}

	if (link[0] == "render_target")
	{
		if (link.get_count() < 3)
		{
			def.set_enabled(false);
			def.set_enabled(true);
			return true;
		}

		String rtName = link.get(1);
		if (rtName == "screen")
		{
			String var = link.get(2);
			if (var == "size")
			{
				def.set_getter(std::bind(&Command_Stream::get_uniform_screen_size, std::placeholders::_1));
				def.set_precision(Uniform_Def::Precision::HIGH);
				def.set_enabled(true);
				return true;
			}
		}
		int idx = find_render_target_idx_by_name(rtName);
		if (idx >= 0 && idx < 50)
		{
			String var = link.get(2);
			if (var == "size")
			{
				def.set_getter(std::bind(&Command_Stream::get_uniform_render_target_size, std::placeholders::_1, rtName));
				def.set_precision(Uniform_Def::Precision::HIGH);
				def.set_enabled(true);
				return true;
			}
		}
	}

	return false;
}
bool GLES_Renderer::link_sampler(Sampler_Def& def, Path const& link) const
{
	if (link.is_empty())
	{
		return false;
	}

	def.set_enabled(true);
	String name = link.get(0);

	if (link[0] == "user" && link.get_count() == 2)
	{
		auto it = m_user_sampler_getters.find(link[1]);
		if (it == m_user_sampler_getters.end())
		{
			return false;
		}

		def.set_getter(it->second);
		return true;
	}

	if (name == "render_target")
	{
		String sampler = link.get(1);
		if (sampler == "screen")
		{
			String buffer = link.get_count() > 2 ? link.get(2) : "color";
			if (buffer == "color")
			{
				def.set_getter(std::bind(&Command_Stream::get_sampler_screen_color_buffer, std::placeholders::_1));
				return true;
			}
			else if (buffer == "depth")
			{
				def.set_getter(std::bind(&Command_Stream::get_sampler_screen_depth_buffer, std::placeholders::_1));
				return true;
			}
		}
		else
		{
			int idx = find_render_target_idx_by_name(sampler);
			if (idx >= 0 && idx < 50)
			{
				String buffer = link.get_count() > 2 ? link.get(2) : "color";
				if (buffer == "color")
				{
					def.set_getter(std::bind(&Command_Stream::get_sampler_render_target_color_buffer, std::placeholders::_1, sampler));
					return true;
				}
				else if (buffer == "depth")
				{
					def.set_getter(std::bind(&Command_Stream::get_sampler_render_target_depth_buffer, std::placeholders::_1, sampler));
					return true;
				}
			}
		}
	}

	return false;
}

Command_Stream& GLES_Renderer::lock_command_stream()
{
	return *m_command_streams[0];
}

void GLES_Renderer::unlock_command_stream()
{

}

Capabilities GLES_Renderer::get_capabilities() const
{
	QASSERT(m_capabilities_initialized);
	return m_capabilities;
}

void GLES_Renderer::set_hdr_format(Hdr_Format format)
{
	QASSERT(!m_frame_begun);
	m_hdr.format = format;
	if (format != Hdr_Format::NONE)
	{
		set_hdr_resolver(Post_Fx());
	}
}
void GLES_Renderer::set_hdr_resolver(Post_Fx const& hdrResolver)
{
	QASSERT(!m_frame_begun);
	m_hdr.resolver = hdrResolver;
	if (!hdrResolver.get_material().is_valid())
	{
		auto technique = System::inst().get_factory().load_by_name<video::Technique>("hdr_resolver.technique");
		if (technique)
		{
			m_hdr.resolver = Post_Fx();
			Material material(technique);
			m_hdr.resolver.set_material(material);
		}
	}

	if (!m_hdr.resolver.get_material().is_valid())
	{
		m_hdr.format = Hdr_Format::NONE;
	}
}
void GLES_Renderer::set_hdr_parameters(Hdr_Params const& /*params*/)
{
	QASSERT(!m_frame_begun);

}

void GLES_Renderer::apply_hdr_params()
{
	QASSERT(m_hdr.format != Hdr_Format::NONE);
	Material material = m_hdr.resolver.get_material();
	for (size_t p = 0; p < material.get_pass_count(); p++)
	{
		auto idx = material.find_uniform_idx_by_name(p, "hdr_range");
		if (idx >= 0)
		{
			material.set_uniform(p, idx, Uniform(math::vec2f(m_hdr.params.start_range, m_hdr.params.end_range)));
		}
	}
	m_hdr.resolver.set_material(material);
}

bool GLES_Renderer::needs_full_clear_every_frame() const
{
	return m_needs_full_clear_every_frame;
}

math::vec2u32 GLES_Renderer::get_viewport_size() const
{
	return m_default_render_target ? m_default_render_target->get_size() : math::vec2u32::zero;
}

void GLES_Renderer::set_viewport_size(math::vec2u32 const& size)
{
	if (m_default_render_target)
	{
		if (m_default_render_target->get_size() == size)
		{
			return;
		}

		//default rt
		{
			auto& rt = static_cast<GLES_Fbo_Wrapper&>(*m_default_render_target);
			rt.allocate(size, rt.get_color_format(), rt.get_depth_format(), rt.get_stencil_format(), rt.get_aa_format(), false);
		}

		//destroy render targets so they can be re-created in the correct size
		for (auto& rt: m_post_fx_data.render_targets)
		{
			rt.reset();
		}
	}
}

