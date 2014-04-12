#include "QStdAfx.h"
#include "video/Uniform_Buffer.h"
#include "video/Renderer.h"


using namespace q;
using namespace video;

Uniform_Buffer_ptr Uniform_Buffer::create()
{
	Renderer_ptr const& renderer = System::inst().get_renderer();
	return renderer ? renderer->create_uniform_buffer() : Uniform_Buffer_ptr();
}
