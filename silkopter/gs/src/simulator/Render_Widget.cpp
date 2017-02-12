#include "Render_Widget.h"

#include <QtOpenGL/QGLFunctions>

//////////////////////////////////////////////////////////////////////////

Render_Widget::Render_Widget(QWidget* parent)
    : QWidget(parent)
{
}

//////////////////////////////////////////////////////////////////////////

Render_Widget::Render_Widget(Flags flags, QWidget* parent)
    : QWidget(parent)
    , m_flags(flags)
{
}

//////////////////////////////////////////////////////////////////////////

void Render_Widget::init()
{
	setMouseTracking(true);

	m_buffers_full = false;
	q::video::gles::Interface interf;
    for (size_t i = 0; i < m_buffers.size(); i++)
    {
        interf.iglGenBuffers(1, &m_buffers[i].color_pbo);
        interf.iglBindBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, m_buffers[i].color_pbo);
        //write some data to avoid out of memory from driver
        interf.iglBufferData(q::video::gles::iGL_PIXEL_PACK_BUFFER, 1024, 0, q::video::gles::iGL_STREAM_READ);
        interf.iglBufferData(q::video::gles::iGL_PIXEL_PACK_BUFFER, 1024, 0, q::video::gles::iGL_STREAM_READ);


        if (m_flags.test(Flag::DEPTH_ACCESS))
        {
            interf.iglGenBuffers(1, &m_buffers[i].depth_pbo);
            interf.iglBindBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, m_buffers[i].depth_pbo);
            //write some data to avoid out of memory from driver
            interf.iglBufferData(q::video::gles::iGL_PIXEL_PACK_BUFFER, 1024, 0, q::video::gles::iGL_STREAM_READ);
        }
    }

    interf.iglBindBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, 0);

	m_is_initialized = true;
}

//////////////////////////////////////////////////////////////////////////

Render_Widget::~Render_Widget()
{
	for (size_t i = 0; i < m_buffers.size(); i++)
	{
		auto& buffer = m_buffers[i];
        std::lock_guard<std::mutex> lg(buffer.mutex);

        // to make sure there is no transfer going on when deleting the framebuffer
		finish_buffer_transfer(buffer);
	}
}

//////////////////////////////////////////////////////////////////////////

Render_Widget::Flags Render_Widget::get_flags() const
{
	return m_flags;
}

//////////////////////////////////////////////////////////////////////////

void Render_Widget::set_flags(Render_Widget::Flags flags)
{
	m_flags = flags;
}

//////////////////////////////////////////////////////////////////////////

void Render_Widget::finish_buffer_transfer(Buffer& buffer)
{
	q::video::gles::Interface interf;
    interf.iglBindBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, buffer.color_pbo);
    auto ptr = interf.iglMapBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, q::video::gles::iGL_READ_ONLY);
    if (ptr)
    {
        interf.iglUnmapBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER);
    }
    if (m_flags.test(Flag::DEPTH_ACCESS))
    {
        interf.iglBindBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, buffer.depth_pbo);
        ptr = interf.iglMapBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, q::video::gles::iGL_READ_ONLY);
        if (ptr)
        {
            interf.iglUnmapBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER);
        }
    }
    interf.iglBindBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, 0);
}

//////////////////////////////////////////////////////////////////////////

void Render_Widget::begin_rendering()
{
	auto& buffer = m_buffers[m_write_buffer_idx];
    std::lock_guard<std::mutex> lg(buffer.mutex);

	auto size = math::max(math::vec2u32(width(), height()), math::vec2u32(1, 1));
	if (!buffer.framebuffer || size != buffer.framebuffer->get_size())
	{
		// to make sure there is no transfer going on when deleting the framebuffer
		finish_buffer_transfer(buffer);

		buffer.framebuffer = q::video::Render_Target::create();
		buffer.framebuffer->allocate(size, 
			q::video::Render_Target::Color_Format::RGBA_8, 
			q::video::Render_Target::Depth_Format::FULL, 
			q::video::Render_Target::Stencil_Format::FULL, 
			q::video::Render_Target::AA_Format::NONE,
			false);
	}

	q::System::inst().get_renderer()->set_render_target(buffer.framebuffer);
}

//////////////////////////////////////////////////////////////////////////

