#include "QStdAfx.h"
#include "draw/Painter.h"

using namespace q;
using namespace draw;
using namespace video;

//#define MAX_VERTICES	5000
static const int32_t MAX_VERTICES = 5000;
static const int32_t MAX_INDICES = 10000;
static const int32_t MAX_SIN_COS_TABLE_SIZE = 2048;

std::vector<math::vec2f> Painter::ms_cos_sin_table;

String k_painter_lines_str("Painter Lines");
String k_painter_tri_quad_str("Painter TriQuad");

//////////////////////////////////////////////////////////////////////////

Painter::Painter()
    : m_line_vertex_buffer_pool(10000, 0, video::Vertex_Buffer::Usage::STREAM)
    , m_triangle_index_buffer_pool(10000, 0, video::Index_Buffer::Usage::STREAM)
	, m_triangle_vertex_buffer_pool(10000, 0, video::Vertex_Buffer::Usage::STREAM)
{	
	m_has_pre_clip_transform = false;
	m_has_post_clip_transform = false;
	m_has_combined_transform = false;

	const float pi = 3.1415926535f;
	const float invMax = 1.f / (float)MAX_SIN_COS_TABLE_SIZE;
	if (ms_cos_sin_table.empty())
	{
		ms_cos_sin_table.resize(MAX_SIN_COS_TABLE_SIZE);
		for (uint32_t i = 0; i < MAX_SIN_COS_TABLE_SIZE; i++)
		{
			float angle = (float)i * 2.f * pi * invMax;
			ms_cos_sin_table[i] = math::vec2f(math::cos(angle), math::sin(angle));
		}
	}//if costable created

	//m_cameraPtr = 0;
	//m_end_frame_callback = 0;

// 	m_clippingParameters.clipRectangle = false;
// 	m_clippingParameters.enableClipStack = false;

	m_is_initialized = false;

	m_crt_camera_idx = -1;
	m_crt_material_idx = -1;

	m_bytecode.resize(16384);
	m_bytecode_size = 0;
	m_bytecode_idx = 0;

	m_last_operation = -1;
}

//////////////////////////////////////////////////////////////////////////

Painter::~Painter()
{
	if (System::inst().get_renderer())
	{
		flush();
		if (m_end_frame_callback)
		{
			System::inst().get_renderer()->remove_end_frame_callback(&m_end_frame_callback);
		}
	}
	m_triangle_index_buffer_pool.clear();
	m_triangle_vertex_buffer_pool.clear();
	m_line_vertex_buffer_pool.clear();
}

//////////////////////////////////////////////////////////////////////////

bool Painter::is_initialized() const
{
	return m_is_initialized;
}

//////////////////////////////////////////////////////////////////////////

void Painter::init()
{
	if (is_initialized())
	{
		return;
	}

	m_is_initialized = true;

	if (m_materials.empty() || !m_materials[0].is_valid())
	{
		Material mat;
		mat.set_technique(System::inst().get_factory().load_by_name<video::Technique>("primitive.technique"));	 
		if (mat.is_valid())
		{
			Render_State& rs = mat.get_render_state(0);
			rs.set_culling(false);
			rs.set_depth_test(false);
			rs.set_depth_write(false);

			if (m_materials.empty())
			{
				set_material(mat);
			}
			else
			{
				m_materials[0] = mat;
			}
		}
	}
	QASSERT(m_crt_material_idx >= 0);

	{
		auto indexBuffer = m_triangle_index_buffer_pool.get_buffer();
		auto vertexBuffer = m_triangle_vertex_buffer_pool.get_buffer();
		auto lineVertexBuffer = m_line_vertex_buffer_pool.get_buffer();

		m_triangle_vertex_declaration.set_index_buffer(indexBuffer);
		m_triangle_vertex_declaration.set_index_count(MAX_INDICES);
		int idx = m_triangle_vertex_declaration.add_attribute(vertexBuffer, Vertex_Declaration::Semantic::POSITIONS, Vertex_Declaration::Type::FLOAT, 3, 0, 24);
		QASSERT(idx == 0);
		idx = m_triangle_vertex_declaration.add_attribute(vertexBuffer, Vertex_Declaration::Semantic::TEX_COORDS0, Vertex_Declaration::Type::FLOAT, 2, 12, 24);
		QASSERT(idx == 1);
		idx = m_triangle_vertex_declaration.add_attribute(vertexBuffer, Vertex_Declaration::Semantic::COLORS, Vertex_Declaration::Type::FP_U8, 4, 20, 24);
		QASSERT(idx == 2);
		m_triangle_vertex_declaration.set_vertex_count(MAX_VERTICES);
	}
	{
		auto vertexBuffer = m_line_vertex_buffer_pool.get_buffer();

		int idx = m_line_vertex_declaration.add_attribute(vertexBuffer, Vertex_Declaration::Semantic::POSITIONS, Vertex_Declaration::Type::FLOAT, 3, 0, 16);
		QASSERT(idx == 0);
		idx = m_line_vertex_declaration.add_attribute(vertexBuffer, Vertex_Declaration::Semantic::COLORS, Vertex_Declaration::Type::FP_U8, 4, 12, 16);
		QASSERT(idx == 1);
		m_line_vertex_declaration.set_vertex_count(MAX_VERTICES);
	}
	m_end_frame_callback = std::bind(&Painter::flush, this);
	System::inst().get_renderer()->add_end_frame_callback(&m_end_frame_callback);   
}

//////////////////////////////////////////////////////////////////////////

void Painter::draw_polyline(Vertex const* vertices, size_t count)
{
	QASSERT(vertices && count > 2);
	if (!vertices || count < 3)
	{
		return;
	}

	for (size_t i = 0; i < count - 1; i++)
	{
		draw_line(vertices[i], vertices[i + 1]);
	}

	draw_line(vertices[count - 1], vertices[0]); 
}

//////////////////////////////////////////////////////////////////////////

void Painter::fill_triangle_fan(Vertex const* vertices, size_t count)
{
	QASSERT(vertices && count > 2);
	if (!vertices || count < 3)
	{
		return;
	}

	for (size_t i = 1; i < count - 1; i++)
	{
		fill_triangle(vertices[0], vertices[i], vertices[i + 1]);
	} 
}

//////////////////////////////////////////////////////////////////////////

void Painter::draw_rectangle(Vertex const& ul, Vertex const& dr)
{
	float z = (dr.position.z + ul.position.z)*0.5f;
	math::vec3f pur(dr.position.x, ul.position.y, z);
	math::vec2f tur(dr.tex_coord.x, ul.tex_coord.y);

	math::vec3f pdl(ul.position.x, dr.position.y, z);
	math::vec2f tdl(ul.tex_coord.x, dr.tex_coord.y);

	draw_quad(ul, Vertex(pur, tur, ul.color), dr, Vertex(pdl, tdl, ul.color));
}

//////////////////////////////////////////////////////////////////////////

void Painter::fill_rectangle(Vertex const& ul, Vertex const& dr)
{
	float z = (dr.position.z + ul.position.z)*0.5f;
	math::vec3f pur(dr.position.x, ul.position.y, z);
	math::vec2f tur(dr.tex_coord.x, ul.tex_coord.y);

	math::vec3f pdl(ul.position.x, dr.position.y, z);
	math::vec2f tdl(ul.tex_coord.x, dr.tex_coord.y);

	fill_quad(ul, Vertex(pur, tur, ul.color), dr, Vertex(pdl, tdl, ul.color));
}

//////////////////////////////////////////////////////////////////////////

void Painter::draw_quad(Vertex const& v0, Vertex const& v1, Vertex const& v2, Vertex const& v3)
{
	draw_line(v0, v1);
	draw_line(v1, v2);
	draw_line(v2, v3);
	draw_line(v3, v0);
}

//////////////////////////////////////////////////////////////////////////

void Painter::draw_triangle(Vertex const& v0, Vertex const& v1, Vertex const& v2)
{
	draw_line(v0, v1);
	draw_line(v1, v2);
	draw_line(v2, v0);
}

//////////////////////////////////////////////////////////////////////////

void Painter::draw_circle(Vertex const& center, float radius, size_t tesselation)
{
	draw_ellipse(center, math::vec2f(radius), tesselation);
}

//////////////////////////////////////////////////////////////////////////

void Painter::fill_circle(Vertex const& center, float radius, size_t tesselation)
{
	fill_ellipse(center, math::vec2f(radius), tesselation);
}

