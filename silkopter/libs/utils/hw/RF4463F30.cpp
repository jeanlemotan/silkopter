#include "RF4463F30.h"

#include "RF4463F30_Config.h"

namespace util
{
namespace hw
{

volatile bool s_force_compiler_to_keep_loop = true;


RF4463F30::RF4463F30()
{
}

RF4463F30::~RF4463F30()
{
    shutdown();
}

bool RF4463F30::init(hw::ISPI& spi, uint8_t sdn_gpio, uint8_t nirq_gpio)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    if (m_is_initialized)
    {
        return true;
    }

    if (!m_chip.init(spi, sdn_gpio, nirq_gpio))
    {
        return false;
    }

    m_is_initialized = true;

    return init();
}

bool RF4463F30::init()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

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

    if (!m_chip.boot())
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

    if (!m_chip.check_part())
    {
        goto error;
    }


    set_fifo_mode(FIFO_Mode::HALF_DUPLEX);

    return true;

error:
    m_chip.shutdown();
    return false;
}

void RF4463F30::shutdown()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    if (m_is_initialized)
    {
        m_chip.shutdown();
    }
}

void RF4463F30::reset()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    if (m_is_initialized)
    {
        m_chip.reset();
    }
}

bool RF4463F30::set_fifo_mode(FIFO_Mode mode)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    uint8_t args[1] = { 0 };
    if (!m_chip.get_properties(Si4463::Property::GLOBAL_CONFIG, 1, args, sizeof(args)))
    {
        QLOGE("Failed to get global config, to set the fifo mode to {}", mode);
        return false;
    }

    if (mode == FIFO_Mode::SPLIT)
    {
        args[0] &= ~(1 << 4);
    }
    else
    {
        args[0] |= (1 << 4);
    }

    if (!m_chip.set_properties(Si4463::Property::GLOBAL_CONFIG, 1, args, sizeof(args)))
    {
        QLOGE("Failed to set global config, to set the fifo mode to {}", mode);
        return false;
    }

    //reset the FIFOs to make the change active
    if (!m_chip.call_api_raw({ static_cast<uint8_t>(Si4463::Command::FIFO_INFO), 0x03 }))
    {
        return false;
    }

    m_fifo_mode = mode;

    m_tx_fifo_threshold = 16;
    m_chip.set_properties(Si4463::Property::PKT_TX_THRESHOLD, 1, &m_tx_fifo_threshold, 1);
    m_chip.get_properties(Si4463::Property::PKT_TX_THRESHOLD, 1, &m_tx_fifo_threshold, 1);

    m_rx_fifo_threshold = 16;
    m_chip.set_properties(Si4463::Property::PKT_RX_THRESHOLD, 1, &m_rx_fifo_threshold, 1);
    m_chip.get_properties(Si4463::Property::PKT_RX_THRESHOLD, 1, &m_rx_fifo_threshold, 1);
    QLOGI("FIFO thresholds: rx {}, tx {}", m_rx_fifo_threshold, m_tx_fifo_threshold);

    return true;
}

uint8_t* RF4463F30::get_tx_fifo_payload_ptr(size_t fifo_size)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    if (fifo_size > 30000)
    {
        return nullptr;
    }

    m_tx_fifo.resize(fifo_size + 2);
    return m_tx_fifo.data() + 2;
}

size_t RF4463F30::get_rx_fifo_payload_size() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    return m_rx_fifo.size();
}

uint8_t* RF4463F30::get_rx_fifo_payload_ptr()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    return m_rx_fifo.data();
}

uint8_t RF4463F30::get_channel() const
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    return m_channel;
}

bool RF4463F30::set_channel(uint8_t channel)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    m_channel = channel;
    return true;
}

void RF4463F30::set_xtal_adjustment(float adjustment)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    if (!m_is_initialized)
    {
        return;
    }

    uint8_t adj = static_cast<uint8_t>(math::clamp(64.f + (adjustment * 64.f), 0.f, 127.f));
    uint8_t args[2] = { adj };
    if (!m_chip.set_properties(Si4463::Property::GLOBAL_XO_TUNE, 1, args, sizeof(args)))
    {
        QLOGE("Failed to set xtal adjustment of {}", adjustment);
        return;
    }
}

