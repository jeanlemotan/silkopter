#pragma once

#include "res/Loader.h"

namespace q
{
namespace res
{
namespace impl
{

	class Free_Type_Font_Loader : public res::Loader<text::Font>
	{
	public:
		virtual void load(Path const& path, data::Source& source, text::Font& r) const;
		virtual bool can_load_from_source(data::Source& source) const;

	private:
	};


}
}
}