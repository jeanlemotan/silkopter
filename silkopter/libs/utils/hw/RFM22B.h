#pragma once

#include <string>
#include "SPI_Dev.h"
#include "utils/Clock.h"

namespace util
{
namespace hw
{

class RFM22B
{
public:
    RFM22B();
    ~RFM22B();

    enum class Register : uint8_t
    {
        DEVICE_TYPE_00 								= 0x00,
        DEVICE_VERSION_01							= 0x01,
        DEVICE_STATUS_02							= 0x02,
        INTERRUPT_STATUS_1_03						= 0x03,
        INTERRUPT_STATUS_2_04						= 0x04,
        INTERRUPT_ENABLE_1_05						= 0x05,
        INTERRUPT_ENABLE_2_06						= 0x06,
        OPERATING_MODE_AND_FUNCTION_CONTROL_1_07	= 0x07,
        OPERATING_MODE_AND_FUNCTION_CONTROL_2_08	= 0x08,
        CRYSTAL_OSCILLATOR_LOAD_CAPACITANCE_09		= 0x09,
        MICROCONTROLLER_OUTPUT_CLOCK_0A				= 0x0A,
        GPIO0_CONFIGURATION_0B						= 0x0B,
        GPIO1_CONFIGURATION_0C						= 0x0C,
        GPIO2_CONFIGURATION_0D						= 0x0D,
        IO_PORT_CONFIGURATION_0E					= 0x0E,
        ADC_CONFIGURATION_0F						= 0x0F,
        ADC_SENSOR_AMPLIFIER_OFFSET_10				= 0x10,
        ADC_VALUE_11								= 0x11,
        TEMPERATURE_SENSOR_CONTROL_12				= 0x12,
        TEMPERATURE_VALUE_OFFSET_13					= 0x13,
        WAKE_UP_TIMER_PERIOD_1_14					= 0x14,
        WAKE_UP_TIMER_PERIOD_2_15					= 0x15,
        WAKE_UP_TIMER_PERIOD_3_16					= 0x16,
        WAKE_UP_TIMER_VALUE_1_17					= 0x17,
        WAKE_UP_TIMER_VALUE_2_18					= 0x18,
        LOW_DUTY_CYCLE_MODE_DURATION_19				= 0x19,
        LOW_BATTERY_DETECTOR_THRESHOLD_1A			= 0x1A,
        BATTERY_VOLTAGE_LEVEL_1B					= 0x1B,
        IF_FILTER_BANDWIDTH_1C						= 0x1C,
        AFC_LOOP_GEARSHIFT_OVERRIDE_1D				= 0x1D,
        AFC_TIMING_CONTROL_1E						= 0x1E,
        CLOCK_RECOVERY_GEARSHIFT_OVERRIDE_1F		= 0x1F,
        CLOCK_RECOVERY_OVERSAMPLING_RATIO_20		= 0x20,
        CLOCK_RECOVERY_OFFSET_2_21					= 0x21,
        CLOCK_RECOVERY_OFFSET_1_22					= 0x22,
        CLOCK_RECOVERY_OFFSET_0_23					= 0x23,
        CLOCK_RECOVERT_TIMING_LOOP_GAIN_1_24		= 0x24,
        CLOCK_RECOVERT_TIMING_LOOP_GAIN_0_25		= 0x25,
        RECEIVED_SIGNAL_STRENGTH_INDICATOR_26		= 0x26,
        RSSI_THRESHOLD_FOR_CLEAR_CHANNEL_INDICATOR_27	= 0x27,
        ANTENNA_DIVERSITY_REGISTER_1_28				= 0x28,
        ANTENNA_DIVERSITY_REGISTER_2_29				= 0x29,
        AFC_LIMITER_2A								= 0x2A,
        AFC_CORRECTION_READ_2B						= 0x2B,
        OOK_COUNTER_VALUE_1_2C						= 0x2C,
        OOK_COUNTER_VALUE_2_2D						= 0x2D,
        SLICER_PEAK_HOLD_2E							= 0x2E,
    //	RESERVED_2F 								= 0x2F,
        DATA_ACCESS_CONTROL_30						= 0x30,
        EXMAC_STATUS_31								= 0x31,
        HEADER_CONTROL_1_32							= 0x32,
        HEADER_CONTROL_2_33							= 0x33,
        PREAMBLE_LENGTH_34							= 0x34,
        PREAMBLE_DETECTION_CONTROL_35				= 0x35,
        SYNC_WORD_3_36								= 0x36,
        SYNC_WORD_2_37								= 0x37,
        SYNC_WORD_1_38								= 0x38,
        SYNC_WORD_0_39								= 0x39,
        TRANSMIT_HEADER_3_3A						= 0x3A,
        TRANSMIT_HEADER_2_3B						= 0x3B,
        TRANSMIT_HEADER_1_3C						= 0x3C,
        TRANSMIT_HEADER_0_3D						= 0x3D,
        TRANSMIT_PACKET_LENGTH_3E					= 0x3E,
        CHECK_HEADER_3_3F							= 0x3F,
        CHECK_HEADER_2_40							= 0x40,
        CHECK_HEADER_1_41							= 0x41,
        CHECK_HEADER_0_42							= 0x42,
        HEADER_ENABLE_3_43							= 0x43,
        HEADER_ENABLE_2_44							= 0x44,
        HEADER_ENABLE_1_45							= 0x45,
        HEADER_ENABLE_0_46							= 0x46,
        RECEIVED_HEADER_3_47						= 0x47,
        RECEIVED_HEADER_2_48						= 0x48,
        RECEIVED_HEADER_1_49						= 0x49,
        RECEIVED_HEADER_0_4A						= 0x4A,
        RECEIVED_PACKET_LENGTH_4B					= 0x4B,
    //	RESERVED_4C									= 0x4C,
    //	RESERVED_4D									= 0x4D,
    //	RESERVED_4E									= 0x4E,
        ADC8_CONTROL_4F								= 0x4F,
    //	RESERVED									= 0x50,
    //	RESERVED									= 0x51,
    //	RESERVED									= 0x52,
    //	RESERVED									= 0x53,
    //	RESERVED									= 0x54,
    //	RESERVED									= 0x55,
    //	RESERVED									= 0x56,
    //	RESERVED									= 0x57,
    //	RESERVED									= 0x58,
    //	RESERVED									= 0x59,
    //	RESERVED									= 0x5A,
    //	RESERVED									= 0x5B,
    //	RESERVED									= 0x5C,
    //	RESERVED									= 0x5D,
    //	RESERVED									= 0x5E,
    //	RESERVED									= 0x5F,
        CHANNEL_FILTER_COEFFICIENT_ADDRESS_60		= 0x60,
    //	RESERVED									= 0x61,
        CRYSTAL_OSCILLATOR_CONTROL_TES_62			= 0x62,
    //	RESERVED									= 0x63,
    //	RESERVED									= 0x64,
    //	RESERVED									= 0x65,
    //	RESERVED									= 0x66,
    //	RESERVED									= 0x67,
    //	RESERVED									= 0x68,
        AGC_OVERRIDE_1_69							= 0x69,
    //	RESERVED									= 0x6A,
    //	RESERVED									= 0x6B,
    //	RESERVED									= 0x6C,
        TX_POWER_6D									= 0x6D,
        TX_DATA_RATE_1_6E							= 0x6E,
        TX_DATA_RATE_0_6F							= 0x6F,
        MODULATION_MODE_CONTROL_1_70				= 0x70,
        MODULATION_MODE_CONTROL_2_71				= 0x71,
        FREQUENCY_DEVIATION_72						= 0x72,
        FREQUENCY_OFFSET_1_73						= 0x73,
        FREQUENCY_OFFSET_2_74						= 0x74,
        FREQUENCY_BAND_SELECT_75					= 0x75,
        NOMINAL_CARRIER_FREQUENCY_1_76				= 0x76,
        NOMINAL_CARRIER_FREQUENCY_0_77				= 0x77,
    //	RESERVED									= 0x78,
        FREQUENCY_HOPPING_CHANNEL_SELECT_79			= 0x79,
        FREQUENCY_HOPPING_STEP_SIZE_7A				= 0x7A,
    //	RESERVED									= 0x7B,
        TX_FIFO_CONTROL_1_7C						= 0x7C,
        TX_FIFO_CONTROL_2_7D						= 0x7D,
        RX_FIFO_CONTROL_7E							= 0x7E,
        FIFO_ACCESS_7F								= 0x7F
    };

