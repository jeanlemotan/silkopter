#include "RFM22B.h"
#include "SPI_Dev.h"

#include <thread>

#ifdef RASPBERRY_PI
extern "C"
{
    #include "pigpio.h"
}
extern std::chrono::microseconds PIGPIO_PERIOD;

#endif

namespace util
{
namespace hw
{

constexpr size_t MAX_PACHET_SIZE = 64;

/////////////////////////////////////////////////////////////////

RFM22B::RFM22B()
{

}

RFM22B::~RFM22B()
{

}

bool RFM22B::init(ISPI& spi, uint8_t sdn_gpio)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    if (m_spi)
    {
        return true;
    }
    m_spi = &spi;

    m_sdn_gpio = sdn_gpio;

    //configure the GPIOs
#if defined RASPBERRY_PI

    int sdn_gpio_mode = gpioGetMode(m_sdn_gpio);
    if (sdn_gpio_mode == PI_BAD_GPIO)
    {
        QLOGE("The SDN GPIO {} is bad", m_sdn_gpio);
        return false;
    }

    int res = gpioSetMode(m_sdn_gpio, PI_OUTPUT);
    if (res != 0)
    {
        QLOGE("The SDN GPIO {} is bad", m_sdn_gpio);
        goto error;
    }

    shutdown();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    powerup();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Software reset the device
    reset();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    idle_mode();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Get the device type and check it
    // This also tests whether we are really connected to a device
    {
        uint8_t device_type = get_register(Register::DEVICE_TYPE_00);
        if (device_type != (uint8_t)Device_Type::RX_TRX && device_type != (uint8_t)Device_Type::TX)
        {
            QLOGE("Wrong device type: {}", device_type);
            goto error;
        }
    }

    set_gpio_function(RFM22B::GPIO::GPIO0, RFM22B::GPIO_Function::TX_STATE);
    set_gpio_function(RFM22B::GPIO::GPIO1, RFM22B::GPIO_Function::RX_STATE);

#endif

    return true;

error:
    shutdown();
    m_spi = nullptr;

#if defined RASPBERRY_PI

    gpioSetMode(m_sdn_gpio, sdn_gpio_mode);

#endif
    return false;
}

bool RFM22B::shutdown()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    if (!m_spi)
    {
        return false;
    }
#if defined RASPBERRY_PI
    int res = gpioWrite(m_sdn_gpio, 1);
    if (res != 0)
    {
        QLOGE("Failed to shutdown");
        return false;
    }
#endif
    return true;
}

bool RFM22B::powerup()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    if (!m_spi)
    {
        return false;
    }

#if defined RASPBERRY_PI
    int res = gpioWrite(m_sdn_gpio, 0);
    if (res != 0)
    {
        QLOGE("Failed to powerup");
        return false;
    }
#endif
    return true;
}

void RFM22B::reset()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    set_operating_mode((uint16_t)Operating_Mode::READY_MODE | (uint16_t)Operating_Mode::RESET);
}