size_t RF4463F30::get_fifo_capacity() const
{
    if (m_fifo_mode == FIFO_Mode::SPLIT)
    {
        return 64;
    }
    return 129;
}

bool RF4463F30::tx()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    if (!m_is_initialized)
    {
        return false;
    }

    //TODO - set idle mode here, to avoid RX mode interfering with the fifo

    //reset TX & RX fifos
    if (!m_chip.call_api_raw({ static_cast<uint8_t>(Si4463::Command::FIFO_INFO), 0x03 }))
    {
        return false;
    }

    size_t payload_size = m_tx_fifo.size() - 2;
    m_tx_fifo[0] = (uint8_t)(payload_size >> 8);
    m_tx_fifo[1] = (uint8_t)(payload_size & 0xFF);

    size_t pending_to_upload = m_tx_fifo.size();
    uint8_t const* fifo_ptr = m_tx_fifo.data();

    constexpr size_t initial_upload = 32u;
    QASSERT(initial_upload <= get_fifo_capacity());

    //QLOGI("Start");

    //do the initial upload in the fifo.
    //I don't do it all here to be able to start TX faster
    {
        size_t written = std::min(pending_to_upload, initial_upload);
        if (!m_chip.write_tx_fifo(fifo_ptr, written))
        {
            QLOGW("Write FIFO failed 1.");
            return false;
        }
        pending_to_upload -= written;
        fifo_ptr += written;
    }

    //set the packet size
    {
        uint8_t args[2] = { (uint8_t)(payload_size >> 8), (uint8_t)(payload_size & 0xFF) };
        if (!m_chip.set_properties(Si4463::Property::PKT_FIELD_2_LENGTH_12_8, 2, args, sizeof(args)))
        {
            return false;
        }
    }

    //enter tx state
    {
        uint8_t args[] =
        {
            (uint8_t)Si4463::Command::START_TX,
            m_channel,

            3 << 4 | //ready
            0 << 0, //start immediately

            0, 0 //(uint8_t)(payload_size >> 8), (uint8_t)(payload_size & 0xFF)//0x00, 0x00 //use the field1 length
        };
        if (!m_chip.call_api_raw(args, sizeof(args), nullptr, 0))
        {
            return false;
        }
    }

    //do the rest of the upload up to fifo capacity
    //this cannot overflow, by definition
    if (pending_to_upload > 0)
    {
        size_t written = std::min(pending_to_upload, get_fifo_capacity() - initial_upload);
        if (!m_chip.write_tx_fifo(fifo_ptr, written))
        {
            QLOGW("Write FIFO failed 2.");
            return false;
        }
        pending_to_upload -= written;
        fifo_ptr += written;
    }

    //calculate the time it would take to transmit the bytes in the fifo
    float bitrate = 1000000.f / 8.f;
    Clock::duration duration_per_byte = std::chrono::duration_cast<Clock::duration>(std::chrono::duration<float>(1.0f / bitrate));
    Clock::time_point start_tp = Clock::now();
    Clock::duration max_duration = duration_per_byte * std::max(20u, m_tx_fifo.size()) * 2 + std::chrono::milliseconds(1);

    size_t spin = 0;
    size_t uploads = 0;
    do
    {
        spin++;

        {
            uint8_t response[8] = { 0 };
            uint8_t request[] = { static_cast<uint8_t>(Si4463::Command::GET_INT_STATUS) };
            m_chip.call_api_raw(request, sizeof(request), response, sizeof(response));
            {
                uint8_t ph_flags = response[3];
                uint8_t chip_flags = response[7];

                if (chip_flags & (1 << 5))
                {
                    if (!m_chip.call_api_raw({ static_cast<uint8_t>(Si4463::Command::FIFO_INFO), 0x03 }))
                    {
                        return false;
                    }
                    QLOGI("Overflow: spin {}, uploads {}, data left {}", spin, uploads, pending_to_upload);
                    return false;
                }

                if ((ph_flags & (1 << 5)) != 0)
                {
                    if (pending_to_upload > 0)
                    {
                        QLOGW("TX finished with {} bytes left in the fifo !!!", pending_to_upload);
                    }
                    break;
                }


                if (pending_to_upload > 0)
                {
                    uint8_t response[2] = { 0 };
                    uint8_t request[2] = { static_cast<uint8_t>(Si4463::Command::FIFO_INFO), 0x0 };
                    if (m_chip.call_api_raw(request, sizeof(request), response, sizeof(response)))
                    {
                        size_t available = response[1];
                        if (available > 0)
                        {
                            size_t written = std::min<size_t>(pending_to_upload, available);
                            if (!m_chip.write_tx_fifo(fifo_ptr, written))
                            {
                                QLOGW("Write FIFO failed 3.");
                                return false;
                            }
                            pending_to_upload -= written;
                            fifo_ptr += written;
                            uploads++;
                        }
                    }
                }

//                if (((ph_flags & 0x2) != 0) && pending_to_upload > 0)
//                {
//                    size_t written = std::min<size_t>(pending_to_upload, m_tx_fifo_threshold);

//                    if (!m_chip.write_tx_fifo(fifo_ptr, written))
//                    {
//                        QLOGW("Write FIFO failed 3.");
//                        return false;
//                    }
//                    pending_to_upload -= written;
//                    fifo_ptr += written;
//                    uploads++;
//                }
            }

            //std::this_thread::sleep_for(std::chrono::microseconds(100));
        }

        if (Clock::now() - start_tp > max_duration)
        {
            QLOGW("Timeout");
            break;
        }
    } while (true);

    return true;
}

