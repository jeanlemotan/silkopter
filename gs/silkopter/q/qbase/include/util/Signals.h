#pragma once

namespace q
{
namespace util
{

//signal/slots
typedef boost::signals2::connection Connection;
typedef boost::signals2::shared_connection_block Shared_Connection_Block;

template <class Signature>
class Signal : util::Noncopyable
{
public:
	typedef boost::signals2::signal<Signature> signal_t;

	signal_t& get_signal()
	{
		if (!m_signal)
		{
			m_signal.reset(new signal_t());
		}
		return *m_signal;
	}
	template<class SlotType>
	Connection connect(SlotType const& slot)
	{
		return get_signal().connect(slot);
	}

	void execute()
	{
		if (m_signal)
		{
			(*m_signal)();
		}
	}
	template<class P0>
	void execute(P0&& p0)
	{
		if (m_signal)
		{
			(*m_signal)(std::forward<P0>(p0));
		}
	}
	template<class P0, class P1>
	void execute(P0&& p0, P1&& p1)
	{
		if (m_signal)
		{
			(*m_signal)(std::forward<P0>(p0), std::forward<P1>(p1));
		}
	}
	template<class P0, class P1, class P2>
	void execute(P0&& p0, P1&& p1, P2&& p2)
	{
		if (m_signal)
		{
			(*m_signal)(std::forward<P0>(p0), std::forward<P1>(p1), std::forward<P2>(p2));
		}
	}
	template<class P0, class P1, class P2, class P3>
	void execute(P0&& p0, P1&& p1, P2&& p2, P3&& p3)
	{
		if (m_signal)
		{
			(*m_signal)(std::forward<P0>(p0), std::forward<P1>(p1), std::forward<P2>(p2), std::forward<P3>(p3));
		}
	}
private:
	std::unique_ptr<boost::signals2::signal<Signature>> m_signal;
};

class Scoped_Connection : public boost::noncopyable
{
public:
	Scoped_Connection()
		: m_ptr(std::unique_ptr<Connection, decltype(&disconnect_and_delete)>(nullptr, &disconnect_and_delete))
	{
	}
	Scoped_Connection(Connection const& c)
		: m_ptr(std::unique_ptr<Connection, decltype(&disconnect_and_delete)>(new Connection(c), &disconnect_and_delete))
	{
	}
	Scoped_Connection(Scoped_Connection&& c)
		: m_ptr(std::move(c.m_ptr))
	{
	}
	Scoped_Connection& operator=(Scoped_Connection&& c)
	{
		m_ptr = std::move(c.m_ptr);
		return *this;
	}

	Connection& get()
	{
		return *m_ptr;
	}

	void reset()
	{
		m_ptr.reset(nullptr);
	}

private:
	static void disconnect_and_delete(Connection* c)
	{
		if (c)
		{
			c->disconnect();
			delete c;
		}
	}

	std::unique_ptr<Connection, decltype(&disconnect_and_delete)> m_ptr;
};

}
}
