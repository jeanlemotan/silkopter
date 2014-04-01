#pragma once



namespace util
{

	class Noncopyable
	{
	public:
		Noncopyable() = default;
	private:
		Noncopyable(const Noncopyable&) = delete;
		Noncopyable& operator=(const Noncopyable&) = delete;
	};

}
