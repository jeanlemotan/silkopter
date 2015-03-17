#include "QStdAfx.h"
#include "video/Shader.h"
#include "video/Renderer.h"
#include "system/System.h"

using namespace q;
using namespace video;

Shader_ptr Shader::create(Type type, Shader_Source const& vertex, Shader_Source const& fragment)
{
	auto const& renderer = System::inst().get_renderer();
	return renderer ? renderer->create_shader(type, vertex, fragment) : Shader_ptr();
}

