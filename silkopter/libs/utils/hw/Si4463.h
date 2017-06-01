#pragma once

#include <string>
#include "ISPI.h"
#include "utils/Clock.h"

namespace util
{
namespace hw
{

class Si4463
{
public:
    Si4463();
    ~Si4463();

    bool init(hw::ISPI& spi, uint8_t sdn_gpio, uint8_t nirq_gpio);

    enum class Command : uint8_t
    {
        NOP = 0x00,
        POWER_UP = 0x02,
        PART_INFO = 0x01,
        FUNC_INFO = 0x10,
        SET_PROPERTY = 0x11,
        GET_PROPERTY = 0x12,
        GPIO_PIN_CFG = 0x13,
        GET_ADC_READING = 0x14,
        FIFO_INFO = 0x15,
        PACKET_INFO = 0x16,
        IRCAL = 0x17,
        PROTOCOL_CFG = 0x18,
        GET_INT_STATUS = 0x20,
        GET_PH_STATUS = 0x21,
        GET_MODEM_STATUS = 0x22,
        GET_CHIP_STATUS = 0x23,
        START_TX = 0x31,
        START_RX = 0x32,
        REQUEST_DEVICE_STATE = 0x33,
        CHANGE_STATE = 0x34,
        READ_CMD_BUFF = 0x44,
        FRR_A_READ = 0x50,
        FRR_B_READ = 0x51,
        FRR_C_READ = 0x53,
        FRR_D_READ = 0x57,
        WRITE_TX_FIFO = 0x66,
        READ_RX_FIFO = 0x77,
        RX_HOP = 0x36,
    };