//////////////////////////////////////////////////////////////////////////

video::Material const& Painter::get_material() const
{
	if (!is_initialized())
	{
		((Painter*)this)->init();
	}

	return m_crt_material_idx >= 0 ? m_materials[m_crt_material_idx] : m_material;
}

//////////////////////////////////////////////////////////////////////////

void Painter::set_camera(scene::Camera const& camera)
{
	if (!m_cameras.empty())
	{
		scene::Camera& oldCamera = m_cameras.back();
		if (oldCamera.get_viewport() == camera.get_viewport() &&
			oldCamera.get_view_projection_matrix() == camera.get_view_projection_matrix())
		{
			return;
		}
	}

	if (m_cameras.size() > 100)
	{
		flush();
	}

	m_cameras.emplace_back(camera);
	QASSERT(m_cameras.size() - 1 < 128);
	m_crt_camera_idx = static_cast<int8_t>(m_cameras.size() - 1);
}

//////////////////////////////////////////////////////////////////////////

math::trans3df const& Painter::get_pre_clip_transform() const
{
	return m_pre_clip_transform;
}

//////////////////////////////////////////////////////////////////////////

math::trans3df const& Painter::get_post_clip_transform() const
{
	return m_post_clip_transform;
}

//////////////////////////////////////////////////////////////////////////

void Painter::draw_line(Vertex const& v0, Vertex const& v1)
{
//	if (m_clippingParameters.clipRectangle == false)
	{
		if (!m_has_combined_transform)
		{
			draw_line_raw(v0, v1);
		}
		else
		{
			Vertex t0 = v0;
			Vertex t1 = v1;
			t0.position = math::transform(m_combined_transform, v0.position);
			t1.position = math::transform(m_combined_transform, v1.position);
			draw_line_raw(t0, t1);
		}
		return;
	}

/*
	const size_t elementSize = 2 + 2 + 4;
	math::vec4f c0 = math::color::u32ToRgbaVec4<float>(v0.color);
	math::vec4f c1 = math::color::u32ToRgbaVec4<float>(v1.color);
	// prepare data
	math::vec3f t0 = v0.position;
	math::vec3f t1 = v1.position;
	if (m_has_pre_clip_transform)
	{
		t0 = math::transform(m_pre_clip_transform, t0);
		t1 = math::transform(m_pre_clip_transform, t1);
	}
	float data[2][elementSize];
	data[0][0] = t0.x;
	data[0][1] = t0.y;
	data[0][2] = v0.texCoord.x;
	data[0][3] = v0.texCoord.y;
	data[0][4] = c0.x;
	data[0][5] = c0.y;
	data[0][6] = c0.z;
	data[0][7] = c0.w;
	data[1][0] = t1.x;
	data[1][1] = t1.y;
	data[1][2] = v1.texCoord.x;
	data[1][3] = v1.texCoord.y;
	data[1][4] = c1.x;
	data[1][5] = c1.y;
	data[1][6] = c1.z;
	data[1][7] = c1.w;

	// clip primitives
	util::RectClipper& clipper = m_clippingParameters.clipper;
	if (m_clippingParameters.enableClipStack)
	{
		clipper.SetRect(m_clippingParameters.clipStack.GetTop());
	}
	clipper.ClipPrimitive(&data[0][0], 2, elementSize);

	if (clipper.m_numberOfOutputPoints == 0)
	{
		return;
	}
	for (size_t i = 0; i < clipper.m_numberOfOutputPoints-1; i++)
	{
		size_t j = (i+1)%clipper.m_numberOfOutputPoints;
		const float* p0 = &clipper.m_output[i*elementSize + 0];
		const float* p1 = &clipper.m_output[j*elementSize + 0];

		math::vec4f c0(p0[4],p0[5],p0[6],p0[7]);
		math::vec4f c1(p1[4],p1[5],p1[6],p1[7]);
		float z = v0.position.z;

		math::vec3f pos0(math::vec3f(p0[0], p0[1], z));
		math::vec3f pos1(math::vec3f(p1[0], p1[1], z));
		if (m_has_post_clip_transform)
		{
			pos0 = math::transform(m_post_clip_transform, pos0);
			pos1 = math::transform(m_post_clip_transform, pos1);
		}

		Vertex a(pos0, math::vec2f(p0[2], p0[3]), math::color::rgbaVec4ToU32(c0));
		Vertex b(pos1, math::vec2f(p1[2], p1[3]), math::color::rgbaVec4ToU32(c1));
		a.flags = v0.flags;
		b.flags = v0.flags;
		drawLineRaw(a, b);
	}
*/
}

//////////////////////////////////////////////////////////////////////////

void Painter::draw_line_raw(Vertex const& s, Vertex const& e)
{
	if (!is_initialized())
	{
		init();
	}

	size_t start = m_bytecode_size;
	m_bytecode_size +=  + 4 + (12 + 4) * 2;
	if (m_bytecode_size > m_bytecode.size())
	{
		m_bytecode.resize(m_bytecode_size);
	}
	uint8_t* ptr = &m_bytecode[start];

	*ptr = (uint8_t)Operation::DRAW_LINE; ptr++;
	*ptr = m_crt_material_idx; ptr++;
	*ptr = m_crt_camera_idx; ptr++;
	ptr++; //alignment

	memcpy(ptr, &s.position, 12); ptr += 12;
	*(uint32_t*)ptr = math::color::swap_rb(s.color); ptr += 4;

	memcpy(ptr, &e.position, 12); ptr += 12;
	*(uint32_t*)ptr = math::color::swap_rb(e.color); ptr += 4;

	size_t size = m_bytecode_size - start;

	if (m_last_operation != (int)Operation::DRAW_LINE)
	{
		Batch_Info info;
		info.operation = Operation::DRAW_LINE;
		info.element_count = 1;
		info.byte_count = size;
		m_batch_infos.emplace_back(info);
	}
	else
	{
		Batch_Info& info = m_batch_infos.back();
		info.element_count++;
		info.byte_count += size;
	}

	m_last_operation = (int)Operation::DRAW_LINE;
}

//////////////////////////////////////////////////////////////////////////

void Painter::draw_ellipse(Vertex const& center, math::vec2f const& radiuses, size_t tesselation)
{
	//const float pi = 3.1415926535f;
	math::vec3f const& c = center.position;
	math::vec2f c2 = math::vec2f(c);
	math::vec2f r = radiuses;
	size_t color = center.color;
	if (r.x > 0.f && r.y > 0)
	{
		if (tesselation < 3)
		{
			tesselation = get_tesselation_from_radius(math::max(r.x, r.y));
		}

		math::vec2f prev(c2 + ms_cos_sin_table[0]*r);

		size_t angleInc = (MAX_SIN_COS_TABLE_SIZE << 16) / tesselation;
		size_t angleIdx = angleInc;
		for (size_t i = 1; i < tesselation; i++)
		{
			math::vec2f pos(c2 + ms_cos_sin_table[angleIdx >> 16]*r);
			draw_line(Vertex(math::vec3f(prev, c.z), color), Vertex(math::vec3f(pos, c.z), color));
			prev = pos;
			angleIdx += angleInc;
			if ((angleIdx >> 16) >= MAX_SIN_COS_TABLE_SIZE)
			{
				angleIdx -= MAX_SIN_COS_TABLE_SIZE << 16;
			}
		}

		//last segment
		math::vec2f pos = c2 + ms_cos_sin_table[0]*r;
		draw_line(Vertex(math::vec3f(prev, c.z), color), Vertex(math::vec3f(pos, c.z), color));
	}
}

//////////////////////////////////////////////////////////////////////////