    enum class Device_Type : uint8_t
    {
        RX_TRX                 						= 0x08,
        TX                     						= 0x07
    };

    enum class Modulation_Type  : uint8_t
    {
        UNMODULATED_CARRIER							= 0x00,
        OOK											= 0x01,
        FSK											= 0x02,
        GFSK										= 0x03
    };

    enum class Modulation_Data_Source : uint8_t
    {
        DIRECT_GPIO									= 0x00,
        DIRECT_SDI									= 0x01,
        FIFO										= 0x02,
        PN9											= 0x03
    };

    enum class CRC_Mode : uint8_t
    {
        CRC_DISABLED,
        CRC_DATA_ONLY,
        CRC_NORMAL,
    };

    enum class CRC_Polynomial : uint8_t
    {
        CCITT										= 0x00,
        CRC16										= 0x01,
        IEC16										= 0x02,
        BIACHEVA									= 0x03,
    };
    #define BAICHEVA BIACHEVA // Seems like Biacheva is a typo in the data sheet

    enum class Data_Clock_Configuration : uint8_t
    {
        NONE										= 0x00,
        GPIO										= 0x01,
        SDO											= 0x02,
        NIRQ										= 0x03
    };

    enum class GPIO : uint8_t
    {
        GPIO0										= 0x0B,
        GPIO1										= 0x0C,
        GPIO2										= 0x0D
    };

