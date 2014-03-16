#pragma once

namespace std
{
	class mutex
	{
		mutex(const mutex&) = delete;
		mutex& operator=(const mutex&) = delete;
	public:
		mutex();
		~mutex();
		
		void	lock();
		bool	lock(uint32_t timeout_ms);
		bool	try_lock();
		void	unlock();

	protected:
		volatile bool	m_is_locked;
	};

	inline mutex::mutex()
	: m_is_locked(false)
	{
	}

	inline void mutex::lock()
	{
		while (!try_lock())
		{
			board::clock::delay_micros(100);
		}
	}
	inline bool mutex::lock(uint32_t timeout_ms)
	{
		if (try_lock())
		{
			return true;
		}
		uint32_t us = timeout_ms * 1000ULL;
		do
		{
			uint32_t delay = std::min(100ULL, us);
			board::clock::delay_micros(delay);
			us -= delay;
			if (try_lock())
			{
				return true;
			}
		}
		while (us > 0);
		return false;
	}

	inline void mutex::unlock()
	{
		m_is_locked = false;
	}

	inline bool mutex::try_lock()
	{
		uint8_t sreg = SREG;
		cli();
		bool result = !m_is_locked;
		m_is_locked = true;
		SREG = sreg;
		return result;
	}

	//////////////////////////////////////////////////////////////////////////

	class lock_guard
	{
		lock_guard(const lock_guard&) = delete;
		lock_guard& operator=(const lock_guard&) = delete;
	public:
		lock_guard(mutex& mutex);
		~lock_guard();
		void	lock();
		void	unlock();
	private:
		bool	m_is_locked;
		mutex&	m_mutex;
	};

	inline lock_guard::lock_guard(mutex& mutex)
		: m_mutex(mutex)
	{
		m_mutex.lock();
		m_is_locked = true;
	}
	inline lock_guard::~lock_guard()
	{
		unlock();
	}
	inline void lock_guard::lock()
	{
		if (!m_is_locked)
		{
			m_mutex.lock();
			m_is_locked = true;
		}
	}
	inline void lock_guard::unlock()
	{
		if (m_is_locked)
		{
			m_mutex.unlock();
			m_is_locked = false;
		}
	}
}
