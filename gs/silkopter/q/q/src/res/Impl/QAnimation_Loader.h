#pragma once

namespace q
{
namespace res
{
namespace impl
{

	class QAnimation_Loader : public res::Loader<anim::Animation>
	{
		virtual void load(Path const& path, data::Source& source, anim::Animation& r) const;
		virtual bool can_load_from_source(data::Source& source) const;
	};


}
}
}
