#include "RF4463F30.h"

#include "RF4463F30_Config.h"

namespace util
{




RF4463F30::RF4463F30()
{

}

RF4463F30::~RF4463F30()
{
    if (m_is_initialized)
    {
        m_chip.shutdown();
        m_is_initialized = false;
    }
}

bool RF4463F30::init(const std::string& device, uint32_t speed, uint8_t sdn_gpio, uint8_t nirq_gpio)
{
    if (m_is_initialized)
    {
        return true;
    }

    if (!m_chip.init(device, speed, sdn_gpio, nirq_gpio))
    {
        return false;
    }

    //GPIO_PIN_CFG
    {
        uint8_t tx_data[] =
        {
            0x14, 		// gpio 0 ,Rx data
            0x02, 		// gpio1, output 0
            0x21,  		// gpio2, hign while in receive mode
            0x20, 		// gpio3, hign while in transmit mode
            0x27, 		// nIRQ
            0x0b,  		// sdo
        };

        uint8_t rx_data[6];
        if (!m_chip.call_api(Si4463::Command::GPIO_PIN_CFG, tx_data, sizeof(tx_data), rx_data, sizeof(rx_data)))
        {
            QLOGI("Cannot configure GPIO pins");
            goto error;
        }
    }

    //tune frequency
    {
        uint8_t tx_data[] =
        {
            98,  			// freq  adjustment
        };

        if (!m_chip.set_property(Si4463::Property::GLOBAL_XO_TUNE, tx_data, sizeof(tx_data)))
        {
            QLOGI("Failed to tune XO");
            goto error;
        }
    }

    //configure clocks
    {
        uint8_t tx_data[] =
        {
            0x40,  		// tx = rx = 64 byte,PH,high performance mode
        };

        if (!m_chip.set_property(Si4463::Property::GLOBAL_CONFIG, tx_data, sizeof(tx_data)))
        {
            QLOGI("Failed to set global config");
            goto error;
        }
    }

    //configure fast registers
    {
        uint8_t tx_data[] =
        {
            0x00, //disabled
            0x00, //disabled
            0x00, //disabled
            0x00, //disabled
        };

        if (!m_chip.set_properties(Si4463::Property::FRR_CTL_A_MODE, 4, tx_data, sizeof(tx_data)))
        {
            QLOGI("Failed to set properties");
            goto error;
        }
    }

    //PA - important!!!
    {
        uint8_t tx_data[] =
        {
            0x08,
            0x7F, //max power
            0x00,
            0x3d,
        };

        if (!m_chip.set_properties(Si4463::Property::PA_MODE, 4, tx_data, sizeof(tx_data)))
        {
            QLOGI("Failed to set properties");
            goto error;
        }
    }

    if (!m_chip.call_api_raw({RF_PREAMBLE_TX_LENGTH_9}) ||
        !m_chip.call_api_raw({RF_SYNC_CONFIG_6}) ||
        !m_chip.call_api_raw({RF_PKT_CRC_CONFIG_7}) ||
        !m_chip.call_api_raw({RF_PKT_LEN_12}) ||
        !m_chip.call_api_raw({RF_PKT_FIELD_2_CRC_CONFIG_12}) ||
        !m_chip.call_api_raw({RF_PKT_FIELD_5_CRC_CONFIG_12}) ||
        !m_chip.call_api_raw({RF_PKT_RX_FIELD_3_CRC_CONFIG_9}) ||
        !m_chip.call_api_raw({RF_PKT_CRC_SEED_31_24_4}) ||
        !m_chip.call_api_raw({RF_MODEM_MOD_TYPE_12}) ||
        !m_chip.call_api_raw({RF_MODEM_FREQ_DEV_0_1}) ||
        !m_chip.call_api_raw({RF_MODEM_TX_RAMP_DELAY_12}) ||
        !m_chip.call_api_raw({RF_MODEM_BCR_NCO_OFFSET_2_12}) ||
        !m_chip.call_api_raw({RF_MODEM_AFC_LIMITER_1_3}) ||
        !m_chip.call_api_raw({RF_MODEM_AGC_CONTROL_1}) ||
        !m_chip.call_api_raw({RF_MODEM_AGC_WINDOW_SIZE_12}) ||
        !m_chip.call_api_raw({RF_MODEM_RAW_CONTROL_8}) ||
        !m_chip.call_api_raw({RF_MODEM_RSSI_CONTROL_3}) ||
        !m_chip.call_api_raw({RF_MODEM_RAW_SEARCH2_2}) ||
        !m_chip.call_api_raw({RF_MODEM_SPIKE_DET_2}) ||
        !m_chip.call_api_raw({RF_MODEM_RSSI_MUTE_1}) ||
        !m_chip.call_api_raw({RF_MODEM_DSA_CTRL1_5}) ||
        !m_chip.call_api_raw({RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12}) ||
        !m_chip.call_api_raw({RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12}) ||
        !m_chip.call_api_raw({RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12}) ||
        !m_chip.call_api_raw({RF_SYNTH_PFDCP_CPFF_7}) ||
        !m_chip.call_api_raw({RF_MATCH_VALUE_1_12}) ||
        !m_chip.call_api_raw({RF_FREQ_CONTROL_INTE_8}))
    {
        QLOGI("Failed to configure modem");
        goto error;
    }


    m_is_initialized = true;

    return true;

error:
    m_chip.shutdown();
    return false;
}


bool RF4463F30::write_tx_fifo(void const* data, uint8_t size)
{
    if (!m_is_initialized)
    {
        return false;
    }
    return m_chip.write_tx_fifo(data, size);
}
bool RF4463F30::begin_tx(size_t size, uint8_t channel)
{
    if (!m_is_initialized)
    {
        return false;
    }

    //set the packet size
//    uint8_t args[2] = { (uint8_t)(size >> 8), (uint8_t)(size & 0xFF) };
//    if (!m_chip.set_properties(Si4463::Property::PKT_FIELD_1_LENGTH_12_8, 2, args, sizeof(args)))
//    {
//        return false;
//    }

    //enter tx state
    uint8_t condition =
            3 << 4 | //ready state
            0 << 0; //start immediately

    if (!m_chip.call_api_raw(
            {
                (uint8_t)Si4463::Command::START_TX,
                channel,
                condition,
                (uint8_t)(size >> 8), (uint8_t)(size & 0xFF)//0x00, 0x00 //use the field1 length
            }))
    {
        return false;
    }

    m_tx_started = true;

    return true;
}

bool RF4463F30::end_tx()
{
    if (!m_is_initialized)
    {
        return false;
    }
    if (!m_tx_started)
    {
        QASSERT(false);
        return false;
    }

    auto start = std::chrono::high_resolution_clock::now();

    uint8_t response[2] = { 0 };

    do
    {
        if (!m_chip.call_api(Si4463::Command::GET_PH_STATUS, nullptr, 0, response, sizeof(response)))
        {
            return false;
        }
        if ((response[1] & (1 << 5)) != 0)
        {
            break;
        }

        if (std::chrono::high_resolution_clock::now() - start > std::chrono::milliseconds(1000))
        {
            QLOGE("Timeout");
            return false;
        }

        std::this_thread::sleep_for(std::chrono::microseconds(10));
    } while (true);

    m_tx_started = false;

    return true;
}

bool RF4463F30::tx(size_t size, uint8_t channel)
{
    if (!begin_tx(size, channel))
    {
        return false;
    }
    return end_tx();
}

bool RF4463F30::begin_rx(uint8_t channel)
{
    if (!m_is_initialized)
    {
        return false;
    }

    return m_chip.call_api_raw(
    {
        (uint8_t)Si4463::Command::START_RX,
        channel,
        0x00, //start immediately
        0x00, 0x00, //size = 0
    });
}
bool RF4463F30::end_rx(size_t& size)
{
    if (!m_is_initialized)
    {
        return false;
    }

    //first check if we got a packet
    {
        uint8_t response[2] = { 0 };
        if (!m_chip.call_api(Si4463::Command::GET_PH_STATUS, nullptr, 0, response, sizeof(response)))
        {
            return false;
        }
        if ((response[1] & (1 << 4)) == 0)
        {
            size = 0;
            return true; //no error but no packet either
        }
    }

    uint8_t response[2];
    if (!m_chip.call_api(Si4463::Command::PACKET_INFO, nullptr, 0, response, sizeof(response)))
    {
        return false;
    }

    size = ((uint16_t)response[0] << 8) & 0xFF00;
    size |= (uint16_t)response[1] & 0x00FF;

    return true;
}
bool RF4463F30::read_rx_fifo(void* data, size_t& size)
{
    if (!m_is_initialized)
    {
        return false;
    }

    return m_chip.read_rx_fifo(data, size);
}


}