    enum class GPIO_Function : uint8_t
    {
        POWER_ON_RESET								= 0x00,		// This depends on the GPIO!
        WAKE_UP_TIMER_1								= 0x01,
        LOW_BATTERY_DETECT_1						= 0x02,
        DIRECT_DIGITAL_INPUT						= 0x03,
        EXTERNAL_INTERRUPT_FALLING					= 0x04,
        EXTERNAL_INTERRUPT_RISING					= 0x05,
        EXTERNAL_INTERRUPT_CHANGE					= 0x06,
        ADC_ANALOGUE_INPUT							= 0x07,
    //	RESERVED									= 0x08,
    //	RESERVED									= 0x09,
        DIRECT_DIGITAL_OUTPUT						= 0x0A,
    //	RESERVED									= 0x0B,
    //	RESERVED									= 0x0C,
    //	RESERVED									= 0x0D,
        REFERENCE_VOLTAGE							= 0x0E,
        DATA_CLOCK_OUTPUT							= 0x0F,
        DATA_INPUT									= 0x10,
        EXTERNAL_RETRANSMISSION_REQUEST				= 0x11,
        TX_STATE									= 0x12,
        TX_FIFO_ALMOST_FULL							= 0x13,
        RX_DATA										= 0x14,
        RX_STATE									= 0x15,
        RX_FIFO_ALMOST_FULL							= 0x16,
        ANTENNA_1_SWITCH							= 0x17,
        ANTENNA_2_SWITCH							= 0x18,
        VALID_PREAMBLE_DETECTED						= 0x19,
        INVALID_PREAMBLE_DETECTED					= 0x1A,
        SYNC_WORD_DETECTED							= 0x1B,
        CLEAR_CHANNEL_ASSESSMENT					= 0x1C,
        VDD											= 0x1D,
        GND											= 0x1E
    };

    // Interrupt Enable spans 2 registers, but for the purpose of this enum they are treated as one (16 bit) register
    enum class Interrupt : uint16_t
    {
        INT_POWER_ON_RESET_INT							= (1 << 0),
        INT_CHIP_READY									= (1 << 1),
        INT_LOW_BATTERY_DETECT							= (1 << 2),
        INT_WAKE_UP_TIMER								= (1 << 3),
        INT_RSSI										= (1 << 4),
        INT_INVALID_PREAMBLE							= (1 << 5),
        INT_VALID_PREAMBLE								= (1 << 6),
        INT_SYNC_WORD									= (1 << 7),
        INT_CRC_ERROR									= (1 << 8),
        INT_VALID_PACKET_RECEIVED						= (1 << 9),
        INT_PACKET_SENT									= (1 << 10),
        INT_EXTERNAL									= (1 << 11),
        INT_RX_FIFO_ALMOST_FULL_INT						= (1 << 12),
        INT_TX_FIFO_ALMOST_EMPTY_INT					= (1 << 13),
        INT_TX_FIFO_ALMOST_FULL_INT						= (1 << 14),
        INT_FIFO_UNDERFLOW_OVERFLOW						= uint16_t(1 << 15)
    };

    // Currently no support for antenna diversity settings
    // Treat registers 1 and 2 as a single 16 bit register (as above)
    enum class Operating_Mode : uint16_t
    {
        TX_FIFO_RESET								= (1 << 0),
        RX_FIFO_RESET								= (1 << 1),
        LOW_DUTY_CYCLE_MODE							= (1 << 2),
        AUTOMATIC_TRANSMISSION						= (1 << 3),
        RX_MULTI_PACKET								= (1 << 4),
    //	Antenna diversity (bits 5-7) not supported
        READY_MODE									= (1 << 8),
        TUNE_MODE									= (1 << 9),
        RX_MODE										= (1 << 10),
        TX_MODE										= (1 << 11),
        CRYSTAL_OSCILLATOR_SELECT					= (1 << 12),
        ENABLE_WAKE_UP_TIMER						= (1 << 13),
        ENABLE_LOW_BATTERY_DETECT					= (1 << 14),
        RESET										= uint16_t(1 << 15)
    };

