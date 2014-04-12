#pragma once

namespace q
{
namespace logging
{

	class Logger : util::Noncopyable
	{
	public:
		virtual ~Logger() {}

		// This function is responsible of logging the message.
		virtual void log(Level level, String const& message) = 0;
	};

	typedef std::unique_ptr<q::logging::Logger> Logger_uptr;

}
}