    enum class Property : uint16_t
    {
        GLOBAL_XO_TUNE = 0x0000, //0x40 Configure crystal oscillator frequency tuning
        GLOBAL_CLK_CFG = 0x0001, //0x0 Clock configuration options
        GLOBAL_LOW_BATT_THRESH = 0x0002, //0x18 Low battery threshold
        GLOBAL_CONFIG = 0x0003, //0x0 Global configuration settings
        GLOBAL_WUT_CONFIG = 0x0004, //0x00 GLOBAL WUT configuation
        GLOBAL_WUT_M_15_8 = 0x0005, //0x00 Configure WUT_M_15_8
        GLOBAL_WUT_M_7_0 = 0x0006, //0x01 Configure WUT_M_7_0
        GLOBAL_WUT_R = 0x0007, //0x00 Configure WUT_R
        GLOBAL_WUT_LDC = 0x0008, //0x00 Configure WUT_LDC
        INT_CTL_ENABLE = 0x0100, //0x04 Interrupt enable property
        INT_CTL_PH_ENABLE = 0x0101, //0x00 Packet handler interrupt enable property
        INT_CTL_MODEM_ENABLE = 0x0102, //0x00 Modem interrupt enable property
        INT_CTL_CHIP_ENABLE = 0x0103, //0x04 Chip interrupt enable property
        FRR_CTL_A_MODE = 0x0200, //0x01 Fast Response Register A Configuration
        FRR_CTL_B_MODE = 0x0201, //0x02 Fast Response Register B Configuration
        FRR_CTL_C_MODE = 0x0202, //0x09 Fast Response Register C Configuration
        FRR_CTL_D_MODE = 0x0203, //0x00 Fast Response Register D Configuration
        PREAMBLE_TX_LENGTH = 0x1000, //0x08 Preamble length
        PREAMBLE_CONFIG_STD_1 = 0x1001, //0x14 Standard preamble configuration
        PREAMBLE_CONFIG_NSTD = 0x1002, //0x00 Non-standard preamble configuation
        PREAMBLE_CONFIG_STD_2 = 0x1003, //0x0F Standard preamble configuration
        PREAMBLE_CONFIG = 0x1004, //0x21 Preamble configuration bits
        PREAMBLE_PATTERN_31_24 = 0x1005, //0x0 Preamble pattern
        PREAMBLE_PATTERN_23_16 = 0x1006, //0x0 Preamble pattern
        PREAMBLE_PATTERN_15_8 = 0x1007, //0x0 Preamble pattern
        PREAMBLE_PATTERN_7_0 = 0x1008, //0x0 Preamble pattern
        SYNC_CONFIG = 0x1100, //0x01 Sync configuration bits
        SYNC_BITS_31_24 = 0x1101, //0x2D Byte 3 of sync word
        SYNC_BITS_23_16 = 0x1102, //0xD4 Byte 2 of sync word
        SYNC_BITS_15_8 = 0x1103, //0x2D Byte 1 of sync word
        SYNC_BITS_7_0 = 0x1104, //0xD4 Byte 0 of sync word
        PKT_LEN_ADJUST = 0x120A, //0x0 Adjust length field by this amount to derive the byte count of the variable length field.
        PKT_TX_THRESHOLD = 0x120B, //0x30 TX almost empty threshold.
        PKT_RX_THRESHOLD = 0x120C, //0x30 RX almost full threshold.
        PKT_FIELD_1_LENGTH_12_8 = 0x120D, //0x00 Byte 1 of field length
        PKT_FIELD_1_LENGTH_7_0 = 0x120E, //0x00 Byte 0 of field length
        PKT_FIELD_1_CONFIG = 0x120F, //0x00 Field 1 configuration bits.
        PKT_CRC_CONFIG = 0x1200, //0x0 Select a CRC polynomial and seed
        PKT_CONFIG1 = 0x1206, //0x0 General packet configuration bits
        PKT_LEN = 0x1208, //0x00 Provides information regarding how to use the length from the received packet.
        PKT_LEN_FIELD_SOURCE = 0x1209, //0x0 Field number containing the embedded length field.
        PKT_FIELD_4_LENGTH_7_0 = 0x121A, //0x00 Byte 0 of field length
        PKT_FIELD_4_CONFIG = 0x121B, //0x00 Field 4 configuration bits.
        PKT_FIELD_4_CRC_CONFIG = 0x121C, //0x00 Field 4 CRC configuration bits.
        PKT_FIELD_5_LENGTH_12_8 = 0x121D, //0x00 Byte 1 of field length
        PKT_FIELD_5_LENGTH_7_0 = 0x121E, //0x00 Byte 0 of field length
        PKT_FIELD_5_CONFIG = 0x121F, //0x00 Field 5 configuration bits.
        PKT_FIELD_1_CRC_CONFIG = 0x1210, //0x00 Field 1 CRC configuration bits.
        PKT_FIELD_2_LENGTH_12_8 = 0x1211, //0x00 Byte 1 of field length
        PKT_FIELD_2_LENGTH_7_0 = 0x1212, //0x00 Byte 0 of field length
        PKT_FIELD_2_CONFIG = 0x1213, //0x00 Field 2 configuration bits.
        PKT_FIELD_2_CRC_CONFIG = 0x1214, //0x00 Field 2 CRC configuration bits.
        PKT_FIELD_3_LENGTH_12_8 = 0x1215, //0x00 Byte 1 of field length
        PKT_FIELD_3_LENGTH_7_0 = 0x1216, //0x00 Byte 0 of field length
        PKT_FIELD_3_CONFIG = 0x1217, //0x00 Field 3 configuration bits.
        PKT_FIELD_3_CRC_CONFIG = 0x1218, //0x00 Field 3 CRC configuration bits.
        PKT_FIELD_4_LENGTH_12_8 = 0x1219, //0x00 Byte 1 of field length
        PKT_RX_FIELD_3_LENGTH_7_0 = 0x122A, //0x00 Byte 0 of field length for RX
        PKT_RX_FIELD_3_CONFIG = 0x122B, //0x00 Field 3 configuration bits for RX.
        PKT_RX_FIELD_3_CRC_CONFIG = 0x122C, //0x00 Field 3 CRC configuration bits for RX.
        PKT_RX_FIELD_4_LENGTH_12_8 = 0x122D, //0x00 Byte 1 of field length for RX
        PKT_RX_FIELD_4_LENGTH_7_0 = 0x122E, //0x00 Byte 0 of field length for RX
        PKT_RX_FIELD_4_CONFIG = 0x122F, //0x00 Field 4 configuration bits for RX.
        PKT_FIELD_5_CRC_CONFIG = 0x1220, //0x00 Field 5 CRC configuration bits.
        PKT_RX_FIELD_1_LENGTH_12_8 = 0x1221, //0x00 Byte 1 of field length for RX
        PKT_RX_FIELD_1_LENGTH_7_0 = 0x1222, //0x00 Byte 0 of field length for RX
        PKT_RX_FIELD_1_CONFIG = 0x1223, //0x00 Field 1 configuration bits for RX.
        PKT_RX_FIELD_1_CRC_CONFIG = 0x1224, //0x00 Field 1 CRC configuration bits for RX.
        PKT_RX_FIELD_2_LENGTH_12_8 = 0x1225, //0x00 Byte 1 of field length for RX
        PKT_RX_FIELD_2_LENGTH_7_0 = 0x1226, //0x00 Byte 0 of field length for RX
        PKT_RX_FIELD_2_CONFIG = 0x1227, //0x00 Field 2 configuration bits for RX.
        PKT_RX_FIELD_2_CRC_CONFIG = 0x1228, //0x00 Field 2 CRC configuration bits for RX.
        PKT_RX_FIELD_3_LENGTH_12_8 = 0x1229, //0x00 Byte 1 of field length for RX
        PKT_RX_FIELD_4_CRC_CONFIG = 0x1230, //0x00 Field 4 CRC configuration bits for RX.
        PKT_RX_FIELD_5_LENGTH_12_8 = 0x1231, //0x00 Byte 1 of field length for RX
        PKT_RX_FIELD_5_LENGTH_7_0 = 0x1232, //0x00 Byte 0 of field length for RX
        PKT_RX_FIELD_5_CONFIG = 0x1233, //0x00 Field 5 configuration bits for RX.
        PKT_RX_FIELD_5_CRC_CONFIG = 0x1234, //0x00 Field 5 CRC configuration bits for RX.
        MODEM_FREQ_DEV_2 = 0x200A, //0x00 Byte 2 of TX frequency deviation (a 17-bit unsigned number). This only programs the MSB of TX frequency deviation.
        MODEM_FREQ_DEV_1 = 0x200B, //0x06 Byte 1 of frequency deviation.
        MODEM_FREQ_DEV_0 = 0x200C, //0xD3 Byte 0 of frequency deviation.
        MODEM_MOD_TYPE = 0x2000, //0x02 Modulation Type
        MODEM_MAP_CONTROL = 0x2001, //0x80 Controls bit mapping.
        MODEM_DATA_RATE_2 = 0x2003, //0x0F Byte 2 of TX data rate in bps (bits per second).
        MODEM_DATA_RATE_1 = 0x2004, //0x42 Byte 1 of TX data rate in bps (bits per second).
        MODEM_DATA_RATE_0 = 0x2005, //0x40 Byte 0 of TX data rate in bps (bits per second).
        MODEM_RSSI_THRESH = 0x204A, //0xFF RSSI threshold control
        MODEM_RSSI_JUMP_THRESH = 0x204B, //0x0C RSSI jumping detection threshold.
        MODEM_RSSI_CONTROL = 0x204C, //0x01 RSSI control
        MODEM_RSSI_CONTROL2 = 0x204D, //0x00 RSSI control
        MODEM_RSSI_COMP = 0x204E, //0x32 RSSI reading offset.
        MODEM_ANT_DIV_CONTROL = 0x2049, //0x80 Specifies antenna diversity controls. Antenna diversity mode is valid for standard packet only.
        PA_MODE = 0x2200, //0x08 PA operating mode and groups.
        PA_PWR_LVL = 0x2201, //0x7F PA Level Configuration
        PA_BIAS_CLKDUTY = 0x2202, //0x00 PA Bias and TX clock duty cycle configuration
        PA_TC = 0x2203, //0x5D PA cascode ramping Configuration
        MATCH_MASK_4 = 0x300A, //0x00 Match 4 mask.
        MATCH_CTRL_4 = 0x300B, //0x00 Match 4 configuration.
        MATCH_VALUE_1 = 0x3000, //0x00 Match 1 value.
        MATCH_MASK_1 = 0x3001, //0x00 Match 1 mask.
        MATCH_CTRL_1 = 0x3002, //0x00 Packet match enable and match 1 configuration.
        MATCH_VALUE_2 = 0x3003, //0x00 Match 2 value.
        MATCH_MASK_2 = 0x3004, //0x00 Match 2 mask.
        MATCH_CTRL_2 = 0x3005, //0x00 Match 2 configuration.
        MATCH_VALUE_3 = 0x3006, //0x00 Match 3 value.
        MATCH_MASK_3 = 0x3007, //0x00 Match 3 mask.
        MATCH_CTRL_3 = 0x3008, //0x00 Match 3 configuration.
        MATCH_VALUE_4 = 0x3009, //0x00 Match 4 value.
        FREQ_CONTROL_INTE = 0x4000, //0x3C Frac-N PLL integer number.
        FREQ_CONTROL_FRAC_2 = 0x4001, //0x08 Byte 2 of Frac-N PLL fraction number.
        FREQ_CONTROL_FRAC_1 = 0x4002, //0x00 Byte 1 of Frac-N PLL fraction number.
        FREQ_CONTROL_FRAC_0 = 0x4003, //0x00 Byte 0 of Frac-N PLL fraction number.
        FREQ_CONTROL_CHANNEL_STEP_SIZE_1 = 0x4004, //0x00 Byte 1 of channel step size.
        FREQ_CONTROL_CHANNEL_STEP_SIZE_0 = 0x4005, //0x00 Byte 0 of channel step size.
        FREQ_CONTROL_VCOCNT_RX_ADJ = 0x4007, //0xFF VCO target count adjustment for RX
        RX_HOP_CONTROL = 0x5000, //0x04 RX hop control.
        RX_HOP_TABLE_SIZE = 0x5001, //0x01 Number of entries in the RX hop table.
        RX_HOP_TABLE_ENTRY_0 = 0x5002, //0x0 No.1 entry in RX hopping table.
        RX_HOP_TABLE_ENTRY_1 = 0x5003, //0x1 Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_2 = 0x5004, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_3 = 0x5005, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_4 = 0x5006, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_5 = 0x5007, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_6 = 0x5008, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_7 = 0x5009, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_8 = 0x500A, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_9 = 0x500B, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_10 = 0x500C, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_11 = 0x500D, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_12 = 0x500E, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_13 = 0x500F, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_14 = 0x5010, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_15 = 0x5011, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_16 = 0x5012, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_17 = 0x5013, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_18 = 0x5014, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_19 = 0x5015, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_20 = 0x5016, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_21 = 0x5017, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_22 = 0x5018, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_23 = 0x5019, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_24 = 0x501A, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_25 = 0x501B, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_26 = 0x501C, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_27 = 0x501D, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_28 = 0x501E, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_29 = 0x501F, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_30 = 0x5020, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_31 = 0x5021, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_32 = 0x5022, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_33 = 0x5023, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_34 = 0x5024, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_35 = 0x5025, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_36 = 0x5026, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_37 = 0x5027, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_38 = 0x5028, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_39 = 0x5029, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_40 = 0x502A, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_41 = 0x502B, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_42 = 0x502C, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_43 = 0x502D, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_44 = 0x502E, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_45 = 0x502F, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_46 = 0x5030, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_47 = 0x5031, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_48 = 0x5032, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_49 = 0x5033, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_50 = 0x5034, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_51 = 0x5035, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_52 = 0x5036, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_53 = 0x5037, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_54 = 0x5038, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_55 = 0x5039, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_56 = 0x503A, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_57 = 0x503B, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_58 = 0x503C, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_59 = 0x503D, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_60 = 0x503E, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_61 = 0x503F, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_62 = 0x5040, //0xX Entries 2-63 in RX hopping table.
        RX_HOP_TABLE_ENTRY_63 = 0x5041, //0x63 No.64 entry in RX hopping table.
    };


