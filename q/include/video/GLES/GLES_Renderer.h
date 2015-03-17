#pragma once

#include "video/Renderer.h"
#include "GLES_Command_Stream.h"
#include "video/Post_FX.h"

namespace q
{
namespace video
{
	class Command_Stream;

	class GLES_Renderer : public Renderer
	{
		friend class GLES_Fbo_Wrapper;
		friend class GLES_Vertex_Buffer;
		friend class GLES_Index_Buffer;
		friend class GLES_Uniform_Buffer;
		friend class GLES_Shader;
	public:
		GLES_Renderer();
        virtual ~GLES_Renderer();

		void			init(math::vec2u32 const& size,
							Render_Target::Color_Format color_format,
							Render_Target::Depth_Format depth_format,
							Render_Target::Stencil_Format stencil_format);

		math::vec2u32	get_viewport_size() const;
		void			set_viewport_size(math::vec2u32 const& size);

		void			set_hdr_format(Hdr_Format format);
		void			set_hdr_resolver(Post_Fx const& hdr_resolver);
		void			set_hdr_parameters(Hdr_Params const& params);

		Capabilities	get_capabilities() const;

		Command_Stream&	lock_command_stream();
		void			unlock_command_stream();

		void			add_end_frame_callback(End_Frame_Callback const* callback);
		void			remove_end_frame_callback(End_Frame_Callback const* callback);
		void			add_flush_callback(Flush_Callback const* callback);
		void			remove_flush_callback(Flush_Callback const* callback);

		void			set_user_uniform_getter(String const& name, Uniform_Def::Getter const& getter);
		void			set_user_sampler_getter(String const& name, Sampler_Def::Getter const& getter);

		void			add_render_target(String const& name, Render_Target_ptr rt);
		Render_Target_ptr	find_render_target_by_name(String const& name);

		Dynamic_Image_uptr	read_color_pixels(math::vec2u32 const& position, math::vec2u32 const& size);
		Dynamic_Image_uptr	read_depth_pixels(math::vec2u32 const& position, math::vec2u32 const& size);

		void			begin_frame();
		void			end_frame();

		void			set_camera(scene::Camera const& camera);

		void			set_render_target(Render_Target_ptr const& rt);
		Render_Target_ptr const& get_render_target() const;

		void			begin_post_fx(Post_Fx const& pfx, Hdr_Stage stage);
		void			end_post_fx();

		void			render(Render_Job const& job);
		void			flush();
		void			wait_for_gpu();

		size_t			get_frame_idx() const;

	protected:

		bool			needs_full_clear_every_frame() const;

		//////////////////////////////////////////////////////////////////////////

		Vertex_Buffer_ptr	create_vertex_buffer();
		Index_Buffer_ptr	create_index_buffer();
		Uniform_Buffer_ptr	create_uniform_buffer();
		Render_Target_ptr	create_render_target();
		Texture_ptr		create_texture(Path const& path);
		Shader_ptr		create_shader(Shader::Type type, Shader_Source const& vertex, Shader_Source const& fragment);

		bool			link_uniform(Uniform_Def& def) const;
		bool			link_sampler(Sampler_Def& def, Path const& link) const;

		int				find_render_target_idx_by_name(String const& name) const;

		class Render_Context
		{
		public:
            Command_Stream*	command_stream = nullptr;
			Command_Stream::Material_Handle material_handle;
			Command_Stream::Render_Job_Handle render_job_handle;
			Command_Stream::Vertex_Declaration_Handle vertex_declaration_handle;
			Command_Stream::Render_Target_Handle render_target_handle;
			Command_Stream::Camera_Handle camera_handle;
            size_t pass_idx = 0;
		};

		std::vector<scene::Camera> m_cameras;

	private:
		size_t		get_render_bucket_idx(Render_Job const& job, size_t pass_idx) const;

		Render_Target_ptr create_default_render_target(math::vec2u32 const& size,
			Render_Target::Color_Format color_format,
			Render_Target::Depth_Format depth_format,
			Render_Target::Stencil_Format stencil_format);


		Render_Target_ptr m_default_render_target;
		std::vector<std::pair<String, Render_Target_ptr>> m_render_targets;

		std::map<String, Uniform_Def::Getter>	m_user_uniform_getters;
		std::map<String, Sampler_Def::Getter>	m_user_sampler_getters;

