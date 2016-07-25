#include "RF4463F30.h"

namespace util
{

RF4463F30::RF4463F30()
{

}

RF4463F30::~RF4463F30()
{

}

bool RF4463F30::init(const std::string& device, uint32_t speed, uint8_t sdn_gpio, uint8_t nirq_gpio)
{
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
            m_chip.shutdown();
            return false;
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
            m_chip.shutdown();
            return false;
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
            m_chip.shutdown();
            return false;
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
            m_chip.shutdown();
            return false;
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
            m_chip.shutdown();
            return false;
        }
    }

    return true;
}


}
