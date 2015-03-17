#pragma once

#include "video/Material.h"
#include "video/Renderer.h"
#include "video/Render_Job.h"
#include "video/Buffer_Pool.h"
#include "video/Vertex_Buffer.h"
#include "video/Index_Buffer.h"

namespace q
{
	namespace text
	{
		class Texter;
	}

	namespace scene
	{
		class Camera;
	}

	namespace draw
	{
		struct Vertex
		{
			friend class Painter;

			Vertex();
			Vertex(math::vec3f const& p, math::vec2f const& t, uint32_t c);
			Vertex(math::vec3f const& p, math::vec2f const& t);
			Vertex(math::vec3f const& p, uint32_t c);
			Vertex(math::vec3f const& p);

			Vertex(math::vec2f const& p, math::vec2f const& t, uint32_t c);
			Vertex(math::vec2f const& p, math::vec2f const& t);
			Vertex(math::vec2f const& p, uint32_t c);
			Vertex(math::vec2f const& p);

		public:
			void set(math::vec3f const& p, math::vec2f const& t, uint32_t c);
			void set(math::vec3f const& p, math::vec2f const& t);
			void set(math::vec3f const& p, uint32_t c);
			void set(math::vec2f const& p, math::vec2f const& t, uint32_t c);
			void set(math::vec2f const& p, math::vec2f const& t);
			void set(math::vec2f const& p, uint32_t c);
			void set_position(math::vec3f const& p);
			void set_position(math::vec2f const& p);
			void set_color(uint32_t color);
			void set_tex_coord(math::vec2f const& t);

			math::vec3f const& get_position() const;
			math::vec2f const& get_tex_coord() const;
			uint32_t	get_color() const;

		protected:
			enum class Flag
			{
				HAS_TEX_COORD	= 1 << 0,
				HAS_COLOR		= 1 << 1
			};
			typedef util::Flag_Set<Flag, uint8_t> Flags;

			math::vec3f		position;
			math::vec2f		tex_coord;
			uint32_t		color;
			Flags			flags;		
		};

		// this helper class handles the rendering of simple primitives in an efficient way (batching them)
		// note that the render is submitted in the flush method
		class Painter
		{
			friend class text::Texter;
		public:
			Painter();
			~Painter();

			bool		is_initialized() const;

			void		draw_line(Vertex const& s, Vertex const& e);

			void		draw_rectangle(Vertex const& ul, Vertex const& dr);
			void		fill_rectangle(Vertex const& ul, Vertex const& dr);

			//Quad vertices:
			// 0	1
			// 3	2
			void		draw_quad(Vertex const& v0, Vertex const& v1, Vertex const& v2, Vertex const& v3);
			void		fill_quad(Vertex const& v0, Vertex const& v1, Vertex const& v2, Vertex const& v3);

			void		draw_triangle(Vertex const& v0, Vertex const& v1, Vertex const& v2);
			void		fill_triangle(Vertex const& v0, Vertex const& v1, Vertex const& v2);

			//list of vertices and indices
			void		draw_triangle_list(Vertex const* vertices, size_t vertex_count, uint32_t const* indices, size_t index_count);
			void		fill_triangle_list(Vertex const* vertices, size_t vertex_count, uint32_t const* indices, size_t index_count);

			//list of vertices and indices
			void		draw_line_list(Vertex const* vertices, size_t vertex_count, uint32_t const* indices, size_t index_count);

			//Ellipse in XY plane
			void		draw_ellipse(Vertex const& center, math::vec2f const& radiuses, size_t tesselation = 0);
			void		fill_ellipse(Vertex const& center, math::vec2f const& radiuses, size_t tesselation = 0);

			//Circle in XY plane
			void		draw_circle(Vertex const& center, float radius, size_t tesselation = 0);
			void		fill_circle(Vertex const& center, float radius, size_t tesselation = 0);

			//Arc in XY plane. Range is in radians
			void		draw_arc(Vertex const& center, math::anglef start, math::anglef end, float radius, size_t tesselation = 0);
			void		fill_arc(Vertex const& center, math::anglef start, math::anglef end, float radius, size_t tesselation = 0);

