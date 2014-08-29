#pragma once

namespace util
{

template <class T>
class Atomic
{
public:
	Atomic() = default;
	Atomic(Atomic<T> const& other)
	{
		set(other.get());
	}
	explicit Atomic(T val) : m_value(val) {}

	Atomic<T>& operator=(Atomic<T> const& other)
	{
		set(other.get());
	}

	//sets a new value
	void set(T val)
	{
		uint8_t old_sreg = SREG;
		cli();

		m_value = val;

		SREG = old_sreg;
	}

	//sets a new value and returns the old one
	T exchange(T val)
	{
		uint8_t old_sreg = SREG;
		cli();

		T old = m_value;
		m_value = val;

		SREG = old_sreg;
		return old;
	}

	//returns the current value
	T get() const
	{
		uint8_t old_sreg = SREG;
		cli();

		T val = m_value;

		SREG = old_sreg;
		return val;
	}
	
private:
	volatile T m_value = T();
};


}