#include "QStdAfx.h"
#include "video/Vertex_Buffer.h"
#include "video/Renderer.h"
#include "system/System.h"

using namespace q;
using namespace video;

Vertex_Buffer_ptr Vertex_Buffer::create()
{
	Renderer_ptr const& renderer = System::inst().get_renderer();
	return renderer ? renderer->create_vertex_buffer() : Vertex_Buffer_ptr();
}