void RFM22B::set_modem_configuration(uint8_t data[42])
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    uint8_t const* ptr = data;
    set_register(Register::IF_FILTER_BANDWIDTH_1C, *ptr++);
    set_register(Register::AFC_LOOP_GEARSHIFT_OVERRIDE_1D, *ptr++);

    set_register(Register::CLOCK_RECOVERY_OVERSAMPLING_RATIO_20, *ptr++);
    set_register(Register::CLOCK_RECOVERY_OFFSET_2_21, *ptr++);
    set_register(Register::CLOCK_RECOVERY_OFFSET_1_22, *ptr++);
    set_register(Register::CLOCK_RECOVERY_OFFSET_0_23, *ptr++);

    set_register(Register::CLOCK_RECOVERT_TIMING_LOOP_GAIN_1_24, *ptr++);
    set_register(Register::CLOCK_RECOVERT_TIMING_LOOP_GAIN_0_25, *ptr++);

    set_register(Register::AFC_LIMITER_2A, *ptr++);
    set_register(Register::OOK_COUNTER_VALUE_1_2C, *ptr++);
    set_register(Register::OOK_COUNTER_VALUE_2_2D, *ptr++);
    set_register(Register::SLICER_PEAK_HOLD_2E, *ptr++);

    set_register(Register::DATA_ACCESS_CONTROL_30, *ptr++);
    set_register(Register::HEADER_CONTROL_1_32, *ptr++);
    set_register(Register::HEADER_CONTROL_2_33, *ptr++);
    set_register(Register::PREAMBLE_LENGTH_34, *ptr++);
    set_register(Register::PREAMBLE_DETECTION_CONTROL_35, *ptr++);
    set_register(Register::SYNC_WORD_3_36, *ptr++);
    set_register(Register::SYNC_WORD_2_37, *ptr++);
    set_register(Register::SYNC_WORD_1_38, *ptr++);
    set_register(Register::SYNC_WORD_0_39, *ptr++);

    set_register(Register::TRANSMIT_HEADER_3_3A, *ptr++);
    set_register(Register::TRANSMIT_HEADER_2_3B, *ptr++);
    set_register(Register::TRANSMIT_HEADER_1_3C, *ptr++);
    set_register(Register::TRANSMIT_HEADER_0_3D, *ptr++);
    set_register(Register::TRANSMIT_PACKET_LENGTH_3E, *ptr++);
    set_register(Register::CHECK_HEADER_3_3F, *ptr++);
    set_register(Register::CHECK_HEADER_2_40, *ptr++);
    set_register(Register::CHECK_HEADER_1_41, *ptr++);
    set_register(Register::CHECK_HEADER_0_42, *ptr++);
    set_register(Register::HEADER_ENABLE_3_43, *ptr++);
    set_register(Register::HEADER_ENABLE_2_44, *ptr++);
    set_register(Register::HEADER_ENABLE_1_45, *ptr++);
    set_register(Register::HEADER_ENABLE_0_46, *ptr++);

    set_register(Register::TX_DATA_RATE_1_6E, *ptr++);
    set_register(Register::TX_DATA_RATE_0_6F, *ptr++);

    set_register(Register::MODULATION_MODE_CONTROL_1_70, *ptr++);
    set_register(Register::MODULATION_MODE_CONTROL_2_71, *ptr++);
    set_register(Register::FREQUENCY_DEVIATION_72, *ptr++);

    set_register(Register::FREQUENCY_BAND_SELECT_75, *ptr++);
    set_register(Register::NOMINAL_CARRIER_FREQUENCY_1_76, *ptr++);
    set_register(Register::NOMINAL_CARRIER_FREQUENCY_0_77, *ptr++);
}

void RFM22B::idle_mode()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    set_operating_mode((uint16_t)Operating_Mode::READY_MODE);
}

void RFM22B::sleep_mode()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    set_operating_mode((uint16_t)Operating_Mode::ENABLE_WAKE_UP_TIMER);
}

void RFM22B::stand_by_mode()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    get_register(Register::INTERRUPT_STATUS_1_03);
    get_register(Register::INTERRUPT_STATUS_2_04);
    set_operating_mode(0);
}

// Set the operating mode
//	This function does not toggle individual pins as with other functions
//	It expects a bitwise-ORed combination of the modes you want set
void RFM22B::set_operating_mode(uint16_t mode)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    set_register16(Register::OPERATING_MODE_AND_FUNCTION_CONTROL_1_07, mode);
}

// Get operating mode (bitwise-ORed)
uint16_t RFM22B::get_operating_mode() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    return get_register16(Register::OPERATING_MODE_AND_FUNCTION_CONTROL_1_07);
}

// Manuall enter RX or TX mode
void RFM22B::rx_mode()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    set_operating_mode((uint16_t)Operating_Mode::READY_MODE | (uint16_t)Operating_Mode::RX_MODE);
}
void RFM22B::tx_mode()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    set_operating_mode((uint16_t)Operating_Mode::READY_MODE | (uint16_t)Operating_Mode::TX_MODE);
}

// Set or get the GPIO configuration
void RFM22B::set_gpio_function(GPIO gpio, GPIO_Function func)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    // Get the GPIO register
    uint8_t gpioX = get_register((Register)gpio);

    // Clear gpioX bits
    gpioX &= ~((1<<5)-1);

    // Set the gpioX bits
    gpioX |= (uint8_t)func;

    // Set the register
    set_register((Register)gpio, gpioX);
}

