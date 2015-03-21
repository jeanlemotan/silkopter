#pragma once

namespace util
{

	template<
		class MESSAGE_T,
		class SOCKET_T>
    class Serial_Channel : q::util::Noncopyable
	{
	public:
		typedef MESSAGE_T Message_t;
        typedef SOCKET_T Socket_t;


        Serial_Channel(Socket_t& socket) : m_socket(socket) {}

        void process()  { Message_t message; get_next_message(message); }

		//////////////////////////////////////////////////////////////////////////

		enum class Send_Result : uint8_t
		{
            SENT,               //message sent - this is returned only by get_send_result(). send() never returns it
			SCHEDULED,			//all good - call get_send_result to check the status
            BUSY,               //a message is already in progress. Cancel it with cancel_send
			FAILED,				//a generic error
		};

		//sends a message with confirmation
		template<typename... Params>
        Send_Result send(Message_t message, Params&&... params)
        {
            if (m_ack.in_progress)
            {
                return Send_Result::BUSY;
            }
#ifdef __AVR__
            return _send(message, true, HEADER_SIZE, params...);
#else
            return _send(message, true, HEADER_SIZE, std::forward<Params>(params)...);
#endif
        }

        Send_Result get_send_result() const { return _get_send_result(); }
        void cancel_send() { _cancel_send(); }

		//////////////////////////////////////////////////////////////////////////

		enum class Stream_Result : uint8_t
		{
			OK = 0,				//all good
			FAILED,				//a generic error
		};

		//sends a message without confirmation
		template<typename... Params>
        Stream_Result stream(Message_t message, Params&&... params)
        {
#ifdef __AVR__
            return _stream(message, params...);
#else
            return _stream(message, std::forward<Params>(params)...);
#endif
        }

        void begin_stream()
        {
            m_tx_buffer.resize(HEADER_SIZE);
        }
        template<class Param> void add_to_stream_at(size_t off, Param const& p)
        {
            set_value_fixed(m_tx_buffer, p, HEADER_SIZE + off);
        }
        template<class Param> void add_to_stream(Param const& p)
        {
            auto off = m_tx_buffer.size();
            set_value(m_tx_buffer, p, off);
        }
        Stream_Result end_stream(Message_t message)
        {
            auto res = _send(message, false, m_tx_buffer.size());
            return (res == Send_Result::SCHEDULED || res == Send_Result::SENT) ? Stream_Result::OK : Stream_Result::FAILED;
        }

		//////////////////////////////////////////////////////////////////////////

		//returns the nest message or nothing.
		//the message, if any, has to be decoded with decode_next_message(...)
		bool get_next_message(Message_t& message)  { return _get_next_message(message); }

		enum class Unpack_Result : uint8_t
		{
			OK = 0,				//all good
			FAILED,				//a generic error
		};

		//decodes the next message
		template<typename... Params>
        Unpack_Result unpack(Params&... params) { return _unpack(size_t(0), params...); }

        Unpack_Result begin_unpack()
        {
            QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
            //q::quick_logf("begin_decode: {}, {}", m_decoded.data_size, m_rx_buffer.size());
            if (m_decoded.data_size == 0)
            {
                return Unpack_Result::FAILED;
            }
            return Unpack_Result::OK;
        }
        template<class Param> Unpack_Result unpack_param(Param& p)
        {
            QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
            //q::quick_logf("unpack_param: {}, {}", m_decoded.data_size, m_rx_buffer.size());
            constexpr auto sz = sizeof(Param);
            if (m_decoded.data_size < sz || m_rx_buffer.size() < sz)
            {
                return Unpack_Result::FAILED;
            }
            p = get_value_fixed<Param>(m_rx_buffer, 0);
            m_decoded.data_size -= sz;
            pop_front(sz);
            return Unpack_Result::OK;
        }
        void end_unpack()
        {
            QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
            if (m_decoded.data_size > 0)
            {
                pop_front(m_decoded.data_size);
                m_decoded.data_size = 0;
            }
        }

		//////////////////////////////////////////////////////////////////////////

        size_t get_pending_data_size() const { return m_rx_buffer.size(); }
        size_t get_error_count() const { return m_error_count; }

	private:
        typedef uint8_t Magic_t;
        typedef uint8_t Message_Size_t;
        typedef uint8_t Header_Crc_t;
        typedef uint16_t Data_Crc_t;

        static const uint8_t MAGIC = 0x3F;
        static const uint16_t MAGIC_MASK = 0x3F;
        static const uint16_t ACK_BIT = 1 << 6;
        static const uint16_t NEEDS_ACK_BIT = 1 << 7;

