#include "FCStdAfx.h"
#include "UltimateSensorFusion.h"
#include "physics/constants.h"

#include "hal.def.h"

namespace silk
{
namespace node
{

// BMP280 registers
constexpr uint8_t BMP280_TEMP_XLSB   = 0xFC;
constexpr uint8_t BMP280_TEMP_LSB    = 0xFB;
constexpr uint8_t BMP280_TEMP_MSB    = 0xFA;
constexpr uint8_t BMP280_PRESS_XLSB  = 0xF9;
constexpr uint8_t BMP280_PRESS_LSB   = 0xF8;
constexpr uint8_t BMP280_PRESS_MSB   = 0xF7;
constexpr uint8_t BMP280_CONFIG      = 0xF5;
constexpr uint8_t BMP280_CTRL_MEAS   = 0xF4;
constexpr uint8_t BMP280_STATUS      = 0xF3;
constexpr uint8_t BMP280_RESET       = 0xE0;
constexpr uint8_t BMP280_ID          = 0xD0;  // should be 0x58
constexpr uint8_t BMP280_CALIB00     = 0x88;

//Magnetometer Registers
constexpr uint8_t AK8963_ADDRESS   = 0x0C;
constexpr uint8_t WHO_AM_I_AK8963  = 0x00; // should return 0x48
constexpr uint8_t INFO             = 0x01;
constexpr uint8_t AK8963_ST1       = 0x02;  // data ready status bit 0
constexpr uint8_t AK8963_XOUT_L    = 0x03;  // data
constexpr uint8_t AK8963_XOUT_H    = 0x04;
constexpr uint8_t AK8963_YOUT_L    = 0x05;
constexpr uint8_t AK8963_YOUT_H    = 0x06;
constexpr uint8_t AK8963_ZOUT_L    = 0x07;
constexpr uint8_t AK8963_ZOUT_H    = 0x08;
constexpr uint8_t AK8963_ST2       = 0x09;  // Data overflow bit 3 and data read error status bit 2
constexpr uint8_t AK8963_CNTL      = 0x0A;  // Power down (0000), single-measurement (0001), self-test (1000) and Fuse ROM (1111) modes on bits 3:0
constexpr uint8_t AK8963_ASTC      = 0x0C;  // Self test control
constexpr uint8_t AK8963_I2CDIS    = 0x0F;  // I2C disable
constexpr uint8_t AK8963_ASAX      = 0x10;  // Fuse ROM x-axis sensitivity adjustment value
constexpr uint8_t AK8963_ASAY      = 0x11;  // Fuse ROM y-axis sensitivity adjustment value
constexpr uint8_t AK8963_ASAZ      = 0x12;  // Fuse ROM z-axis sensitivity adjustment value

constexpr uint8_t SELF_TEST_X_GYRO = 0x00;
constexpr uint8_t SELF_TEST_Y_GYRO = 0x01;
constexpr uint8_t SELF_TEST_Z_GYRO = 0x02;

/*#define X_FINE_GAIN      0x03 // [7:0] fine gain
#define Y_FINE_GAIN      0x04
#define Z_FINE_GAIN      0x05
#define XA_OFFSET_H      0x06 // User-defined trim values for accelerometer
#define XA_OFFSET_L_TC   0x07
#define YA_OFFSET_H      0x08
#define YA_OFFSET_L_TC   0x09
#define ZA_OFFSET_H      0x0A
#define ZA_OFFSET_L_TC   0x0B */

constexpr uint8_t SELF_TEST_X_ACCEL = 0x0D;
constexpr uint8_t SELF_TEST_Y_ACCEL = 0x0E;
constexpr uint8_t SELF_TEST_Z_ACCEL = 0x0F;

constexpr uint8_t SELF_TEST_A      = 0x10;

constexpr uint8_t XG_OFFSET_H      = 0x13;  // User-defined trim values for gyroscope
constexpr uint8_t XG_OFFSET_L      = 0x14;
constexpr uint8_t YG_OFFSET_H      = 0x15;
constexpr uint8_t YG_OFFSET_L      = 0x16;
constexpr uint8_t ZG_OFFSET_H      = 0x17;
constexpr uint8_t ZG_OFFSET_L      = 0x18;
constexpr uint8_t SMPLRT_DIV       = 0x19;
constexpr uint8_t CONFIG           = 0x1A;
constexpr uint8_t GYRO_CONFIG      = 0x1B;
constexpr uint8_t ACCEL_CONFIG     = 0x1C;
constexpr uint8_t ACCEL_CONFIG2    = 0x1D;
constexpr uint8_t LP_ACCEL_ODR     = 0x1E;
constexpr uint8_t WOM_THR          = 0x1F;

constexpr uint8_t MOT_DUR          = 0x20;  // Duration counter threshold for motion interrupt generation, 1 kHz rate, LSB = 1 ms
constexpr uint8_t ZMOT_THR         = 0x21;  // Zero-motion detection threshold bits [7:0]
constexpr uint8_t ZRMOT_DUR        = 0x22;  // Duration counter threshold for zero motion interrupt generation, 16 Hz rate, LSB = 64 ms

constexpr uint8_t FIFO_EN          = 0x23;
constexpr uint8_t I2C_MST_CTRL     = 0x24;
constexpr uint8_t I2C_SLV0_ADDR    = 0x25;
constexpr uint8_t I2C_SLV0_REG     = 0x26;
constexpr uint8_t I2C_SLV0_CTRL    = 0x27;
constexpr uint8_t I2C_SLV1_ADDR    = 0x28;
constexpr uint8_t I2C_SLV1_REG     = 0x29;
constexpr uint8_t I2C_SLV1_CTRL    = 0x2A;
constexpr uint8_t I2C_SLV2_ADDR    = 0x2B;
constexpr uint8_t I2C_SLV2_REG     = 0x2C;
constexpr uint8_t I2C_SLV2_CTRL    = 0x2D;
constexpr uint8_t I2C_SLV3_ADDR    = 0x2E;
constexpr uint8_t I2C_SLV3_REG     = 0x2F;
constexpr uint8_t I2C_SLV3_CTRL    = 0x30;
constexpr uint8_t I2C_SLV4_ADDR    = 0x31;
constexpr uint8_t I2C_SLV4_REG     = 0x32;
constexpr uint8_t I2C_SLV4_DO      = 0x33;
constexpr uint8_t I2C_SLV4_CTRL    = 0x34;
constexpr uint8_t I2C_SLV4_DI      = 0x35;
constexpr uint8_t I2C_MST_STATUS   = 0x36;
constexpr uint8_t INT_PIN_CFG      = 0x37;
constexpr uint8_t INT_ENABLE       = 0x38;
constexpr uint8_t DMP_INT_STATUS   = 0x39;  // Check DMP interrupt
constexpr uint8_t INT_STATUS       = 0x3A;
constexpr uint8_t ACCEL_XOUT_H     = 0x3B;
constexpr uint8_t ACCEL_XOUT_L     = 0x3C;
constexpr uint8_t ACCEL_YOUT_H     = 0x3D;
constexpr uint8_t ACCEL_YOUT_L     = 0x3E;
constexpr uint8_t ACCEL_ZOUT_H     = 0x3F;
constexpr uint8_t ACCEL_ZOUT_L     = 0x40;
constexpr uint8_t TEMP_OUT_H       = 0x41;
constexpr uint8_t TEMP_OUT_L       = 0x42;
constexpr uint8_t GYRO_XOUT_H      = 0x43;
constexpr uint8_t GYRO_XOUT_L      = 0x44;
constexpr uint8_t GYRO_YOUT_H      = 0x45;
constexpr uint8_t GYRO_YOUT_L      = 0x46;
constexpr uint8_t GYRO_ZOUT_H      = 0x47;
constexpr uint8_t GYRO_ZOUT_L      = 0x48;
constexpr uint8_t EXT_SENS_DATA_00 = 0x49;
constexpr uint8_t EXT_SENS_DATA_01 = 0x4A;
constexpr uint8_t EXT_SENS_DATA_02 = 0x4B;
constexpr uint8_t EXT_SENS_DATA_03 = 0x4C;
constexpr uint8_t EXT_SENS_DATA_04 = 0x4D;
constexpr uint8_t EXT_SENS_DATA_05 = 0x4E;
constexpr uint8_t EXT_SENS_DATA_06 = 0x4F;
constexpr uint8_t EXT_SENS_DATA_07 = 0x50;
constexpr uint8_t EXT_SENS_DATA_08 = 0x51;
constexpr uint8_t EXT_SENS_DATA_09 = 0x52;
constexpr uint8_t EXT_SENS_DATA_10 = 0x53;
constexpr uint8_t EXT_SENS_DATA_11 = 0x54;
constexpr uint8_t EXT_SENS_DATA_12 = 0x55;
constexpr uint8_t EXT_SENS_DATA_13 = 0x56;
constexpr uint8_t EXT_SENS_DATA_14 = 0x57;
constexpr uint8_t EXT_SENS_DATA_15 = 0x58;
constexpr uint8_t EXT_SENS_DATA_16 = 0x59;
constexpr uint8_t EXT_SENS_DATA_17 = 0x5A;
constexpr uint8_t EXT_SENS_DATA_18 = 0x5B;
constexpr uint8_t EXT_SENS_DATA_19 = 0x5C;
constexpr uint8_t EXT_SENS_DATA_20 = 0x5D;
constexpr uint8_t EXT_SENS_DATA_21 = 0x5E;
constexpr uint8_t EXT_SENS_DATA_22 = 0x5F;
constexpr uint8_t EXT_SENS_DATA_23 = 0x60;
constexpr uint8_t MOT_DETECT_STATUS = 0x61;
constexpr uint8_t I2C_SLV0_DO      = 0x63;
constexpr uint8_t I2C_SLV1_DO      = 0x64;
constexpr uint8_t I2C_SLV2_DO      = 0x65;
constexpr uint8_t I2C_SLV3_DO      = 0x66;
constexpr uint8_t I2C_MST_DELAY_CTRL = 0x67;
constexpr uint8_t SIGNAL_PATH_RESET  = 0x68;
constexpr uint8_t MOT_DETECT_CTRL  = 0x69;
constexpr uint8_t USER_CTRL        = 0x6A;  // Bit 7 enable DMP, bit 3 reset DMP
constexpr uint8_t PWR_MGMT_1       = 0x6B; // Device defaults to the SLEEP mode
constexpr uint8_t PWR_MGMT_2       = 0x6C;
constexpr uint8_t DMP_BANK         = 0x6D;  // Activates a specific bank in the DMP
constexpr uint8_t DMP_RW_PNT       = 0x6E;  // Set read/write pointer to a specific start address in specified DMP bank
constexpr uint8_t DMP_REG          = 0x6F;  // Register in DMP from which to read or to which to write
constexpr uint8_t DMP_REG_1        = 0x70;
constexpr uint8_t DMP_REG_2        = 0x71;
constexpr uint8_t FIFO_COUNTH      = 0x72;
constexpr uint8_t FIFO_COUNTL      = 0x73;
constexpr uint8_t FIFO_R_W         = 0x74;
constexpr uint8_t WHO_AM_I_MPU9250 = 0x75; // Should return 0x71
constexpr uint8_t XA_OFFSET_H      = 0x77;
constexpr uint8_t XA_OFFSET_L      = 0x78;
constexpr uint8_t YA_OFFSET_H      = 0x7A;
constexpr uint8_t YA_OFFSET_L      = 0x7B;
constexpr uint8_t ZA_OFFSET_H      = 0x7D;
constexpr uint8_t ZA_OFFSET_L      = 0x7E;

// EM7180 SENtral register map
// see http://www.emdeveloper.com/downloads/7180/EMSentral_EM7180_Register_Map_v1_3.pdf
//
constexpr uint8_t EM7180_QX                 = 0x00;  // this is a 32-bit normalized floating point number read from registers 0x00-03
constexpr uint8_t EM7180_QY                 = 0x04;  // this is a 32-bit normalized floating point number read from registers 0x04-07
constexpr uint8_t EM7180_QZ                 = 0x08;  // this is a 32-bit normalized floating point number read from registers 0x08-0B
constexpr uint8_t EM7180_QW                 = 0x0C;  // this is a 32-bit normalized floating point number read from registers 0x0C-0F
constexpr uint8_t EM7180_QTIME              = 0x10;  // this is a 16-bit unsigned integer read from registers 0x10-11
constexpr uint8_t EM7180_MX                 = 0x12;  // int16_t from registers 0x12-13
constexpr uint8_t EM7180_MY                 = 0x14;  // int16_t from registers 0x14-15
constexpr uint8_t EM7180_MZ                 = 0x16;  // int16_t from registers 0x16-17
constexpr uint8_t EM7180_MTIME              = 0x18;  // uint16_t from registers 0x18-19
constexpr uint8_t EM7180_AX                 = 0x1A;  // int16_t from registers 0x1A-1B
constexpr uint8_t EM7180_AY                 = 0x1C;  // int16_t from registers 0x1C-1D
constexpr uint8_t EM7180_AZ                 = 0x1E;  // int16_t from registers 0x1E-1F
constexpr uint8_t EM7180_ATIME              = 0x20;  // uint16_t from registers 0x20-21
constexpr uint8_t EM7180_GX                 = 0x22;  // int16_t from registers 0x22-23
constexpr uint8_t EM7180_GY                 = 0x24;  // int16_t from registers 0x24-25
constexpr uint8_t EM7180_GZ                 = 0x26;  // int16_t from registers 0x26-27
constexpr uint8_t EM7180_GTIME              = 0x28;  // uint16_t from registers 0x28-29
constexpr uint8_t EM7180_Baro               = 0x2A;  // start of two-byte MS5637 pressure data, 16-bit signed interger
constexpr uint8_t EM7180_BaroTIME           = 0x2C;  // start of two-byte MS5637 pressure timestamp, 16-bit unsigned
constexpr uint8_t EM7180_Temp               = 0x2E;  // start of two-byte MS5637 temperature data, 16-bit signed interger
constexpr uint8_t EM7180_TempTIME           = 0x30;  // start of two-byte MS5637 temperature timestamp, 16-bit unsigned
constexpr uint8_t EM7180_QRateDivisor       = 0x32;  // uint8_t
constexpr uint8_t EM7180_EnableEvents       = 0x33;
constexpr uint8_t EM7180_HostControl        = 0x34;
constexpr uint8_t EM7180_EventStatus        = 0x35;
constexpr uint8_t EM7180_SensorStatus       = 0x36;
constexpr uint8_t EM7180_SentralStatus      = 0x37;
constexpr uint8_t EM7180_AlgorithmStatus    = 0x38;
constexpr uint8_t EM7180_FeatureFlags       = 0x39;
constexpr uint8_t EM7180_ParamAcknowledge   = 0x3A;
constexpr uint8_t EM7180_SavedParamByte0    = 0x3B;
constexpr uint8_t EM7180_SavedParamByte1    = 0x3C;
constexpr uint8_t EM7180_SavedParamByte2    = 0x3D;
constexpr uint8_t EM7180_SavedParamByte3    = 0x3E;
constexpr uint8_t EM7180_ActualMagRate      = 0x45;
constexpr uint8_t EM7180_ActualAccelRate    = 0x46;
constexpr uint8_t EM7180_ActualGyroRate     = 0x47;
constexpr uint8_t EM7180_ActualBaroRate     = 0x48;
constexpr uint8_t EM7180_ActualTempRate     = 0x49;
constexpr uint8_t EM7180_ErrorRegister      = 0x50;
constexpr uint8_t EM7180_AlgorithmControl   = 0x54;
constexpr uint8_t EM7180_MagRate            = 0x55;
constexpr uint8_t EM7180_AccelRate          = 0x56;
constexpr uint8_t EM7180_GyroRate           = 0x57;
constexpr uint8_t EM7180_BaroRate           = 0x58;
constexpr uint8_t EM7180_TempRate           = 0x59;
constexpr uint8_t EM7180_LoadParamByte0     = 0x60;
constexpr uint8_t EM7180_LoadParamByte1     = 0x61;
constexpr uint8_t EM7180_LoadParamByte2     = 0x62;
constexpr uint8_t EM7180_LoadParamByte3     = 0x63;
constexpr uint8_t EM7180_ParamRequest       = 0x64;
constexpr uint8_t EM7180_ROMVersion1        = 0x70;
constexpr uint8_t EM7180_ROMVersion2        = 0x71;
constexpr uint8_t EM7180_RAMVersion1        = 0x72;
constexpr uint8_t EM7180_RAMVersion2        = 0x73;
constexpr uint8_t EM7180_ProductID          = 0x90;
constexpr uint8_t EM7180_RevisionID         = 0x91;
constexpr uint8_t EM7180_RunStatus          = 0x92;
constexpr uint8_t EM7180_UploadAddress      = 0x94; // uint16_t registers 0x94 (MSB)-5(LSB)
constexpr uint8_t EM7180_UploadData         = 0x96;
constexpr uint8_t EM7180_CRCHost            = 0x97;  // uint32_t from registers 0x97-9A
constexpr uint8_t EM7180_ResetRequest       = 0x9B;
constexpr uint8_t EM7180_PassThruStatus     = 0x9E;
constexpr uint8_t EM7180_PassThruControl    = 0xA0;
constexpr uint8_t EM7180_ACC_LPF_BW         = 0x5B;  //Register GP36
constexpr uint8_t EM7180_GYRO_LPF_BW        = 0x5C;  //Register GP37
constexpr uint8_t EM7180_BARO_LPF_BW        = 0x5D;  //Register GP38

constexpr uint8_t EM7180_ADDRESS            = 0x28;   // Address of the EM7180 SENtral sensor hub
constexpr uint8_t M24512DFM_DATA_ADDRESS    = 0x50;   // Address of the 500 page M24512DRC EEPROM data buffer, 1024 bits (128 8-bit bytes) per page
constexpr uint8_t M24512DFM_IDPAGE_ADDRESS  = 0x58;   // Address of the single M24512DRC lockable EEPROM ID page
constexpr uint8_t MPU9250_ADDRESS           = 0x68;   // Device address of MPU9250 when ADO = 0
constexpr uint8_t BMP280_ADDRESS            = 0x76;   // Address of BMP280 altimeter when ADO = 0


// Set initial input parameters
enum class AScale
{
    _2G = 0,
    _4G,
    _8G,
    _16G
};

enum class GScale
{
    _250DPS = 0,
    _500DPS,
    _1000DPS,
    _2000DPS
};

enum class MScale
{
    _14BITS = 0, // 0.6 mG per LSB
    _16BITS      // 0.15 mG per LSB
};

enum class POSR
{
    _00 = 0,  // no op
    _01,
    _02,
    _04,
    _08,
    _16
};

enum class TOSR
{
    _00 = 0,  // no op
    _01,
    _02,
    _04,
    _08,
    _16
};

enum class IIR_Filter
{
    Full = 0,  // bandwidth at full sample rate
    _223ODR,
    _092ODR,
    _042ODR,
    _021ODR // bandwidth at 0.021 x sample rate
};

enum class Mode
{
    BMP280Sleep = 0,
    Forced,
    Forced2,
    Normal
};

enum class SBy
{
    _00_5ms = 0,
    _62_5ms,
    _125ms,
    _250ms,
    _500ms,
    _1000ms,
    _2000ms,
    _4000ms,
};

//constexpr uint8_t ADDR_UltimateSensorFusion = 0x77;


UltimateSensorFusion::UltimateSensorFusion(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::UltimateSensorFusion_Descriptor())
    , m_config(new hal::UltimateSensorFusion_Config())
{
    m_acceleration = std::make_shared<Acceleration_Stream>();
    m_angular_velocity = std::make_shared<Angular_Velocity_Stream>();
    m_magnetic_field = std::make_shared<Magnetic_Field_Stream>();
    m_frame = std::make_shared<Frame_Stream>();
    m_pressure = std::make_shared<Pressure_Stream>();
    m_temperature = std::make_shared<Temperature_Stream>();
}

auto UltimateSensorFusion::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs;
    outputs.reserve(6);
    if (m_descriptor->get_acceleration_output_enabled())
    {
        outputs.push_back(Output{ std::string("acceleration"), m_acceleration });
    }
    if (m_descriptor->get_angular_velocity_output_enabled())
    {
        outputs.push_back(Output{ std::string("angular_velocity"), m_angular_velocity });
    }
    if (m_descriptor->get_magnetic_field_output_enabled())
    {
        outputs.push_back(Output{ std::string("magnetic_field"), m_magnetic_field });
    }
    if (m_descriptor->get_frame_output_enabled())
    {
        outputs.push_back(Output{ std::string("frame"), m_frame });
    }
    if (m_descriptor->get_pressure_output_enabled())
    {
        outputs.push_back(Output{ std::string("pressure"), m_pressure });
    }
    if (m_descriptor->get_temperature_output_enabled())
    {
        outputs.push_back(Output{ std::string("temperature"), m_temperature });
    }
    return outputs;
}
ts::Result<void> UltimateSensorFusion::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("UltimateSensorFusion::init");

