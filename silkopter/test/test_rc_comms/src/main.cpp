#include "utils/Clock.h"
#include "utils/comms/RC_Protocol.h"
#include "utils/hw/SPI_PIGPIO.h"
#include "utils/hw/SPI_Dev.h"

extern "C"
{
#include "utils/hw/pigpio.h"
}


//#define USE_SPI_PIGPIO

#ifdef USE_SPI_PIGPIO
constexpr size_t SPI_PORT = 0;
constexpr size_t SPI_CHANNEL = 0;
constexpr size_t SPI_SPEED = 4000000;
#else
const char* SPI_DEVICE = "/dev/spidev0.0";
constexpr size_t SPI_SPEED = 10000000;
#endif

constexpr size_t SDN_GPIO = 6;
constexpr size_t NIRQ_GPIO = 26;

///////////////////////////////////////////////////////////////////////////////////////////////////

/* This prints an "Assertion failed" message and aborts.  */
void __assert_fail(const char *__assertion, const char *__file, unsigned int __line, const char *__function)
{
    QASSERT_MSG(false, "assert: {}:{}: {}: {}", __file, __line, __function, __assertion);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

static auto initialize_pigpio() -> bool
{
    static bool initialized = false;
    if (initialized)
    {
        return true;
    }

    QLOGI("Initializing pigpio");
    if (gpioCfgClock(5, 1, 0) < 0 ||
        gpioCfgPermissions(static_cast<uint64_t>(-1)))
    {
        QLOGE("Cannot configure pigpio");
        return false;
    }
    if (gpioInitialise() < 0)
    {
        QLOGE("Cannot initialize pigpio");
        return false;
    }

    initialized = true;
    return true;
}
static auto shutdown_pigpio() -> bool
{
    gpioTerminate();
    return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

std::array<std::string, 3> s_tx_periodic_payloads = { "SLAVE: ABCDEFGHIJ", "SLAVE: KLMNOP", "SLAVE: QRSTUVXYZ" };
std::array<std::string, 3> s_rx_periodic_payloads = { "MASTER: ABCDEFGHIJ", "MASTER: KLMNOP", "MASTER: QRSTUVXYZ" };

std::string s_tx_reliable_payload = { "SLAVE: This is a test string that is long enough to cause multiple packets.\n"
                                      "The second line is here in increase the size.\n"
                                      "There is a third line as well to make the size even longer.\n"
                                      "Four lines are not enough for a decent size.\n"
                                      "Maybe the fifth line will cause the packet to exceed 500 chars?\n"
                                      "Not really, a sixth line is needed, and probably a seventh as well.\n"
                                      "Yes, a seventh line is definitely, positively needed.\n"
                                      "What about an eith line? Could be.\n"
                                      "Well, just 440 chars with eight lines, so a ninth and a tenth will definitely be sufficient.\n"
                                      "This is the tenth and last line, after nine other attepmts at filling the packet.\n"
                                      "The end." };
std::string s_rx_reliable_payload = { "MASTER: This is a test string that is long enough to cause multiple packets.\n"
                                      "The second line is here in increase the size.\n"
                                      "There is a third line as well to make the size even longer.\n"
                                      "Four lines are not enough for a decent size.\n"
                                      "Maybe the fifth line will cause the packet to exceed 500 chars?\n"
                                      "Not really, a sixth line is needed, and probably a seventh as well.\n"
                                      "Yes, a seventh line is definitely, positively needed.\n"
                                      "What about an eith line? Could be.\n"
                                      "Well, just 440 chars with eight lines, so a ninth and a tenth will definitely be sufficient.\n"
                                      "This is the tenth and last line, after nine other attepmts at filling the packet.\n"
                                      "The end." };

std::string s_tx_fec_payload = { "SLAVE: This is a test string that is long enough to cause multiple packets.\n"
                                      "The second line is here in increase the size.\n"
                                      "There is a third line as well to make the size even longer.\n"
                                      "Four lines are not enough for a decent size.\n"
                                      "Maybe the fifth line will cause the packet to exceed 500 chars?\n"
                                      "Not really, a sixth line is needed, and probably a seventh as well.\n"
                                      "Yes, a seventh line is definitely, positively needed.\n"
                                      "What about an eith line? Could be.\n"
                                      "Well, just 440 chars with eight lines, so a ninth and a tenth will definitely be sufficient.\n"
                                      "This is the tenth and last line, after nine other attepmts at filling the packet.\n"
                                      "The end." };
std::string s_rx_fec_payload = { "MASTER: This is a test string that is long enough to cause multiple packets.\n"
                                      "The second line is here in increase the size.\n"
                                      "There is a third line as well to make the size even longer.\n"
                                      "Four lines are not enough for a decent size.\n"
                                      "Maybe the fifth line will cause the packet to exceed 500 chars?\n"
                                      "Not really, a sixth line is needed, and probably a seventh as well.\n"
                                      "Yes, a seventh line is definitely, positively needed.\n"
                                      "What about an eith line? Could be.\n"
                                      "Well, just 440 chars with eight lines, so a ninth and a tenth will definitely be sufficient.\n"
                                      "This is the tenth and last line, after nine other attepmts at filling the packet.\n"
                                      "The end." };

void rx_callback(util::comms::RC_Protocol::RX_Packet const& packet, uint8_t* data, size_t size)
{
    if (size == 0)
    {
        QLOGW("Payload too small: {}", size);
        return;
    }

    if (packet.packet_type == 0xFF)
    {
        //std::string payload((char*)data, (char*)data + size);
        //QLOGI("FEC: \n{}", payload);
        static Clock::time_point last_tp = Clock::now();
        static size_t accumulated_size = 0;
        static size_t average_size = 0;
        accumulated_size += size;

        if (Clock::now() - last_tp >= std::chrono::seconds(1))
        {
            average_size = (average_size * 9 + accumulated_size) / 10;
            QLOGI("FEC bandwidth: {}bps", average_size);
            accumulated_size = 0;
            last_tp = Clock::now();
        }
    }
    else if (packet.packet_type == 10)
    {
        std::string payload((char*)data, (char*)data + size);
        if (s_rx_reliable_payload != payload)
        {
            QLOGW("Incorrect reliable payload. xpected {}, got {}", s_rx_reliable_payload, payload);
        }
        else
        {
            //QLOGW("Correct reliable payload");
        }
    }
    else
    {
        uint32_t state = packet.packet_type;
        if (state >= s_tx_periodic_payloads.size())
        {
            QLOGW("Corrupted state {}. Max is {}", state, s_tx_periodic_payloads.size());
            return;
        }
        std::string payload((char*)data, (char*)data + size);
        if (s_rx_periodic_payloads[state] != payload)
        {
            QLOGW("Incorrect periodic payload, state {}. xpected {}, got {}", state, s_rx_periodic_payloads[state], payload);
        }
        else
        {
            //QLOGI("Correct periodic payload");
        }
    }
}

bool tx_callback(util::comms::RC_Protocol::Buffer& buffer, uint8_t& packet_type)
{
    static uint32_t state = 0;

    packet_type = state;
    std::string payload = s_tx_periodic_payloads[state];

    state = (state + 1) % s_tx_periodic_payloads.size();

    size_t offset = buffer.size();
    buffer.resize(offset + payload.size());
    memcpy(buffer.data() + offset, payload.data(), payload.size());

    return true;
}

int main(int argc, char *argv[])
{
    q::logging::add_logger(q::logging::Logger_uptr(new q::logging::Console_Logger()));
    q::logging::set_decorations(q::logging::Decorations(q::logging::Decoration::TIMESTAMP, q::logging::Decoration::LEVEL, q::logging::Decoration::TOPIC));

    if (!initialize_pigpio())
    {
        QLOGW("Cannot initialize pigpio");
        exit(1);
    }

    util::comms::RC_Phy rc_phy(false);

#ifdef USE_SPI_PIGPIO
    util::hw::SPI_PIGPIO* spi = new util::hw::SPI_PIGPIO();
    ts::Result<void> result = spi->init(SPI_PORT, SPI_CHANNEL, SPI_SPEED, 0);
#else
    util::hw::SPI_Dev* spi = new util::hw::SPI_Dev();
    ts::Result<void> result = spi->init(SPI_DEVICE, SPI_SPEED);
#endif
    if (result != ts::success)
    {
        QLOGW("Cannot start spi: {}", result.error().what());
        exit(1);
    }
    if (!rc_phy.init(*spi, SDN_GPIO, NIRQ_GPIO))
    {
        QLOGW("Cannot init phy");
        exit(1);
    }

    util::comms::RC_Protocol rc_protocol(rc_phy, std::bind(&rx_callback, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    if (!rc_protocol.init(5, 7))
    {
        QLOGW("Cannot init protocol");
        exit(1);
    }

    rc_protocol.add_periodic_packet(std::chrono::milliseconds(50), std::bind(&tx_callback, std::placeholders::_1, std::placeholders::_2));

    Clock::time_point last_reliable_tp = Clock::now();

    while (true)
    {
        rc_protocol.process();

        if (Clock::now() - last_reliable_tp >= std::chrono::seconds(3))
        {
            last_reliable_tp = Clock::now();
            rc_protocol.send_reliable_packet(10, s_tx_reliable_payload.data(), s_tx_reliable_payload.size());
            //rc_protocol.send_fec_packet(s_tx_fec_payload.data(), s_tx_fec_payload.size());
        }
    }

    shutdown_pigpio();

    return 0;
}