        static const size_t MAGIC_OFFSET = 0;
        static const size_t MESSAGE_OFFSET = MAGIC_OFFSET + sizeof(Magic_t);
        static const size_t SIZE_OFFSET = MESSAGE_OFFSET + sizeof(Message_t);
        static const size_t HEADER_CRC_OFFSET = SIZE_OFFSET + sizeof(Message_Size_t);
        static const size_t DATA_CRC_OFFSET = HEADER_CRC_OFFSET + sizeof(Header_Crc_t);
        static const size_t HEADER_SIZE = DATA_CRC_OFFSET + sizeof(Data_Crc_t);

#ifdef __AVR__
        typedef q::util::Circular_Buffer<uint8_t, 256> RX_Buffer_t;
        typedef util::Buffer<uint8_t, 256> TX_Buffer_t;
#else
        typedef std::vector<uint8_t> RX_Buffer_t;
        typedef std::vector<uint8_t> TX_Buffer_t;
#endif
        typedef Serial_Channel<MESSAGE_T, SOCKET_T> This_t;


        struct Decoded
		{
            Magic_t magic = 0;
			Message_t message;
            Message_Size_t data_size = 0;
            Header_Crc_t header_crc = 0;
            Data_Crc_t data_crc = 0;
		} m_decoded;

		struct Sent
		{
			Send_Result result = Send_Result::FAILED;
			bool in_progress = false;
			Message_t message;
            Data_Crc_t data_crc = 0;
        } m_ack;


        template<class T> T get_value_fixed(RX_Buffer_t const& t, size_t off)
		{
            QASSERT(off + sizeof(T) <= t.size());
            T val;
            auto* dst = reinterpret_cast<uint8_t*>(&val);
            for (uint8_t i = 0, sz = sizeof(T); i < sz; i++)
            {
                *dst++ = t[off + i];
            }
			return val;
		}
        template<class T> T get_value(RX_Buffer_t const& t, size_t& off)
        {
            auto val = get_value_fixed<T>(t, off);
            off += sizeof(T);
            return val;
        }
        template<class Container, class T> void set_value_fixed(Container& t, T const& val, size_t off)
        {
            QASSERT_MSG(off + sizeof(T) <= t.size(), "off {}, sizet {}, t.size {}", off, sizeof(T), t.size());
//#ifdef __AVR__
            auto const* src = reinterpret_cast<uint8_t const*>(&val);
            for (size_t i = 0, sz = sizeof(T); i < sz; i++)
            {
                t[off + i] = *src++;
            }
//#else
//            *reinterpret_cast<T*>(t.data() + off) = val;
//#endif
        }
        template<class Container, class T> void set_value(Container& t, T const& val, size_t& off)
        {
            if (off + sizeof(T) > t.size())
            {
                t.resize(off + sizeof(T));
            }
            set_value_fixed(t, val, off);
            off += sizeof(T);
        }

        void pop_front(size_t size)
        {
            QASSERT(size <= m_rx_buffer.size());
#ifdef __AVR__
            m_rx_buffer.pop_front(size);
#else
            m_rx_buffer.erase(m_rx_buffer.begin(), m_rx_buffer.begin() + size);
#endif
        }

		//returns the nest message or nothing.
		//the message, if any, has to be decoded with decode_next_message(...)
		bool _get_next_message(Message_t& message)  
		{
            if (m_decoded.data_size > 0)
            {
                QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
                pop_front(m_decoded.data_size);
                m_decoded.data_size = 0;
            }

            //check for incoming messages
            while (decode_message()) {}
            if (m_decoded.magic == 0)
            {
                //read from the socket
                m_socket.read(m_rx_buffer);
                return false;
            }

			message = m_decoded.message;
			return true;
		}

        void send_ack(Message_t message, Data_Crc_t original_data_crc)
		{
            size_t data_size = sizeof(Data_Crc_t);
            size_t total_size = HEADER_SIZE + data_size;
            if (m_tx_buffer.size() < total_size)
            {
                m_tx_buffer.resize(total_size);
            }

            Magic_t magic = MAGIC | ACK_BIT;
            set_value_fixed(m_tx_buffer, magic, MAGIC_OFFSET);
            set_value_fixed(m_tx_buffer, message, MESSAGE_OFFSET);
            set_value_fixed(m_tx_buffer, Message_Size_t(data_size), SIZE_OFFSET);
            set_value_fixed(m_tx_buffer, Header_Crc_t(0), HEADER_CRC_OFFSET);

            //header crc
            auto header_crc = q::util::compute_crc<Header_Crc_t>(m_tx_buffer, HEADER_CRC_OFFSET);
            set_value_fixed(m_tx_buffer, header_crc, HEADER_CRC_OFFSET);

            //payload
            set_value_fixed(m_tx_buffer, original_data_crc, DATA_CRC_OFFSET + sizeof(Data_Crc_t));

            //data crc
            set_value_fixed(m_tx_buffer, Data_Crc_t(0), DATA_CRC_OFFSET);
            auto data_crc = q::util::compute_crc<Data_Crc_t>(m_tx_buffer, total_size);
            set_value_fixed(m_tx_buffer, data_crc, DATA_CRC_OFFSET);

            //send
            m_socket.write(m_tx_buffer, total_size);
		}