    auto specialized = dynamic_cast<hal::UltimateSensorFusion_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}
ts::Result<void> UltimateSensorFusion::init()
{
    m_i2c_bus = m_hal.get_bus_registry().find_by_name<bus::II2C_Bus>(m_descriptor->get_bus());

    std::shared_ptr<bus::II2C_Bus> bus = m_i2c_bus.lock();
    if (!bus)
    {
        return make_error("No bus configured");
    }

    util::hw::II2C& i2c = bus->get_i2c();

    //    m_descriptor->pressure_rate = math::clamp<size_t>(m_descriptor->pressure_rate, 10, 100);
    //    m_descriptor->temperature_rate_ratio = math::clamp<size_t>(m_descriptor->temperature_rate_ratio, 1, 10);

    QLOGI("Probing UltimateSensorFusion on {}", m_descriptor->get_bus());

    std::this_thread::sleep_for(std::chrono::milliseconds(120));

    // Read SENtral device information
    uint16_t rom1, rom2;
    bool ok = i2c.read_register_u16(EM7180_ADDRESS, EM7180_ROMVersion1, rom1);
    ok &= i2c.read_register_u16(EM7180_ADDRESS, EM7180_ROMVersion2, rom2);

    uint16_t ram1, ram2;
    ok &= i2c.read_register_u16(EM7180_ADDRESS, EM7180_RAMVersion1, ram1);
    ok &= i2c.read_register_u16(EM7180_ADDRESS, EM7180_RAMVersion2, ram2);

    uint8_t pid;
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_ProductID, pid);

    uint8_t rid;
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_RevisionID, rid);

    if (!ok)
    {
        return make_error("UltimateSensorFusion not found!");
    }

    QLOGI("UltimateSensorFusion:");
    QLOGI("\tROM Version: {X}/{X}", rom1, rom2);
    QLOGI("\tRAM Version: {X}/{X}", ram1, ram2);
    QLOGI("\tProductID: {}", pid);
    QLOGI("\tRevisionID: {}", rid);

    std::this_thread::sleep_for(std::chrono::milliseconds(120));

    // Check which sensors can be detected by the EM7180
    uint8_t feature_flag = 0;
    if (!i2c.read_register_u8(EM7180_ADDRESS, EM7180_FeatureFlags, feature_flag))
    {
        return make_error("UltimateSensorFusion not responsive!");
    }

    if (feature_flag & 0x01) { QLOGI("\tA barometer is installed"); }
    if (feature_flag & 0x02) { QLOGI("\tA humidity sensor is installed"); }
    if (feature_flag & 0x04) { QLOGI("\tA temperature sensor is installed"); }
    if (feature_flag & 0x08) { QLOGI("\tA custom sensor is installed"); }
    if (feature_flag & 0x10) { QLOGI("\tA second custom sensor is installed"); }
    if (feature_flag & 0x20) { QLOGI("\tA third custom sensor is installed"); }

    QLOGI("\tChecking status...");
    // Check SENtral status, make sure EEPROM upload of firmware was accomplished
    uint8_t status = 0;
    Clock::time_point start = Clock::now();
    do
    {
        ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_ResetRequest, 0x01);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_SentralStatus, status);
        if (status & 0x01) { QLOGW("\tStatus: EEPROM detected on the sensor bus!"); }
        if (status & 0x02) { QLOGW("\tStatus: EEPROM uploaded config file!"); }
        if (status & 0x04) { QLOGW("\tStatus: EEPROM CRC incorrect!"); }
        if (status & 0x08) { QLOGW("\tStatus: EM7180 in initialized state!"); }
        if (status & 0x10) { QLOGW("\tStatus: No EEPROM detected!"); }
    }
    while ((status & 0x01) == 0 && ok && Clock::now() - start < std::chrono::seconds(3));

    if ((status & 0x01) == 0 || //eeprom not detected
            (status & 0x04) != 0 || //crc error
            !ok) //i2c error
    {
        QLOGI("Initialization failed: {}", status);
        return make_error("Failed to initialize UltimateSensorFusion ({})!", status);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_HostControl, 0x00); // set SENtral in initialized state to configure registers
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_PassThruControl, 0x00); // make sure pass through mode is off
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_HostControl, 0x01); // Force initialize
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_HostControl, 0x00); // set SENtral in initialized state to configure registers

    //Setup LPF bandwidth (BEFORE setting ODR's)
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_ACC_LPF_BW, (uint8_t)IIR_Filter::_042ODR);
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_GYRO_LPF_BW, (uint8_t)IIR_Filter::_042ODR);
    // Set accel/gyro/mage desired ODR rates
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_QRateDivisor, m_descriptor->get_frame_rate_divisor()); // Gyro/X Hz
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_MagRate, m_descriptor->get_magnetometer_rate()); // X Hz
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_AccelRate, (uint8_t)(uint32_t(m_descriptor->get_imu_rate()) / 10)); // X/10 Hz
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_GyroRate, (uint8_t)(uint32_t(m_descriptor->get_imu_rate()) / 10)); // X/10 Hz
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_BaroRate, 0x80 | m_descriptor->get_barometer_thermometer_rate());  // set enable bit and set Baro rate to 25 Hz
    // ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_TempRate, 0x19);  // set enable bit and set rate to 25 Hz

    // Configure operating mode
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // read scale sensor data
    // Enable interrupt to host upon certain events
    // choose host interrupts when any sensor updated (0x40), new gyro data (0x20), new accel data (0x10),
    // new mag data (0x08), quaternions updated (0x04), an error occurs (0x02), or the SENtral needs to be reset(0x01)
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_EnableEvents, 0x07);
    // Enable EM7180 run mode
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_HostControl, 0x01); // set SENtral in normal run mode

    //Disable stillness mode
    ok &= EM7180_set_integer_param(i2c, 0x49, 0x00);
    if (!ok)
    {
        return make_error("Failed to configure UltimateSensorFusion!");
    }

    //Write desired sensor full scale ranges to the EM7180
    ok &= EM7180_set_mag_acc_FS(i2c, 1000, 8); // 1000 uT, 8 g
    ok &= EM7180_set_gyro_FS(i2c, 2000); // 2000 dps
    if (!ok)
    {
        return make_error("Failed to set FS for UltimateSensorFusion!");
    }

    // Read EM7180 status
    uint8_t run_status = 0;
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_RunStatus, run_status);
    if (run_status & 0x01) { QLOGI("\tEM7180 run status = normal mode"); }

    uint8_t algo_status = 0;
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_AlgorithmStatus, algo_status);
    if (algo_status & 0x01) { QLOGI("\tEM7180 standby status"); }
    if (algo_status & 0x02) { QLOGI("\tEM7180 algorithm slow"); }
    if (algo_status & 0x04) { QLOGI("\tEM7180 in stillness mode"); }
    if (algo_status & 0x08) { QLOGI("\tEM7180 mag calibration completed"); }
    if (algo_status & 0x10) { QLOGI("\tEM7180 magnetic anomaly detected"); }
    if (algo_status & 0x20) { QLOGI("\tEM7180 unreliable sensor data"); }

    uint8_t passthru_status = 0;
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_PassThruStatus, passthru_status);
    if (passthru_status & 0x01) { QLOGI("\tEM7180 in passthru mode!"); }

    uint8_t event_status = 0;
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_EventStatus, event_status);
    if (event_status & 0x01) { QLOGI("\tEM7180 CPU reset"); }
    if (event_status & 0x02) { QLOGI("\tEM7180 Error"); }
    if (event_status & 0x04) { QLOGI("\tEM7180 new quaternion result"); }
    if (event_status & 0x08) { QLOGI("\tEM7180 new mag result"); }
    if (event_status & 0x10) { QLOGI("\tEM7180 new accel result"); }
    if (event_status & 0x20) { QLOGI("\tEM7180 new gyro result"); }

    // Check sensor status
    uint8_t sensor_status = 0;
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_SensorStatus, sensor_status);
    QLOGI("\tEM7180 sensor status: {}", sensor_status);
    if (sensor_status & 0x01) { QLOGI("\t\tMagnetometer not acknowledging!"); }
    if (sensor_status & 0x02) { QLOGI("\t\tAccelerometer not acknowledging!"); }
    if (sensor_status & 0x04) { QLOGI("\t\tGyro not acknowledging!"); }
    if (sensor_status & 0x10) { QLOGI("\t\tMagnetometer ID not recognized!"); }
    if (sensor_status & 0x20) { QLOGI("\t\tAccelerometer ID not recognized!"); }
    if (sensor_status & 0x40) { QLOGI("\t\tGyro ID not recognized!"); }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    uint8_t rate = 0;
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_ActualAccelRate, rate);
    QLOGI("\tAccelerometer Rate: {}", rate * 10);
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_ActualGyroRate, rate);
    QLOGI("\tGyroscope Rate: {}", rate * 10);
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_ActualMagRate, rate);
    QLOGI("\tMagnetometer Rate: {}", rate);
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_ActualBaroRate, rate);
    QLOGI("\tBarometer Rate: {}", rate);

    if (!ok)
    {
        return make_error("Failed finalize setting up the UltimateSensorFusion!");
    }

    m_acceleration->set_rate((uint32_t)m_descriptor->get_imu_rate() / m_descriptor->get_imu_output_rate_divisor());
    m_angular_velocity->set_rate((uint32_t)m_descriptor->get_imu_rate() / m_descriptor->get_imu_output_rate_divisor());
    m_magnetic_field->set_rate(m_descriptor->get_magnetometer_rate());
    m_frame->set_rate((uint32_t)m_descriptor->get_imu_rate() / m_descriptor->get_frame_rate_divisor());
    m_pressure->set_rate(m_descriptor->get_barometer_thermometer_rate());
    m_temperature->set_rate(m_descriptor->get_barometer_thermometer_rate());

    m_min_dt = std::min(m_acceleration->get_dt(), m_angular_velocity->get_dt());
    m_min_dt = std::min(m_min_dt, m_magnetic_field->get_dt());
    m_min_dt = std::min(m_min_dt, m_frame->get_dt());
    m_min_dt = std::min(m_min_dt, m_pressure->get_dt());
    m_min_dt = std::min(m_min_dt, m_temperature->get_dt());

    m_min_dt /= 2;

    return ts::success;
}

