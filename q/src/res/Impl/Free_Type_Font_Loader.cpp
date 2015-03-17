#include "QStdAfx.h"
#include "res/Resource.h"
#include "text/Font.h"
#include "Free_Type_Font_Loader.h"


using namespace q;
using namespace res;
using namespace impl;
using namespace data;
using namespace text;

bool Free_Type_Font_Loader::can_load_from_source(data::Source& /*source*/) const
{
	return true;
}

void Free_Type_Font_Loader::load(Path const& /*path*/, data::Source& source, Font& font) const
{
	font.unload();
	source.seek(0);
	font.load_free_type(source);
}