void Painter::fill_ellipse(Vertex const& center, math::vec2f const& radiuses, size_t tesselation)
{
	if (!is_initialized())
	{
		init();
	}

	//const float pi = 3.1415926535f;
	math::vec3f const& c = center.position;
	math::vec2f t = center.tex_coord;
	math::vec2f c2 = math::vec2f(c);
	math::vec2f r = radiuses;
	uint32_t color = center.color;
	if (r.x > 0.f && r.y > 0)
	{
		if (tesselation < 3)
		{
			tesselation = get_tesselation_from_radius(math::max(r.x, r.y));
		}

		math::vec2f prev(c2 + ms_cos_sin_table[0]*r);
		math::vec2f prev_tc(t + ms_cos_sin_table[0]*0.5f);

		size_t angleInc = (MAX_SIN_COS_TABLE_SIZE << 16) / tesselation;
		size_t angleIdx = angleInc;
		for (size_t i = 1; i < tesselation; i++)
		{
			math::vec2f unitPos(ms_cos_sin_table[angleIdx >> 16]);
			math::vec2f pos(c2 + unitPos*r);
			math::vec2f tc(t + unitPos*0.5f);

			fill_triangle(
				Vertex(c, t, color),
				Vertex(math::vec3f(prev, c.z), prev_tc, color),
				Vertex(math::vec3f(pos, c.z), tc, color));

			prev = pos;
			prev_tc = tc;
			angleIdx += angleInc;
			if ((angleIdx >> 16) >= MAX_SIN_COS_TABLE_SIZE)
			{
				angleIdx -= MAX_SIN_COS_TABLE_SIZE << 16;
			}
		}

		//last segment
		math::vec2f pos = c2 + ms_cos_sin_table[0]*r;
		math::vec2f tc = t + ms_cos_sin_table[0]*0.5f;
		fill_triangle(
			Vertex(c, t, color),
			Vertex(math::vec3f(prev, c.z), prev_tc, color),
			Vertex(math::vec3f(pos, c.z), tc, color));
	}
}

//////////////////////////////////////////////////////////////////////////

void Painter::draw_arc(Vertex const& center, math::anglef start_angle, math::anglef end_angle, float radius, size_t tesselation)
{
	const float pi = 3.1415926535f;
	math::vec3f const& c = center.position;
	math::vec2f c2 = math::vec2f(c);
	uint32_t color = center.color;

    if (start_angle.radians > end_angle.radians)
	{
		end_angle += math::anglef::_2pi;//this is ok
	}
	start_angle = math::anglef::_2pi - start_angle.radians;//ccw
	end_angle = math::anglef::_2pi - end_angle.radians;//ccw
    if (start_angle.radians > end_angle.radians)
	{
		std::swap(start_angle, end_angle);
	}

	float interval = math::abs(end_angle.radians - start_angle.radians);
	const float invMax = 1.f / (float)MAX_SIN_COS_TABLE_SIZE;

	if (interval > 0.0f && radius > 0.0f)
	{
		if (tesselation < 3)
		{
			tesselation = get_tesselation_from_radius(radius);
            tesselation = math::max<size_t>(tesselation * (interval/(2.0f*pi)), 4);// tesselation is in perimeter space so here we apply a ratio because of the arc length
		}

		// compute start,end indices into table
		int32_t start = (int32_t) (start_angle.radians/(2.f*pi*invMax));
		int32_t end = (int32_t) (end_angle.radians/(2.f*pi*invMax));

		QASSERT(end > start);
		
		// work in fixed point
		size_t angle_inc = ((size_t)(MAX_SIN_COS_TABLE_SIZE*(interval/(2.0f*pi))) << 16) / tesselation;
		
		if (start <  0) start = MAX_SIN_COS_TABLE_SIZE + start;
		if (start >=  MAX_SIN_COS_TABLE_SIZE) start = start - MAX_SIN_COS_TABLE_SIZE;
		
		if (end <  0) end = MAX_SIN_COS_TABLE_SIZE + end;
		if (end >=  MAX_SIN_COS_TABLE_SIZE) end = end - MAX_SIN_COS_TABLE_SIZE;
		
		math::vec2f prev(c2 + ms_cos_sin_table[start]*radius);

		size_t angle_idx = start<<16;
		if ((angle_idx >> 16) >= MAX_SIN_COS_TABLE_SIZE)
		{
			angle_idx -= MAX_SIN_COS_TABLE_SIZE << 16;
		}

		for (size_t i = 1; i < tesselation; i++)
		{
			math::vec2f pos(c2 + ms_cos_sin_table[angle_idx>>16]*radius);
			draw_line(Vertex(math::vec3f(prev, c.z), color), Vertex(math::vec3f(pos, c.z), color));
			prev = pos;
			angle_idx += angle_inc;
			if ((angle_idx >> 16) >= MAX_SIN_COS_TABLE_SIZE)
			{
				angle_idx -= MAX_SIN_COS_TABLE_SIZE << 16;
			}
		}

		//last segment
		math::vec2f pos = c2 + ms_cos_sin_table[end]*radius;
		draw_line(Vertex(math::vec3f(prev, c.z), color), Vertex(math::vec3f(pos, c.z), color));
	}
}

//////////////////////////////////////////////////////////////////////////

void Painter::fill_arc(Vertex const& center, math::anglef start_angle, math::anglef end_angle, float radius, size_t tesselation)
{
	if (!is_initialized())
	{
		init();
	}

	const float pi = 3.1415926535f;
	math::vec3f const& c = center.position;
	math::vec2f c2 = math::vec2f(c);
	math::vec2f t = center.tex_coord;
	uint32_t color = center.color;

    if (start_angle.radians > end_angle.radians)
	{
		end_angle += math::anglef::_2pi;//this is ok
	}
	start_angle = math::anglef::_2pi - start_angle.radians;//ccw
	end_angle = math::anglef::_2pi - end_angle.radians;//ccw
    if (start_angle.radians > end_angle.radians)
	{
		std::swap(start_angle, end_angle);
	}

	float interval = math::abs(end_angle.radians - start_angle.radians);
	const float invMax = 1.f / (float)MAX_SIN_COS_TABLE_SIZE;

	if (interval > 0.0f && radius > 0.0f)
	{
		if (tesselation < 3)
		{
			tesselation = get_tesselation_from_radius(radius);
            tesselation = math::max<size_t>(tesselation * (interval/(2.0f*pi)), 4);// tesselation is in perimeter space so here we apply a ratio because of the arc length
		}

		// compute start,end indices into table
		int32_t start = (int32_t) (start_angle.radians/(2.f*pi*invMax));
		int32_t end = (int32_t) (end_angle.radians/(2.f*pi*invMax));

		QASSERT(end > start);

		// work in fixed point
		size_t angle_inc = ((size_t)(MAX_SIN_COS_TABLE_SIZE*(interval/(2.0f*pi))) << 16) / tesselation;

		while (start <  0) start = MAX_SIN_COS_TABLE_SIZE + start;
		while (start >=  MAX_SIN_COS_TABLE_SIZE) start = start - MAX_SIN_COS_TABLE_SIZE;

		while (end <  0) end = MAX_SIN_COS_TABLE_SIZE + end;
		while (end >=  MAX_SIN_COS_TABLE_SIZE) end = end - MAX_SIN_COS_TABLE_SIZE;
		
		math::vec2f prev(c2 + ms_cos_sin_table[start]*radius);
		math::vec2f prev_tc(t + ms_cos_sin_table[start]*0.5f);

		size_t angle_idx = start<<16;
		if ((angle_idx >> 16) >= MAX_SIN_COS_TABLE_SIZE)
		{
			angle_idx -= MAX_SIN_COS_TABLE_SIZE << 16;
		}

		for (size_t i = 1; i < tesselation; i++)
		{
			math::vec2f unit_pos(ms_cos_sin_table[angle_idx >> 16]);
			math::vec2f pos(c2 + unit_pos*radius);
			math::vec2f tc(t + unit_pos*0.5f);

			fill_triangle(
				Vertex(c, t, color),
				Vertex(math::vec3f(prev, c.z), prev_tc, color), 
				Vertex(math::vec3f(pos, c.z), tc, color));

			prev = pos;
			prev_tc = tc;
			angle_idx += angle_inc;
			if ((angle_idx >> 16) >= MAX_SIN_COS_TABLE_SIZE)
			{
				angle_idx -= MAX_SIN_COS_TABLE_SIZE << 16;
			}
		}

		//last segment
		math::vec2f pos = c2 + ms_cos_sin_table[end]*radius;
		math::vec2f tc = t + ms_cos_sin_table[end]*0.5f;
		fill_triangle(
			Vertex(c, t, color),
			Vertex(math::vec3f(prev, c.z), prev_tc, color), 
			Vertex(math::vec3f(pos, c.z), tc, color));
	}
}

//////////////////////////////////////////////////////////////////////////

