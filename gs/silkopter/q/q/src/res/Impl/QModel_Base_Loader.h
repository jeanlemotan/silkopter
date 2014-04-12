#pragma once

namespace q
{
namespace res
{
namespace impl
{

	class QModel_Base_Loader : public res::Loader<scene::Model_Base>
	{
		virtual void load(Path const& path, data::Source& source, scene::Model_Base& r) const;
		virtual bool can_load_from_source(data::Source& source) const;
	};


}
}
}