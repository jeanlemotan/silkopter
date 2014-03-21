#pragma once


namespace util
{

	class Noncopyable
	{
		Noncopyable(const Noncopyable&) = delete;
		Noncopyable& operator=(const Noncopyable&) = delete;
	};

}
