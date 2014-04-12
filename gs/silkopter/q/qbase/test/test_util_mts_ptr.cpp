#include "QBase.h"

#include <boost/test/unit_test.hpp>
#include <iostream>


template<class T>
class xshared_ptr : private std::shared_ptr<T>
{
public:
	template<class _Ux>
	explicit xshared_ptr(_Ux *_Px) : std::shared_ptr<T>(_Px)
		, m_mutex(std::make_shared<std::mutex>())
	{
	}

	template<class _Ux,
	class _Dx>
		xshared_ptr(_Ux *_Px, _Dx _Dt)  : std::shared_ptr<T>(_Px, _Dt)
		, m_mutex(std::make_shared<std::mutex>())
	{
	}

	auto lock_for_reading() -> T const&
	{	
		m_mutex->lock();
		return std::shared_ptr<T>::operator*();
	}
	bool try_lock_for_reading()
	{
		if (m_mutex->try_lock())
		{
			return std::shared_ptr<T>::operator*();
		}
	}

	auto lock_for_writing() -> T&
	{
		m_mutex->lock();
		return std::shared_ptr<T>::operator*();
	}
	bool try_lock_for_writing()
	{
		if (m_mutex->try_lock())
		{
			return std::shared_ptr<T>::operator*();
		}
	}


private:
	std::shared_ptr<std::mutex> m_mutex;
};

BOOST_AUTO_TEST_CASE(test_util_xshared_ptr)
{
	xshared_ptr<int> x(new int(5));
	xshared_ptr<int> y(std::move(x));

	x.lock_for_writing() = 8;
	y.lock_for_writing() = 7;
}