        constexpr size_t _sizeof() const
        {
            return 0;
        }
        template<typename Param, typename... Params>
        constexpr size_t _sizeof(Param& p, Params&... params) const
        {
            return sizeof(p) + _sizeof(params...);
        }

		//decodes the next message
        Unpack_Result _unpack(size_t off)
		{
            if (off == 0)
            {
                //q::quick_logf("unpacking {} {}, size {}/0, hcrc {}, dcrc {}", (m_decoded.magic & ACK_BIT) ? "ack" : "msg", m_decoded.message, m_decoded.data_size, m_decoded.header_crc, m_decoded.data_crc);
            }
            if (m_decoded.data_size > 0)
			{
                QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
                pop_front(m_decoded.data_size);
                m_decoded.data_size = 0;
			}
			return Unpack_Result::OK;
		}

		//decodes the next message
		template<typename Param, typename... Params>
        Unpack_Result _unpack(size_t off, Param& p, Params&... params)
		{
            if (off == 0)
            {
                //q::quick_logf("unpacking {} {}, size {}/{}, hcrc {}, dcrc {}", (m_decoded.magic & ACK_BIT) ? "ack" : "msg", m_decoded.message, _sizeof(p) + _sizeof(params...), m_decoded.data_size, m_decoded.header_crc, m_decoded.data_crc);
            }
            QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());
            if (m_decoded.data_size == 0 || m_rx_buffer.size() < off + sizeof(Param))
            {
                return Unpack_Result::FAILED;
            }
            p = get_value<Param>(m_rx_buffer, off);
            return _unpack(off, params...);
        }

        bool decode_message()
		{
            m_decoded.magic = 0;
            m_decoded.data_size = 0;

            //check if we have enough data
            if (m_rx_buffer.size() < HEADER_SIZE)
			{
                //read from the socket and check again
                m_socket.read(m_rx_buffer);
                if (m_rx_buffer.size() < HEADER_SIZE)
                {
                    return false;
                }
			}

            //try to decode a message HEADER
            auto magic = get_value_fixed<Magic_t>(m_rx_buffer, MAGIC_OFFSET);
            if ((magic & MAGIC_MASK) != MAGIC)
            {
                m_error_count++;
                q::quick_logf("malformed package {}: magic {}, ack {}, needs_ack {}", magic, magic & MAGIC_MASK, magic & ACK_BIT, magic & NEEDS_ACK_BIT);
                pop_front(1);
                return true;
            }
            auto message = get_value_fixed<Message_t>(m_rx_buffer, MESSAGE_OFFSET);
            auto size = get_value_fixed<Message_Size_t>(m_rx_buffer, SIZE_OFFSET);
            auto header_crc = get_value_fixed<Header_Crc_t>(m_rx_buffer, HEADER_CRC_OFFSET);

            //verify header crc
            {
                Header_Crc_t computed_header_crc = q::util::compute_crc<Header_Crc_t>(m_rx_buffer, HEADER_CRC_OFFSET);
                if (header_crc != computed_header_crc)
                {
                    m_error_count++;
                    q::quick_logf("header crc failed {} / {} for {} {} size {}", header_crc, computed_header_crc, (magic & ACK_BIT) ? "ack" : "msg", message, size);
                    pop_front(1);
                    return true;
                }
            }

            if (m_rx_buffer.size() < HEADER_SIZE + size)
            {
                //read from the socket and check again
                m_socket.read(m_rx_buffer);
                if (m_rx_buffer.size() < HEADER_SIZE + size)
                {
                    return false;
                }
            }

            auto data_crc = get_value_fixed<Data_Crc_t>(m_rx_buffer, DATA_CRC_OFFSET);

            //clear crc bytes and compute crc
            set_value_fixed(m_rx_buffer, Data_Crc_t(0), DATA_CRC_OFFSET);
            auto computed_data_crc = q::util::compute_crc<Data_Crc_t>(m_rx_buffer, HEADER_SIZE + size);
            if (data_crc != computed_data_crc)
            {
                m_error_count++;
                q::quick_logf("data crc failed {} / {} for {} {} size {}", data_crc, computed_data_crc, (magic & ACK_BIT) ? "ack" : "msg",message, size);
                set_value_fixed(m_rx_buffer, data_crc, DATA_CRC_OFFSET);
                pop_front(1);
                return true;
            }
            pop_front(HEADER_SIZE);

            m_decoded.magic = magic;
            m_decoded.message = message;
            m_decoded.data_size = size;
            m_decoded.header_crc = header_crc;
            m_decoded.data_crc = data_crc;

            QASSERT_MSG(m_decoded.data_size <= m_rx_buffer.size(), "{}, {}", m_decoded.data_size, m_rx_buffer.size());

            //if the message is the ack of a previous send, validare it
            if (magic & ACK_BIT)
            {
                if (m_decoded.message == m_ack.message)
                {
                    m_ack.in_progress = false;

                    Data_Crc_t crc = 0;
                    _unpack(size_t(0), crc);
                    m_ack.result = (crc == m_ack.data_crc) ? Send_Result::SENT : Send_Result::FAILED;
                }
                else
                {
                    m_error_count++;
                    q::quick_logf("received ack for wrong message {} / {}", m_decoded.message, m_ack.message);
                    pop_front(sizeof(Data_Crc_t));
                }
                m_decoded.magic = 0;
                m_decoded.data_size = 0;
                return true;
            }

            //q::quick_logf("received msg {}, size {}, hcrc {}, dcrc {}", m_decoded.message, m_decoded.data_size, m_decoded.header_crc, m_decoded.data_crc);

            if (m_decoded.magic & NEEDS_ACK_BIT)
            {
                send_ack(m_decoded.message, m_decoded.data_crc);
            }

			return false;
		}