RF4463F30::RX_Result RF4463F30::rx(size_t max_expected_size, Clock::duration packet_timeout, Clock::duration payload_timeout)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    if (!m_is_initialized)
    {
        return RX_Result::RX_FAILED;
    }

    m_rx_fifo.clear();


    //set the packet size
    //this needs to be maximum otherwise the rx fifo will not get data.
    //The actual data that is in the rx fifo will be the max(this value, actual data size received)
    uint8_t args[2] = { (uint8_t)(max_expected_size >> 8), (uint8_t)(max_expected_size & 0xFF) };
    if (!m_chip.set_properties(Si4463::Property::PKT_FIELD_2_LENGTH_12_8, 2, args, sizeof(args)))
    {
        return RX_Result::RX_FAILED;
    }

    if (!m_chip.call_api_raw({ static_cast<uint8_t>(Si4463::Command::FIFO_INFO), 0x03 }))
    {
        return RX_Result::RX_FAILED;
    }

    return resume_rx(packet_timeout, payload_timeout);
}

RF4463F30::RX_Result RF4463F30::resume_rx(Clock::duration packet_timeout, Clock::duration payload_timeout)
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    if (!m_is_initialized)
    {
        return RX_Result::RX_FAILED;
    }

    m_rx_fifo.clear();

    //reset RX fifo
