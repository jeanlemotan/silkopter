#include "stdafx.h"
#include "Silkopter.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));

	QApplication a(argc, argv);
	QCoreApplication::setOrganizationName("Silkopter");
	QCoreApplication::setOrganizationDomain("silkopter.com");
	QCoreApplication::setApplicationName("Silkopter");

    a.setQuitOnLastWindowClosed(true);

// 	std::vector<uint8_t> buffer1;
// 	std::vector<uint8_t> buffer2;
// 
// 	enum class Message
// 	{
// 		MSG_1,
// 		MSG_2
// 	};
// 
// 	struct Socket
// 	{
// 		Socket(std::vector<uint8_t>& rx, std::vector<uint8_t>& tx)
// 			: m_rx(rx), m_tx(tx) {}
// 		void begin_rx() {}
// 		void end_rx() {}Continuare
// 		size_t get_rx_size() const { return m_rx.size(); }
// 		void get_rx_data(uint8_t* buffer, size_t size) const { QASSERT(m_rx.size() >= size); std::copy(m_rx.begin(), m_rx.begin() + size, buffer); m_rx.erase(m_rx.begin(), m_rx.begin() + size); }
// 		void send(uint8_t const* data, size_t size) { std::copy(data, data + size, std::back_inserter(m_tx)); }
// 		std::vector<uint8_t>& m_rx;
// 		std::vector<uint8_t>& m_tx;
// 	};
// 
// 	Socket s1(buffer1, buffer2);
// 	Socket s2(buffer2, buffer1);
// 
// 	util::Secure_Channel<Message, uint8_t, std::vector<uint8_t>, std::vector<uint8_t>, Socket> m_channel1(s1);
// 	util::Secure_Channel<Message, uint8_t, std::vector<uint8_t>, std::vector<uint8_t>, Socket> m_channel2(s2);
// 
// 	Message msg;
// 	auto res = m_channel1.get_next_message(msg);
// 	res = m_channel2.get_next_message(msg);
// 
// 	for (size_t i = 0; i < 999999; i++)
// 	{
// 		auto sr = m_channel1.send(Message::MSG_1, uint8_t(32), size_t(42), int(-7));
// 		auto ssr = m_channel1.get_send_result();
// 
// 		res = m_channel2.get_next_message(msg);
// 		{
// 			uint8_t p1 = 0;
// 			size_t p2 = 0;
// 			int p3 = 0;
// 			auto res = m_channel2.unpack_next_message(p1, p2, p3);
// 			res = res;
// 		}
// 
// 		res = m_channel1.get_next_message(msg);
// 		ssr = m_channel1.get_send_result();
// 	}


	Silkopter w;
	w.show();
	return a.exec();
}