			//This will draw a closed poly-line like this: v[0]->v[1], v[1]->v[2] ... v[count - 1]->v[0]
			void		draw_polyline(Vertex const* vertices, size_t count);

			//This fills a triangle fan. v[0] is the center
			void		fill_triangle_fan(Vertex const* vertices, size_t count);

			//This fills a sphere.
			void		fill_sphere(Vertex const& center, float radius, size_t tesselation);

			//Cone with base in XY plane and pointint in the Z axis
			void		fill_cone(Vertex const& base_center, float base_radius, float height, size_t tesselation = 0);

			//Renders the wireframe for the box
			void		draw_box(math::aabb3f const& box, uint32_t color);

			//Renders axis in +X +Y +Z
			void		draw_axis(math::trans3df const& m);
			void		draw_axis(math::quatf const& q, math::vec3f const& pos);

			void		set_material(video::Material const& material);
			video::Material const& get_material() const;
			void		set_texture(video::Texture_cptr const& texture);

			//transform to be applied before clipping
			void		set_pre_clip_transform(math::trans3df const& transform);
			math::trans3df const& get_pre_clip_transform() const;
			void		push_pre_clip_transform(math::trans3df const& transform);
			void		pop_pre_clip_transform();

			//transform to be applied after clipping
			void		set_post_clip_transform(math::trans3df const& transform);
			math::trans3df const& get_post_clip_transform() const;
			void		push_post_clip_transform(math::trans3df const& transform);
			void		pop_post_clip_transform();

			void		set_camera(scene::Camera const& camera);

		public:
			// you need to call flush 
			void		flush();
			void		unload();

		private:
			void		draw_line_raw(Vertex const& s, Vertex const& e);
			void		fill_triangle_raw(Vertex const& v0, Vertex const& v1, Vertex const& v2);
		private:
			void		init();

			void		add_lines();
			void		add_quads_and_tris();

			size_t		get_tesselation_from_radius(float radius);

			enum class Operation
			{
				DRAW_LINE,
				FILL_QUAD,
				FILL_TRI
			};

			std::function<void()>	m_end_frame_callback;

			math::trans3df			m_pre_clip_transform;
			math::trans3df			m_post_clip_transform;
			math::trans3df			m_combined_transform;
			bool					m_has_pre_clip_transform;
			bool					m_has_post_clip_transform;
			bool					m_has_combined_transform;

			std::vector<math::trans3df> m_pre_clip_transform_stack;
			std::vector<math::trans3df> m_post_clip_transform_stack;

			//////////////////////////////////////////////////////////////////////////
			// New way to render

			std::vector<scene::Camera> m_cameras;
			int8_t						m_crt_camera_idx;

			video::Material			m_material;
			std::vector<video::Material> m_materials;
			int8_t						m_crt_material_idx;

			std::vector<uint8_t>	m_bytecode;
			size_t					m_bytecode_idx;
			size_t					m_bytecode_size;

			struct Batch_Info
			{
				Operation operation;
				size_t element_count;
				size_t byte_count;
			};
			std::vector<Batch_Info>	m_batch_infos;
			size_t					m_batch_infos_idx;
			int						m_last_operation;

			struct Render_Job_Data
			{
				video::Render_Job job;
				int material_idx;
				int camera_idx;
			};

			std::vector<Render_Job_Data> m_render_jobs;

			void flush_list(std::vector<Render_Job_Data>& render_jobs, size_t count);

			//////////////////////////////////////////////////////////////////////////

			video::Buffer_Pool<video::Vertex_Buffer> m_line_vertex_buffer_pool;
			video::Buffer_Pool<video::Index_Buffer> m_triangle_index_buffer_pool;
			video::Buffer_Pool<video::Vertex_Buffer> m_triangle_vertex_buffer_pool;

			video::Vertex_Declaration m_triangle_vertex_declaration;
			video::Vertex_Declaration m_line_vertex_declaration;

