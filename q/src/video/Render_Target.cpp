#include "QStdAfx.h"
#include "video/Render_Target.h"
#include "system/System.h"
#include "video/Renderer.h"

using namespace q;
using namespace video;

Render_Target_ptr Render_Target::create()
{
	auto const& renderer = System::inst().get_renderer();
	return renderer ? renderer->create_render_target() : Render_Target_ptr();
}