    bool upload_patch(void const* data);
    bool upload_config(void const* data);

    bool boot();
    bool shutdown();
    bool reset();
    bool check_part();

    bool call_api_raw(void const* tx_data, size_t tx_size, void* rx_data, size_t rx_size);

    bool get_properties(Property start_prop, size_t prop_count, void* rx_data, size_t rx_size);

    bool set_property(Property prop, void const* tx_data, size_t tx_size);
    bool set_properties(Property start_prop, size_t prop_count, void const* tx_data, size_t tx_size);

    bool call_api_raw(std::initializer_list<uint8_t> const& tx_data);

    bool write_tx_fifo(void const* data, size_t size);
    bool read_rx_fifo(void* data, size_t size);

    bool wait_for_ph_interrupt(bool& got_it, uint8_t clear_interrupts, uint8_t& pending, uint8_t& status, Clock::duration timeout);

    bool read_frr_a(uint8_t* values, size_t value_count);
    bool read_frr_b(uint8_t* values, size_t value_count);
    bool read_frr_c(uint8_t* values, size_t value_count);
    bool read_frr_d(uint8_t* values, size_t value_count);

    bool wait_for_cts();
    bool get_nirq_level();

private:

    uint8_t m_sdn_gpio = 0;
    uint8_t m_nirq_gpio = 0;

    std::vector<uint8_t> m_tx_data;
    std::vector<uint8_t> m_rx_data;

    hw::ISPI* m_spi = nullptr;
    bool m_is_initialized = false;
    bool m_needs_to_wait_for_cts = false;
};

inline bool Si4463::call_api_raw(std::initializer_list<uint8_t> const& tx_data)
{
    m_tx_data.resize(tx_data.size());
    std::copy(tx_data.begin(), tx_data.end(), m_tx_data.begin());
    return call_api_raw(m_tx_data.data(), m_tx_data.size(), nullptr, 0);
}


}
}
