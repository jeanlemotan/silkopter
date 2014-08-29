#pragma once

#include "res/Loader.h"

namespace q
{
namespace res
{
namespace impl
{

	class ZupSpriteLoader : public res::Loader<draw::SpriteBase>
	{
	public:
		virtual void load(Path const& path, data::Source& source, draw::SpriteBase& r) const;
		virtual bool can_load_from_source(data::Source& source) const;

	private:
	};


}
}
}