bool UltimateSensorFusion::EM7180_set_gyro_FS(util::hw::II2C& i2c, uint16_t gyro_fs)
{
    uint8_t bytes[4], STAT;
    bytes[0] = gyro_fs & (0xFF);
    bytes[1] = (gyro_fs >> 8) & (0xFF);
    bytes[2] = 0x00;
    bytes[3] = 0x00;
    bool ok = i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte0, bytes[0]); //Gyro LSB
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte1, bytes[1]); //Gyro MSB
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte2, bytes[2]); //Unused
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte3, bytes[3]); //Unused
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_ParamRequest, 0xCB); //Parameter 75; 0xCB is 75 decimal with the MSB set high to indicate a paramter write processs
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x80); //Request parameter transfer procedure
    if (!ok)
    {
        return false;
    }

    Clock::time_point start = Clock::now();
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_ParamAcknowledge, STAT); //Check the parameter acknowledge register and loop until the result matches parameter request byte
    while (!(STAT==0xCB) && ok)
    {
        ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_ParamAcknowledge, STAT); //Check the parameter acknowledge register and loop until the result matches parameter request byte
        if (Clock::now() - start > std::chrono::seconds(1))
        {
            return false;
        }
    }
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_ParamRequest, 0x00); //Parameter request = 0 to end parameter transfer process
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // Re-start algorithm

    return ok;
}

