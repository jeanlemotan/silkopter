#pragma once

#include "ts_assert.h"
#include "Source_Location.h"

namespace ts
{

namespace detail
{
    struct success_helper{};
}

typedef int detail::success_helper::*success_t;
constexpr success_t success = (static_cast<success_t>(0));

//////////////////////////////////////////////////////////////////////////

class Error
{
public:

    Error(const Source_Location& source_location, const std::string& what)
        : m_source_location(source_location)
        , m_what(what)
	{}

    Error(const std::string& what)
        : m_what(what)
	{}

    Source_Location const& source_location() const{ return m_source_location; }
    std::string const& what() const { return m_what; }

protected:

    Source_Location m_source_location;
    std::string m_what;
};

//////////////////////////////////////////////////////////////////////////

template<typename T>
class Result final
{
public:

    inline ~Result() noexcept
	{
        if (m_is_success)
		{
			reinterpret_cast<T*>(&m_data)->~T();
		}
		else
		{
            reinterpret_cast<Error*>(&m_data)->~Error();
		}
#ifndef NDEBUG
        TS_ASSERT(!m_check_pending);
#endif
	}

    inline Result(const Error& error) noexcept
        : m_is_success(false)
	{
        new (&m_data) Error(error);
	}

    inline Result(Error&& error) noexcept
        : m_is_success(false)
	{
        new (&m_data) Error(std::move(error));
	}

    template<typename T2>
    inline Result(T2&& payload) noexcept
        : m_is_success(true)
	{
        new (&m_data) T(std::forward<T2>(payload));
	}

    inline Result(const Result<T>& other) = delete;

	template<typename T2>
    inline Result(const Result<T2>& other) = delete;

    template<typename T2>
    inline Result(Result<T2>&& other) noexcept
        : m_is_success(other.m_is_success)
	{
    #ifndef NDEBUG
        m_check_pending = other.m_check_pending;
        other.m_check_pending = false;
	#endif

        if (other.m_is_success)
		{
            new (&m_data) T(std::move(*reinterpret_cast<T2*>(&other.m_data)));
		}
		else
		{
            new (&m_data) Error(std::move(*reinterpret_cast<Error*>(&other.m_data)));
		}
	}

    inline Result<T>& operator=(const Result<T>& other) = delete;

	template<typename T2>
    inline Result<T>& operator=(const Result<T2>& other) = delete;

    template<typename T2>
    inline Result<T>& operator=(Result<T2>&& other)
	{
#ifndef NDEBUG
        m_check_pending = other.m_check_pending;
        other.m_check_pending = false;
#endif

        if (m_is_success)
		{
			reinterpret_cast<T*>(&m_data)->~T();
		}
		else
		{
            reinterpret_cast<Error*>(&m_data)->~Error();
		}
		
        m_is_success = other.m_is_success;

        if (other.m_is_success)
		{
            new (&m_data) T(std::move(*reinterpret_cast<T2*>(&other.m_data)));
		}
		else
		{
            new (&m_data) Error(std::move(*reinterpret_cast<Error*>(&other.m_data)));
		}
		return *this;
	}

    inline Result<T>& operator=(const Error& error)
    {
        return operator=(Result<T>(error));
    }
    inline Result<T>& operator=(Error&& error)
    {
        return operator=(Result<T>(std::move(error)));
    }

    template<typename T2>
    inline Result<T>& operator=(T2&& payload)
    {
        return operator=(Result<T>(std::forward<T2>(payload)));
    }

	inline T& payload()
	{
        TS_ASSERT(m_is_success);
		return *reinterpret_cast<T*>(&m_data);
	}


	inline T&& extract_payload()
	{
        TS_ASSERT(m_is_success);
		return std::move(*reinterpret_cast<T*>(&m_data));
	}

    inline const Error& error() const
	{
        TS_ASSERT(!m_is_success);
        return *reinterpret_cast<const Error*>(&m_data);
	}

//	explicit inline operator bool() const
//	{
//#ifndef NDEBUG
//        m_check_pending = false;
//#endif
//        return m_is_success;
//	}

    inline bool operator==(success_t) const
	{
#ifndef NDEBUG
        m_check_pending = false;
#endif
        return m_is_success;
	}
    inline bool operator!=(success_t) const
	{
#ifndef NDEBUG
        m_check_pending = false;
#endif
        return !m_is_success;
	}
private:

	union Data
	{
		double _aligment_foo;
		char successBuffer[sizeof(T)];
        char errorBuffer[sizeof(Error)];
	};

	Data m_data;
    bool m_is_success = false;
#ifndef NDEBUG
    mutable bool m_check_pending = true;
#endif
};

//////////////////////////////////////////////////////////////////////////

template<>
class Result<void> final
{
public:

    inline ~Result()
	{
        if (!m_is_success)
		{
            reinterpret_cast<Error*>(&m_data)->~Error();
		}

#ifndef NDEBUG
        TS_ASSERT(!m_check_pending);
#endif
	}

    inline Result(const Error& error)
    : m_is_success(false)
	{
        new (&m_data) Error(error);
	}

    inline Result(const success_t& success)
    : m_is_success(true)
	{
	}

    inline Result(const Result& other) = delete;

    inline Result(Result&& other)
    : m_is_success(other.m_is_success)
	{
#ifndef NDEBUG
        m_check_pending = other.m_check_pending;
        other.m_check_pending = false;
#endif
        if (!other.m_is_success)
		{
            new (&m_data) Error(std::move(*reinterpret_cast<Error*>(&other.m_data)));
		}
	}

    inline Result& operator = (const Result& other) = delete;

    inline Result& operator = (Result&& other)
	{
#ifndef NDEBUG
        m_check_pending = other.m_check_pending;
        other.m_check_pending = false;
#endif

        if (!m_is_success)
		{
            reinterpret_cast<Error*>(&m_data)->~Error();
		}

        m_is_success = other.m_is_success;

        if (!other.m_is_success)
		{
            new (&m_data) Error(std::move(*reinterpret_cast<Error*>(&other.m_data)));
		}
		return *this;
	}

    inline const Error& error() const
	{
        TS_ASSERT(!m_is_success);
        return *reinterpret_cast<const Error*>(&m_data);
	}

    inline void ignore()
    {
#ifndef NDEBUG
        m_check_pending = false;
#endif
    }

    inline bool operator==(success_t) const
	{

#ifndef NDEBUG
        m_check_pending = false;
#endif
        return m_is_success;
	}
    inline bool operator!=(success_t) const
	{

#ifndef NDEBUG
        m_check_pending = false;
#endif
        return !m_is_success;
	}
private:

	union Data
	{
		double _aligment_foo;
        char errorBuffer[sizeof(Error)];
	};

	Data m_data;
    bool m_is_success = false;
#ifndef NDEBUG
    mutable bool m_check_pending = true;
#endif
};

//////////////////////////////////////////////////////////////////////////

inline Result<void> operator&(Result<void> const& a, Result<void> const& b)
{
    if (a == success && b == success)
    {
        return success;
    }
    std::string msg;
    if (a != success)
    {
        msg += a.error().what();
    }
    if (b != success)
    {
        if (!msg.empty())
        {
            msg += "\n";
        }
        msg += b.error().what();
    }
    return Error(msg);
}

}

 
