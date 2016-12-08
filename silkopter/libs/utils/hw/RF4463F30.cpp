#include "RF4463F30.h"

#include "RF4463F30_Config.h"

namespace util
{
namespace hw
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

#if 1
    uint8_t config[] = RADIO_CONFIGURATION_DATA_ARRAY;

    if (!m_chip.upload_config(config))
    {
        QLOGE("Cannot upload chip configuration");
        goto error;
    }
#else
    uint8_t patch_data[] =
    {
        0x08,0x04,0x21,0x71,0x4B,0x00,0x00,0xBA,0x9E,
        0x08,0x05,0x48,0x23,0x2E,0x2B,0x90,0xB1,0x4E,
        0x08,0xEA,0x3F,0xB9,0xE8,0x8B,0xA9,0xCA,0xD6,
        0x08,0x05,0xD2,0xE5,0xBE,0xD1,0x27,0x55,0x82,
        0x08,0xE5,0x56,0x2A,0x3B,0x76,0x76,0x96,0x48,
        0x08,0x05,0x8E,0x26,0xD8,0x5D,0x01,0xA7,0x88,
        0x08,0xE2,0x89,0xCC,0x63,0x79,0x95,0x00,0x4B,
        0x08,0x05,0xE0,0x75,0xCD,0xA4,0xB9,0x46,0xBC,
        0x08,0xEA,0xD3,0x37,0xD2,0x9A,0x89,0x82,0xEA,
        0x08,0x05,0x0C,0xAE,0x4C,0xF5,0xF6,0x3C,0xB3,
        0x08,0xE9,0xA7,0x70,0xDF,0xF1,0x14,0x4F,0x04,
        0x08,0x05,0xFE,0x5B,0xDF,0x47,0x0A,0x7C,0x5B,
        0x08,0xE2,0xFB,0x3E,0x21,0xA2,0x1B,0xAA,0x93,
        0x08,0x05,0xBF,0xFD,0xAB,0x69,0x6C,0xA8,0x5A,
        0x08,0xE2,0x66,0xB7,0x2E,0x2C,0x45,0x2D,0xFB,
        0x08,0x05,0x0D,0x55,0xBD,0xC2,0x37,0x00,0x72,
        0x08,0xE2,0xFF,0x57,0x4D,0x7C,0x6C,0x00,0x2C,
        0x08,0x05,0x9E,0xF2,0x46,0xFD,0xD3,0x16,0x1B,
        0x08,0xEA,0x16,0x7F,0x67,0x4D,0xE5,0xE2,0xC8,
        0x08,0x05,0x37,0x33,0x1C,0xFA,0xBB,0xEE,0xEF,
        0x08,0xEA,0x00,0x5F,0xBE,0xA4,0xFC,0xBF,0xC1,
        0x08,0x05,0x95,0x12,0x2F,0x0A,0xCF,0x55,0x8C,
        0x08,0xE7,0x70,0xC2,0xD4,0xF0,0x81,0x95,0xC8,
        0x08,0xE7,0x72,0x00,0xF9,0x8D,0x15,0x01,0xA3,
        0x08,0xE7,0x18,0xE5,0x6C,0x51,0x1F,0x86,0x9F,
        0x08,0xE7,0xDD,0x37,0x59,0x4B,0xAD,0xB0,0x9C,
        0x08,0xE7,0xC8,0xE8,0x84,0xCD,0x55,0x41,0x83,
        0x08,0xEF,0x4F,0x8E,0x38,0xCB,0x37,0x02,0x87,
        0x08,0xE7,0xF5,0x00,0x88,0x4C,0x09,0x65,0xCE,
        0x08,0xEF,0xDD,0xBC,0x65,0x62,0xAC,0x75,0x62,
        0x08,0xE7,0xC0,0xF1,0x5D,0x98,0xB0,0xDD,0x43,
        0x08,0xE7,0x19,0xB4,0xF8,0x9F,0x6D,0x8C,0xCB,
        0x08,0xE1,0xDE,0x63,0xC2,0x32,0xC6,0xE4,0x2F,
        0x08,0x05,0xF4,0x33,0xB7,0x2E,0x72,0x9A,0xF9,
        0x08,0xE7,0x65,0xD9,0x38,0xB8,0xFE,0x31,0x16,
        0x08,0xE7,0xF3,0x06,0x2D,0xF5,0xFE,0x0C,0x38,
        0x08,0xE7,0x70,0x4F,0xE7,0x49,0xB4,0x58,0x39,
        0x08,0xEF,0xF1,0x46,0xA9,0x23,0x38,0x64,0xC0,
        0x08,0xE7,0x09,0x4E,0x04,0xD3,0x46,0x15,0x02,
        0x08,0xEF,0x8D,0xC7,0x20,0xC3,0x90,0x87,0x4D,
        0x08,0xEF,0x00,0xAB,0x7F,0x27,0x02,0xC6,0xA0,
        0x08,0xE7,0x23,0xA6,0xA6,0xA4,0x27,0x11,0x7D,
        0x08,0xEF,0xB3,0xF1,0x9E,0x6A,0xB3,0x19,0xAF,
        0x08,0xE7,0xAB,0xF5,0x15,0x78,0x5E,0x48,0xF8,
        0x08,0xEF,0x5B,0xB1,0x2E,0xAF,0x2A,0xFF,0x16,
        0x08,0xE7,0x30,0x62,0x5C,0x82,0x7A,0x3F,0x83,
        0x08,0xEF,0x91,0xA7,0xD3,0x1B,0x64,0x85,0xBE,
        0x08,0xE7,0x4D,0x81,0x94,0xE4,0xAA,0xE8,0xDB,
        0x08,0xEF,0xA0,0xCC,0x4A,0x23,0xA5,0x7E,0x36,
        0x08,0xEF,0x0C,0x72,0x4C,0xFB,0x26,0x5A,0xEC,
        0x08,0xEF,0x0E,0x42,0xFA,0xAF,0x49,0xA0,0xA8,
        0x08,0xE7,0x6D,0x12,0xDF,0x2B,0x0C,0x61,0x58,
        0x08,0xEA,0xB6,0x9B,0xDE,0x81,0xB9,0xFF,0xFF,
        0x08,0x05,0x04,0xEB,0xD8,0x12,0xD6,0x8D,0xE0,
        0x08,0xEC,0x29,0x66,0x4B,0xDE,0xB7,0xDE,0x36,
        0x08,0x05,0x0D,0x28,0xB9,0x0A,0x89,0x31,0x1A,
        0x00
    };