//    if (!m_chip.call_api_raw({ static_cast<uint8_t>(Si4463::Command::FIFO_INFO), 0x03 }))
//    {
//        return RX_Result::RX_FAILED;
//    }

    Clock::time_point start = Clock::now();

    if (!m_chip.call_api_raw(
    {
        (uint8_t)Si4463::Command::START_RX,
        m_channel,
        0x00, //start immediately
        0x00, 0x00, //size = 0
        0, //preamble timeout -> no change
        3, //valid packet -> ready
        3  //invalid packet -> ready
    }))
    {
        return RX_Result::RX_FAILED;
    }

    //-------------------------------------------------

    size_t spin = 0;
    size_t downloads = 0;

    do
    {
        spin++;

        uint8_t modem_flags = 0;

        {
            uint8_t response[8] = { 0 };
            uint8_t request[] = { static_cast<uint8_t>(Si4463::Command::GET_INT_STATUS) };
            if (m_chip.call_api_raw(request, sizeof(request), response, sizeof(response)))
            {
                uint8_t ph_flags = response[3];
                modem_flags = response[5];
                uint8_t chip_flags = response[7];

                if (chip_flags & (1 << 5))
                {
                    QLOGI("Overflow: spin {}, downloads {}, after {}us", spin, downloads, std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - start).count());

                    if (!m_chip.call_api_raw({ static_cast<uint8_t>(Si4463::Command::FIFO_INFO), 0x03 }))
                    {
                        return RX_Result::RX_FAILED;
                    }
                    return RX_Result::FIFO_FAILED;
                }

                //crc error?
                if ((ph_flags & (1 << 2)) != 0 || (ph_flags & (1 << 3)) != 0)
                {
                    if (!m_chip.call_api_raw({ static_cast<uint8_t>(Si4463::Command::FIFO_INFO), 0x03 }))
                    {
                        return RX_Result::RX_FAILED;
                    }
                    return RX_Result::CRC_FAILED;
                }

                {
                    uint8_t response[1] = { 0 };
                    uint8_t request[2] = { static_cast<uint8_t>(Si4463::Command::FIFO_INFO), 0x0 };
                    if (m_chip.call_api_raw(request, sizeof(request), response, sizeof(response)))
                    {
                        size_t available = response[0];
                        if (available > 0)
                        {
                            size_t offset = m_rx_fifo.size();
                            m_rx_fifo.resize(offset + available);
                            if (!m_chip.read_rx_fifo(m_rx_fifo.data() + offset, available))
                            {
                                QLOGW("Read FIFO failed.");
                                return RX_Result::FIFO_FAILED;
                            }
                        }
                        downloads++;
                    }
                }

                //data in the fifo is ready?
//                if (ph_flags & 0x1)
//                {
//                    size_t available = m_rx_fifo_threshold;
//                    size_t offset = m_rx_fifo.size();
//                    m_rx_fifo.resize(offset + available);
//                    if (!m_chip.read_rx_fifo(m_rx_fifo.data() + offset, available))
//                    {
//                        QLOGW("Read FIFO failed.");
//                        return RX_Result::FIFO_FAILED;
//                    }
//                    downloads++;
//                }

                //are we done?
                if ((ph_flags & (1 << 4)) != 0)
                {
                    break;
                }
            }
        }

        if (modem_flags & (0x1) == 0)
        {
            if (Clock::now() - start >= payload_timeout)
            {
                return RX_Result::TIMEOUT;
            }
        }
        else
        {
            if (Clock::now() - start > packet_timeout)
            {
                return RX_Result::TIMEOUT;
            }
        }
    } while (true);

    //read the rest of the fifo
//    {
//        uint8_t response[2] = { 0 };
//        uint8_t request[] = { static_cast<uint8_t>(Si4463::Command::PACKET_INFO) };
//        if (m_chip.call_api_raw(request, sizeof(request), response, sizeof(response)))
//        {
//            size_t total_size = (response[0] << 8) | response[1];
//            if (total_size != 255)
//            {
//                QLOGI("total size {} - crt {} / downloads: {}", total_size, m_rx_fifo.size(), downloads);
//            }
//            QLOGI("Done: spin {}, downloads {}, reported {}, downloaded {}, after {}us", spin, downloads, total_size, m_rx_fifo.size(), std::chrono::duration_cast<std::chrono::microseconds>(Clock::now() - start).count());

//            if (total_size > m_rx_fifo.size())
//            {
//                size_t available = total_size - m_rx_fifo.size();
//                size_t offset = m_rx_fifo.size();
//                m_rx_fifo.resize(offset + available);
//                if (!m_chip.read_rx_fifo(m_rx_fifo.data() + offset, available))
//                {
//                    QLOGW("Read FIFO failed.");
//                    return RX_Result::FIFO_FAILED;
//                }
//            }
//        }
//    }

    return RX_Result::OK;
}

int8_t RF4463F30::get_input_dBm()
{
    std::lock_guard<std::recursive_mutex> lg(m_mutex);

    if (!m_is_initialized)
    {
        return false;
    }

    int8_t dBm = 0;
    if (!m_chip.read_frr(Si4463::FRR::A, reinterpret_cast<uint8_t*>(&dBm), 1))
    {
        QLOGW("FRR read failed");
        return 0;
    }
    return dBm;
}


}
}
