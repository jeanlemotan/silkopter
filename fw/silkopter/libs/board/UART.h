#pragma once

#include <stdint.h>
#include <stddef.h>
#include "util/Noncopyable.h"
#include "util/FString.h"

namespace board
{

//////////////////////////////////////////////////////////////////////////
class UART : util::Noncopyable
{
public:
	//Selects between blocking writes and non-blocking
	//NOTE: non-blocking writes will skip data when the buffer is full and result ERR_TX_OVERFLOW
	//By default the UART is blocking
	//This can be called anytime
	virtual void set_blocking(bool blocking) = 0;
	virtual bool is_blocking() const = 0;

	//when false, writes are sent directly without any buffering
	//this only works with blocking writes
	virtual void set_buffered(bool buffered) = 0;
	virtual bool is_buffered() const = 0;
	
	//starts communication at the specified speed
    virtual void begin(uint32_t baud) = 0;
	virtual void end() = 0;

	enum class Error
	{
		NONE,
		RX_OVERFLOW,
		RX_UNDERFLOW,
		TX_OVERFLOW,
	};

	//returns the number of bytes available for reading
	virtual size_t get_data_size() const = 0;
	
	//returns true of there is data available for reading. 
	//If true, the next read_byte is guaranteed not to block in blocking mode
	virtual bool has_data() const = 0;
	
	//reads some data.
	//In blocking mode this can block to wait for all data.
	//In non-blocking mode it will flag an RX_UNDERFLOW error if there is not enough data but some data might still be read
	//It returns the number of byte successfully read
	virtual size_t read(uint8_t* buf, size_t size) = 0;

	template<typename T>
	bool read(T& t)
	{
		auto sz = read(reinterpret_cast<uint8_t*>(&t), sizeof(t));
		return sz == sizeof(t);
	}
	
	//writes data in various formats.
	//In blocking mode this can block if the buffer gets full.
	//In non-blocking mode it will flag an TX_OVERFLOW error if there is not enough room for all the data
	//It returns the number of byte successfully written
	virtual size_t write(util::Flash_String const& str) = 0;
    virtual size_t write(const uint8_t* buf, size_t size) = 0;
	
	template<typename T>
	size_t write(T t)
	{
		return write(reinterpret_cast<uint8_t*>(&t), sizeof(t));
	}
	
	template<class Fmt, typename... Params>
	void printf(Fmt const& fmt, Params... params)
	{
		util::format(*this, fmt, params...);
	}
	
	//this blocks until the tx buffer is empty.
	virtual void flush() = 0;
	
	//returns the last error.
	virtual Error get_last_error() const = 0;
	
	//returns the number of bytes received ever
	virtual size_t get_rx_data_counter() const = 0;
};

}

namespace util
{
	namespace formatting
	{

		//////////////////////////////////////////////////////////////////////////
		//utility class to be able to format directly in the uart
		template<> struct Dst_Adapter<board::UART>
		{
			Dst_Adapter(board::UART& uart) : m_uart(uart) {}
			void append(char ch) { m_uart.write(ch); }
			void append(char const* start, char const* end) { m_uart.write(reinterpret_cast<uint8_t const*>(start), end - start); }
			void clear() {}
			board::UART& m_uart;
		};

	}
}

		