bool UltimateSensorFusion::EM7180_set_mag_acc_FS(util::hw::II2C& i2c, uint16_t mag_fs, uint16_t acc_fs)
{
    uint8_t bytes[4], STAT;
    bytes[0] = mag_fs & (0xFF);
    bytes[1] = (mag_fs >> 8) & (0xFF);
    bytes[2] = acc_fs & (0xFF);
    bytes[3] = (acc_fs >> 8) & (0xFF);
    bool ok = i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte0, bytes[0]); //Mag LSB
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte1, bytes[1]); //Mag MSB
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte2, bytes[2]); //Acc LSB
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte3, bytes[3]); //Acc MSB
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_ParamRequest, 0xCA); //Parameter 74; 0xCA is 74 decimal with the MSB set high to indicate a paramter write processs
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x80); //Request parameter transfer procedure
    if (!ok)
    {
        return false;
    }

    Clock::time_point start = Clock::now();
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_ParamAcknowledge, STAT); //Check the parameter acknowledge register and loop until the result matches parameter request byte
    while (!(STAT==0xCA) && ok)
    {
        ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_ParamAcknowledge, STAT); //Check the parameter acknowledge register and loop until the result matches parameter request byte
        if (Clock::now() - start > std::chrono::seconds(1))
        {
            return false;
        }
    }
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_ParamRequest, 0x00); //Parameter request = 0 to end parameter transfer process
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // Re-start algorithm

    return ok;
}

