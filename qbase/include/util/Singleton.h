#pragma once

namespace q
{
namespace util
{

template<class T>
class Singleton : util::Noncopyable
{
public:
	static T& inst()
	{
		QASSERT_MSG(s_instance, "Create an instance manually first with create().");
		return *s_instance;
	}
	static bool has_inst()
	{
		return s_instance != false;
	}
	static T& create()
	{
		QASSERT_MSG(!s_instance, "Instance already created.");
		s_creating_instance = true;
		s_instance.reset(new T());
		s_creating_instance = false;
		return *s_instance;
	}
	static void destroy()
	{
		s_instance.reset();
	}
protected:
	Singleton()
	{
		if (!s_creating_instance)
		{
            QLOGE("Singleton instance created with new T instead of T::create. Exiting...");
			exit(1);
			//if we don't exit here, the app will crash for sure later on in weird way. 
			//So choose instead to crash in a nice place
		}
	}
private:
	static bool s_creating_instance;
	static std::unique_ptr<T> s_instance;
};

template<class T>
bool Singleton<T>::s_creating_instance = false;
template<class T>
std::unique_ptr<T> Singleton<T>::s_instance;

} //util
}