void Render_Widget::end_rendering()
{
	q::System::inst().get_renderer()->flush();

	q::video::gles::Interface interf;
	interf.iglFlush();

	{
		auto& buffer = m_buffers[m_write_buffer_idx];
        std::lock_guard<std::mutex> lg(buffer.mutex);

		buffer.size = buffer.framebuffer->get_size();

		//read color
		buffer.color_is_resolved = false;
		size_t data_size = buffer.size.x * buffer.size.y * sizeof(uint32_t);
		data_size = math::get_min_pot(data_size);
		//allocate memory in the pbo buffer
        interf.iglBindBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, buffer.color_pbo);
        interf.iglBufferData(q::video::gles::iGL_PIXEL_PACK_BUFFER, data_size, 0, q::video::gles::iGL_STREAM_READ);

		//read pixels in the pbo
        interf.iglReadPixels(0, 0, buffer.size.x, buffer.size.y, q::video::gles::iGL_BGRA, q::video::gles::iGL_UNSIGNED_BYTE, (void*)0);

//        std::vector<uint8_t> xxx(data_size);
//        auto x = q::System::inst().get_renderer()->read_color_pixels(math::vec2u32::zero, buffer.size);

//        interf.iglReadPixels(0, 0, buffer.size.x, buffer.size.y, q::video::gles::iGL_BGRA, q::video::gles::iGL_UNSIGNED_BYTE, (void*)xxx.data());

		//read depth
        if (m_flags.test(Flag::DEPTH_ACCESS))
        {
            buffer.depth_is_resolved = false;
            size_t data_size = buffer.size.x * buffer.size.y * sizeof(float);
            //allocate memory in the pbo buffer
            interf.iglBindBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, buffer.depth_pbo);
            interf.iglBufferData(q::video::gles::iGL_PIXEL_PACK_BUFFER, data_size, 0, q::video::gles::iGL_STREAM_READ);

            //read pixels in the pbo
            interf.iglReadPixels(0, 0, buffer.size.x, buffer.size.y, q::video::gles::iGL_DEPTH_COMPONENT, q::video::gles::iGL_FLOAT, (void*)0);
        }

        //unbind the pbo
        interf.iglBindBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, 0);

		//give access to reads only when buffers are full
		if (m_write_buffer_idx + 1 == m_buffers.size())
		{
			m_buffers_full = true;
		}

		//increment the buffers
		m_write_buffer_idx = (m_write_buffer_idx + 1) % m_buffers.size();
		if (m_buffers_full)
		{
			m_read_buffer_idx = (m_write_buffer_idx + 1) % m_buffers.size();
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void Render_Widget::paintEvent(QPaintEvent* event)
{
	if (!m_is_initialized)
	{
		QWidget::paintEvent(event);
		return;
	}

	QPainter painter(this);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

	if (m_buffers_full)
	{
 		auto& buffer = m_buffers[m_read_buffer_idx];
        std::lock_guard<std::mutex> lg(buffer.mutex);

        resolve_color_buffer(buffer);

 		QImage img(buffer.color_data.data(), buffer.size.x, buffer.size.y, buffer.size.x * 4, QImage::Format_RGB32);
 		painter.drawImage(QPoint(0, 0), img);
	}
}

//////////////////////////////////////////////////////////////////////////

void Render_Widget::resolve_color_buffer(Buffer& buffer)
{
	if (!buffer.color_is_resolved)
	{
		buffer.color_is_resolved = true;

		q::video::gles::Interface interf;

        interf.iglBindBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, buffer.color_pbo);
        auto* ptr = static_cast<uint8_t*>(interf.iglMapBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, q::video::gles::iGL_READ_ONLY));
        QASSERT(ptr);

        size_t data_size = buffer.size.x * buffer.size.y * sizeof(uint32_t);
        QASSERT(data_size > 0);
        buffer.color_data.resize(data_size);

        //flip vertically
        size_t row_size = buffer.size.x * sizeof(uint32_t);
        for (size_t i = 0; i < buffer.size.y; i++)
        {
            memcpy(buffer.color_data.data() + (buffer.size.y - 1 - i) * row_size, ptr + i * row_size, row_size);
        }

        interf.iglUnmapBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER);
        interf.iglBindBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, 0);
	}
}

//////////////////////////////////////////////////////////////////////////

void Render_Widget::resolve_depth_buffer(Buffer& buffer)
{
	if (!buffer.depth_is_resolved)
	{
		buffer.depth_is_resolved = true;

        q::video::gles::Interface interf;

        interf.iglBindBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, buffer.depth_pbo);
        auto* ptr = static_cast<uint8_t*>(interf.iglMapBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, q::video::gles::iGL_READ_ONLY));

        size_t data_size = buffer.size.x * buffer.size.y * sizeof(uint32_t);
        buffer.depth_data.resize(data_size);
        memcpy(buffer.depth_data.data(), ptr, data_size);

        interf.iglUnmapBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER);
        interf.iglBindBuffer(q::video::gles::iGL_PIXEL_PACK_BUFFER, 0);
	}
}

//////////////////////////////////////////////////////////////////////////

float Render_Widget::get_depth_at(const math::vec2u32& coords)
{
	if (!m_flags.test(Flag::DEPTH_ACCESS))
	{
		QASSERT(0);
		return 0;
	}

	if (m_buffers_full)
	{
		auto& buffer = m_buffers[m_read_buffer_idx];
        std::lock_guard<std::mutex> lg(buffer.mutex);

        resolve_depth_buffer(buffer);

		auto c = math::min(coords, buffer.size - math::vec2u32(1, 1));

		//convert from top-left origin to openGL bottom-left origin
		c.y = buffer.size.y - c.y - 1;

		return buffer.depth_data[c.y * buffer.size.x + c.x];
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
