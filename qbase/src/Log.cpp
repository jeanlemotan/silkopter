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
        Topic() : is_enabled(true), level(q::logging::Level::DBG), decorations(-1) {}

        bool is_enabled;
		q::logging::Level level;
		int decorations;
	};

    static std::map<String, Topic> s_topics;
    static std::vector<std::unique_ptr<q::logging::Logger>> s_loggers;

    static q::logging::Decorations s_decorations(q::logging::Decoration::LOCATION, q::logging::Decoration::TOPIC, q::logging::Decoration::TIME, q::logging::Decoration::LEVEL);
    static q::logging::Level s_level = q::logging::Level::DBG;
    static std::mutex s_mutex;
    static __thread std::vector<char const*>* s_topic_stack = nullptr;
}
}
}

//////////////////////////////////////////////////////////////////////////////////////////////////

void q::logging::push_topic(char const* topic)
{
    if (!detail::s_topic_stack)
    {
        detail::s_topic_stack = new std::vector<char const*>();
    }
    detail::s_topic_stack->push_back(topic);
}

void q::logging::pop_topic()
{
    QASSERT(detail::s_topic_stack && !detail::s_topic_stack->empty());
    detail::s_topic_stack->pop_back();
}


void q::logging::add_logger(std::unique_ptr<Logger> logger)
{
	QASSERT(logger);
	if (logger)
	{
        std::lock_guard<std::mutex> lg(detail::s_mutex);

        detail::s_loggers.push_back(std::move(logger));
	}
}
void q::logging::set_logger(std::unique_ptr<Logger> logger)
{
    std::lock_guard<std::mutex> lg(detail::s_mutex);

    detail::s_loggers.clear();
	if (logger)
	{
        detail::s_loggers.push_back(std::move(logger));
	}
}
void q::logging::set_topic_enabled(const String& topic, bool enabled)
{
    std::lock_guard<std::mutex> lg(detail::s_mutex);

    detail::s_topics[topic].is_enabled = enabled;
}
void q::logging::set_decorations(Decorations decorations)
{
    std::lock_guard<std::mutex> lg(detail::s_mutex);

    detail::s_decorations = decorations;
}
void q::logging::set_decorations(const String& topic, Decorations decorations)
{
    std::lock_guard<std::mutex> lg(detail::s_mutex);

    detail::s_topics[topic].decorations = decorations;
}
void q::logging::set_level(Level level)
{
    std::lock_guard<std::mutex> lg(detail::s_mutex);

    detail::s_level = level;
}
void q::logging::set_level(const String& topic, Level level)
{
    std::lock_guard<std::mutex> lg(detail::s_mutex);

    detail::s_topics[topic].level = level;
}
void q::log(logging::Level level, const char* file, int line, const String& message)
{
	using namespace logging;

    std::lock_guard<std::mutex> lg(detail::s_mutex);

    if (detail::s_loggers.empty())
	{
		return;
	}

	//the default settings
    Level allowedLevel = detail::s_level;
    auto decorations = detail::s_decorations;

//	//search for overrides for this specific topic
//    if (topic)
//    {
//        auto it = detail::sTopic.find(topic);
//        if (it != detail::sTopic.end())
//        {
//            if (!it->second.isEnabled)
//            {
//                return;
//            }
//            allowedLevel = static_cast<Level>(std::max(static_cast<int>(allowedLevel), static_cast<int>(it->second.level)));
//            decorations = it->second.decorations >= 0 ? Decorations(it->second.decorations) : decorations;
//        }
//    }

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
        case Level::DBG: str.append("[D]"); break;
        case Level::INFO: str.append("[I]"); break;
        case Level::WARNING: str.append("[W]"); break;
        case Level::ERR: str.append("[E]"); break;
		default: QASSERT(0); break;
		}
	}

    if (decorations.test(Decoration::TOPIC) && detail::s_topic_stack && !detail::s_topic_stack->empty())
	{
        str.append('[');
        for (size_t i = 0; i + 1 < detail::s_topic_stack->size(); i++)
        {
            str.append((*detail::s_topic_stack)[i]);
            str.append('/');
        }
        str.append(detail::s_topic_stack->back());
        str.append(']');
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
    QASSERT(!detail::s_loggers.empty());
    for (auto const& logger: detail::s_loggers)
	{
		logger->log(level, str);
	}
	if (level == logging::Level::ERR)
	{
//        QASSERT(0);
	}
}