			bool					m_is_initialized;

			static std::vector<math::vec2f> ms_cos_sin_table;//only a table for all painter instances
		};

		inline Vertex::Vertex()
			: color(0xFFFFFFFF)
			, flags(0) {}
		inline Vertex::Vertex(math::vec3f const& p, math::vec2f const& t, uint32_t c)
			: position(p)
			, tex_coord(t)
			, color(c)
			, flags(Flag::HAS_TEX_COORD, Flag::HAS_COLOR) {}
		inline Vertex::Vertex(math::vec3f const& p, math::vec2f const& t)
			: position(p)
			, tex_coord(t)
			, color(0xFFFFFFFF)
			, flags(Flag::HAS_TEX_COORD) {}
		inline Vertex::Vertex(math::vec3f const& p, uint32_t c)
			: position(p)
			, color(c)
			, flags(Flag::HAS_COLOR) {}
		inline Vertex::Vertex(math::vec3f const& p)
			: position(p)
			, color(0xFFFFFFFF)
			, flags(0) {}
		inline Vertex::Vertex(math::vec2f const& p, math::vec2f const& t, uint32_t c)
			: position(p)
			, tex_coord(t)
			, color(c)
			, flags(Flag::HAS_TEX_COORD, Flag::HAS_COLOR) {}
		inline Vertex::Vertex(math::vec2f const& p, math::vec2f const& t)
			: position(p)
			, tex_coord(t)
			, color(0xFFFFFFFF)
			, flags(Flag::HAS_TEX_COORD) {}
		inline Vertex::Vertex(math::vec2f const& p, uint32_t c)
			: position(p)
			, color(c)
			, flags(Flag::HAS_COLOR) {}
		inline Vertex::Vertex(math::vec2f const& p)
			: position(p)
			, color(0xFFFFFFFF)
			, flags(0) {}

		inline void Vertex::set(math::vec3f const& p, math::vec2f const& t, uint32_t c)
		{
			position = p;
			tex_coord = t;
			color = c;
			flags.set(Flag::HAS_COLOR);
			flags.set(Flag::HAS_TEX_COORD);
		}
		inline void Vertex::set(math::vec3f const& p, math::vec2f const& t)
		{
			position = p;
			tex_coord = t;
			flags.set(Flag::HAS_TEX_COORD);
		}
		inline void Vertex::set(math::vec3f const& p, uint32_t c)
		{
			position = p;
			color = c;
			flags.set(Flag::HAS_COLOR);
		}
		inline void Vertex::set(math::vec2f const& p, math::vec2f const& t, uint32_t c)
		{
			position.set(p.x, p.y, 0.f);
			tex_coord = t;
			color = c;
			flags.set(Flag::HAS_COLOR);
			flags.set(Flag::HAS_TEX_COORD);
		}
		inline void Vertex::set(math::vec2f const& p, math::vec2f const& t)
		{
			position.set(p.x, p.y, 0.f);
			tex_coord = t;
			flags.set(Flag::HAS_TEX_COORD);
		}
		inline void Vertex::set(math::vec2f const& p, uint32_t c)
		{
			position.set(p.x, p.y, 0.f);
			color = c;
			flags.set(Flag::HAS_COLOR);
		}
		inline void Vertex::set_position(math::vec3f const& p)
		{
			position = p;
		}
		inline void Vertex::set_position(math::vec2f const& p)
		{
			position.set(p.x, p.y, 0.f);
		}
		inline void Vertex::set_color(uint32_t c)
		{
			color = c;
			flags.set(Flag::HAS_COLOR);
		}
		inline void Vertex::set_tex_coord(math::vec2f const& t)
		{
			tex_coord = t;
			flags.set(Flag::HAS_TEX_COORD);
		}
		inline math::vec3f const& Vertex::get_position() const
		{
			return position;
		}
		inline math::vec2f const& Vertex::get_tex_coord() const
		{
			return tex_coord;
		}
		inline uint32_t Vertex::get_color() const
		{
			return color;
		}


	}//video
}