// Set or get the transmission power
void RFM22B::set_tx_power_dBm(uint8_t power)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    // Saturate to maximum power
    if (power > 20)
    {
        power = 20;
    }

    // Get the TX power register
    uint8_t txp = get_register(Register::TX_POWER_6D);

    // Clear txpow bits
    txp &= ~(0x07);

    // Calculate txpow bits (See Section 5.7.1 of datasheet)
    uint8_t txpow = (power + 1) / 3;

    // Set txpow bits
    txp |= txpow;

    // Set the register
    set_register(Register::TX_POWER_6D, txp);
}
uint8_t RFM22B::get_tx_power_dBm()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    // Get the TX power register
    uint8_t txp = get_register(Register::TX_POWER_6D);

    // Get the txpow bits
    uint8_t txpow = txp & 0x07;

    // Calculate power (see Section 5.7.1 of datasheet)
    if (txpow == 0)
    {
        return 1;
    }
    else
    {
        return txpow * 3 - 1;
    }
}

uint8_t RFM22B::get_channel() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    return get_register(Register::FREQUENCY_HOPPING_CHANNEL_SELECT_79);
}

bool RFM22B::set_channel(uint8_t channel)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    set_register(Register::FREQUENCY_HOPPING_CHANNEL_SELECT_79, channel);
    return true;
}

void RFM22B::set_xtal_adjustment(float adjustment)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    uint8_t adj = static_cast<uint8_t>(math::clamp(128.f + (adjustment * 128.f), 0.f, 255.f));

    //uint8_t reg = get_register(Register::CRYSTAL_OSCILLATOR_LOAD_CAPACITANCE_09);
    //reg &= ~(0x7F);
    //reg |= adjustment & 0x7F;
    set_register(Register::CRYSTAL_OSCILLATOR_LOAD_CAPACITANCE_09, adj);
}


// Get input power (in dBm)
//	Coefficients approximated from the graph in Section 8.10 of the datasheet
int8_t RFM22B::get_input_dBm() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    return 0.56*get_register(Register::RECEIVED_SIGNAL_STRENGTH_INDICATOR_26)-128.8;
}

uint8_t* RFM22B::get_tx_fifo_payload_ptr(size_t fifo_size)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    if (fifo_size > 64)
    {
        return nullptr;
    }

    m_tx_fifo.resize(fifo_size);
    return m_tx_fifo.data();
}

size_t RFM22B::get_rx_fifo_payload_size() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    return m_rx_fifo.size();
}

uint8_t* RFM22B::get_rx_fifo_payload_ptr()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    return m_rx_fifo.data();
}

bool RFM22B::write_tx_fifo(void const* data, size_t size)
{
    if (!m_spi)
    {
        return false;
    }

    //Toggle ffclrtx bit high and low to clear TX FIFO
    set_register(Register::OPERATING_MODE_AND_FUNCTION_CONTROL_2_08, 1);
    set_register(Register::OPERATING_MODE_AND_FUNCTION_CONTROL_2_08, 0);

    if (data == nullptr || size == 0 || size > MAX_PACHET_SIZE)
    {
        QASSERT(false);
        return false;
    }

    uint8_t buffer[MAX_PACHET_SIZE + 1];
    buffer[0] = (uint8_t)Register::FIFO_ACCESS_7F | (1<<7);

    memcpy(buffer + 1, data, size);

    return m_spi->transfer(buffer, nullptr, size + 1);
}

bool RFM22B::read_rx_fifo(void* data, size_t size)
{
    if (!m_spi)
    {
        return false;
    }

    if (data == nullptr || size == 0 || size > 64)
    {
        QASSERT(false);
        return false;
    }

    uint8_t buffer[MAX_PACHET_SIZE + 1] = { 0xFF };
    buffer[0] = (uint8_t)Register::FIFO_ACCESS_7F;

    if (!m_spi->transfer(buffer, buffer, size + 1))
    {
        return false;
    }

    memcpy(data, buffer + 1, size);

    return true;
}

// Helper function to read a single byte from the device
uint8_t RFM22B::get_register(Register reg) const
{
    // rx and tx arrays must be the same length
    // Must be 2 elements as the device only responds whilst it is being sent
    // data. tx[0] should be set to the requested register value and tx[1] left
    // clear. Once complete, rx[0] will be left clear (no data was returned whilst
    // the requested register was being sent), and rx[1] will contain the value
    uint8_t tx_data[2] = {uint8_t(reg), 0x00};
    uint8_t rx_data[2] = { 0 };
    if (!m_spi->transfer(tx_data, rx_data, 2))
    {
        QLOGE("Failed to transfer SPI data");
        return 0;
    }
    return rx_data[1];
}

