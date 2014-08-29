#pragma once


namespace q
{
namespace video
{
	namespace gles
	{
		class Interface;
	}

	class GLES_Renderer;
	class GLES_Shader;
	class GLES_Fbo_Wrapper;

	class Command_Stream
	{
		friend GLES_Renderer;
		friend class GLES_Shader;
	public:
		Command_Stream();

		struct Material_Handle
		{
			Material_Handle(uint32_t id = 0) : id(id) {}
			uint32_t id;
		};
		struct Render_Job_Handle
		{
			Render_Job_Handle(uint32_t id = 0) : id(id) {}
			uint32_t id;
		};
		struct Vertex_Declaration_Handle
		{
			Vertex_Declaration_Handle(uint32_t id = 0) : id(id) {}
			uint32_t id;
		};

		struct Camera_Handle
		{
			Camera_Handle(uint32_t id = 0) : id(id) {}
			uint32_t id;
		};

		struct Render_Target_Handle
		{
			Render_Target_Handle(uint32_t id = 0) : id(id) {}
			uint32_t id;
		};

		//resources
		Render_Target_Handle	add_render_target(std::shared_ptr<GLES_Fbo_Wrapper> const& renderTarget);
		Camera_Handle			add_camera(scene::Camera const& camera);

		Material_Handle			add_material(Material const& material);
		Material const&			get_material(Material_Handle mh) const;

		Render_Job_Handle			add_render_job(Render_Job const& render_job);
		Vertex_Declaration_Handle	add_vertex_declaration(Vertex_Declaration const& declaration);

		//commands
		void do_clear_fbo(Render_Target_Handle render_target, bool color, bool depth, bool stencil);
		void do_render(Render_Target_Handle screen_render_target,
			Render_Target_Handle render_target,
			Vertex_Declaration_Handle declaration,
			Render_Job_Handle job,
			Material_Handle material,
			Camera_Handle camera,
			size_t pass_idx);

		//rendering
		void execute();
		void reset();

	private:
		struct Render_Target_Data
		{
			std::shared_ptr<GLES_Fbo_Wrapper> render_target;
			math::vec4f clear_color;
			float clear_depth;
		};

		struct Render_Job_Data
		{
			Render_Job_Data()
				: shader_type(Shader::Type::NORMAL)
				, has_world_transform(false)
				, node_transform_start_idx(0)
				, node_transform_count(0)
				, node_transforms(nullptr)
				//, primitive_count(0)
			{}

			Shader::Type		shader_type;
			util::Bounds		bounds;
			math::trans3df		world_transform;
			bool				has_world_transform;
			size_t				node_transform_start_idx;
			size_t				node_transform_count;
			math::trans3df*		node_transforms;
			//size_t					primitive_count;
		};

		std::vector<math::trans3df> m_node_transforms;

		std::vector<scene::Camera> m_cameras;
		std::vector<Material> m_materials;
		size_t m_material_count;
		std::vector<Render_Job_Data> m_render_jobs;
		std::vector<Vertex_Declaration> m_vertex_declarations;
		std::vector<Render_Target_Data> m_render_targets;

		data::Memory_Sink	m_sink;
		data::Memory_Source	m_source;

//to create human readable names for commands. Makes debugging easier
#define MAKE_ID(a, b, c, d) ((a) | (b << 8) | (c << 16) | (d << 24))

		enum class Command
		{
			RENDER			= MAKE_ID('r', 'e', 'n', 'd'),
			CLEAR_FBO		= MAKE_ID('c', 'l', 'r', 'f'),
		};

#undef MAKE_ID

		//Getters for auto uniforms/samplers
		//////////////////////////////////////////////////////////////////////////

		static Sampler const& get_sampler_screen_color_buffer(void* data);
		static Sampler const& get_sampler_screen_depth_buffer(void* data);
		static Sampler const& get_sampler_render_target_color_buffer(void* data, String const& name);
		static Sampler const& get_sampler_render_target_depth_buffer(void* data, String const& name);

		//////////////////////////////////////////////////////////////////////////

		static Uniform const& get_uniform_matrix_w(void* data);
		static Uniform const& get_uniform_matrix_v(void* data);
		static Uniform const& get_uniform_matrix_p(void* data);
		static Uniform const& get_uniform_matrix_vp(void* data);
		static Uniform const& get_uniform_matrix_wvp(void* data);
		static Uniform const& get_uniform_matrix_wvp_i(void* data);

		static Uniform const& get_uniform_viewport_size(void* data);

		static Uniform const& get_uniform_camera_near(void* data);
		static Uniform const& get_uniform_camera_far(void* data);
		static Uniform const& get_uniform_camera_range(void* data);
		static Uniform const& get_uniform_camera_range_packed(void* data);
		static Uniform const& get_uniform_camera_position(void* data);
		static Uniform const& get_uniform_camera_front(void* data);
		static Uniform const& get_uniform_camera_right(void* data);
		static Uniform const& get_uniform_camera_up(void* data);

		static Uniform const& get_uniform_time(void* data);
		static Uniform const& get_uniform_fract_time(void* data);

		static Uniform const& get_uniform_screen_size(void* data);
		static Uniform const& get_uniform_render_target_size(void* data, String const& name);

		//////////////////////////////////////////////////////////////////////////

		//rendering helpers
		void apply_render_state(gles::Interface& interf, Render_State const& rs);

		//commands
		void execute_render_command(data::Source& source);
		void execute_clear_fbo_command(data::Source& source);
		void execute_bind_fbo_command(data::Source& source);

		//////////////////////////////////////////////////////////////////////////

		struct Render_Context
		{
			Render_Context()
				: _this(nullptr)
				, renderer(nullptr)
				, material(nullptr)
				, render_job_data(nullptr)
				, vertex_declaration(nullptr)
				, shader(nullptr)
				, render_target(nullptr)
				, camera(nullptr)
				, pass_idx(0) {}

			Command_Stream*		_this;
			GLES_Renderer*		renderer;
			Material const*		material;
			Render_Job_Data*	render_job_data;
			Vertex_Declaration const* vertex_declaration;
			GLES_Shader const*	shader;
			GLES_Fbo_Wrapper*	screen_render_target;
			GLES_Fbo_Wrapper*	render_target;
			scene::Camera const* camera;
			size_t				pass_idx;
		};

		//////////////////////////////////////////////////////////////////////////

		void render(gles::Interface& interf, Render_Context const& context) const;
		bool bind_attributes(gles::Interface& interf, Render_Context const& context) const;
		void unbind_attributes(gles::Interface& interf) const;

		mutable std::vector<uint32_t> m_active_attributes;

		//////////////////////////////////////////////////////////////////////////


		Clock::time_point m_last_timestamp;
		float m_time;
		Sampler m_sampler;
		Uniform m_uniform;
		Render_Context m_context;
	};


}
}