bool UltimateSensorFusion::EM7180_set_integer_param(util::hw::II2C& i2c, uint8_t param, uint32_t param_val)
{
    uint8_t bytes[4], STAT;
    bytes[0] = param_val & (0xFF);
    bytes[1] = (param_val >> 8) & (0xFF);
    bytes[2] = (param_val >> 16) & (0xFF);
    bytes[3] = (param_val >> 24) & (0xFF);
    param = param | 0x80; //Parameter is the decimal value with the MSB set high to indicate a paramter write processs
    bool ok = i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte0, bytes[0]); //Param LSB
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte1, bytes[1]);
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte2, bytes[2]);
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte3, bytes[3]); //Param MSB
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_ParamRequest, param);
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x80); //Request parameter transfer procedure
    if (!ok)
    {
        return false;
    }

    Clock::time_point start = Clock::now();
    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_ParamAcknowledge, STAT); //Check the parameter acknowledge register and loop until the result matches parameter request byte
    while (STAT != param && ok)
    {
        ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_ParamAcknowledge, STAT); //Check the parameter acknowledge register and loop until the result matches parameter request byte
        if (Clock::now() - start > std::chrono::seconds(3))
        {
            return false;
        }
    }
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_ParamRequest, 0x00); //Parameter request = 0 to end parameter transfer process
    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // Re-start algorithm

    return ok;
}