		struct Post_FX_Data
		{
			std::vector<Post_Fx> effects;
            bool			is_rendering = false; //true when this pfx is being rendered into
            bool			is_flushing = false; //true when this pfx is being rendered on screen
			Render_Target_ptr old_render_target;
			Command_Stream::Render_Target_Handle screen_render_target_handle;

			std::array<Render_Target_ptr, 3> render_targets;

			Vertex_Declaration declaration;
			//VertexBufferPool vertexBufferPool;
		} m_post_fx_data;

		Render_Target_ptr get_post_fx_render_target(Hdr_Format format);

		std::vector<End_Frame_Callback const*> m_end_frame_callbacks;
		std::vector<Flush_Callback const*> m_flush_callbacks;

        bool			m_frame_begun = false;
        bool			m_ending_frame = false;
        size_t			m_frame_idx = 999;

		mutable Clock::time_point		m_last_timestamp;
        mutable float	m_shader_time = 0.f;

		class Render_Pass
		{
		public:
            Render_Pass() = default;
			Render_Pass(Render_Context const& c, float camera_distance)
				: context(c)
				, camera_distance(camera_distance) {}

			Render_Context		context;
            float				camera_distance = 0;
		};
		struct Distance_Technique_Sorter
		{
			bool operator() (Render_Pass const& i, Render_Pass const& j)
			{
				Material const& mi = i.context.command_stream->get_material(i.context.material_handle);
				Material const& mj = i.context.command_stream->get_material(j.context.material_handle);
				return i.camera_distance < j.camera_distance ||
					(i.camera_distance == j.camera_distance && mi.get_technique() < mj.get_technique());
			}
		};
		struct Inv_Distance_Technique_Sorter
		{
			bool operator() (Render_Pass const& i, Render_Pass const& j)
			{
				Material const& mi = i.context.command_stream->get_material(i.context.material_handle);
				Material const& mj = i.context.command_stream->get_material(j.context.material_handle);
				return i.camera_distance > j.camera_distance ||
					(i.camera_distance == j.camera_distance && mi.get_technique() < mj.get_technique());
			}
		};
		struct Technique_Sorter
		{
			bool operator() (Render_Pass const& i, Render_Pass const& j)
			{
				Material const& mi = i.context.command_stream->get_material(i.context.material_handle);
				Material const& mj = i.context.command_stream->get_material(j.context.material_handle);
				return mi.get_technique() < mj.get_technique();
			}
		};

		enum class Bucket_Type
		{
			SOLID,
			TRANSPARENT_UNORDERED,
			TRANSPARENT_ORDERED,
			COUNT
		};

		typedef std::vector<Render_Pass> Render_Pass_Bucket;
		std::vector<Render_Pass_Bucket>	m_render_buckets;

		void			flush_bucket(Render_Pass_Bucket& bucket);

        size_t		m_triangle_count = 0;
        size_t		m_line_count = 0;
        size_t		m_primitive_count = 0;
        size_t		m_vertex_count = 0;
        size_t		m_draw_calls = 0;

        bool	m_needs_full_clear_every_frame = false;

		struct Hdr
		{
			Post_Fx	resolver;
            Hdr_Format format = Hdr_Format::NONE;
			Hdr_Params params;
		} m_hdr;

		void				apply_hdr_params();

		Render_Target_ptr	m_crt_render_target;
        bool				m_default_render_target_binded = false;

		Command_Stream*		m_command_streams[2];

		Capabilities		m_capabilities;
        bool				m_capabilities_initialized = false;
		void				init_capabilities();
	};

	inline void GLES_Renderer::set_camera(scene::Camera const& camera)
	{
		m_cameras.emplace_back(camera);
	}

	inline void GLES_Renderer::set_render_target(Render_Target_ptr const& rt)
	{
		flush();
		m_crt_render_target = rt;
	}
	inline Render_Target_ptr const& GLES_Renderer::get_render_target() const
	{
		return m_crt_render_target ? m_crt_render_target : m_default_render_target;
	}
	inline size_t GLES_Renderer::get_frame_idx() const
	{
		return m_frame_idx;
	}


	DECLARE_CLASS_PTR(GLES_Renderer);
}
}