void Painter::draw_box(math::aabb3f const& box, uint32_t color)
{
//	return;

	math::vec3f edges[8];
	box.get_edges(edges);

	/*
		Edges are stored in this way:
		Hey, am I an ascii artist, or what? :) niko.
              /3--------/7
             /  |      / |
            /   |     /  |
            1---------5  |
            |   2- - -| -6
            |  /      |  /
            |/        | /
            0---------4/
    */

	draw_line(Vertex(edges[0], color), Vertex(edges[1], color));
	draw_line(Vertex(edges[0], color), Vertex(edges[4], color));
	draw_line(Vertex(edges[4], color), Vertex(edges[5], color));
	draw_line(Vertex(edges[1], color), Vertex(edges[5], color));

	draw_line(Vertex(edges[2], color), Vertex(edges[6], color));
	draw_line(Vertex(edges[2], color), Vertex(edges[3], color));
	draw_line(Vertex(edges[3], color), Vertex(edges[7], color));
	draw_line(Vertex(edges[6], color), Vertex(edges[7], color));

	draw_line(Vertex(edges[1], color), Vertex(edges[3], color));
	draw_line(Vertex(edges[5], color), Vertex(edges[7], color));
	draw_line(Vertex(edges[4], color), Vertex(edges[6], color));
	draw_line(Vertex(edges[0], color), Vertex(edges[2], color));
}

//////////////////////////////////////////////////////////////////////////
//Renders axis in +X +Y +Z
void Painter::draw_axis(math::trans3df const& m)
{
	math::vec3f pos = m.get_translation();
	draw_line(Vertex(pos, 0xFFFF0000), Vertex(pos + m.get_axis_x(), 0xFFFF0000));
	draw_line(Vertex(pos, 0xFF00FF00), Vertex(pos + m.get_axis_y(), 0xFF00FF00));
	draw_line(Vertex(pos, 0xFF0000FF), Vertex(pos + m.get_axis_z(), 0xFF0000FF));
}

//////////////////////////////////////////////////////////////////////////

void Painter::draw_axis(math::quatf const& q, math::vec3f const& pos)
{
	math::trans3df m(math::vec3f::zero, q, math::vec3f::one);
	draw_line(Vertex(pos, 0xFFFF0000), Vertex(pos + m.get_axis_x(), 0xFFFF0000));
	draw_line(Vertex(pos, 0xFF00FF00), Vertex(pos + m.get_axis_y(), 0xFF00FF00));
	draw_line(Vertex(pos, 0xFF0000FF), Vertex(pos + m.get_axis_z(), 0xFF0000FF));
}	

//////////////////////////////////////////////////////////////////////////

inline void computeMinMax(float value, float& min_value, float& max_value)
{
	if (value < min_value) 
	{
		min_value = value;
	}
	else if (value > max_value) 
	{
		max_value = value;
	}
}

//////////////////////////////////////////////////////////////////////////

inline bool testSeparateAxis(float min0, float max0, float min1, float max1)
{
	return (max0 < min1 || max1 < min0);
}

//////////////////////////////////////////////////////////////////////////

void Painter::fill_quad(Vertex const& v0, Vertex const& v1, Vertex const& v2, Vertex const& v3)
{
//	return;

/*
	if (m_clippingParameters.clipRectangle)
	{
		if (!m_clippingParameters.enableClipStack)
		{
			fillTriangle(v0, v1, v2);
			fillTriangle(v0, v2, v3);
			return;
		}
		// perform SAT
		// compute primitive projection intervals in screen axes
		math::vec2f vmin(FLT_MAX, FLT_MAX);
		math::vec2f vmax(-FLT_MAX, -FLT_MAX);

		math::rectf cr = GetClippingRectangle();
		if (m_has_pre_clip_transform)
		{
			math::vec2f p0, p1, p2, p3;
			p0.x = m_pre_clip_transform.dotColumn(0, v0.position);
			p1.x = m_pre_clip_transform.dotColumn(0, v1.position);
			p2.x = m_pre_clip_transform.dotColumn(0, v2.position);
			p3.x = m_pre_clip_transform.dotColumn(0, v3.position);
			vmin.x = vmax.x = p0.x;
			//ComputeMinMax(p0.x, vmin.x, vmax.x);
			ComputeMinMax(p1.x, vmin.x, vmax.x);
			ComputeMinMax(p2.x, vmin.x, vmax.x);
			ComputeMinMax(p3.x, vmin.x, vmax.x);
			if (TestSeparateAxis(vmin.x, vmax.x, cr.upperLeftCorner.x, cr.lowerRightCorner.x))
			{
				return;//early out
			}
			p0.y = m_pre_clip_transform.dotColumn(1, v0.position);
			p1.y = m_pre_clip_transform.dotColumn(1, v1.position);
			p2.y = m_pre_clip_transform.dotColumn(1, v2.position);
			p3.y = m_pre_clip_transform.dotColumn(1, v3.position);
			vmin.y = vmax.y = p0.y;
			//ComputeMinMax(p0.y, vmin.y, vmax.y);
			ComputeMinMax(p1.y, vmin.y, vmax.y);
			ComputeMinMax(p2.y, vmin.y, vmax.y);
			ComputeMinMax(p3.y, vmin.y, vmax.y);
			if (TestSeparateAxis(vmin.y, vmax.y, cr.upperLeftCorner.y, cr.lowerRightCorner.y))
			{
				return;//early out
			}
		}
		else
		{
			vmin.x = vmax.x = v0.position.x;
			//ComputeMinMax(v0.position.x, vmin.x, vmax.x);
			ComputeMinMax(v1.position.x, vmin.x, vmax.x);
			ComputeMinMax(v2.position.x, vmin.x, vmax.x);
			ComputeMinMax(v3.position.x, vmin.x, vmax.x);
			if (TestSeparateAxis(vmin.x, vmax.x, cr.upperLeftCorner.x, cr.lowerRightCorner.x))
			{
				return;//early out
			}
			vmin.y = vmax.y = v0.position.y;
			//ComputeMinMax(v0.position.y, vmin.y, vmax.y);
			ComputeMinMax(v1.position.y, vmin.y, vmax.y);
			ComputeMinMax(v2.position.y, vmin.y, vmax.y);
			ComputeMinMax(v3.position.y, vmin.y, vmax.y);
			if (TestSeparateAxis(vmin.y, vmax.y, cr.upperLeftCorner.y, cr.lowerRightCorner.y))
			{
				return;//early out
			}
		}
		if (vmin.x > cr.upperLeftCorner.x && vmax.x < cr.lowerRightCorner.x && 
			vmin.y > cr.upperLeftCorner.y && vmax.y < cr.lowerRightCorner.y)
		{
			// early in
		}
		else
		{
			fillTriangle(v0, v1, v2);
			fillTriangle(v0, v2, v3);
			return;
		}
	}
*/
	if (!is_initialized())
	{
		init();
	}

	size_t start = m_bytecode_size;
	m_bytecode_size += 4 + (12 + 8 + 4) * 4;
	if (m_bytecode_size > m_bytecode.size())
	{
		m_bytecode.resize(m_bytecode_size);
	}
	uint8_t* ptr = &m_bytecode[start];

	*ptr = (uint8_t)Operation::FILL_QUAD; ptr++;
	*ptr = m_crt_material_idx; ptr++;
	*ptr = m_crt_camera_idx; ptr++;
	ptr++; //alignment

	if (m_has_combined_transform)
	{
		*(math::vec3f*)ptr = math::transform(m_combined_transform, v0.position); ptr += 12;
		*(math::vec2f*)ptr = v0.tex_coord; ptr += 8;
		*(uint32_t*)ptr = math::color::swap_rb(v0.color); ptr += 4;

		*(math::vec3f*)ptr = math::transform(m_combined_transform, v0.position); ptr += 12;
		*(math::vec2f*)ptr = v1.tex_coord; ptr += 8;
		*(uint32_t*)ptr = math::color::swap_rb(v1.color); ptr += 4;

		*(math::vec3f*)ptr = math::transform(m_combined_transform, v0.position); ptr += 12;
		*(math::vec2f*)ptr = v2.tex_coord; ptr += 8;
		*(uint32_t*)ptr = math::color::swap_rb(v2.color); ptr += 4;

		*(math::vec3f*)ptr = math::transform(m_combined_transform, v0.position); ptr += 12;
		*(math::vec2f*)ptr = v3.tex_coord; ptr += 8;
		*(uint32_t*)ptr = math::color::swap_rb(v3.color); ptr += 4;
	}
	else
	{
		*(math::vec3f*)ptr = v0.position; ptr += 12;
		*(math::vec2f*)ptr = v0.tex_coord; ptr += 8;
		*(uint32_t*)ptr = math::color::swap_rb(v0.color); ptr += 4;

		*(math::vec3f*)ptr = v1.position; ptr += 12;
		*(math::vec2f*)ptr = v1.tex_coord; ptr += 8;
		*(uint32_t*)ptr = math::color::swap_rb(v1.color); ptr += 4;

		*(math::vec3f*)ptr = v2.position; ptr += 12;
		*(math::vec2f*)ptr = v2.tex_coord; ptr += 8;
		*(uint32_t*)ptr = math::color::swap_rb(v2.color); ptr += 4;

		*(math::vec3f*)ptr = v3.position; ptr += 12;
		*(math::vec2f*)ptr = v3.tex_coord; ptr += 8;
		*(uint32_t*)ptr = math::color::swap_rb(v3.color); ptr += 4;
	}

	size_t size = m_bytecode_size - start;

	if (m_last_operation < 0 || m_last_operation == (int)Operation::DRAW_LINE)
	{
		Batch_Info info;
		info.operation = Operation::FILL_QUAD;
		info.element_count = 1;
		info.byte_count = size;
		m_batch_infos.emplace_back(info);
	}
	else
	{
		Batch_Info& info = m_batch_infos.back();
		info.element_count++;
		info.byte_count += size;
	}

	m_last_operation = (int)Operation::FILL_QUAD;
}

