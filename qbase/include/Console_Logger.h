#pragma once

namespace q
{	
namespace logging
{

	//Simple implementation that writes to console
	class Console_Logger : public Logger
	{
	public:
        virtual void log(Level level, std::string const& message) override;
	};

}
}