		//sends a message with confirmation
        Send_Result _send(Message_t message, bool needs_ack, size_t total_size)
		{
            QASSERT(total_size < 255);
            QASSERT(total_size >= HEADER_SIZE);
            size_t data_size = total_size - HEADER_SIZE;
			//header
            Magic_t magic = MAGIC;
            if (needs_ack)
            {
                magic |= NEEDS_ACK_BIT;
            }
            set_value_fixed(m_tx_buffer, magic, MAGIC_OFFSET);
            set_value_fixed(m_tx_buffer, message, MESSAGE_OFFSET);
            set_value_fixed(m_tx_buffer, Message_Size_t(data_size), SIZE_OFFSET);
            set_value_fixed(m_tx_buffer, Header_Crc_t(0), HEADER_CRC_OFFSET);

            //header crc
            auto header_crc = q::util::compute_crc<Header_Crc_t>(m_tx_buffer, HEADER_CRC_OFFSET);
            set_value_fixed(m_tx_buffer, header_crc, HEADER_CRC_OFFSET);

            //data crc
            set_value_fixed(m_tx_buffer, Data_Crc_t(0), DATA_CRC_OFFSET);
            auto data_crc = q::util::compute_crc<Data_Crc_t>(m_tx_buffer, total_size);
            set_value_fixed(m_tx_buffer, data_crc, DATA_CRC_OFFSET);

            //q::quick_logf("sending msg {}, size {}, hcrc {}, dcrc {}", message, data_size, header_crc, data_crc);

			//send
            m_socket.write(m_tx_buffer, total_size);

			if (needs_ack)
			{
                m_ack.message = message;
                m_ack.in_progress = true;
                m_ack.result = Send_Result::SCHEDULED;
                m_ack.data_crc = data_crc;
                return m_ack.result;
			}

            return Send_Result::SENT;
		}

        //sends a message with confirmation
        template<typename Param, typename... Params>
        Send_Result _send(Message_t message, bool needs_ack, size_t off, Param const& param, Params&&... params)
        {
            QSTATIC_ASSERT(sizeof(Param) + _sizeof(params...) + HEADER_SIZE < 255);
            set_value(m_tx_buffer, param, off);
#ifdef __AVR__
            return _send(message, needs_ack, off, params...);
#else
            return _send(message, needs_ack, off, std::forward<Params>(params)...);
#endif
        }

		Send_Result _get_send_result() const
		{ 
            return m_ack.result;
		}
        void _cancel_send()
        {
            m_ack.in_progress = false;
            m_ack.result = Send_Result::FAILED;
        }