//////////////////////////////////////////////////////////////////////////

void Painter::fill_triangle(Vertex const& v0, Vertex const& v1, Vertex const& v2)
{
	//if (m_clippingParameters.clipRectangle == false)
	{
		if (!m_has_combined_transform)
		{
			fill_triangle_raw(v0, v1, v2);
		}
		else
		{
			Vertex t0 = v0;
			Vertex t1 = v1;
			Vertex t2 = v2;
			t0.position = math::transform(m_combined_transform, v0.position);
			t1.position = math::transform(m_combined_transform, v1.position);
			t2.position = math::transform(m_combined_transform, v2.position);
			fill_triangle_raw(t0, t1, t2);
		}
		return;
	}
/*
	if (m_clippingParameters.enableClipStack && m_clippingParameters.clipStack.GetSize() == 0)
	{
		return;
	}
	const size_t elementSize = 2 + 2 + 4;
	math::vec4f c0 = math::color::u32ToRgbaVec4<float>(v0.color);
	math::vec4f c1 = math::color::u32ToRgbaVec4<float>(v1.color);
	math::vec4f c2 = math::color::u32ToRgbaVec4<float>(v2.color);

	math::vec3f t0 = v0.position;
	math::vec3f t1 = v1.position;
	math::vec3f t2 = v2.position;
	if (m_has_pre_clip_transform)
	{
		t0 = math::transform(m_pre_clip_transform, t0);
		t1 = math::transform(m_pre_clip_transform, t1);
		t2 = math::transform(m_pre_clip_transform, t2);
	}

	// prepare data
	float data[3][elementSize];
	data[0][0] = t0.x;
	data[0][1] = t0.y;
	data[0][2] = v0.texCoord.x;
	data[0][3] = v0.texCoord.y;
	data[0][4] = c0.x;
	data[0][5] = c0.y;
	data[0][6] = c0.z;
	data[0][7] = c0.w;
	data[1][0] = t1.x;
	data[1][1] = t1.y;
	data[1][2] = v1.texCoord.x;
	data[1][3] = v1.texCoord.y;
	data[1][4] = c1.x;
	data[1][5] = c1.y;
	data[1][6] = c1.z;
	data[1][7] = c1.w;
	data[2][0] = t2.x;
	data[2][1] = t2.y;
	data[2][2] = v2.texCoord.x;
	data[2][3] = v2.texCoord.y;
	data[2][4] = c2.x;
	data[2][5] = c2.y;
	data[2][6] = c2.z;
	data[2][7] = c2.w;

	util::RectClipper& clipper = m_clippingParameters.clipper;
	if (m_clippingParameters.enableClipStack)
	{
		clipper.SetRect(m_clippingParameters.clipStack.GetTop());
	}
	clipper.ClipPrimitive(&data[0][0], 3, elementSize);
	if (clipper.m_numberOfOutputPoints == 0)
	{
		return;
	}

	size_t tris = clipper.m_numberOfOutputPoints - 2;
	for (size_t i = 0; i < tris; i++)
	{
		size_t index0 = 0;
		size_t index1 = (i+2) % clipper.m_numberOfOutputPoints;
		size_t index2 = (i+1) % clipper.m_numberOfOutputPoints;

		const float* p0 = &clipper.m_output[index0*elementSize + 0];
		const float* p1 = &clipper.m_output[index1*elementSize + 0];
		const float* p2 = &clipper.m_output[index2*elementSize + 0];
		
		math::vec4f c0(p0[4],p0[5],p0[6],p0[7]);
		math::vec4f c1(p1[4],p1[5],p1[6],p1[7]);
		math::vec4f c2(p2[4],p2[5],p2[6],p2[7]);
		float z = v0.position.z;

		math::vec3f pos0(math::vec3f(p0[0], p0[1], z));
		math::vec3f pos1(math::vec3f(p1[0], p1[1], z));
		math::vec3f pos2(math::vec3f(p2[0], p2[1], z));
		if (m_has_post_clip_transform)
		{
			pos0 = math::transform(m_post_clip_transform, pos0);
			pos1 = math::transform(m_post_clip_transform, pos1);
			pos2 = math::transform(m_post_clip_transform, pos2);
		}

		Vertex a(pos0, math::vec2f(p0[2], p0[3]), math::color::rgbaVec4ToU32(c0));
		Vertex b(pos1, math::vec2f(p1[2], p1[3]), math::color::rgbaVec4ToU32(c1));
		Vertex c(pos2, math::vec2f(p2[2], p2[3]), math::color::rgbaVec4ToU32(c2));
		a.flags = v0.flags;
		b.flags = v0.flags;
		c.flags = v0.flags;
		fillTriangleRaw(a,b,c);
	}
*/
}

//////////////////////////////////////////////////////////////////////////

void Painter::fill_triangle_raw(Vertex const& v0, Vertex const& v1, Vertex const& v2)
{
	if (!is_initialized())
	{
		init();
	}

	size_t start = m_bytecode_size;
	m_bytecode_size += 4 + (12 + 8 + 4) * 3;
	if (m_bytecode_size > m_bytecode.size())
	{
		m_bytecode.resize(m_bytecode_size);
	}
	uint8_t* ptr = &m_bytecode[start];

	*ptr = (uint8_t)Operation::FILL_TRI; ptr++;
	*ptr = m_crt_material_idx; ptr++;
	*ptr = m_crt_camera_idx; ptr++;
	ptr++; //alignment

	memcpy(ptr, &v0.position, 12); ptr += 12;
	memcpy(ptr, &v0.tex_coord, 8); ptr += 8;
	*(uint32_t*)ptr = math::color::swap_rb(v0.color); ptr += 4;
	memcpy(ptr, &v1.position, 12); ptr += 12;
	memcpy(ptr, &v1.tex_coord, 8); ptr += 8;
	*(uint32_t*)ptr = math::color::swap_rb(v1.color); ptr += 4;
	memcpy(ptr, &v2.position, 12); ptr += 12;
	memcpy(ptr, &v2.tex_coord, 8); ptr += 8;
	*(uint32_t*)ptr = math::color::swap_rb(v2.color); ptr += 4;

	size_t size = m_bytecode_size - start;

	if (m_last_operation < 0 || m_last_operation == (int)Operation::DRAW_LINE)
	{
		Batch_Info info;
		info.operation = Operation::FILL_TRI;
		info.element_count = 1;
		info.byte_count = size;
		m_batch_infos.emplace_back(info);
	}
	else
	{
		Batch_Info& info = m_batch_infos.back();
		info.element_count++;
		info.byte_count += size;
	}

	m_last_operation = (int)Operation::FILL_TRI;
}

//////////////////////////////////////////////////////////////////////////