//bool UltimateSensorFusion::EM7180_set_float_param(util::hw::II2C& i2c, uint8_t param, float param_val)
//{
//    uint8_t bytes[4], STAT;
//    float_to_bytes(param_val, &bytes[0]);
//    param = param | 0x80; //Parameter is the decimal value with the MSB set high to indicate a paramter write processs
//    bool ok = i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte0, bytes[0]); //Param LSB
//    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte1, bytes[1]);
//    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte2, bytes[2]);
//    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_LoadParamByte3, bytes[3]); //Param MSB
//    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_ParamRequest, param);
//    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x80); //Request parameter transfer procedure
//    if (!ok)
//    {
//        return false;
//    }

//    Clock::time_point start = Clock::now();
//    ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_ParamAcknowledge, STAT); //Check the parameter acknowledge register and loop until the result matches parameter request byte
//    while (!(STAT==param) && ok)
//    {
//        ok &= i2c.read_register_u8(EM7180_ADDRESS, EM7180_ParamAcknowledge, STAT); //Check the parameter acknowledge register and loop until the result matches parameter request byte
//        if (Clock::now() - start > std::chrono::seconds(1))
//        {
//            return false;
//        }
//    }
//    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_ParamRequest, 0x00); //Parameter request = 0 to end parameter transfer process
//    ok &= i2c.write_register_u8(EM7180_ADDRESS, EM7180_AlgorithmControl, 0x00); // Re-start algorithm

//    return ok;
//}

ts::Result<void> UltimateSensorFusion::start(Clock::time_point tp)
{
    m_acceleration->set_tp(tp);
    m_angular_velocity->set_tp(tp);
    m_magnetic_field->set_tp(tp);
    m_frame->set_tp(tp);
    m_pressure->set_tp(tp);
    m_temperature->set_tp(tp);

    //m_last_temperature_reading_tp = tp;
    //m_last_pressure_reading_tp = tp;
    //m_pressure->set_tp(tp);
    //m_temperature->set_tp(tp);
    return ts::success;
}

