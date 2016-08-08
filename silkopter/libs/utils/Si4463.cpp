#include "Si4463.h"
#include "SPI_Dev.h"

#include <thread>

#ifdef RASPBERRY_PI
extern "C"
{
    #include "hw/pigpio.h"
}
extern std::chrono::microseconds PIGPIO_PERIOD;

#endif

namespace util
{

/////////////////////////////////////////////////////////////////

Si4463::Si4463()
{

}

Si4463::~Si4463()
{

}

bool Si4463::init(std::string const& device, uint32_t speed, uint8_t sdn_gpio, uint8_t nirq_gpio)
{
    if (m_is_initialized)
    {
        return true;
    }

    auto result = m_spi_dev.init(device, speed);
    if (result != ts::success)
    {
        QLOGE("SPI initialization failed: {}", result.error().what());
        return false;
    }

    m_sdn_gpio = sdn_gpio;
    m_nirq_gpio = nirq_gpio;

    //configure the GPIOs

    int sdn_gpio_mode = gpioGetMode(m_sdn_gpio);
    if (sdn_gpio_mode == PI_BAD_GPIO)
    {
        QLOGE("The SDN GPIO {} is bad", m_sdn_gpio);
        return false;
    }
    int nirq_gpio_mode = gpioGetMode(m_nirq_gpio);
    if (nirq_gpio_mode == PI_BAD_GPIO)
    {
        QLOGE("The nIRQ GPIO {} is bad", m_nirq_gpio);
        return false;
    }

    int res = gpioSetMode(m_sdn_gpio, PI_OUTPUT);
    if (res != 0)
    {
        QLOGE("The SDN GPIO {} is bad", m_sdn_gpio);
        goto error;
    }
    res = gpioSetMode(m_nirq_gpio, PI_INPUT);
    if (res != 0)
    {
        QLOGE("The nIRQ GPIO {} is bad", m_nirq_gpio);
        goto error;
    }

    //power on procedure

    res = gpioWrite(m_sdn_gpio, 1);
    if (res != 0)
    {
        QLOGE("Failed to set SDN");
        goto error;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    res = gpioWrite(m_sdn_gpio, 0);
    if (res != 0)
    {
        QLOGE("Failed to set SDN");
        goto error;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    m_is_initialized = true; //set to true so we can call call_api

    {
        uint8_t data[6] = { 0x01, 0x00, 0x01, 0xC9, 0xC3, 0x80 }; //bot up to main application, use xtal, 30MHz
        if (!call_api(Command::POWER_UP, data, 6, nullptr, 0))
        {
            QLOGE("The chip is not powering up");
            goto error;
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(20));

    {
        uint8_t data[8];
        if (!call_api(Command::PART_INFO, nullptr, 0, data, 8))
        {
            QLOGE("The chip is not responding to api calls");
            goto error;
        }

        uint16_t part = (data[1] << 8) | data[2];
        if (part != 17507) //0x4463
        {
            QLOGE("The chip is not a Si4463. Part = {}", part);
            goto error;
        }
    }

    m_is_initialized = true;

    return true;

error:
    shutdown();

    m_is_initialized = false;

    gpioSetMode(m_sdn_gpio, sdn_gpio_mode);
    gpioSetMode(m_nirq_gpio, nirq_gpio_mode);
    return false;
}

bool Si4463::shutdown()
{
    if (!m_is_initialized)
    {
        return false;
    }
    int res = gpioWrite(m_sdn_gpio, 1);
    if (res != 0)
    {
        QLOGE("Failed to shutdown");
        return false;
    }
    return true;
}

bool Si4463::wait_for_cts()
{
    if (!m_is_initialized)
    {
        return false;
    }

    auto start = std::chrono::high_resolution_clock::now();

    uint8_t tx_data[] = { (uint8_t)Command::READ_CMD_BUFF, 0 };
    uint8_t rx_data[] = { 0, 0 };
    while (1)
    {
        if (!m_spi_dev.transfer(tx_data, rx_data, 2))
        {
            return false;
        }
        if (rx_data[1] == 0xFF)
        {
            return true;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(10));

        if (std::chrono::high_resolution_clock::now() - start > std::chrono::milliseconds(1000))
        {
            QLOGE("Timeout");
            return false;
        }
    }
}

bool Si4463::call_api(Command cmd, void const* tx_data, size_t tx_size, void* rx_data, size_t rx_size)
{
    return _call_api(true, cmd, tx_data, tx_size, rx_data, rx_size);
}
bool Si4463::call_api_no_cts(Command cmd, void const* tx_data, size_t tx_size, void* rx_data, size_t rx_size)
{
    return _call_api(false, cmd, tx_data, tx_size, rx_data, rx_size);
}

bool Si4463::_call_api(bool cts, Command cmd, void const* tx_data, size_t tx_size, void* rx_data, size_t rx_size)
{
    if (!m_is_initialized)
    {
        return false;
    }

    //compose tx message
    m_tx_data.resize(1 + tx_size);
    m_tx_data[0] = static_cast<uint8_t>(cmd);
    if (tx_size > 0)
    {
        memcpy(m_tx_data.data() + 1, tx_data, tx_size);
    }

    return _call_api_raw(cts, m_tx_data.data(), m_tx_data.size(), rx_data, rx_size);
}

bool Si4463::call_api_raw(void const* tx_data, size_t tx_size, void* rx_data, size_t rx_size)
{
    return _call_api_raw(true, tx_data, tx_size, rx_data, rx_size);
}

bool Si4463::call_api_raw_no_cts(void const* tx_data, size_t tx_size, void* rx_data, size_t rx_size)
{
    return _call_api_raw(false, tx_data, tx_size, rx_data, rx_size);
}

bool Si4463::_call_api_raw(bool cts, void const* tx_data, size_t tx_size, void* rx_data, size_t rx_size)
{
    if (!m_is_initialized)
    {
        return false;
    }

    //wait forthe chip to be ready
    if (cts && !wait_for_cts())
    {
        return false;
    }

    //send command
    if (!m_spi_dev.transfer(tx_data, nullptr, tx_size))
    {
        return false;
    }

    if (rx_size > 0)
    {
        //compose CTS request
        m_tx_data.clear();
        m_tx_data.resize(2 + rx_size); //this will zero out all the data
        m_rx_data.resize(m_tx_data.size());

        m_tx_data[0] = static_cast<uint8_t>(Command::READ_CMD_BUFF);

        auto start = std::chrono::high_resolution_clock::now();

        //wait for CTS and then read the message
        //This is needed because after getting the CTS the NSEL line has to be kept active.
        while (1)
        {
            if (!m_spi_dev.transfer(m_tx_data.data(), m_rx_data.data(), m_tx_data.size()))
            {
                return false;
            }
            if (m_rx_data[1] == 0xFF) //CTS clear
            {
                memcpy(rx_data, m_rx_data.data() + 2, rx_size);
                return true;
            }

            if (std::chrono::high_resolution_clock::now() - start > std::chrono::milliseconds(1000))
            {
                QLOGE("Timeout");
                return false;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }

    return true;
}

bool Si4463::set_property(Property prop, void const* tx_data, size_t tx_size)
{
    return set_properties(prop, 1, tx_data, tx_size);
}

bool Si4463::set_properties(Property start_prop, size_t prop_count, void const* tx_data, size_t tx_size)
{
    if (!m_is_initialized)
    {
        return false;
    }
    auto start = std::chrono::high_resolution_clock::now();

    //compose tx message
    m_tx_data.resize(4 + tx_size);
    m_tx_data[0] = static_cast<uint8_t>(Command::SET_PROPERTY);
    m_tx_data[1] = static_cast<uint8_t>((static_cast<uint16_t>(start_prop) >> 8)); //group
    m_tx_data[2] = prop_count; //count
    m_tx_data[3] = static_cast<uint8_t>((static_cast<uint16_t>(start_prop) & 0xFF)); //prop
    if (tx_size > 0)
    {
        memcpy(m_tx_data.data() + 4, tx_data, tx_size);
    }

    //wait forthe chip to be ready
    if (!wait_for_cts())
    {
        return false;
    }

    //send command
    if (!m_spi_dev.transfer(m_tx_data.data(), nullptr, m_tx_data.size()))
    {
        return false;
    }

    //wait forthe chip to be ready
    if (!wait_for_cts())
    {
        return false;
    }

    auto now = std::chrono::high_resolution_clock::now();
    QLOGI("DURATIONNNNNN: {}", std::chrono::duration_cast<std::chrono::microseconds>(now - start).count());

    return true;
}

bool Si4463::write_tx_fifo(void const* data, size_t size)
{
    if (!m_is_initialized)
    {
        return false;
    }

    if (data == nullptr || size == 0 || size > 64)
    {
        QASSERT(false);
        return false;
    }

    //wait forthe chip to be ready
    if (!wait_for_cts())
    {
        return false;
    }

    m_tx_data.resize(1 + size);
    memcpy(m_tx_data.data() + 1, data, size);

    m_tx_data[0] = static_cast<uint8_t>(Command::WRITE_TX_FIFO);

    return m_spi_dev.transfer(m_tx_data.data(), nullptr, m_tx_data.size());
}

bool Si4463::read_rx_fifo(void* data, size_t size)
{
    if (!m_is_initialized)
    {
        return false;
    }

    if (data == nullptr || size == 0 || size > 64)
    {
        QASSERT(false);
        return false;
    }

    //wait forthe chip to be ready
    if (!wait_for_cts())
    {
        return false;
    }

    m_tx_data.clear();
    m_tx_data.resize(1 + size, 0xFF);
    m_rx_data.resize(m_tx_data.size());

    m_tx_data[0] = static_cast<uint8_t>(Command::READ_RX_FIFO);

    if (!m_spi_dev.transfer(m_tx_data.data(), m_rx_data.data(), m_tx_data.size()))
    {
        return false;
    }

    memcpy(data, m_rx_data.data() + 1, size);
    return true;
}

}