void Painter::draw_line_list(Vertex const* vertices, size_t vertex_count, uint32_t const* indices, size_t index_count)
{
	QASSERT(vertices && indices);
	QASSERT(vertex_count > 0 && index_count > 0);

	size_t line_count = index_count / 2;
	for (size_t i = 0; i < line_count; i++)
	{
		uint32_t i0 = indices[i*2 + 0];
		uint32_t i1 = indices[i*2 + 1];
		QASSERT(i0 < vertex_count && i1 < vertex_count);
		draw_line(vertices[i0], vertices[i1]);
	}
}

//////////////////////////////////////////////////////////////////////////

void Painter::draw_triangle_list(Vertex const* vertices, size_t vertex_count, uint32_t const* indices, size_t index_count)
{
	QASSERT(vertices && indices);
	QASSERT(vertex_count > 0 && index_count > 0);

	size_t triangle_count = index_count / 3;
	for (size_t i = 0; i < triangle_count; i++)
	{
		uint32_t i0 = indices[i*3 + 0];
		uint32_t i1 = indices[i*3 + 1];
		uint32_t i2 = indices[i*3 + 2];
		QASSERT(i0 < vertex_count && i1 < vertex_count && i2 < vertex_count);
		draw_triangle(vertices[i0], vertices[i1], vertices[i2]);
	}
}

//////////////////////////////////////////////////////////////////////////

void Painter::fill_triangle_list(Vertex const* vertices, size_t vertex_count, uint32_t const* indices, size_t index_count)
{
	QASSERT(vertices && indices);
	QASSERT(vertex_count > 0 && index_count > 0);

	size_t triangle_count = index_count / 3;
	for (size_t i = 0; i < triangle_count; i++)
	{
		uint32_t i0 = indices[i*3 + 0];
		uint32_t i1 = indices[i*3 + 1];
		uint32_t i2 = indices[i*3 + 2];
		QASSERT(i0 < vertex_count && i1 < vertex_count && i2 < vertex_count);
		fill_triangle(vertices[i0], vertices[i1], vertices[i2]);
	}
}

//////////////////////////////////////////////////////////////////////////

