#include "BrainStdAfx.h"
#include "Sim_Comms.h"

#ifndef RASPBERRY_PI

using namespace silk;
using namespace boost::asio;

Sim_Comms::Sim_Comms(io_service& io_service)
    : m_io_service(io_service)
    , m_socket(io_service)
    , m_channel(m_socket)
{
}

auto Sim_Comms::connect() -> bool
{
    if (is_connected())
    {
        return true;
    }

    uint16_t port = 52523;
    try
    {
        m_acceptor = std::make_unique<ip::tcp::acceptor>(m_io_service, ip::tcp::v4());
        m_acceptor->set_option(ip::tcp::acceptor::reuse_address(true));
        m_acceptor->bind(ip::tcp::endpoint(ip::tcp::v4(), port));
        m_acceptor->listen();

        m_acceptor->async_accept(m_socket, boost::bind(&Sim_Comms::handle_accept, this, boost::asio::placeholders::error));
    }
    catch(...)
    {
        SILK_WARNING("Cannot start listening on port {}", port);
        return false;
    }

    SILK_INFO("Started listening on port {}", port);
    while (!m_socket.is_open())
    {
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
        SILK_INFO("Waiting for sim to connect");
    }

    set_state(State::HANDSHAKE);
    return true;
}

void Sim_Comms::handle_accept(boost::system::error_code const& error)
{
    if (error)
    {
        SILK_WARNING("Error occured while accepting connection: {}", error.message());
        return;
    }

    m_channel.start();
    SILK_INFO("Connected to {}:{}", m_socket.remote_endpoint().address().to_string(), m_socket.remote_endpoint().port());
}

void Sim_Comms::disconnect()
{
    m_acceptor.reset();
    set_state(State::DISCONNECTED);
}
bool Sim_Comms::is_connected() const
{
    return m_state != State::DISCONNECTED;
}

//----------------------------------------------------------------------------------

void Sim_Comms::process_state_disconnected()
{

}
void Sim_Comms::process_state_handshake()
{
    if (m_has_message)
    {
        if (m_message == Message::CONFIG)
        {
            auto result = m_channel.unpack(m_received_config.version,
                                           m_received_config.has_barometer,
                                           m_received_config.has_sonar,
                                           m_received_config.has_gps,
                                           m_received_config.has_voltage_sensor,
                                           m_received_config.has_current_sensor);
            if (result)
            {
                SILK_INFO("Sim version {}:\b \t{} barometer, \t{} gps, \t{} sonar, \t{} voltage sensor, \t{} current sensor",
                           m_received_config.version,
                           m_received_config.has_barometer ? "has a" : "doesn't have",
                           m_received_config.has_gps ? "has a" : "doesn't have",
                           m_received_config.has_sonar ? "has a" : "doesn't have",
                           m_received_config.has_voltage_sensor ? "has a" : "doesn't have",
                           m_received_config.has_current_sensor ? "has a" : "doesn't have");

                set_state(State::RUNNING);
            }
            else
            {
                SILK_WARNING("Failed to communicate with the Simulator");
                m_error_count++;
            }
        }
        else
        {
            SILK_WARNING("Received wrong message: {}", static_cast<int>(m_message));
            m_error_count++;
        }
    }
}

size_t Sim_Comms::get_error_count() const
{
    return m_error_count;
}


void Sim_Comms::process()
{
    if (!m_socket.is_open())
    {
        disconnect();
    }

    if (!is_connected())
    {
        connect();
        return;
    }

    do
    {
        m_has_message = m_channel.get_next_message(m_message);
        if (m_has_message)
        {
            m_message_count++;
            m_last_message_timestamp = q::Clock::now();

            message_received_signal.execute(m_message, m_channel);
        }

        switch (m_state)
        {
        case State::DISCONNECTED: process_state_disconnected(); break;
        case State::HANDSHAKE: process_state_handshake(); break;
        case State::RUNNING: break;
        default: QASSERT(0); break;
        }

    } while (m_has_message);
}

void Sim_Comms::set_state(State state)
{
    SILK_INFO("Switching from state {} to state {}", static_cast<int>(m_state), static_cast<int>(state));
    m_state = state;
}


#endif