void UltimateSensorFusion::process()
{
    m_acceleration->clear();
    m_angular_velocity->clear();
    m_magnetic_field->clear();
    m_frame->clear();
    m_pressure->clear();
    m_temperature->clear();

    auto now = Clock::now();
    if (now - m_last_process_tp < m_min_dt)
    {
        return;
    }
    m_last_process_tp = now;

    std::shared_ptr<bus::II2C_Bus> bus = m_i2c_bus.lock();
    if (!bus)
    {
        return;
    }
    util::hw::II2C& i2c = bus->get_i2c();

    QLOG_TOPIC("UltimateSensorFusion::process");

    // Check event status register, way to chech data ready by polling rather than interrupt
    uint8_t event_status = 0;
    if (!i2c.read_register_u8(EM7180_ADDRESS, EM7180_EventStatus, event_status)) // reading clears the register
    {
        return;
    }

    // Check for errors
    if (event_status & 0x02) // error detected, what is it?
    {
        uint8_t error_status = 0;
        if (!i2c.read_register_u8(EM7180_ADDRESS, EM7180_ErrorRegister, error_status)) // reading clears the register
        {
            return;
        }
        if (error_status != 0x00) // non-zero value indicates error, what is it?
        {
            QLOGW("EM7180 error status = {}", error_status);
            //            if (error_status == 0x11) Serial.print("Magnetometer failure!");
            //            if (error_status == 0x12) Serial.print("Accelerometer failure!");
            //            if (error_status == 0x14) Serial.print("Gyro failure!");
            //            if (error_status == 0x21) Serial.print("Magnetometer initialization failure!");
            //            if (error_status == 0x22) Serial.print("Accelerometer initialization failure!");
            //            if (error_status == 0x24) Serial.print("Gyro initialization failure!");
            //            if (error_status == 0x30) Serial.print("Math error!");
            //            if (error_status == 0x80) Serial.print("Invalid sample rate!");
        }
        return;
    }

    constexpr size_t k_max_sample_difference = 3;

    // if no errors, see if new data is ready
    if (m_descriptor->get_acceleration_output_enabled() && (event_status & 0x10))  // new acceleration data available
    {
        now = Clock::now();
        Clock::duration stream_dt = m_acceleration->get_dt();
        if (now - m_acceleration->last_process_tp >= m_acceleration->get_dt())
        {
            m_acceleration->last_process_tp = now;

            if (m_acceleration->get_tp() > now + stream_dt*k_max_sample_difference)
            {
                //skipped
            }
            else
            {
                uint8_t raw_data[6];  // x/y/z accel register data stored here
                if (i2c.read_register(EM7180_ADDRESS, EM7180_AX, raw_data, 6))       // Read the six raw data registers into data array
                {
                    int16_t qvalue[3];
                    qvalue[0] = (int16_t) (((int16_t)raw_data[1] << 8) | raw_data[0]);  // Turn the MSB and LSB into a signed 16-bit value
                    qvalue[1] = (int16_t) (((int16_t)raw_data[3] << 8) | raw_data[2]);
                    qvalue[2] = (int16_t) (((int16_t)raw_data[5] << 8) | raw_data[4]);

                    math::vec3f value((float)qvalue[0]*(0.000488f*physics::constants::g),
                            (float)qvalue[1]*(0.000488f*physics::constants::g),
                            (float)qvalue[2]*(0.000488f*physics::constants::g));

                    value = math::transform(m_rotation_matrix, value);
                    m_acceleration->push_sample(value, true);
                }
            }
        }
    }

    if (m_descriptor->get_angular_velocity_output_enabled() && (event_status & 0x20)) // new gyro data available
    {
        now = Clock::now();
        Clock::duration stream_dt = m_angular_velocity->get_dt();
        if (now - m_angular_velocity->last_process_tp >= m_angular_velocity->get_dt())
        {
            m_angular_velocity->last_process_tp = now;

            if (m_angular_velocity->get_tp() > now + stream_dt*k_max_sample_difference)
            {
                //skipped
            }
            else
            {
                uint8_t raw_data[6];  // x/y/z gyro register data stored here
                if (i2c.read_register(EM7180_ADDRESS, EM7180_GX, raw_data, 6))       // Read the six raw data registers into data array
                {
                    int16_t qvalue[3];
                    qvalue[0] = (int16_t) (((int16_t)raw_data[1] << 8) | raw_data[0]);  // Turn the MSB and LSB into a signed 16-bit value
                    qvalue[1] = (int16_t) (((int16_t)raw_data[3] << 8) | raw_data[2]);
                    qvalue[2] = (int16_t) (((int16_t)raw_data[5] << 8) | raw_data[4]);

                    math::vec3f value((float)qvalue[0]*(0.153f*0.0174533f),
                            (float)qvalue[1]*(0.153f*0.0174533f),
                            (float)qvalue[2]*(0.153f*0.0174533f));

                    value = math::transform(m_rotation_matrix, value);
                    m_angular_velocity->push_sample(value, true);
                }
            }
        }
    }

    if (m_descriptor->get_magnetic_field_output_enabled() && (event_status & 0x08)) // new mag data available
    {
        now = Clock::now();
        Clock::duration stream_dt = m_magnetic_field->get_dt();
        if (m_magnetic_field->get_tp() > now + stream_dt*k_max_sample_difference)
        {
            //skipped
        }
        else
        {
            uint8_t raw_data[6];  // x/y/z gyro register data stored here
            if (i2c.read_register(EM7180_ADDRESS, EM7180_MX, raw_data, 6))       // Read the six raw data registers into data array
            {
                int16_t qvalue[3];
                qvalue[0] = (int16_t) (((int16_t)raw_data[1] << 8) | raw_data[0]);  // Turn the MSB and LSB into a signed 16-bit value
                qvalue[1] = (int16_t) (((int16_t)raw_data[3] << 8) | raw_data[2]);
                qvalue[2] = (int16_t) (((int16_t)raw_data[5] << 8) | raw_data[4]);

                math::vec3f value((float)qvalue[0]*0.305176f,
                        (float)qvalue[1]*0.305176f,
                        (float)qvalue[2]*0.305176f);

                value = math::transform(m_rotation_matrix, value);
                m_magnetic_field->push_sample(value, true);
            }
        }
    }

    if (m_descriptor->get_frame_output_enabled() && (event_status & 0x04))  // new quaternion data available
    {
        now = Clock::now();
        Clock::duration stream_dt = m_frame->get_dt();
        if (m_frame->get_tp() > now + stream_dt*k_max_sample_difference)
        {
            //skipped
        }
        else
        {
            math::quatf data;  // x/y/z quaternion register data stored here
            if (i2c.read_register(EM7180_ADDRESS, EM7180_QX, reinterpret_cast<uint8_t*>(&data), 16))       // Read the six raw data registers into data array
            {
                data = data * m_rotation_quaternion; //Why are they in this order?! No one knows...
                m_frame->push_sample(data, true);
            }
        }
    }

    // get BMP280 pressure
    if (event_status & 0x40)  // new baro/temp data available
    {
        uint8_t raw_data[2];

        if (m_descriptor->get_pressure_output_enabled())
        {
            now = Clock::now();
            Clock::duration stream_dt = m_pressure->get_dt();
            if (m_pressure->get_tp() > now + stream_dt*k_max_sample_difference)
            {
                //skipped
            }
            else
            {
                if (i2c.read_register(EM7180_ADDRESS, EM7180_Baro, raw_data, 2))       // Read the six raw data registers into data array
                {
                    int16_t qvalue = (int16_t) (((int16_t)raw_data[1] << 8) | raw_data[0]);  // Turn the MSB and LSB into a signed 16-bit value
                    float value = (float)qvalue*0.01f + 1013.25f;
                    m_pressure->push_sample(value, true);
                }
            }
        }

        if (m_descriptor->get_temperature_output_enabled())
        {
            now = Clock::now();
            Clock::duration stream_dt = m_temperature->get_dt();
            if (m_temperature->get_tp() > now + stream_dt*k_max_sample_difference)
            {
                //skipped
            }
            else
            {
                if (i2c.read_register(EM7180_ADDRESS, EM7180_Temp, raw_data, 2))       // Read the six raw data registers into data array
                {
                    int16_t qvalue = (int16_t) (((int16_t)raw_data[1] << 8) | raw_data[0]);  // Turn the MSB and LSB into a signed 16-bit value
                    float value = (float)qvalue*0.01;
                    m_temperature->push_sample(value, true);
                }
            }
        }
    }


    //handle and report the slow clock
    {
        constexpr size_t MAX_SKIPPED_SAMPLES = 8;
        if (m_descriptor->get_acceleration_output_enabled())
        {
            Clock::duration dt = m_acceleration->get_dt();
            if (m_acceleration->get_tp() <= now - dt * MAX_SKIPPED_SAMPLES)
            {
                while (m_acceleration->get_tp() <= now - dt)
                {
                    m_acceleration->push_last_sample(false);
                    //m_stats.acc.added++;
                }
            }
        }
        if (m_descriptor->get_angular_velocity_output_enabled())
        {
            Clock::duration dt = m_angular_velocity->get_dt();
            if (m_angular_velocity->get_tp() <= now - dt * MAX_SKIPPED_SAMPLES)
            {
                while (m_angular_velocity->get_tp() <= now - dt)
                {
                    m_angular_velocity->push_sample(math::vec3f::zero, false);
                    //m_stats.av.added++;
                }
            }
        }
        if (m_descriptor->get_magnetic_field_output_enabled())
        {
            Clock::duration dt = m_magnetic_field->get_dt();
            if (m_magnetic_field->get_tp() <= now - dt * MAX_SKIPPED_SAMPLES)
            {
                while (m_magnetic_field->get_tp() <= now - dt)
                {
                    m_magnetic_field->push_last_sample(false);
                    //m_stats.av.added++;
                }
            }
        }
        if (m_descriptor->get_frame_output_enabled())
        {
            Clock::duration dt = m_frame->get_dt();
            if (m_frame->get_tp() <= now - dt * MAX_SKIPPED_SAMPLES)
            {
                while (m_frame->get_tp() <= now - dt)
                {
                    m_frame->push_last_sample(false);
                    //m_stats.av.added++;
                }
            }
        }
        if (m_descriptor->get_pressure_output_enabled())
        {
            Clock::duration dt = m_pressure->get_dt();
            if (m_pressure->get_tp() <= now - dt * MAX_SKIPPED_SAMPLES)
            {
                while (m_pressure->get_tp() <= now - dt)
                {
                    m_pressure->push_last_sample(false);
                    //m_stats.av.added++;
                }
            }
        }
        if (m_descriptor->get_temperature_output_enabled())
        {
            Clock::duration dt = m_temperature->get_dt();
            if (m_temperature->get_tp() <= now - dt * MAX_SKIPPED_SAMPLES)
            {
                while (m_temperature->get_tp() <= now - dt)
                {
                    m_temperature->push_last_sample(false);
                    //m_stats.av.added++;
                }
            }
        }
    }

}