		template<typename... Params>
        Stream_Result _stream(Message_t message, Params&&... params)
		{
#ifdef __AVR__
            auto res = _send(message, false, HEADER_SIZE, params...);
#else
            auto res = _send(message, false, HEADER_SIZE, std::forward<Params>(params)...);
#endif
			return (res == Send_Result::SCHEDULED || res == Send_Result::SENT) ? Stream_Result::OK : Stream_Result::FAILED;
		}

		//////////////////////////////////////////////////////////////////////////

        Socket_t& m_socket;
		RX_Buffer_t m_rx_buffer;
		TX_Buffer_t m_tx_buffer;
        size_t m_error_count = 0;
	};


#if !defined (Q_AVR)

	template<class S>
	class Asio_Socket_Adapter : q::util::Noncopyable
	{
        typedef std::vector<uint8_t> TX_Buffer_t;
        typedef std::vector<uint8_t> RX_Buffer_t;

	public:
		typedef S Socket_t;

		Asio_Socket_Adapter(Socket_t& s) 
			: m_socket(s) 
		{
		}

		void start()
		{
            m_socket.async_read_some(boost::asio::buffer(m_rx_temp_buffer.data(), m_rx_temp_buffer.size()),
                                    boost::bind(&Asio_Socket_Adapter<Socket_t>::handle_receive, this,
                                    boost::asio::placeholders::error,
                                    boost::asio::placeholders::bytes_transferred));
//            m_socket.async_receive(
//                boost::asio::buffer(m_rx_temp_buffer.data(), m_rx_temp_buffer.size()),
//                boost::bind(&Asio_Socket_Adapter<Socket_t>::handle_receive, this,
//				boost::asio::placeholders::error,
//				boost::asio::placeholders::bytes_transferred));
		}

        template<class Container>
        void read(Container& dst)
		{ 
            std::lock_guard<std::mutex> lg(m_rx_mutex);
            auto off = dst.size();
            dst.resize(off + m_rx_buffer.size());
            std::copy(m_rx_buffer.begin(), m_rx_buffer.end(), dst.begin() + off);
            m_rx_buffer.clear();
		}
        template<class Container>
        void write(Container const& data, size_t size)
		{ 
			if (size)
			{
                auto buffer_ptr = get_tx_buffer();
                buffer_ptr->resize(size);
                std::copy(begin(data), begin(data) + size, buffer_ptr->begin());

                m_socket.async_write_some(boost::asio::buffer(*buffer_ptr),
                    boost::bind(&Asio_Socket_Adapter<Socket_t>::handle_send, this,
                        buffer_ptr,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
			}
		}

	private:
        void handle_send(std::shared_ptr<TX_Buffer_t> buffer_ptr,
                         const boost::system::error_code& /*error*/,
                         std::size_t /*bytes_transferred*/)
		{
            std::lock_guard<std::mutex> lg(m_tx_buffer_pool_mutex);
            m_tx_buffer_pool.push_back(buffer_ptr);
        }

		void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
		{
			if (error)
			{
				m_socket.close();
			}
			else
			{
                if (bytes_transferred > 0)
				{
					std::lock_guard<std::mutex> lg(m_rx_mutex);
                    auto off = m_rx_buffer.size();
                    m_rx_buffer.resize(off + bytes_transferred);
                    std::copy(m_rx_temp_buffer.begin(), m_rx_temp_buffer.begin() + bytes_transferred, m_rx_buffer.begin() + off);
//                    std::copy(m_rx_temp_buffer.begin(), m_rx_temp_buffer.begin() + bytes_transferred, std::back_inserter(m_rx_buffer));
                }
				start();
			}
		}

        std::shared_ptr<TX_Buffer_t> get_tx_buffer()
        {
            std::lock_guard<std::mutex> lg(m_tx_buffer_pool_mutex);
            std::shared_ptr<TX_Buffer_t> buffer_ptr;
            if (!m_tx_buffer_pool.empty())
            {
                buffer_ptr = m_tx_buffer_pool.back();
                m_tx_buffer_pool.pop_back();
            }
            else
            {
                buffer_ptr = std::make_shared<TX_Buffer_t>();
            }
            return buffer_ptr;
        }

		Socket_t& m_socket;
        RX_Buffer_t m_rx_buffer;
        std::array<uint8_t, 512> m_rx_temp_buffer;
		std::mutex m_rx_mutex;

        std::vector<std::shared_ptr<TX_Buffer_t>> m_tx_buffer_pool;
        std::mutex m_tx_buffer_pool_mutex;
	};

#endif


}
