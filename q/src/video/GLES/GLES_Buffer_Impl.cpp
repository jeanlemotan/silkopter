#include "QStdAfx.h"
#include "video/GLES/GLES_Buffer_Impl.h"
#include "video/GLES/GLES_Interface.h"
#include "video/Renderer.h"


using namespace q;
using namespace video;

GLES_Buffer_Impl::GLES_Buffer_Impl(Type type)
	: m_type(type)
	, m_gl_id(0)
	, m_size(0)
	, m_usage(Buffer::Usage::STATIC)
	, m_read_address(nullptr)
	, m_write_address(nullptr)
    , m_read_count(0)
    , m_write_count(0)
	, m_last_frame_rendered_idx(0)
{
	if (m_type == Type::INDEX)
	{
		m_target = gles::iGL_ELEMENT_ARRAY_BUFFER;
	}
	else if (m_type == Type::VERTEX)
	{
		m_target = gles::iGL_ARRAY_BUFFER;
	}
	else
	{

	}
}

GLES_Buffer_Impl::~GLES_Buffer_Impl()
{
	QASSERT(!is_locked_for_reading() && !is_locked_for_writing());

	if (m_gl_id != 0)
	{
		gles::Interface interf;
		destroy_gl_object(interf);
	}
}

bool GLES_Buffer_Impl::allocate(size_t size, Buffer::Usage usage)
{
	QASSERT(!is_locked_for_reading() && !is_locked_for_writing());
	if (is_locked_for_reading() || is_locked_for_writing())
	{
		return false;
	}

	m_usage = usage;

	if (size == 0)
	{
		if (m_gl_id != 0)
		{
			gles::Interface interf;
			destroy_gl_object(interf);
		}
		std::vector<uint8_t> tmp;
		m_data.swap(tmp);
		return true;
	}

	if (m_type == Type::VERTEX || m_type == Type::INDEX)
	{
		gles::Interface interf;
		if (!m_gl_id)
		{
			create_gl_object(interf);
		}
		QASSERT(m_gl_id);

		interf.iglBindBuffer(m_target, m_gl_id);
        interf.iglBufferData(m_target, size, nullptr, m_usage == Buffer::Usage::STATIC ? gles::iGL_STATIC_DRAW : gles::iGL_DYNAMIC_DRAW);
	}
	else
	{
		m_data.resize(size);
	}

	m_size = size;

	return true;
}

void GLES_Buffer_Impl::destroy_gl_object(gles::Interface& interf)
{
	if (m_gl_id)
	{
		interf.iglDeleteBuffers(1, &m_gl_id);
	}
	m_gl_id = 0;
	m_size = 0;
}

void GLES_Buffer_Impl::create_gl_object(gles::Interface& interf)
{
	destroy_gl_object(interf);
	interf.iglGenBuffers(1, &m_gl_id);
    interf.iglGenBuffers(1, &m_gl_id);
    interf.iglGenBuffers(1, &m_gl_id);
}

size_t GLES_Buffer_Impl::get_size() const
{
	return m_size;
}
Buffer::Usage GLES_Buffer_Impl::get_usage() const
{
	return m_usage;
}

bool GLES_Buffer_Impl::is_locked_for_reading() const
{
	return (long)m_read_count > 0;
}
uint8_t const* GLES_Buffer_Impl::lock_for_reading()
{
	QASSERT(!is_locked_for_writing());
	if (is_locked_for_writing())
	{
		return nullptr;
	}

	if (m_size == 0)
	{
		return nullptr;
	}
	if (!m_read_address)
	{
		if (m_type == Type::VERTEX || m_type == Type::INDEX)
		{
			if (m_gl_id == 0)
			{
				return nullptr;
			}
			gles::Interface interf;
			interf.iglBindBuffer(m_target, m_gl_id);
			m_read_address = (uint8_t const*)interf.iglMapBuffer(m_target, gles::iGL_READ_ONLY);
		}
		else
		{
			m_read_address = m_data.data();
		}
	}

	if (m_read_address)
	{
		++m_read_count;
	}
	return m_read_address;
}
void GLES_Buffer_Impl::unlock_for_reading()
{
	if (m_read_count > 0)
	{
		--m_read_count;
		if (m_read_count == 0)
		{
			if (m_type == Type::VERTEX || m_type == Type::INDEX)
			{
				gles::Interface interf;
				interf.iglBindBuffer(m_target, m_gl_id);
				interf.iglUnmapBuffer(m_target);
			}
			else
			{
				//nothing
			}
			m_read_address = 0;
		}
	}
}

bool GLES_Buffer_Impl::is_locked_for_writing() const
{
	return (long)m_write_count > 0;
}
uint8_t* GLES_Buffer_Impl::lock_for_writing()
{
	QASSERT(!is_locked_for_reading() && !is_locked_for_consuming());
	if (is_locked_for_reading() || is_locked_for_consuming())
	{
		return nullptr;
	}

	if (m_size == 0)
	{
		return nullptr;
	}
	if (!m_write_address)
	{
		if (m_type == Type::VERTEX || m_type == Type::INDEX)
		{
			if (m_gl_id == 0)
			{
				return nullptr;
			}
			gles::Interface interf;
			interf.iglBindBuffer(m_target, m_gl_id);
			m_write_address = (uint8_t*)interf.iglMapBuffer(m_target, gles::iGL_WRITE_ONLY);
		}
		else
		{
			m_write_address = m_data.data();
		}
	}

	if (m_write_address)
	{
		++m_write_count;
	}
	return m_write_address;
}
void GLES_Buffer_Impl::unlock_for_writing()
{
	if (m_write_count > 0)
	{
		--m_write_count;
		if (m_write_count == 0)
		{
			if (m_type == Type::VERTEX || m_type == Type::INDEX)
			{
				gles::Interface interf;
				interf.iglBindBuffer(m_target, m_gl_id);
				interf.iglUnmapBuffer(m_target);
			}
			else
			{
				//nothing
			}

			m_write_address = 0;
		}
	}
}

bool GLES_Buffer_Impl::is_locked_for_consuming() const
{
	return System::inst().get_renderer()->get_frame_idx() - m_last_frame_rendered_idx < 3;
}

uint32_t GLES_Buffer_Impl::get_gl_id() const
{
	return m_gl_id;
}
void GLES_Buffer_Impl::set_last_rendered_frame_idx(size_t frame_idx)
{
	m_last_frame_rendered_idx = frame_idx;
}

bool GLES_Buffer_Impl::is_locked() const
{
	return is_locked_for_consuming() || is_locked_for_reading() || is_locked_for_writing();
}