void Painter::fill_sphere(Vertex const& center, float radius, size_t tesselation)
{
	if (!is_initialized())
	{
		init();
	}
	if (math::is_zero(radius))
	{
		return;
	}
	//const float pi = 3.1415926535f;
	if (tesselation < 1)
	{
		tesselation = 1;
	}

	const float x = .525731112119133606f;
	const float z = .850650808352039932f;

	static float vdata[12][3] = 
	{
		{-x, 0.0, z}, {x, 0.0, z}, {-x, 0.0, -z}, {x, 0.0, -z},
		{0.0, z, x}, {0.0, z, -x}, {0.0, -z, x}, {0.0, -z, -x},
		{z, x, 0.0}, {-z, x, 0.0}, {z, -x, 0.0}, {-z, -x, 0.0}
	};

	static uint16_t tindices[20][3] = 
	{
		{0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
		{8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
		{7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
		{6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} 
	};

	std::vector<math::vec3f> vertices;
	vertices.reserve(1024);
	// init with an icosahedron
	for (size_t i = 0; i < 12; i++)
	{
		vertices.emplace_back(vdata[i][0], vdata[i][1], vdata[i][2]);
	}

	std::vector<std::vector<uint32_t> > all_indices;
	all_indices.resize(tesselation);

	for (int i = 0; i < 20; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			all_indices[0].push_back(tindices[i][k]);
		}
	}

	size_t level = 1;
	while (level < tesselation)
	{
        //typedef unsigned long long Key;
		//std::map<Key, uint32_t> edgeMap;
		const std::vector<uint32_t>& indices = all_indices[level - 1];
		std::vector<uint32_t>& refined_indices = all_indices[level];
		level++;

		refined_indices.reserve(indices.size() * 4);
		size_t end = indices.size();
		for (size_t i = 0; i < end; i += 3)
		{
			uint32_t ids0[3],  // indices of outer vertices
				ids1[3];  // indices of edge vertices
			for (uint32_t k = 0; k < 3; ++k)
			{
				uint32_t k1 = (k + 1)%3;
				uint32_t e0 = indices[i + k];
				uint32_t e1 = indices[i + k1];
				ids0[k] = e0;
				if (e1 > e0)
				{
					std::swap(e0, e1);
				}
				//Key edgeKey = Key(e0) | (Key(e1)<<32);
				//std::map<Key, uint32_t>::iterator it = edgeMap.find(edgeKey);
				//if (it == edgeMap.end())
				{
					ids1[k] = vertices.size();
					//edgeMap[edgeKey] = ids1[k];
					vertices.emplace_back(math::normalized(vertices[e0] + vertices[e1]));
				}
				//else
				{
				//	ids1[k] = it->second;
				}
			}
			refined_indices.push_back(ids0[0]); refined_indices.push_back(ids1[0]); refined_indices.push_back(ids1[2]);
			refined_indices.push_back(ids0[1]); refined_indices.push_back(ids1[1]); refined_indices.push_back(ids1[0]);
			refined_indices.push_back(ids0[2]); refined_indices.push_back(ids1[2]); refined_indices.push_back(ids1[1]);
			refined_indices.push_back(ids1[0]); refined_indices.push_back(ids1[1]); refined_indices.push_back(ids1[2]);
		}
	}

	std::vector<Vertex> v(vertices.size());
	for (size_t i = 0; i < vertices.size(); i++)
	{
		v[i].set(center.get_position() + vertices[i] * radius, center.get_color());
	}

	fill_triangle_list(&v[0], v.size(), &all_indices.back()[0], all_indices.back().size());
}

//////////////////////////////////////////////////////////////////////////

void Painter::fill_cone(Vertex const& base_center, float base_radius, float height, size_t tesselation)
{
	if (!is_initialized())
	{
		init();
	}
	if (math::is_zero(base_radius))
	{
		return;
	}
	if (math::is_zero(height)) 
	{
		fill_circle(base_center, base_radius, tesselation);
		return;
	}

	//const float pi = 3.1415926535f;
	math::vec3f const& c = base_center.position;
	math::vec3f hc = base_center.position + math::vec3f(0, 0, height);
	math::vec2f t = base_center.tex_coord;
	math::vec2f c2 = math::vec2f(c);
	float r = base_radius;
	uint32_t color = base_center.color;
	if (tesselation < 3)
	{
		tesselation = get_tesselation_from_radius(base_radius);
	}

	math::vec2f prev(c2 + ms_cos_sin_table[0]*r);
	math::vec2f prev_tc(t + ms_cos_sin_table[0]*0.5f);

	size_t angle_inc = (MAX_SIN_COS_TABLE_SIZE << 16) / tesselation;
	size_t angle_idx = angle_inc;
	for (size_t i = 1; i < tesselation; i++)
	{
		math::vec2f unit_pos(ms_cos_sin_table[angle_idx >> 16]);
		math::vec2f pos(c2 + unit_pos*r);
		math::vec2f tc(t + unit_pos*0.5f);

		fill_triangle(
			Vertex(c, t, color),
			Vertex(math::vec3f(pos, c.z), prev_tc, color),
			Vertex(math::vec3f(prev, c.z), tc, color));

		fill_triangle(
			Vertex(hc, t, color),
			Vertex(math::vec3f(prev, c.z), prev_tc, color),
			Vertex(math::vec3f(pos, c.z), tc, color));

		prev = pos;
		prev_tc = tc;
		angle_idx += angle_inc;
		if ((angle_idx >> 16) >= MAX_SIN_COS_TABLE_SIZE)
		{
			angle_idx -= MAX_SIN_COS_TABLE_SIZE << 16;
		}
	}

	//last segment
	math::vec2f pos = c2 + ms_cos_sin_table[0]*r;
	math::vec2f tc = t + ms_cos_sin_table[0]*0.5f;
	fill_triangle(
		Vertex(c, t, color),
		Vertex(math::vec3f(pos, c.z), prev_tc, color),
		Vertex(math::vec3f(prev, c.z), tc, color));
	fill_triangle(
		Vertex(hc, t, color),
		Vertex(math::vec3f(prev, c.z), prev_tc, color),
		Vertex(math::vec3f(pos, c.z), tc, color));
}

//////////////////////////////////////////////////////////////////////////

void Painter::set_texture(video::Texture_cptr const& texture)
{
	if (m_materials.empty())
	{
		return;
	}
	auto const& mat = m_materials.back();
	if (mat.get_sampler_count(0) == 0)
	{
		return;
	}
	if (mat.get_sampler(0, 0).get_texture() != texture)
	{
		if (m_materials.size() > 100)
		{
			flush();
		}
		else
		{
			m_materials.push_back(mat);
		}
		
		auto const& mat = m_materials.back();
		video::Sampler s = mat.get_sampler(0, 0);
		s.set_texture(texture);
		m_materials.back().set_sampler(0, 0, s);
		QASSERT(m_materials.size() - 1 < 128);
		m_crt_material_idx = static_cast<int8_t>(m_materials.size() - 1);
	}
}

//////////////////////////////////////////////////////////////////////////

void Painter::set_material(video::Material const& material)
{
	if (m_materials.empty() || material != get_material())
	{
		if (m_materials.size() > 100)
		{
			flush();
		}

		m_materials.emplace_back(material);
		QASSERT(m_materials.size() - 1 < 128);
		m_crt_material_idx = static_cast<int8_t>(m_materials.size() - 1);
	}
}

//////////////////////////////////////////////////////////////////////////

void Painter::set_pre_clip_transform(math::trans3df const& transform)
{
	m_pre_clip_transform = transform;
	m_has_pre_clip_transform = !math::is_identity(transform);
	if (m_has_pre_clip_transform || m_has_post_clip_transform)
	{
		math::multiply(m_combined_transform, m_pre_clip_transform, m_post_clip_transform);
		m_has_combined_transform = true;
	}
	else
	{
		m_has_combined_transform = false;
	}
}

//////////////////////////////////////////////////////////////////////////

void Painter::set_post_clip_transform(math::trans3df const& transform)
{
	m_post_clip_transform = transform;
	m_has_post_clip_transform = !math::is_identity(transform);
	if (m_has_pre_clip_transform || m_has_post_clip_transform)
	{
		math::multiply(m_combined_transform, m_pre_clip_transform, m_post_clip_transform);
		m_has_combined_transform = true;
	}
	else
	{
		m_has_combined_transform = false;
	}
}

//////////////////////////////////////////////////////////////////////////

void Painter::flush()
{
	if (m_bytecode_size > 0 && !m_batch_infos.empty())
	{
		m_bytecode_idx = 0;
		m_batch_infos_idx = 0;
		do
		{
			Operation op = m_batch_infos[m_batch_infos_idx].operation;
			if (op == Operation::DRAW_LINE)
			{
				add_lines();
			}
			else
			{
				add_quads_and_tris();
			}
		} while(m_batch_infos_idx < m_batch_infos.size());
	}

	m_last_operation = -1;
	m_batch_infos.clear();
	m_bytecode_size = 0;
	m_bytecode_idx = 0;
	
	if (!m_materials.empty())
	{
		m_materials[0] = m_materials.back();
	}
	m_materials.resize(1);
	m_crt_material_idx = 0;
	
	if (m_crt_camera_idx >= 0 && !m_cameras.empty())
	{
		m_cameras[0] = m_cameras.back();
		m_cameras.resize(1);
		m_crt_camera_idx = 0;
	}
	else
	{
		m_cameras.clear();
		m_crt_camera_idx = -1;
	}
}

size_t s_painterDips;

//////////////////////////////////////////////////////////////////////////

void Painter::add_lines()
{
	size_t start_pos = m_bytecode_idx;
	size_t end_pos = start_pos + m_batch_infos[m_batch_infos_idx].byte_count;
    size_t count = math::min<size_t>(m_batch_infos[m_batch_infos_idx].element_count, 32767);
	m_batch_infos_idx++;
	m_bytecode_idx = end_pos;

	const uint8_t* data_ptr = &m_bytecode[start_pos];

	QASSERT(count > 0);

	Vertex_Buffer_ptr buffer = m_line_vertex_buffer_pool.get_buffer();
	if (!buffer)
	{
		return;
	}

	m_line_vertex_declaration.replace_vertex_buffer(0, buffer);
	m_line_vertex_declaration.set_vertex_count(count*2);

	auto dst_ptr = m_line_vertex_declaration.lock_for_writing<math::vec3f>(0);
	auto col_ptr = m_line_vertex_declaration.lock_for_writing<math::vec4u8>(1);
	QASSERT(dst_ptr.is_valid() && col_ptr.is_valid());
    (void)col_ptr;

#ifndef NDEBUG
	size_t pos_stride = m_line_vertex_declaration.get_attribute(0).stride;
	QASSERT(pos_stride == 16);
#endif

	size_t render_job_count = 0;
	m_render_jobs.resize(100);

	int8_t old_material_idx = -1, old_camera_idx = -1;

	size_t start_index_count = 0;
	size_t index_count = 0;

	//size_t index = 0;
	for (size_t i = 0; i < count; i++)
	{
		/*auto op = Operation(*(uint8_t*)dataPtr); */data_ptr += sizeof(uint8_t);
		int8_t material_idx = *(int8_t*)data_ptr; data_ptr += sizeof(int8_t);
		int8_t camera_idx = *(int8_t*)data_ptr; data_ptr += sizeof(int8_t);
		data_ptr += sizeof(int8_t);

		if (i > 0)
		{
			//flush old data if it's not compatible with the new one
			if (material_idx != old_material_idx || camera_idx != old_camera_idx)
			{
				if (render_job_count >= m_render_jobs.size())
				{
					m_render_jobs.resize(m_render_jobs.size() + 10);
				}
				Render_Job_Data& job_data = m_render_jobs[render_job_count++];
				job_data.job.set_vertex_declaration(m_line_vertex_declaration);
				job_data.job.get_vertex_declaration().set_topology(video::Vertex_Declaration::Topology::LINE_LIST);
				job_data.job.get_vertex_declaration().set_range(start_index_count, index_count - start_index_count);
				job_data.job.set_material(m_materials[old_material_idx]);
				job_data.material_idx = old_material_idx;
				job_data.camera_idx = old_camera_idx;

				start_index_count = index_count;
			}
		}

		old_material_idx = material_idx;
		old_camera_idx = camera_idx;

		size_t size = (12 + 4) * 2;
		memcpy(dst_ptr.get(), data_ptr, size);
		dst_ptr += 2;
		data_ptr += size;

		index_count += 2;
	}

	if (index_count - start_index_count > 0)
	{
		if (render_job_count >= m_render_jobs.size())
		{
			m_render_jobs.resize(m_render_jobs.size() + 10);
		}
		Render_Job_Data& job_data = m_render_jobs[render_job_count++];
		job_data.job.set_vertex_declaration(m_line_vertex_declaration);
		job_data.job.set_material(m_materials[old_material_idx]);
		job_data.job.get_vertex_declaration().set_topology(video::Vertex_Declaration::Topology::LINE_LIST);
		job_data.job.get_vertex_declaration().set_range(start_index_count, index_count - start_index_count);
		job_data.material_idx = old_material_idx;
		job_data.camera_idx = old_camera_idx;
	}

	m_line_vertex_declaration.unlock_all();
	flush_list(m_render_jobs, render_job_count);
}

//////////////////////////////////////////////////////////////////////////

void Painter::add_quads_and_tris()
{
	size_t start_pos = m_bytecode_idx;
	size_t end_pos = start_pos + m_batch_infos[m_batch_infos_idx].byte_count;
    size_t count = math::min<size_t>(m_batch_infos[m_batch_infos_idx].element_count, 16384);
	m_batch_infos_idx++;
	m_bytecode_idx = end_pos;

	const uint8_t* data_ptr = &m_bytecode[start_pos];

	QASSERT(count > 0);

	Index_Buffer_ptr index_buffer = m_triangle_index_buffer_pool.get_buffer();
	Vertex_Buffer_ptr vertex_buffer = m_triangle_vertex_buffer_pool.get_buffer();
	if (!index_buffer || !vertex_buffer)
	{
		return;
	}

	m_triangle_vertex_declaration.set_index_buffer(index_buffer);
	m_triangle_vertex_declaration.replace_vertex_buffer(0, vertex_buffer);

	m_triangle_vertex_declaration.set_index_count(count*6);
	m_triangle_vertex_declaration.set_vertex_count(count*4);

	auto dst_ptr = m_triangle_vertex_declaration.lock_for_writing<math::vec3f>(0);
	auto tex_ptr = m_triangle_vertex_declaration.lock_for_writing<math::vec2f>(1);
	auto col_ptr = m_triangle_vertex_declaration.lock_for_writing<math::vec4u8>(2);
	QASSERT(dst_ptr.is_valid() && tex_ptr.is_valid() && col_ptr.is_valid());
    (void)tex_ptr;
    (void)col_ptr;

#ifndef NDEBUG
	size_t pos_stride = m_triangle_vertex_declaration.get_attribute(0).stride;
	QASSERT(pos_stride == 24);
#endif

	auto index_ptr = m_triangle_vertex_declaration.lock_indices_for_writing();
	QASSERT(index_ptr.is_valid());

	size_t render_job_count = 0;
	m_render_jobs.resize(100);

	int old_material_idx = -1, old_camera_idx = -1;

	size_t start_index_count = 0;
	size_t index_count = 0;

	size_t vertex = 0;
	for (size_t i = 0; i < count; i++)
	{
		auto op = Operation(*(uint8_t*)data_ptr); data_ptr += sizeof(uint8_t);
		int8_t material_idx = *(int8_t*)data_ptr; data_ptr += sizeof(int8_t);
		int8_t camera_idx = *(int8_t*)data_ptr; data_ptr += sizeof(int8_t);
		data_ptr += sizeof(int8_t);

		if (i > 0)
		{
			//flush the old data if the new one is not compatible
			if (material_idx != old_material_idx || camera_idx != old_camera_idx)
			{
				if (render_job_count >= m_render_jobs.size())
				{
					m_render_jobs.resize(m_render_jobs.size() + 10);
				}
				Render_Job_Data& job_data = m_render_jobs[render_job_count++];
				job_data.job.set_vertex_declaration(m_triangle_vertex_declaration);
				job_data.job.set_material(m_materials[old_material_idx]);
				job_data.job.get_vertex_declaration().set_topology(video::Vertex_Declaration::Topology::TRIANGLE_LIST);
				job_data.job.get_vertex_declaration().set_range(start_index_count, index_count - start_index_count);
				job_data.material_idx = old_material_idx;
				job_data.camera_idx = old_camera_idx;

				start_index_count = index_count;
			}
		}

		old_material_idx = material_idx;
		old_camera_idx = camera_idx;

		if (op == Operation::FILL_QUAD)
		{
			size_t size = (12 + 8 + 4) * 4;
			memcpy(dst_ptr.get(), data_ptr, size);
			dst_ptr += 4;
			data_ptr += size;

			QASSERT(vertex < 65533);
			*index_ptr++ = static_cast<uint16_t>(vertex + 0);
			*index_ptr++ = static_cast<uint16_t>(vertex + 1);
			*index_ptr++ = static_cast<uint16_t>(vertex + 2);

			*index_ptr++ = static_cast<uint16_t>(vertex + 0);
			*index_ptr++ = static_cast<uint16_t>(vertex + 2);
			*index_ptr++ = static_cast<uint16_t>(vertex + 3);

			vertex += 4;
			index_count += 6;
		}
		else
		{
			size_t size = (12 + 8 + 4) * 3;
			memcpy(dst_ptr.get(), data_ptr, size);
			dst_ptr += 3;
			data_ptr += size;

			QASSERT(vertex < 65534);
			*index_ptr++ = static_cast<uint16_t>(vertex + 0);
			*index_ptr++ = static_cast<uint16_t>(vertex + 1);
			*index_ptr++ = static_cast<uint16_t>(vertex + 2);

			vertex += 3;
			index_count += 3;
		}
	}

	if (index_count - start_index_count > 0)
	{
		if (render_job_count >= m_render_jobs.size())
		{
			m_render_jobs.resize(m_render_jobs.size() + 10);
		}
		Render_Job_Data& job_data = m_render_jobs[render_job_count++];
		job_data.job.set_vertex_declaration(m_triangle_vertex_declaration);
		job_data.job.get_vertex_declaration().set_topology(video::Vertex_Declaration::Topology::TRIANGLE_LIST);
		job_data.job.get_vertex_declaration().set_range(start_index_count, index_count - start_index_count);
		job_data.job.set_material(m_materials[old_material_idx]);
		job_data.material_idx = old_material_idx;
		job_data.camera_idx = old_camera_idx;
	}

	m_triangle_vertex_declaration.unlock_all();
	flush_list(m_render_jobs, render_job_count);
}

//////////////////////////////////////////////////////////////////////////

void Painter::flush_list(std::vector<Render_Job_Data>& render_jobs, size_t count)
{
	if (count == 0)
	{
		return;
	}

	for (size_t i = 0; i < count; i++)
	{
		Render_Job_Data& data = render_jobs[i];
		//Material& material = m_materials[data.material_idx];

		if (data.camera_idx >= 0)
		{
			const scene::Camera& camera = m_cameras[data.camera_idx];
			System::inst().get_renderer()->set_camera(camera);
			data.job.set_bounds(util::Bounds(camera.get_position(), 0.f));
		}
		else
		{
			data.job.set_bounds(util::Bounds());
		}

		System::inst().get_renderer()->render(data.job);
		System::inst().get_renderer()->flush();
	}
}

//////////////////////////////////////////////////////////////////////////

/*
bool Painter::IsClippingEnabled() const
{
	return m_clippingParameters.clipRectangle;
}
*/

//////////////////////////////////////////////////////////////////////////

/*
void Painter::SetClippingEnabled(bool enable)
{
	m_clippingParameters.clipRectangle = enable;
	m_clippingParameters.enableClipStack = true;
}
*/

//////////////////////////////////////////////////////////////////////////

/*
math::rectf Painter::GetClippingRectangle() const
{
	QASSERT(m_clippingParameters.enableClipStack);
	if (m_clippingParameters.clipStack.GetSize() == 0)
	{
		QASSERT(0);
		return math::rectf();
	}
	const math::rectf& r = m_clippingParameters.clipStack.GetTop();
	return r;
}
*/

//////////////////////////////////////////////////////////////////////////

/*
void Painter::SetClippingRectangle(const math::rectf& rect)
{
	m_clippingParameters.enableClipStack = true;
	if (m_clippingParameters.clipStack.GetSize() == 0)
	{
		m_clippingParameters.clipStack.Push(rect);
		return;
	}
	m_clippingParameters.clipStack.GetTop() = rect;
}
*/

//////////////////////////////////////////////////////////////////////////

/*
void Painter::SetClippingShape(const math::vec2f* points, size_t numberOfPoints, bool closeShape)
{
	m_clippingParameters.enableClipStack = false;
	m_clippingParameters.clipper.SetShape(points, numberOfPoints, closeShape);
}
*/

//////////////////////////////////////////////////////////////////////////

/*
util::ClipStack& Painter::GetClipStack()
{
	return m_clippingParameters.clipStack;
}
*/

//////////////////////////////////////////////////////////////////////////

/*
const util::ClipStack& Painter::GetClipStack() const
{
	return m_clippingParameters.clipStack;
}
*/

//////////////////////////////////////////////////////////////////////////

size_t Painter::get_tesselation_from_radius(float radius)
{
	const float pi = 3.1415926535f;
	float cir = 2.f*pi*radius;
	return (size_t)math::clamp(cir * 0.2f, 8.f, 100.f) - 1;
}

void Painter::push_pre_clip_transform(math::trans3df const& transform)
{
	if (m_pre_clip_transform_stack.empty())
	{
		m_pre_clip_transform_stack.emplace_back(math::trans3df::identity);
		set_pre_clip_transform(transform);
	}
	else
	{
		m_pre_clip_transform_stack.emplace_back(get_pre_clip_transform());
		math::trans3df trans;
		math::multiply(trans, get_pre_clip_transform(), transform);
		set_pre_clip_transform(trans);
	}
}
void Painter::pop_pre_clip_transform()
{
	set_pre_clip_transform(m_pre_clip_transform_stack.back());
	m_pre_clip_transform_stack.pop_back();
}

void Painter::push_post_clip_transform(math::trans3df const& transform)
{
	if (m_post_clip_transform_stack.empty())
	{
		m_post_clip_transform_stack.emplace_back(math::trans3df::identity);
		set_post_clip_transform(transform);
	}
	else
	{
		m_post_clip_transform_stack.emplace_back(get_post_clip_transform());
		math::trans3df trans;
		math::multiply(trans, get_post_clip_transform(), transform);
		set_post_clip_transform(trans);
	}
}
void Painter::pop_post_clip_transform()
{
	set_post_clip_transform(m_post_clip_transform_stack.back());
	m_post_clip_transform_stack.pop_back();
}

