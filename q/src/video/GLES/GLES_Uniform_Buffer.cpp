#include "QStdAfx.h"
#include "video/GLES/GLES_Uniform_Buffer.h"
#include "video/GLES/GLES_Interface.h"
#include "video/Renderer.h"


using namespace q;
using namespace video;

GLES_Uniform_Buffer::GLES_Uniform_Buffer()
	: m_buffer(GLES_Buffer_Impl::Type::UNIFORM)
{

}

GLES_Uniform_Buffer::~GLES_Uniform_Buffer()
{
}

bool GLES_Uniform_Buffer::allocate(size_t size, Usage usage)
{
	return m_buffer.allocate(size, usage);
}

size_t GLES_Uniform_Buffer::get_size() const
{
	return m_buffer.get_size();
}
Buffer::Usage GLES_Uniform_Buffer::get_usage() const
{
	return m_buffer.get_usage();
}

bool GLES_Uniform_Buffer::is_locked_for_reading() const
{
	return m_buffer.is_locked_for_reading();
}
uint8_t const* GLES_Uniform_Buffer::lock_for_reading()
{
	return m_buffer.lock_for_reading();
}
void GLES_Uniform_Buffer::unlock_for_reading()
{
	m_buffer.unlock_for_reading();
}

bool GLES_Uniform_Buffer::is_locked_for_writing() const
{
	return m_buffer.is_locked_for_writing();
}
uint8_t* GLES_Uniform_Buffer::lock_for_writing()
{
	return m_buffer.lock_for_writing();
}
void GLES_Uniform_Buffer::unlock_for_writing()
{
	m_buffer.unlock_for_writing();
}

bool GLES_Uniform_Buffer::is_locked_for_consuming() const
{
	return m_buffer.is_locked_for_consuming();
}

GLES_Buffer_Impl& GLES_Uniform_Buffer::get_buffer()
{
	return m_buffer;
}
GLES_Buffer_Impl const& GLES_Uniform_Buffer::get_buffer() const
{
	return m_buffer;
}




