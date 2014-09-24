#include "QBaseStdAfx.h"
#include "QBase.h"

#include <map>
#include <ctime>

namespace q
{
namespace logging
{
namespace detail
{
	struct Topic
	{
		Topic() : isEnabled(true), level(q::logging::Level::DBG), decorations(-1) {}

		bool isEnabled;
		q::logging::Level level;
		int decorations;
	};

	static std::map<String, Topic> sTopic;
	static std::vector<std::unique_ptr<q::logging::Logger>> sLoggers;

	static q::logging::Decorations sDecorations(q::logging::Decoration::LOCATION, q::logging::Decoration::TOPIC, q::logging::Decoration::TIME, q::logging::Decoration::LEVEL);
	static q::logging::Level sLevel = q::logging::Level::DBG;
	static std::mutex sMutex;
}
}
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void q::logging::add_logger(std::unique_ptr<Logger> logger)
{
	QASSERT(logger);
	if (logger)
	{
		std::lock_guard<std::mutex> lg(detail::sMutex);

		detail::sLoggers.push_back(std::move(logger));
	}
}
void q::logging::set_logger(std::unique_ptr<Logger> logger)
{
	std::lock_guard<std::mutex> lg(detail::sMutex);

	detail::sLoggers.clear();
	if (logger)
	{
		detail::sLoggers.push_back(std::move(logger));
	}
}
void q::logging::set_topic_enabled(const String& topic, bool enabled)
{
	std::lock_guard<std::mutex> lg(detail::sMutex);

	detail::sTopic[topic].isEnabled = enabled;
}
void q::logging::set_decorations(Decorations decorations)
{
	std::lock_guard<std::mutex> lg(detail::sMutex);

	detail::sDecorations = decorations;
}
void q::logging::set_decorations(const String& topic, Decorations decorations)
{
	std::lock_guard<std::mutex> lg(detail::sMutex);

	detail::sTopic[topic].decorations = decorations;
}
void q::logging::set_level(Level level)
{
	std::lock_guard<std::mutex> lg(detail::sMutex);

	detail::sLevel = level;
}
void q::logging::set_level(const String& topic, Level level)
{
	std::lock_guard<std::mutex> lg(detail::sMutex);

	detail::sTopic[topic].level = level;
}
void q::log(logging::Level level, const String& topic, const char* file, int line, const String& message)
{
	using namespace logging;

	std::lock_guard<std::mutex> lg(detail::sMutex);

	if (detail::sLoggers.empty())
	{
		return;
	}

	//the default settings
	Level allowedLevel = detail::sLevel;
	auto decorations = detail::sDecorations;

	//search for overrides for this specific topic
	auto it = detail::sTopic.find(topic);
	if (it != detail::sTopic.end())
	{
		if (!it->second.isEnabled)
		{
			return;
		}
		allowedLevel = static_cast<Level>(std::max(static_cast<int>(allowedLevel), static_cast<int>(it->second.level)));
		decorations = it->second.decorations >= 0 ? Decorations(it->second.decorations) : decorations;
	}

	//level too low? ignore
	if (level < allowedLevel)
	{
		return;
	}

	//build the final string from the decorations and the message
	String str;
	//str.reserve(message.size() + 128);

	if (decorations.test(Decoration::LEVEL))
	{
		switch (level)
		{
		case Level::DBG: str.append("[DEBUG]"); break;
		case Level::INFO: str.append("[INFO]"); break;
		case Level::WARNING: str.append("[WARNING]"); break;
		case Level::ERR: str.append("[ERROR]"); break;
		default: QASSERT(0); break;
		}
	}

	if (decorations.test(Decoration::TOPIC) && !topic.empty())
	{
		str.append("[");
		str.append(topic);
		str.append("]");
	}

	if (decorations.test(Decoration::LOCATION) && file)
	{
		str.push_back('[');
		str.append(file);
		str.push_back(':');
		char mbstr[100];
		sprintf(mbstr, "%d]", line);
		str.append(mbstr);
	}

	if (decorations.test(Decoration::DATE) && decorations.test(Decoration::TIME))
	{
		char mbstr[100];
        std::time_t t = std::time(nullptr);
		if (std::strftime(mbstr, 100, "[%e-%m-%Y %H:%M:%S]", std::localtime(&t))) 
		{
			str.append(mbstr);
		}
	}
	else if (decorations.test(Decoration::DATE))
	{
		char mbstr[100];
        std::time_t t = std::time(nullptr);
		if (std::strftime(mbstr, 100, "[%e-%m-%Y]", std::localtime(&t))) 
		{
			str.append(mbstr);
		}
	}
	else if (decorations.test(Decoration::TIME))
	{
		char mbstr[100];
        std::time_t t = std::time(nullptr);
		if (std::strftime(mbstr, 100, "[%H:%M:%S]", std::localtime(&t))) 
		{
			str.append(mbstr);
		}
	}

	str.append(message);

	//send to loggers
	QASSERT(!detail::sLoggers.empty());
	for (auto const& logger: detail::sLoggers)
	{
		logger->log(level, str);
	}
	if (level == logging::Level::ERR)
	{
//        QASSERT(0);
	}
}