// Similar to function above, but for readying 2 consequtive registers as one
uint16_t RFM22B::get_register16(Register reg) const
{
    uint8_t tx_data[3] = {uint8_t(reg), 0x00, 0x00};
    uint8_t rx_data[3] = { 0 };
    if (!m_spi->transfer(tx_data, rx_data, 3))
    {
        QLOGE("Failed to transfer SPI data");
        return 0;
    }
    return (rx_data[1] << 8) | rx_data[2];
}

// Helper function to write a single byte to a register
void RFM22B::set_register(Register reg, uint8_t value)
{
    uint8_t buffer[] = {uint8_t(uint8_t(reg) | (1 << 7)), value};
    if (!m_spi->transfer(buffer, nullptr, 2))
    {
        QLOGE("Failed to transfer SPI data");
    }
}

// As above, but for 2 consequitive registers
void RFM22B::set_register16(Register reg, uint16_t value)
{
    uint8_t buffer[] = {uint8_t(uint8_t(reg) | (1 << 7)), uint8_t(value >> 8), uint8_t(value & 0xFF)};
    if (!m_spi->transfer(buffer, nullptr, 3))
    {
        QLOGE("Failed to transfer SPI data");
    }
}

bool RFM22B::tx(Clock::duration timeout)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    size_t size = m_tx_fifo.size();
    if (size > 0)
    {
        if (!write_tx_fifo(m_tx_fifo.data(), size))
        {
            return false;
        }
    }
    set_register(Register::TRANSMIT_PACKET_LENGTH_3E, size);

    tx_mode();

    // Timing for the interrupt loop timeout
    auto start = Clock::now();
    auto elapsed = Clock::now() - start;

    uint32_t count = 0;

    // Loop until packet has been sent (device has left TX mode)
    while (((get_register(Register::OPERATING_MODE_AND_FUNCTION_CONTROL_1_07)>>3) & 1) && elapsed < timeout)
    {
        count++;
        elapsed = Clock::now() - start;
    }

    // If timeout occured, return -1
    if (elapsed >= timeout)
    {
        QLOGW("Timeout while sending: {}", count);
        //reset();
        idle_mode();
        return false;
    }

    return true;
}
RFM22B::RX_Result RFM22B::rx(size_t max_expected_size, Clock::duration packet_timeout, Clock::duration payload_timeout)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    //Toggle ffclrrx bit high and low to clear RX FIFO
    set_register(Register::OPERATING_MODE_AND_FUNCTION_CONTROL_2_08, 2);
    set_register(Register::OPERATING_MODE_AND_FUNCTION_CONTROL_2_08, 0);

    rx_mode();

    auto start = Clock::now();
    auto elapsed = Clock::now() - start;

    uint32_t count = 0;

    // Loop endlessly on interrupt or timeout
    //	Don't use interrupt registers here as these don't seem to behave consistently
    //	Watch the operating mode register for the device leaving RX mode. This is indicitive
    //	of a valid packet being received
    while (((get_register(Register::OPERATING_MODE_AND_FUNCTION_CONTROL_1_07)>>2) & 1) && elapsed < (packet_timeout + payload_timeout))
    {
        count++;
        elapsed = Clock::now() - start;
    }

    // If timeout occured, return -1
    if (elapsed >= (packet_timeout + payload_timeout))
    {
        return RX_Result::TIMEOUT;
    }

    uint8_t is = get_register(Register::INTERRUPT_STATUS_1_03);
    if ((is & 0x2) == 0)
    {
        return RX_Result::TIMEOUT;
    }

    size_t size = get_register(Register::RECEIVED_PACKET_LENGTH_4B);
    if (size > 64)
    {
        return RX_Result::FIFO_FAILED;
    }

    m_rx_fifo.resize(size);
    if (size > 0)
    {
        if (!read_rx_fifo(m_rx_fifo.data(), size))
        {
            return RX_Result::FIFO_FAILED;
        }
    }

    return RX_Result::OK;
}


}
}