ts::Result<void> UltimateSensorFusion::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("UltimateSensorFusion::set_config");

    auto specialized = dynamic_cast<hal::UltimateSensorFusion_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    //compute chip rotation
    m_rotation_quaternion.set_from_euler_xyz(math::radians(m_config->get_rotation()));

    m_rotation_matrix = m_rotation_quaternion.get_as_mat3();
    m_rotation_matrix = math::transposed(m_rotation_matrix);

//    if (m_config->get_flip_x())
//    {
//        //m_rotation_matrix.set_axis_x(-m_rotation_matrix.get_axis_x());
//        math::mat3f scale;
//        scale.set_scale(math::vec3f(-1.f, 1.f, 1.f));
//        m_rotation_matrix = scale * m_rotation_matrix;
//    }
//    if (m_config->get_flip_y())
//    {
//        //m_rotation_matrix.set_axis_y(-m_rotation_matrix.get_axis_y());
//        math::mat3f scale;
//        scale.set_scale(math::vec3f(1.f, -1.f, 1.f));
//        m_rotation_matrix = scale * m_rotation_matrix;
//    }
//    if (m_config->get_flip_z())
//    {
////        m_rotation_matrix.set_axis_z(-m_rotation_matrix.get_axis_z());
//        math::mat3f scale;
//        scale.set_scale(math::vec3f(1.f, 1.f, -1.f));
//        m_rotation_matrix = scale * m_rotation_matrix;
//    }

//    m_rotation_quaternion.set_from_mat3(m_rotation_matrix);

    QLOGI("Rotation matrix: \n{}\n{}\n{}", m_rotation_matrix.get_axis_x(), m_rotation_matrix.get_axis_y(), m_rotation_matrix.get_axis_z());
    QLOGI("Rotation quaternion: {}", m_rotation_quaternion);

    return ts::success;
}
auto UltimateSensorFusion::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto UltimateSensorFusion::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

ts::Result<std::shared_ptr<messages::INode_Message>> UltimateSensorFusion::send_message(messages::INode_Message const& message)
{
    return make_error("Unknown message");
}

}
}
