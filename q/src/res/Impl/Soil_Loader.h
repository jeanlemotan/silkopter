#pragma once

#include "res/Loader.h"
#include "video/Texture.h"
#include "video/Dynamic_Image.h"

namespace q
{
namespace res
{
namespace impl
{

	//handles TGA, PNG, DDS, JPG, BMP
	class Soil_Texture_Loader : public res::Loader<video::Texture>
	{
		virtual void load(Path const& path, data::Source& source, video::Texture& r) const;
		virtual bool can_load_from_source(data::Source& source) const;
	};
	class Soil_Dynamic_Image_Loader : public res::Loader<video::Dynamic_Image>
	{
		virtual void load(Path const& path, data::Source& source, video::Dynamic_Image& r) const;
		virtual bool can_load_from_source(data::Source& source) const;
	};


}
}
}