    if (!m_chip.upload_patch(patch_data))
    {
        return false;
    }

    if (!m_chip.powerup())
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

//    //configure fast registers
//    {
//        uint8_t tx_data[] =
//        {
//            0x00, //disabled
//            0x00, //disabled
//            0x00, //disabled
//            0x00, //disabled
//        };

//        if (!m_chip.set_properties(Si4463::Property::FRR_CTL_A_MODE, 4, tx_data, sizeof(tx_data)))
//        {
//            QLOGI("Failed to set properties");
//            goto error;
//        }
//    }

    //PA - important!!!
    {
        uint8_t tx_data[] =
        {
            0x08,
            0x3F, //max power
            0x00,
            0x3d,
        };

        if (!m_chip.set_properties(Si4463::Property::PA_MODE, 4, tx_data, sizeof(tx_data)))
        {
            QLOGI("Failed to set properties");
            goto error;
        }
    }

    if (!m_chip.call_api_raw({RF_INT_CTL_ENABLE_2}) ||
        !m_chip.call_api_raw({RF_FRR_CTL_A_MODE_4}) ||
        !m_chip.call_api_raw({RF_PREAMBLE_TX_LENGTH_9}) ||
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
        !m_chip.call_api_raw({RF_MODEM_RAW_CONTROL_10}) ||
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
#endif

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

    //reset TX fifo
    if (!m_chip.call_api_raw({ static_cast<uint8_t>(Si4463::Command::FIFO_INFO), 0x01 }))
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
    uint8_t args[2] = { (uint8_t)(size >> 8), (uint8_t)(size & 0xFF) };
    if (!m_chip.set_properties(Si4463::Property::PKT_FIELD_2_LENGTH_12_8, 2, args, sizeof(args)))
    {
        return false;
    }

    //enter tx state
    uint8_t condition =
            8 << 4 | //rx state
            0 << 0; //start immediately

    if (!m_chip.call_api_raw(
            {
                (uint8_t)Si4463::Command::START_TX,
                channel,
                condition,
                0, 0 //(uint8_t)(size >> 8), (uint8_t)(size & 0xFF)//0x00, 0x00 //use the field1 length
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

    m_tx_started = false;

    auto start = std::chrono::high_resolution_clock::now();

    do
    {
        bool got_it = false;
        uint8_t status = 0;
        if (!m_chip.wait_for_ph_interrupt(got_it, status, std::chrono::milliseconds(1000)))
        {
            return false;
        }
        if (got_it && (status & (1 << 5)) != 0)
        {
            break;
        }

        if (std::chrono::high_resolution_clock::now() - start > std::chrono::milliseconds(1000))
        {
            QLOGW("Timeout");
            return false;
        }

        //std::this_thread::sleep_for(std::chrono::microseconds(10));
    } while (true);

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

    uint8_t frr_d = 0;
    if (!m_chip.read_frr_d(frr_d))
    {
        return false;
    }

    if ((frr_d & 0xF) == 8)
    {
        return true;
    }

    //reset RX fifo
    if (!m_chip.call_api_raw({ static_cast<uint8_t>(Si4463::Command::FIFO_INFO), 0x02 }))
    {
        return false;
    }

    //set the packet size
    //this needs to be maximum otherwise the rx fifo will not get data.
    //The actual data that is in the rx fifo will be the max(this value, actual data size received)
    uint8_t args[2] = { (uint8_t)(0), (uint8_t)(64) };
    if (!m_chip.set_properties(Si4463::Property::PKT_FIELD_2_LENGTH_12_8, 2, args, sizeof(args)))
    {
        return false;
    }

    return m_chip.call_api_raw(
    {
        (uint8_t)Si4463::Command::START_RX,
        channel,
        0x00, //start immediately
        0x00, 0x00, //size = 0
        0, //preamble timeout -> rx state
        3, //valid packet -> ready state
        0  //invalid packet -> rx state
    });
}
bool RF4463F30::end_rx(size_t& size, std::chrono::high_resolution_clock::duration timeout)
{
    if (!m_is_initialized)
    {
        return false;
    }

    size = 0;

    auto start = std::chrono::high_resolution_clock::now();

    bool received = false;
    do
    {
        bool got_it = false;
        uint8_t status = 0;
        if (!m_chip.wait_for_ph_interrupt(got_it, status, timeout))
        {
            return false;
        }
        if (got_it && (status & (1 << 4)) != 0) //got a packet
        {
            received = true;
            break;
        }
    }
    while (std::chrono::high_resolution_clock::now() - start < timeout);

    if (!received)
    {
        size = 0;
        return true; //no error, but no packet either
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

bool RF4463F30::rx(size_t& size, uint8_t channel, std::chrono::high_resolution_clock::duration timeout)
{
    if (!m_is_initialized)
    {
        return false;
    }

    if (!begin_rx(channel))
    {
        return false;
    }
    if (!end_rx(size, timeout))
    {
        return false;
    }

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

bool RF4463F30::get_dBm(int8_t& dBm)
{
    if (!m_is_initialized)
    {
        return false;
    }

    return m_chip.read_frr_a(reinterpret_cast<uint8_t&>(dBm));
}


}
}