    // RH_RF22_REG_02_DEVICE_STATUS                    0x02
    enum class Device_Status : uint8_t
    {
        FFOVL                              = 0x80,
        FFUNFL                             = 0x40,
        RXFFEM                             = 0x20,
        HEADERR                            = 0x10,
        FREQERR                            = 0x08,
        LOCKDET                            = 0x04,
        CPS                                = 0x03,
        CPS_IDLE                           = 0x00,
        CPS_RX                             = 0x01,
        CPS_TX                             = 0x10,
    };

    // RH_RF22_REG_75_FREQUENCY_BAND_SELECT            0x75
    enum class Frequency_Band_Select : uint8_t
    {
        SBSEL                              = 0x40,
        HBSEL                              = 0x20,
        FB                                 = 0x1f,
    };


    bool init(std::string const& device, uint32_t speed, uint8_t sdn_gpio, uint8_t nirq_gpio);

    bool shutdown();
    bool powerup();

    void reset();

    // Set or get the frequency hopping channel
    void set_channel(uint8_t channel);
    uint8_t get_channel() const;

    //sets the whole modem block with settings obtained from
    //  http://www.hoperf.com/upload/rf/RF22B%2023B%2031B%2042B%2043B%20Register%20Settings_RevB1-v5.xls
/*
    1C - IF_FILTER_BANDWIDTH
    1D - AFC_LOOP_GEARSHIFT_OVERRIDE

    20 - CLOCK_RECOVERY_OVERSAMPLING_RATIO
    21 - CLOCK_RECOVERY_OFFSET_2
    22 - CLOCK_RECOVERY_OFFSET_1
    23 - CLOCK_RECOVERY_OFFSET_0
    24 - CLOCK_RECOVERT_TIMING_LOOP_GAIN_1
    25 - CLOCK_RECOVERT_TIMING_LOOP_GAIN_0
    2A - AFC_LIMITER
    2C - OOK_COUNTER_VALUE_1
    2D - OOK_COUNTER_VALUE_2
    2E - SLICER_PEAK_HOLD

    30 - DATA_ACCESS_CONTROL
    33 - HEADER_CONTROL_2
    34 - PREAMBLE_LENGTH
    35 - PREAMBLE_DETECTION_CONTROL
    36 - SYNC_WORD_3
    37 - SYNC_WORD_2
    38 - SYNC_WORD_1
    39 - SYNC_WORD_0

    6E - TX_DATA_RATE_1
    6F - TX_DATA_RATE_0

    70 - MODULATION_MODE_CONTROL_1
    71 - MODULATION_MODE_CONTROL_2
    72 - FREQUENCY_DEVIATION

    75 - FREQUENCY_BAND_SELECT
    76 - NOMINAL_CARRIER_FREQUENCY_1
    77 - NOMINAL_CARRIER_FREQUENCY_0
*/
    void set_modem_configuration(uint8_t data[42]);

    // Set or get the transmission power
    void set_tx_power_dBm(uint8_t power);
    uint8_t get_tx_power_dBm();

    int8_t get_input_dBm() const;

    // Set the operating mode
    //	This function does not toggle individual pins as with other functions
    //	It expects a bitwise-ORed combination of the modes you want set
    void set_operating_mode(uint16_t mode);

    // Get operating mode (bitwise-ORed)
    uint16_t get_operating_mode() const;

    void rx_mode();
    void tx_mode();
    void idle_mode();
    void sleep_mode();
    void stand_by_mode();

    void set_gpio_function(GPIO gpio, GPIO_Function func);

    void set_register(Register register, uint8_t value);
    uint8_t get_register(Register register) const;

    void set_register16(Register register, uint16_t value);
    uint16_t get_register16(Register register) const;

    bool write_tx_fifo(void const* data, size_t size);
    bool read_rx_fifo(void* data, size_t size);

    bool tx(size_t size, uint8_t channel);
    bool rx(size_t& size, uint8_t channel, Clock::duration timeout);

    bool get_nirq_level();

private:

    uint8_t m_sdn_gpio = 0;
    uint8_t m_nirq_gpio = 0;

    SPI_Dev m_spi_dev;
    bool m_is_initialized = false;
};


}
}
