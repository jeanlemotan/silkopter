#include "QStdAfx.h"
#include "video/Index_Buffer.h"
#include "video/Renderer.h"
#include "system/System.h"
#include "video/Renderer.h"

using namespace q;
using namespace video;

Index_Buffer_ptr Index_Buffer::create()
{
	Renderer_ptr const& renderer = System::inst().get_renderer();
	return renderer ? renderer->create_index_buffer() : Index_Buffer_ptr();
}
