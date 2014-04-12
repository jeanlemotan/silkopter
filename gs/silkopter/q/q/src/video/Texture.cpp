#include "QStdAfx.h"
#include "video/Texture.h"

using namespace q;
using namespace video;

Texture_ptr Texture::create(Path const& path)
{
	auto const& renderer = System::inst().get_renderer();
	return renderer ? renderer->create_texture(path) : Texture_ptr();
}

Texture::Texture(Path const& path)
	: Resource(path)
{

}