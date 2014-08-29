#pragma once

namespace q
{
namespace res
{

	template<class Res>
	class Loader : public util::Noncopyable
	{
	public:
        virtual ~Loader() {}
        
		virtual void load(Path const& path, data::Source& source, Res& r) const = 0;
		virtual bool can_load_from_source(data::Source& source) const = 0;
	};

}
}