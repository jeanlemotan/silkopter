#pragma once

namespace q
{
namespace res
{

	class Factory;

	template<class Res>
	class Streamer : public util::Noncopyable
	{
		friend class Factory;
	public:
        virtual ~Streamer() {}

	protected: 
		//these are supposed to be called by a factory
		virtual std::shared_ptr<Res> load(Factory const& factory, Path const& path) = 0;
		virtual void		add(Path const& path, const std::shared_ptr<Res>& r) = 0;

		virtual void		update(Factory const& factory, Clock::duration dt) = 0;

		virtual void		reload_all(Factory const& factory) = 0;
	};

}
}