#include "BrainStdAfx.h"
#include "MPU9250.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "hal.def.h"
//#include "sz_Calibration_Data.hpp"
//#include "sz_MPU9250.hpp"

#define USE_AK8963

namespace silk
{
namespace node
{

constexpr uint8_t ADDR_MPU9250 = 0x68;


// mpu9250 registers
constexpr uint8_t MPU_REG_SELF_TEST_X_GYRO               = 0x00;
constexpr uint8_t MPU_REG_SELF_TEST_Y_GYRO               = 0x01;
constexpr uint8_t MPU_REG_SELF_TEST_Z_GYRO               = 0x02;
constexpr uint8_t MPU_REG_SELF_TEST_X_ACCEL              = 0x0D;
constexpr uint8_t MPU_REG_SELF_TEST_Y_ACCEL              = 0x0E;
constexpr uint8_t MPU_REG_SELF_TEST_Z_ACCEL              = 0x0F;
constexpr uint8_t MPU_REG_XG_OFFSET_H                    = 0x13;
constexpr uint8_t MPU_REG_XG_OFFSET_L                    = 0x14;
constexpr uint8_t MPU_REG_YG_OFFSET_H                    = 0x15;
constexpr uint8_t MPU_REG_YG_OFFSET_L                    = 0x16;
constexpr uint8_t MPU_REG_ZG_OFFSET_H                    = 0x17;
constexpr uint8_t MPU_REG_ZG_OFFSET_L                    = 0x18;
constexpr uint8_t MPU_REG_SMPLRT_DIV                     = 0x19;
constexpr uint8_t MPU_REG_CONFIG                         = 0x1A;
    constexpr uint8_t MPU_BIT_CONFIG_FIFO_MODE           = 1 << 6;
    constexpr uint8_t MPU_BIT_CONFIG_EXT_SYNC_SET        = 1 << 3;
    constexpr uint8_t MPU_BIT_DLPF_CFG_250_8             = 0;
    constexpr uint8_t MPU_BIT_DLPF_CFG_184_1             = 1;
    constexpr uint8_t MPU_BIT_DLPF_CFG_92_1              = 2;
    constexpr uint8_t MPU_BIT_DLPF_CFG_41_1              = 3;
    constexpr uint8_t MPU_BIT_DLPF_CFG_20_1              = 4;
    constexpr uint8_t MPU_BIT_DLPF_CFG_10_1              = 5;
    constexpr uint8_t MPU_BIT_DLPF_CFG_5_1               = 6;
    constexpr uint8_t MPU_BIT_DLPF_CFG_3600_8            = 7;

constexpr uint8_t MPU_REG_GYRO_CONFIG                    = 0x1B;
    constexpr uint8_t MPU_BIT_XGYRO_CT_EN                = 1 << 7;
    constexpr uint8_t MPU_BIT_YGYRO_CT_EN                = 1 << 6;
    constexpr uint8_t MPU_BIT_ZGYRO_CT_EN                = 1 << 5;
    constexpr uint8_t MPU_BIT_GYRO_FS_SEL_250_DPS        = 0 << 3;
    constexpr uint8_t MPU_BIT_GYRO_FS_SEL_500_DPS        = 1 << 3;
    constexpr uint8_t MPU_BIT_GYRO_FS_SEL_1000_DPS       = 2 << 3;
    constexpr uint8_t MPU_BIT_GYRO_FS_SEL_2000_DPS       = 3 << 3;
    constexpr uint8_t MPU_BIT_FCHOICE_B                  = 1 << 0;

constexpr uint8_t MPU_REG_ACCEL_CONFIG                   = 0x1C;
    constexpr uint8_t MPU_BIT_AX_ST_EX                   = 1 << 7;
    constexpr uint8_t MPU_BIT_AY_ST_EX                   = 1 << 6;
    constexpr uint8_t MPU_BIT_AZ_ST_EX                   = 1 << 5;
    constexpr uint8_t MPU_BIT_ACCEL_FS_SEL_2_G           = 0 << 3;
    constexpr uint8_t MPU_BIT_ACCEL_FS_SEL_4_G           = 1 << 3;
    constexpr uint8_t MPU_BIT_ACCEL_FS_SEL_8_G           = 2 << 3;
    constexpr uint8_t MPU_BIT_ACCEL_FS_SEL_16_G          = 3 << 3;

constexpr uint8_t MPU_REG_ACCEL_CONFIG2                  = 0x1D;
    constexpr uint8_t MPU_BIT_ACCEL_FCHOICE_B            = 1 << 2;
    constexpr uint8_t MPU_BIT_A_DLPF_CFG_460_1           = 0;
    constexpr uint8_t MPU_BIT_A_DLPF_CFG_184_1           = 1;
    constexpr uint8_t MPU_BIT_A_DLPF_CFG_92_1            = 2;
    constexpr uint8_t MPU_BIT_A_DLPF_CFG_41_1            = 3;
    constexpr uint8_t MPU_BIT_A_DLPF_CFG_20_1            = 4;
    constexpr uint8_t MPU_BIT_A_DLPF_CFG_10_1            = 5;
    constexpr uint8_t MPU_BIT_A_DLPF_CFG_5_1             = 6;
    //constexpr uint8_t MPU_BIT_A_DLPF_CFG_460_1           = 7;
    constexpr uint8_t MPU_BIT_FIFO_SIZE_1024             = 0x40;
    constexpr uint8_t MPU_BIT_FIFO_SIZE_2048             = 0x80;
    constexpr uint8_t MPU_BIT_FIFO_SIZE_4096             = 0xC0;

constexpr uint8_t MPU_REG_LP_ACCEL_ODR                   = 0x1E;
    constexpr uint8_t MPU_BIT_LPOSC_CLKSEL               = 1 << 0;

constexpr uint8_t MPU_REG_FIFO_EN                        = 0x23;
    constexpr uint8_t MPU_BIT_TEMP_FIFO_EN               = 1 << 7;
    constexpr uint8_t MPU_BIT_GYRO_XO_UT                 = 1 << 6;
    constexpr uint8_t MPU_BIT_GYRO_YO_UT                 = 1 << 5;
    constexpr uint8_t MPU_BIT_GYRO_ZO_UT                 = 1 << 4;
    constexpr uint8_t MPU_BIT_ACCEL                      = 1 << 3;
    constexpr uint8_t MPU_BIT_SLV2                       = 1 << 2;
    constexpr uint8_t MPU_BIT_SLV1                       = 1 << 1;
    constexpr uint8_t MPU_BIT_SLV0                       = 1 << 0;

constexpr uint8_t MPU_REG_I2C_MST_CTRL                   = 0x24;
    constexpr uint8_t MPU_BIT_MULT_MST_EN                = 1 << 7;
    constexpr uint8_t MPU_BIT_WAIT_FOR_ES                = 1 << 6;
    constexpr uint8_t MPU_BIT_SLV_3_FIFO_EN              = 1 << 5;
    constexpr uint8_t MPU_BIT_I2C_MST_P_NSR              = 1 << 4;
    constexpr uint8_t MPU_BIT_I2C_MST_CLK                = 0;

constexpr uint8_t MPU_REG_I2C_SLV0_ADDR                  = 0x25;
constexpr uint8_t MPU_REG_I2C_SLV0_REG                   = 0x26;
constexpr uint8_t MPU_REG_I2C_SLV0_CTRL                  = 0x27;
    constexpr uint8_t MPU_BIT_I2C_SLV0_EN                = 0x80;

constexpr uint8_t MPU_REG_I2C_SLV1_ADDR                  = 0x28;
constexpr uint8_t MPU_REG_I2C_SLV1_REG                   = 0x29;
constexpr uint8_t MPU_REG_I2C_SLV1_CTRL                  = 0x2A;
    constexpr uint8_t MPU_BIT_I2C_SLV1_EN                = 0x80;

constexpr uint8_t MPU_REG_I2C_SLV2_ADDR                  = 0x2B;
constexpr uint8_t MPU_REG_I2C_SLV2_REG                   = 0x2C;
constexpr uint8_t MPU_REG_I2C_SLV2_CTRL                  = 0x2D;
    constexpr uint8_t MPU_BIT_I2C_SLV2_EN                = 0x80;

constexpr uint8_t MPU_REG_I2C_SLV3_ADDR                  = 0x2E;
constexpr uint8_t MPU_REG_I2C_SLV3_REG                   = 0x2F;
constexpr uint8_t MPU_REG_I2C_SLV3_CTRL                  = 0x30;
    constexpr uint8_t MPU_BIT_I2C_SLV3_EN                = 0x80;

constexpr uint8_t MPU_REG_I2C_SLV4_ADDR                  = 0x31;
constexpr uint8_t MPU_REG_I2C_SLV4_REG                   = 0x32;
constexpr uint8_t MPU_REG_I2C_SLV4_DO                    = 0x33;
constexpr uint8_t MPU_REG_I2C_SLV4_CTRL                  = 0x34;
    constexpr uint8_t MPU_BIT_I2C_SLV4_EN                = 0x80;
    constexpr uint8_t MPU_BIT_I2C_MST_DLY                = 0; //4 bits

constexpr uint8_t MPU_REG_INT_PIN_CFG                    = 0x37;
    constexpr uint8_t MPU_BIT_ACTL                       = 1 << 7;
    constexpr uint8_t MPU_BIT_OPEN                       = 1 << 6;
    constexpr uint8_t MPU_BIT_LATCH_INT_EN               = 1 << 5;
    constexpr uint8_t MPU_BIT_INT_ANYRD_2CLEAR           = 1 << 4;
    constexpr uint8_t MPU_BIT_ACTL_FSYNC                 = 1 << 3;
    constexpr uint8_t MPU_BIT_FSYNC_INT_MODE_EN          = 1 << 2;
    constexpr uint8_t MPU_BIT_BYPASS_EN                  = 1 << 1;

constexpr uint8_t MPU_REG_INT_ENABLE                     = 0x38;
    constexpr uint8_t MPU_BIT_WOM_EN                     = 1 << 6;
    constexpr uint8_t MPU_BIT_FIFO_OFLOW_EN              = 1 << 4;
    constexpr uint8_t MPU_BIT_FSYNC_INT_EN               = 1 << 3;
    constexpr uint8_t MPU_BIT_RAW_RDY_EN                 = 1 << 0;

constexpr uint8_t MPU_REG_INT_STATUS                     = 0x3A;
    constexpr uint8_t MPU_BIT_WOM_INT                    = 1 << 6;
    constexpr uint8_t MPU_BIT_FIFO_OFLOW_INT             = 1 << 4;
    constexpr uint8_t MPU_BIT_FSYNC_INT                  = 1 << 3;
    constexpr uint8_t MPU_BIT_RAW_DATA_RDY_INT           = 1 << 0;

constexpr uint8_t MPU_REG_ACCEL_XOUT_H                   = 0x3B;
constexpr uint8_t MPU_REG_ACCEL_XOUT_L                   = 0x3C;
constexpr uint8_t MPU_REG_ACCEL_YOUT_H                   = 0x3D;
constexpr uint8_t MPU_REG_ACCEL_YOUT_L                   = 0x3E;
constexpr uint8_t MPU_REG_ACCEL_ZOUT_H                   = 0x3F;
constexpr uint8_t MPU_REG_ACCEL_ZOUT_L                   = 0x40;
constexpr uint8_t MPU_REG_TEMP_OUT_H                     = 0x41;
constexpr uint8_t MPU_REG_TEMP_OUT_L                     = 0x42;
constexpr uint8_t MPU_REG_GYRO_XOUT_H                    = 0x43;
constexpr uint8_t MPU_REG_GYRO_XOUT_L                    = 0x44;
constexpr uint8_t MPU_REG_GYRO_YOUT_H                    = 0x45;
constexpr uint8_t MPU_REG_GYRO_YOUT_L                    = 0x46;
constexpr uint8_t MPU_REG_GYRO_ZOUT_H                    = 0x47;
constexpr uint8_t MPU_REG_GYRO_ZOUT_L                    = 0x48;
constexpr uint8_t MPU_REG_RAW_COMPASS                    = 0x49;

constexpr uint8_t MPU_REG_EXT_SENS_DATA_00               = 0x49;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_01               = 0x4A;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_02               = 0x4B;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_03               = 0x4C;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_04               = 0x4D;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_05               = 0x4E;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_06               = 0x4F;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_07               = 0x50;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_08               = 0x51;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_09               = 0x52;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_10               = 0x53;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_11               = 0x54;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_12               = 0x55;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_13               = 0x56;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_14               = 0x57;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_15               = 0x58;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_16               = 0x59;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_17               = 0x5A;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_18               = 0x5B;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_19               = 0x5C;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_20               = 0x5D;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_21               = 0x5E;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_22               = 0x5F;
constexpr uint8_t MPU_REG_EXT_SENS_DATA_23               = 0x60;


constexpr uint8_t MPU_REG_I2C_SLV0_DO                    = 0x63;
constexpr uint8_t MPU_REG_I2C_SLV1_DO                    = 0x64;
constexpr uint8_t MPU_REG_I2C_SLV2_DO                    = 0x65;
constexpr uint8_t MPU_REG_I2C_SLV3_DO                    = 0x66;

constexpr uint8_t MPU_REG_I2C_MST_DELAY_CTRL             = 0x67;
    constexpr uint8_t MPU_BIT_DELAY_ES_SHADOW            = 1 << 7;
    constexpr uint8_t MPU_BIT_I2C_SLV4_DLY_EN            = 1 << 4;
    constexpr uint8_t MPU_BIT_I2C_SLV3_DLY_EN            = 1 << 3;
    constexpr uint8_t MPU_BIT_I2C_SLV2_DLY_EN            = 1 << 2;
    constexpr uint8_t MPU_BIT_I2C_SLV1_DLY_EN            = 1 << 1;
    constexpr uint8_t MPU_BIT_I2C_SLV0_DLY_EN            = 1 << 0;


constexpr uint8_t MPU_REG_SIGNAL_PATH_RESET              = 0x68;
    constexpr uint8_t MPU_BIT_GYRO_RST                   = 1 << 2;
    constexpr uint8_t MPU_BIT_ACCEL_RST                  = 1 << 1;
    constexpr uint8_t MPU_BIT_TEMP_RST                   = 1 << 0;

constexpr uint8_t MPU_REG_MOT_DETECT_CTRL                = 0x69;
    constexpr uint8_t MPU_BIT_ACCEL_INTEL_EN             = 1 << 7;
    constexpr uint8_t MPU_BIT_ACCEL_INTEL_MODE           = 1 << 6;

constexpr uint8_t MPU_REG_USER_CTRL                      = 0x6A;
    constexpr uint8_t MPU_BIT_FIFO_EN                    = 1 << 6;
    constexpr uint8_t MPU_BIT_I2C_MST                    = 1 << 5;
    constexpr uint8_t MPU_BIT_I2C_IF_DIS                 = 1 << 4;
    constexpr uint8_t MPU_BIT_FIFO_RST                   = 1 << 2;
    constexpr uint8_t MPU_BIT_I2C_MST_RST                = 1 << 1;
    constexpr uint8_t MPU_BIT_SIG_COND_RST               = 1 << 0;

constexpr uint8_t MPU_REG_PWR_MGMT_1                     = 0x6B;
    constexpr uint8_t MPU_BIT_H_RESET                    = 1 << 7;
    constexpr uint8_t MPU_BIT_SLEEP                      = 1 << 6;
    constexpr uint8_t MPU_BIT_CYCLE                      = 1 << 5;
    constexpr uint8_t MPU_BIT_GYRO_STANDBY               = 1 << 4;
    constexpr uint8_t MPU_BIT_PD_PTAT                    = 1 << 3;
    constexpr uint8_t MPU_BIT_CLKSEL_INTERNAL            = 0x00;            // clock set to internal 8Mhz oscillator
    constexpr uint8_t MPU_BIT_CLKSEL_AUTO                = 0x01;            // PLL with X axis gyroscope reference

constexpr uint8_t MPU_REG_PWR_MGMT_2                     = 0x6C;
    constexpr uint8_t MPU_BIT_DIS_XA                     = 1 << 5;
    constexpr uint8_t MPU_BIT_DIS_YA                     = 1 << 4;
    constexpr uint8_t MPU_BIT_DIS_ZA                     = 1 << 3;
    constexpr uint8_t MPU_BIT_DIS_XG                     = 1 << 2;
    constexpr uint8_t MPU_BIT_DIS_YG                     = 1 << 1;
    constexpr uint8_t MPU_BIT_DIS_ZG                     = 1 << 0;

constexpr uint8_t MPU_REG_FIFO_COUNTH                    = 0x72;
constexpr uint8_t MPU_REG_FIFO_COUNTL                    = 0x73;
constexpr uint8_t MPU_REG_FIFO_R_W                       = 0x74;
constexpr uint8_t MPU_REG_WHO_AM_I                       = 0x75;


//////////////////////////////////////////////////////////////////


#ifdef USE_AK8963

//#if defined USE_AK8975
//constexpr uint8_t AK89xx_HIGH_SENS = 0x00;
//constexpr uint8_t AK89xx_FSR       = 9830;
//#elif defined USE_AK8963
//constexpr uint8_t AK89xx_HIGH_SENS = 0x10;
//constexpr uint8_t AK89xx_FSR       = 4915;
//#endif

constexpr uint8_t AKM_REG_WHOAMI                    = 0x00;

constexpr uint8_t AKM_REG_ST1                       = 0x02;

constexpr uint8_t AKM_REG_HXL                       = 0x03;
constexpr uint8_t AKM_REG_HXH                       = 0x04;
constexpr uint8_t AKM_REG_HYL                       = 0x05;
constexpr uint8_t AKM_REG_HYH                       = 0x06;
constexpr uint8_t AKM_REG_HZL                       = 0x07;
constexpr uint8_t AKM_REG_HZH                       = 0x08;

constexpr uint8_t AKM_REG_ST2                       = 0x09;

constexpr uint8_t AKM_REG_CNTL1                     = 0x0A;
constexpr uint8_t AKM_REG_CNTL2                     = 0x0B;
constexpr uint8_t AKM_REG_ASTC                      = 0x0C;
constexpr uint8_t AKM_REG_ASAX                      = 0x10;
constexpr uint8_t AKM_REG_ASAY                      = 0x11;
constexpr uint8_t AKM_REG_ASAZ                      = 0x12;

constexpr uint8_t AKM_DATA_READY                    = 0x01;
constexpr uint8_t AKM_DATA_OVERRUN                  = 0x02;
constexpr uint8_t AKM_OVERFLOW                      = 0x80;
constexpr uint8_t AKM_DATA_ERROR                    = 0x40;

constexpr uint8_t AKM_BIT_SELF_TEST                 = 0x40;

constexpr uint8_t AKM_CNTL1_POWER_DOWN              = 0x00;
constexpr uint8_t AKM_CNTL1_SINGLE_MEASUREMENT      = 0x01;
constexpr uint8_t AKM_CNTL1_CONTINUOUS1_MEASUREMENT = 0x02;
constexpr uint8_t AKM_CNTL1_CONTINUOUS2_MEASUREMENT = 0x06;
constexpr uint8_t AKM_CNTL1_FUSE_ROM_ACCESS         = 0x0F;
constexpr uint8_t AKM_CNTL1_16_BIT_MODE             = 1 << 4;

constexpr uint8_t AKM_CNTL2_RESET                   = 0x01;

constexpr uint8_t AKM_MODE_SELF_TEST                = 0x08;

constexpr uint8_t AKM_WHOAMI                        = 0x48;

#endif


constexpr uint8_t READ_FLAG = 0x80;


constexpr size_t CONFIG_REGISTER_SPEED = 100000;
constexpr size_t MISC_REGISTER_SPEED = 1000000;
constexpr size_t SENSOR_REGISTER_SPEED = 1000000;


MPU9250::MPU9250(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::MPU9250_Descriptor())
    , m_config(new hal::MPU9250_Config())
{
    m_acceleration = std::make_shared<Acceleration_Stream>();
    m_angular_velocity = std::make_shared<Angular_Velocity_Stream>();
    m_magnetic_field = std::make_shared<Magnetic_Field_Stream>();
    m_temperature = std::make_shared<Temperature_Stream>();
}

MPU9250::~MPU9250()
{
}

auto MPU9250::lock(Buses& buses) -> bool
{
    if (buses.i2c)
    {
        buses.i2c->lock(); //lock the bus
        return true;
    }
    if (buses.spi)
    {
        buses.spi->lock(); //lock the bus
        return true;
    }
    return false;
}
void MPU9250::unlock(Buses& buses)
{
    if (buses.i2c)
    {
        buses.i2c->unlock(); //unlock the bus
        return;
    }
    if (buses.spi)
    {
        buses.spi->unlock(); //unlock the bus
        return;
    }
}

auto MPU9250::mpu_read(Buses& buses, uint8_t reg, uint8_t* rx_data, uint32_t size, size_t speed) -> bool
{
    m_dummy_tx_data.resize(size);
    return buses.i2c ? buses.i2c->read_register(ADDR_MPU9250, reg, rx_data, size)
         : buses.spi ? buses.spi->transfer_register(reg | READ_FLAG, m_dummy_tx_data.data(), rx_data, size, speed)
         : false;
}
auto MPU9250::mpu_read_u8(Buses& buses, uint8_t reg, uint8_t& rx_data, size_t speed) -> bool
{
    uint8_t dummy_data = 0;
    return buses.i2c ? buses.i2c->read_register_u8(ADDR_MPU9250, reg, rx_data)
         : buses.spi ? buses.spi->transfer_register_u8(reg | READ_FLAG, dummy_data, rx_data, speed)
         : false;
}
auto MPU9250::mpu_read_u16(Buses& buses, uint8_t reg, uint16_t& rx_data, size_t speed) -> bool
{
    uint16_t dummy_data = 0;
    return buses.i2c ? buses.i2c->read_register_u16(ADDR_MPU9250, reg, rx_data)
         : buses.spi ? buses.spi->transfer_register_u16(reg | READ_FLAG, dummy_data, rx_data, speed)
         : false;
}
auto MPU9250::mpu_write_u8(Buses& buses, uint8_t reg, uint8_t tx_data, size_t speed) -> bool
{
    uint8_t dummy_data = 0;
    return buses.i2c ? buses.i2c->write_register_u8(ADDR_MPU9250, reg, tx_data)
         : buses.spi ? buses.spi->transfer_register_u8(reg, tx_data, dummy_data, speed)
         : false;
}
auto MPU9250::mpu_write_u16(Buses& buses, uint8_t reg, uint16_t tx_data, size_t speed) -> bool
{
    uint16_t dummy_data = 0;
    return buses.i2c ? buses.i2c->write_register_u16(ADDR_MPU9250, reg, tx_data)
         : buses.spi ? buses.spi->transfer_register_u16(reg, tx_data, dummy_data, speed)
         : false;
}
auto MPU9250::akm_read(Buses& buses, uint8_t reg, uint8_t* rx_data, uint32_t size, size_t speed) -> bool
{
    if (buses.i2c)
    {
        return buses.i2c->read_register(m_akm_address, reg, rx_data, size);
    }

    //spi read

    bool res = true;
    constexpr uint8_t READ_FLAG = 0x80;
    constexpr uint8_t akm_address = 0x0C | READ_FLAG;
    if (m_akm_address != akm_address)
    {
        res &= mpu_write_u8(buses, MPU_REG_I2C_SLV0_ADDR, akm_address, speed);
        m_akm_address = akm_address;
    }

    res &= mpu_write_u8(buses, MPU_REG_I2C_SLV0_REG, reg, speed);
    res &= mpu_write_u8(buses, MPU_REG_I2C_SLV0_CTRL, MPU_BIT_I2C_SLV0_EN + size, speed);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    res &= mpu_read(buses, MPU_REG_EXT_SENS_DATA_00, rx_data, size, speed);
    return res;
}
auto MPU9250::akm_read_u8(Buses& buses, uint8_t reg, uint8_t& dst, size_t speed) -> bool
{
    return akm_read(buses, reg, &dst, sizeof(dst), speed);
}
auto MPU9250::akm_read_u16(Buses& buses, uint8_t reg, uint16_t& dst, size_t speed) -> bool
{
    if (buses.i2c)
    {
        return buses.i2c->read_register_u16(m_akm_address, reg, dst);
    }

    //spi read

    bool res = true;
    constexpr uint8_t akm_address = 0x0C | READ_FLAG;
    if (m_akm_address != akm_address)
    {
        res &= mpu_write_u8(buses, MPU_REG_I2C_SLV0_ADDR, akm_address, speed);
        m_akm_address = akm_address;
    }

    res &= mpu_write_u8(buses, MPU_REG_I2C_SLV0_REG,  reg, speed);
    res &= mpu_write_u8(buses, MPU_REG_I2C_SLV0_CTRL, MPU_BIT_I2C_SLV0_EN + sizeof(dst), speed);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    res &= mpu_read_u16(buses, MPU_REG_EXT_SENS_DATA_00, dst, speed);
    return res;
}
auto MPU9250::akm_write_u8(Buses& buses, uint8_t reg, uint8_t t, size_t speed) -> bool
{
    if (buses.i2c)
    {
        return buses.i2c->write_register_u8(m_akm_address, reg, t);
    }

    //spi write

    bool res = true;
    constexpr uint8_t akm_address = 0x0C;
    if (m_akm_address != akm_address)
    {
        res &= mpu_write_u8(buses, MPU_REG_I2C_SLV0_ADDR, akm_address, speed);
        m_akm_address = akm_address;
    }

    res &= mpu_write_u8(buses, MPU_REG_I2C_SLV0_REG,  reg, speed);
    res &= mpu_write_u8(buses, MPU_REG_I2C_SLV0_DO,   t, speed);
    res &= mpu_write_u8(buses, MPU_REG_I2C_SLV0_CTRL, MPU_BIT_I2C_SLV0_EN + 1, speed); //one byte
    return res;
}

auto MPU9250::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(4);
    outputs[0].name = "Angular Velocity";
    outputs[0].stream = m_angular_velocity;
    outputs[1].name = "Acceleration";
    outputs[1].stream = m_acceleration;
    outputs[2].name = "Magnetic Field";
    outputs[2].stream = m_magnetic_field;
    outputs[3].name = "Temperature";
    outputs[3].stream = m_temperature;
    return outputs;
}
auto MPU9250::init(hal::INode_Descriptor const& descriptor) -> bool
{
    QLOG_TOPIC("mpu9250::init");

    auto specialized = dynamic_cast<hal::MPU9250_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        QLOGE("Wrong descriptor type");
        return false;
    }
    *m_descriptor = *specialized;

    return init();
}


auto MPU9250::init() -> bool
{
    m_i2c = m_hal.get_bus_registry().find_by_name<bus::II2C>(m_descriptor->get_bus());
    m_spi = m_hal.get_bus_registry().find_by_name<bus::ISPI>(m_descriptor->get_bus());

    Buses buses = { m_i2c.lock(), m_spi.lock() };
    if (!buses.i2c && !buses.spi)
    {
        QLOGE("No bus configured");
        return false;
    }

    lock(buses);
    At_Exit at_exit([this, &buses]()
    {
        unlock(buses);
    });


//    std::vector<size_t> g_ranges = { 250, 500, 1000, 2000 };
//    std::vector<size_t> a_ranges = { 2, 4, 8, 16 };
//    std::vector<size_t> imu_rates = { 250, 500, 1000 };//, 8000 }; //8Khz has issues with the magnetometer and it fills the fifo too fast

//    auto req_params = *m_descriptor;

//    m_descriptor->set_magnetic_field_rate = math::clamp<size_t>(m_descriptor->magnetic_field_rate, 10, 100);
//    m_descriptor->set_temperature_rate math::clamp<size_t>(m_descriptor->temperature_rate, 10, 50);

//    std::nth_element(g_ranges.begin(), g_ranges.begin(), g_ranges.end(), [&](size_t a, size_t b)
//    {
//        return math::abs(a - m_descriptor->get_angular_velocity_range()) < math::abs(b - m_descriptor->get_angular_velocity_range());
//    });
//    m_descriptor->set_angular_velocity_range(g_ranges.front());

//    std::nth_element(a_ranges.begin(), a_ranges.begin(), a_ranges.end(), [&](size_t a, size_t b)
//    {
//        return math::abs(a - m_descriptor->acceleration_range) < math::abs(b - m_descriptor->acceleration_range);
//    });
//    m_descriptor->acceleration_range = a_ranges.front();

//    std::nth_element(imu_rates.begin(), imu_rates.begin(), imu_rates.end(), [&](size_t a, size_t b)
//    {
//        return math::abs(a - m_descriptor->acceleration_angular_velocity_rate) < math::abs(b - m_descriptor->acceleration_angular_velocity_rate);
//    });
//    m_descriptor->acceleration_angular_velocity_rate = imu_rates.front();

    QLOGI("Probing MPU9250 on {}", m_descriptor->get_bus());
    QLOGI("Angular Velocity range {} DPS", (size_t)m_descriptor->get_angular_velocity_range());
    QLOGI("Acceleration range {}G", m_descriptor->get_acceleration_range());
    QLOGI("Acceleration & Angular Velocity Rate {}Hz", (size_t)m_descriptor->get_imu_rate());
    QLOGI("Magnetic Field Rate {}Hz", m_descriptor->get_magnetometer_rate());
    QLOGI("Temperature Rate {}Hz", m_descriptor->get_thermometer_rate());

    uint8_t gyro_range = MPU_BIT_GYRO_FS_SEL_1000_DPS;
    switch (m_descriptor->get_angular_velocity_range())
    {
    case hal::MPU9250_Descriptor::angular_velocity_range_t::_250:
        gyro_range = MPU_BIT_GYRO_FS_SEL_250_DPS;
        m_angular_velocity_sensor_scale = math::radians(1.f) / (131.f);
        break;
    case hal::MPU9250_Descriptor::angular_velocity_range_t::_500:
        gyro_range = MPU_BIT_GYRO_FS_SEL_500_DPS;
        m_angular_velocity_sensor_scale = math::radians(1.f) / (131.f / 2.f);
        break;
    case hal::MPU9250_Descriptor::angular_velocity_range_t::_1000:
        gyro_range = MPU_BIT_GYRO_FS_SEL_1000_DPS;
        m_angular_velocity_sensor_scale = math::radians(1.f) / (131.f / 4.f);
        break;
    case hal::MPU9250_Descriptor::angular_velocity_range_t::_2000:
        gyro_range = MPU_BIT_GYRO_FS_SEL_2000_DPS;
        m_angular_velocity_sensor_scale = math::radians(1.f) / (131.f / 8.f);
        break;
    default:
        QLOGE("Invalid angular velocity range: {}", m_descriptor->get_angular_velocity_range());
        return false;
    }


    uint8_t accel_range = MPU_BIT_ACCEL_FS_SEL_8_G;
    switch (m_descriptor->get_acceleration_range())
    {
    case hal::MPU9250_Descriptor::acceleration_range_t::_2:
        accel_range = MPU_BIT_ACCEL_FS_SEL_2_G;
        m_acceleration_sensor_scale = physics::constants::g / 16384.f;
        break;
    case hal::MPU9250_Descriptor::acceleration_range_t::_4:
        accel_range = MPU_BIT_ACCEL_FS_SEL_4_G;
        m_acceleration_sensor_scale = physics::constants::g / 8192.f;
        break;
    case hal::MPU9250_Descriptor::acceleration_range_t::_8:
        accel_range = MPU_BIT_ACCEL_FS_SEL_8_G;
        m_acceleration_sensor_scale = physics::constants::g / 4096.f;
        break;
    case hal::MPU9250_Descriptor::acceleration_range_t::_16:
        accel_range = MPU_BIT_ACCEL_FS_SEL_16_G;
        m_acceleration_sensor_scale = physics::constants::g / 2048.f;
        break;
    default:
        QLOGE("Invalid acceleration range: {}", m_descriptor->get_acceleration_range());
        return false;
    }

    // First disable the master I2C to avoid hanging the slaves on the aulixiliar I2C bus
    {
        uint8_t user_ctrl = 0;
        mpu_read_u8(buses, MPU_REG_USER_CTRL, user_ctrl, CONFIG_REGISTER_SPEED);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        QLOGI("i2c master is: {}", (user_ctrl & MPU_BIT_I2C_MST) ? "enabled" : "disabled");
        if (user_ctrl & MPU_BIT_I2C_MST)
        {
            mpu_write_u8(buses, MPU_REG_USER_CTRL, (user_ctrl & ~MPU_BIT_I2C_MST), CONFIG_REGISTER_SPEED);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    auto res = mpu_write_u8(buses, MPU_REG_PWR_MGMT_1, MPU_BIT_H_RESET, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(120));

    if (buses.spi)
    {
        // disable I2C as recommended by the datasheet
        res &= mpu_write_u8(buses, MPU_REG_USER_CTRL, MPU_BIT_I2C_IF_DIS, CONFIG_REGISTER_SPEED);
        std::this_thread::sleep_for(std::chrono::milliseconds(120));
    }

    res &= mpu_write_u8(buses, MPU_REG_PWR_MGMT_1, 0, CONFIG_REGISTER_SPEED); //wakeup
    std::this_thread::sleep_for(std::chrono::milliseconds(120));

    res &= mpu_write_u8(buses, MPU_REG_SIGNAL_PATH_RESET, MPU_BIT_GYRO_RST | MPU_BIT_ACCEL_RST | MPU_BIT_TEMP_RST, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(120));

    res &= mpu_write_u8(buses, MPU_REG_PWR_MGMT_1, MPU_BIT_CLKSEL_AUTO, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    uint8_t who_am_i;
    res &= mpu_read_u8(buses, MPU_REG_WHO_AM_I, who_am_i, CONFIG_REGISTER_SPEED);
    if (!res || who_am_i != 0x71)
    {
        QLOGE("Cannot find mpu9250");
#ifdef RASPBERRY_PI
        return false;
#endif
    }
    QLOGI("Found MPU9250 id: {x}", who_am_i);

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    res &= mpu_write_u8(buses, MPU_REG_PWR_MGMT_2, 0, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    res &= mpu_write_u8(buses, MPU_REG_GYRO_CONFIG, gyro_range, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    res &= mpu_write_u8(buses, MPU_REG_ACCEL_CONFIG, accel_range, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    //compute the rate
    uint8_t g_dlpf = 0;
    uint8_t a_dlpf = 0;
    uint8_t div = 0;
    if (m_descriptor->get_imu_rate() <= hal::MPU9250_Descriptor::imu_rate_t::_1000)
    {
        g_dlpf = MPU_BIT_DLPF_CFG_184_1;
        a_dlpf = MPU_BIT_A_DLPF_CFG_460_1;
        div = 1000 / (int)m_descriptor->get_imu_rate() - 1;
    }
    else
    {
        g_dlpf = MPU_BIT_DLPF_CFG_3600_8;
        a_dlpf = MPU_BIT_ACCEL_FCHOICE_B; //disable lpf - resulting in a 4KHz sample rate. This is unaffected by the sample_rate_div
        div = 0;//8000 / m_descriptor->acceleration_angular_velocity_rate - 1;
    }

    res &= mpu_write_u8(buses, MPU_REG_CONFIG, g_dlpf, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    res &= mpu_write_u8(buses, MPU_REG_SMPLRT_DIV, div, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    res &= mpu_write_u8(buses, MPU_REG_ACCEL_CONFIG2, MPU_BIT_FIFO_SIZE_4096 | 0x8 | a_dlpf, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(5));

    res &= mpu_write_u8(buses, MPU_REG_FIFO_EN, MPU_BIT_GYRO_XO_UT | MPU_BIT_GYRO_YO_UT | MPU_BIT_GYRO_ZO_UT | MPU_BIT_ACCEL, CONFIG_REGISTER_SPEED);
    m_fifo_sample_size = 12;

    m_user_ctrl_value = MPU_BIT_FIFO_EN;
    if (buses.i2c)
    {
        res &= mpu_write_u8(buses, MPU_REG_USER_CTRL, m_user_ctrl_value | MPU_BIT_FIFO_RST | MPU_BIT_SIG_COND_RST, CONFIG_REGISTER_SPEED);
    }
    else
    {
        res &= mpu_write_u8(buses, MPU_REG_USER_CTRL, m_user_ctrl_value | MPU_BIT_FIFO_RST | MPU_BIT_I2C_IF_DIS | MPU_BIT_SIG_COND_RST, CONFIG_REGISTER_SPEED);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    QLOGI("Probing Magnetometer");

    uint32_t tries = 0;
    constexpr uint32_t max_tries = 10;
    while (++tries <= max_tries)
    {
        if (setup_magnetometer(buses))
        {
            QLOGI("Found Magnetometer after {} tries", tries);
            break;
        }
    }
    if (tries > max_tries)
    {
        QLOGE("Failed to initialize Magnetometer");
        return false;
    }

    res &= mpu_write_u8(buses, MPU_REG_USER_CTRL, m_user_ctrl_value | MPU_BIT_FIFO_RST, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (!res)
    {
        QLOGE("Failed to setup mpu9250");
#ifdef RASPBERRY_PI
        return false;
#endif
    }

    reset_fifo(buses);

    m_magnetic_field->set_rate(m_descriptor->get_magnetometer_rate());
    m_acceleration->set_rate((size_t)m_descriptor->get_imu_rate());
    m_angular_velocity->set_rate((size_t)m_descriptor->get_imu_rate());
    m_temperature->set_rate(m_descriptor->get_thermometer_rate());

    return true;
}

auto MPU9250::setup_magnetometer(Buses& buses) -> bool
{
    if (buses.i2c)
    {
        return setup_magnetometer_i2c(buses);
    }
    else if (buses.spi)
    {
        return setup_magnetometer_spi(buses);
    }
    return false;
}

auto MPU9250::setup_magnetometer_i2c(Buses& buses) -> bool
{
    QLOG_TOPIC("mpu9250::setup_magnetometer_i2c");

#ifdef USE_AK8963
    {
        m_user_ctrl_value &= ~MPU_BIT_I2C_MST;
        mpu_write_u8(buses, MPU_REG_USER_CTRL, m_user_ctrl_value, CONFIG_REGISTER_SPEED);
        std::this_thread::sleep_for(std::chrono::milliseconds(3));

        mpu_write_u8(buses, MPU_REG_INT_PIN_CFG, MPU_BIT_BYPASS_EN, CONFIG_REGISTER_SPEED);
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }

    // Find magnetometer. Possible addresses range from 0x0C to 0x0F.
    for (m_akm_address = 0x0C; m_akm_address <= 0x0F; m_akm_address++)
    {
        uint8_t data;
        auto res = akm_read_u8(buses, AKM_REG_WHOAMI, data, CONFIG_REGISTER_SPEED);
        if (res && data == AKM_WHOAMI)
        {
            break;
        }
    }

    if (m_akm_address > 0x0F)
    {
        QLOGE("\tMagnetometer not found.");
        return false;
    }

    QLOGI("\tMagnetometer found at 0x{X}", m_akm_address);

    akm_write_u8(buses, AKM_REG_CNTL1, AKM_CNTL1_POWER_DOWN, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    akm_write_u8(buses, AKM_REG_CNTL1, AKM_CNTL1_FUSE_ROM_ACCESS, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // Get sensitivity adjustment data from fuse ROM.
    uint8_t data[4] = {0};
    akm_read(buses, AKM_REG_ASAX, data, 3, CONFIG_REGISTER_SPEED);
    m_magnetic_field_sensor_scale.x = (long)(data[0] - 128)*0.5f / 128.f + 1.f;
    m_magnetic_field_sensor_scale.y = (long)(data[1] - 128)*0.5f / 128.f + 1.f;
    m_magnetic_field_sensor_scale.z = (long)(data[2] - 128)*0.5f / 128.f + 1.f;
    m_magnetic_field_sensor_scale *= 0.15f;//16 bit mode

    akm_write_u8(buses, AKM_REG_CNTL1, AKM_CNTL1_POWER_DOWN, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    akm_write_u8(buses, AKM_REG_CNTL1, AKM_CNTL1_CONTINUOUS2_MEASUREMENT | AKM_CNTL1_16_BIT_MODE, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

#endif

    return true;
}

auto MPU9250::setup_magnetometer_spi(Buses& buses) -> bool
{
    QLOG_TOPIC("mpu9250::setup_magnetometer_spi");

    bool res = true;


#ifdef USE_AK8963

    {
        // Enable I2C master mode if magnetometer is being used.
        res &= mpu_write_u8(buses, MPU_REG_INT_PIN_CFG, MPU_BIT_LATCH_INT_EN | MPU_BIT_INT_ANYRD_2CLEAR, CONFIG_REGISTER_SPEED);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        m_user_ctrl_value |= MPU_BIT_I2C_MST;
        res &= mpu_write_u8(buses, MPU_REG_USER_CTRL, m_user_ctrl_value, CONFIG_REGISTER_SPEED);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        //  0 348 kHz
        //  1 333 kHz
        //  2 320 kHz
        //  3 308 kHz
        //  4 296 kHz
        //  5 286 kHz
        //  6 276 kHz
        //  7 267 kHz
        //  8 258 kHz
        //  9 500 kHz
        // 10 471 kHz
        // 11 444 kHz
        // 12 421 kHz
        // 13 400 kHz
        // 14 381 kHz
        // 15 364 kHz
        res &= mpu_write_u8(buses, MPU_REG_I2C_MST_CTRL, 13, CONFIG_REGISTER_SPEED); //i2c speed
        std::this_thread::sleep_for(std::chrono::milliseconds(30));

        //how often to read the magnetometer
        int delay = (size_t)m_descriptor->get_imu_rate() / m_descriptor->get_magnetometer_rate();
        delay = math::clamp(delay - 1, 0, 31);

        //i2c slave delay
        res &= mpu_write_u8(buses, MPU_REG_I2C_SLV4_CTRL, delay, CONFIG_REGISTER_SPEED);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        //i2c slave delay enable
        res &= mpu_write_u8(buses, MPU_REG_I2C_MST_DELAY_CTRL, MPU_BIT_DELAY_ES_SHADOW | MPU_BIT_I2C_SLV0_DLY_EN, CONFIG_REGISTER_SPEED);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    m_akm_address = 0;

//    res &= akm_write_u8(buses, AKM_REG_CNTL2, AKM_CNTL2_RESET);
//    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    {
        uint8_t data;
        res &= akm_read_u8(buses, AKM_REG_WHOAMI, data, CONFIG_REGISTER_SPEED);
        if (!res || data != AKM_WHOAMI)
        {
            QLOGW("\tMagnetometer not found ({}, {})!!", res, data);
            return false;
        }
    }

    res &= akm_write_u8(buses, AKM_REG_CNTL1, AKM_CNTL1_FUSE_ROM_ACCESS, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Get sensitivity adjustment data from fuse ROM.
    uint8_t data[4] = {0};
    res &= akm_read(buses, AKM_REG_ASAX, data, 3, CONFIG_REGISTER_SPEED);
    m_magnetic_field_sensor_scale.x = (long)(data[0] - 128)*0.5f / 128.f + 1.f;
    m_magnetic_field_sensor_scale.y = (long)(data[1] - 128)*0.5f / 128.f + 1.f;
    m_magnetic_field_sensor_scale.z = (long)(data[2] - 128)*0.5f / 128.f + 1.f;
    m_magnetic_field_sensor_scale *= 0.15f;//16 bit mode

    res &= akm_write_u8(buses, AKM_REG_CNTL1, AKM_CNTL1_CONTINUOUS2_MEASUREMENT | AKM_CNTL1_16_BIT_MODE, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    //now request the transfer again
    constexpr uint8_t akm_address = 0x0C | READ_FLAG;
    if (m_akm_address != akm_address)
    {
        mpu_write_u8(buses, MPU_REG_I2C_SLV0_ADDR, akm_address, CONFIG_REGISTER_SPEED);
        m_akm_address = akm_address;
    }
    mpu_write_u8(buses, MPU_REG_I2C_SLV0_REG,  AKM_REG_ST1, CONFIG_REGISTER_SPEED);
    mpu_write_u8(buses, MPU_REG_I2C_SLV0_CTRL, MPU_BIT_I2C_SLV0_EN + 8, CONFIG_REGISTER_SPEED);

#endif

    return res;
}

//void MPU9250::set_bypass(Buses& buses, bool on)
//{
//    QLOG_TOPIC("mpu9250::set_bypass");

//    if (on)
//    {
//        uint8_t tmp;
//        mpu_read_u8(buses, MPU_REG_USER_CTRL, tmp);
//        tmp &= ~MPU_BIT_I2C_MST;
//        mpu_write_u8(buses, MPU_REG_USER_CTRL, tmp);
//        std::this_thread::sleep_for(std::chrono::milliseconds(3));
//        tmp = MPU_BIT_BYPASS_EN;
//        mpu_write_u8(buses, MPU_REG_INT_PIN_CFG, tmp);
//    }
//    else
//    {
//        // Enable I2C master mode if magnetometer is being used.
//        uint8_t tmp;
//        mpu_read_u8(buses, MPU_REG_USER_CTRL, tmp);
//        tmp |= MPU_BIT_I2C_MST;
//        mpu_write_u8(buses, MPU_REG_USER_CTRL, tmp);
//        std::this_thread::sleep_for(std::chrono::milliseconds(3));
//        tmp = 0;
//        mpu_write_u8(buses, MPU_REG_INT_PIN_CFG, tmp);
//    }
//}

auto MPU9250::start(q::Clock::time_point tp) -> bool
{
    m_magnetic_field->set_tp(tp);
    m_acceleration->set_tp(tp);
    m_angular_velocity->set_tp(tp);
    m_temperature->set_tp(tp);
    m_magnetic_field->set_tp(tp);

    return true;
}

void MPU9250::reset_fifo(Buses& buses)
{
    QLOG_TOPIC("mpu9250::reset_fifo");

    mpu_write_u8(buses, MPU_REG_USER_CTRL, m_user_ctrl_value | MPU_BIT_FIFO_RST, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    mpu_write_u8(buses, MPU_REG_FIFO_EN, 0, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    mpu_write_u8(buses, MPU_REG_USER_CTRL, m_user_ctrl_value | MPU_BIT_FIFO_RST, CONFIG_REGISTER_SPEED);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    mpu_write_u8(buses, MPU_REG_FIFO_EN, MPU_BIT_GYRO_XO_UT | MPU_BIT_GYRO_YO_UT | MPU_BIT_GYRO_ZO_UT | MPU_BIT_ACCEL, CONFIG_REGISTER_SPEED);
}

template<class Calibration_Data>
auto get_calibration_scale(Calibration_Data const& points, float temperature) -> math::vec3f
{
    if (points.empty())
    {
        return math::vec3f::one;
    }
    auto it = std::upper_bound(points.begin(), points.end(), temperature, [](float temperature, const typename Calibration_Data::value_type& a)
    {
        return temperature < a.get_temperature();
    });
    if (it == points.end()) //temp is too big, use the last point
    {
        return points.back().get_scale();
    }
    if (it == points.begin()) //temp is too small, use the first point
    {
        return points.front().get_scale();
    }
    auto const& prev = *(it - 1);
    auto const& next = *(it);
    QASSERT(temperature >= prev.get_temperature() && temperature < next.get_temperature());
    float mu = (temperature - prev.get_temperature()) / (next.get_temperature() - prev.get_temperature());
    QASSERT(mu >= 0.f && mu < 1.f);
    return math::lerp(prev.get_scale(), next.get_scale(), mu);
}

template<class Calibration_Data>
auto get_calibration_bias(Calibration_Data const& points, float temperature) -> math::vec3f
{
    if (points.empty())
    {
        return math::vec3f::zero;
    }
    auto it = std::upper_bound(points.begin(), points.end(), temperature, [](float temperature, const typename Calibration_Data::value_type& a)
    {
        return temperature < a.get_temperature();
    });
    if (it == points.end()) //temp is too big, use the last point
    {
        return points.back().get_bias();
    }
    if (it == points.begin()) //temp is too small, use the first point
    {
        return points.front().get_bias();
    }
    auto const& prev = *(it - 1);
    auto const& next = *(it);
    QASSERT(temperature >= prev.get_temperature() && temperature < next.get_temperature());
    float mu = (temperature - prev.get_temperature()) / (next.get_temperature() - prev.get_temperature());
    QASSERT(mu >= 0.f && mu < 1.f);
    return math::lerp(prev.get_bias(), next.get_bias(), mu);
}

void MPU9250::process()
{
    QLOG_TOPIC("mpu9250::process");

    m_acceleration->clear();
    m_angular_velocity->clear();
    m_magnetic_field->clear();
    m_temperature->clear();

    Buses buses = { m_i2c.lock(), m_spi.lock() };
    if (!buses.i2c && !buses.spi)
    {
        return;
    }

    lock(buses);
    At_Exit at_exit([this, &buses]()
    {
        unlock(buses);
    });

    //TODO - add health indication

    //auto now = q::Clock::now();
    //static q::Clock::time_point last_timestamp = q::Clock::now();
   // auto dt = now - last_timestamp;
   // last_timestamp = now;

    uint16_t fifo_count;
    auto res = mpu_read_u16(buses, MPU_REG_FIFO_COUNTH, fifo_count, MISC_REGISTER_SPEED);
    if (!res)
    {
        m_stats.bus_failures++;
    }

   // float xxx = (float(fifo_count) / std::chrono::duration_cast<std::chrono::microseconds>(dt).count()) * 1000.f;
   // LOG_INFO("{.2}b/ms", xxx);

    //uint16_t fc2 = 0;
    if (res && fifo_count >= m_fifo_sample_size)
    {
        if (fifo_count >= 4000)
        {
            QLOGW("Resetting FIFO: {}", fifo_count);
            reset_fifo(buses);
            fifo_count = 0;
        }
        else
        {
            auto sample_count = fifo_count / m_fifo_sample_size;
            auto to_read = sample_count * m_fifo_sample_size;
            QASSERT(sample_count >= 1);

            m_fifo_buffer.resize(to_read);
            if (mpu_read(buses, MPU_REG_FIFO_R_W, m_fifo_buffer.data(), m_fifo_buffer.size(), MISC_REGISTER_SPEED))
            {
//                static q::Clock::time_point s_last_stat_tp = q::Clock::now();
//                static size_t s_sample_count = 0;
//                s_sample_count += sample_count;
//                if (now - s_last_stat_tp >= std::chrono::seconds(1))
//                {
//                    QLOGI("SPS: {}", s_sample_count);
//                    s_sample_count = 0;
//                    s_last_stat_tp = now;
//                }

//                m_angular_velocity->samples.resize(sample_count);
//                m_acceleration->samples.resize(sample_count);

                constexpr size_t k_max_sample_difference = 3;
                auto now = q::Clock::now();
                auto acc_dt = m_acceleration->get_dt();
                auto av_dt = m_angular_velocity->get_dt();

                auto* data = m_fifo_buffer.data();
                for (size_t i = 0; i < sample_count; i++)
                {
                    int16_t x = static_cast<int16_t>((data[0] << 8) | data[1]); data += 2;
                    int16_t y = static_cast<int16_t>((data[0] << 8) | data[1]); data += 2;
                    int16_t z = static_cast<int16_t>((data[0] << 8) | data[1]); data += 2;

                    if (m_acceleration->get_tp() > now + acc_dt*k_max_sample_difference)
                    {
                        m_stats.acc.skipped++;
                    }
                    else
                    {
                        math::vec3f value(x, y, z);
                        value = math::transform(m_imu_rotation, value);
                        value = value * m_acceleration_scale - m_acceleration_bias;
                        m_acceleration->push_sample(value, true);
                    }

                    x = static_cast<int16_t>((data[0] << 8) | data[1]); data += 2;
                    y = static_cast<int16_t>((data[0] << 8) | data[1]); data += 2;
                    z = static_cast<int16_t>((data[0] << 8) | data[1]); data += 2;

                    if (m_angular_velocity->get_tp() > now + av_dt*k_max_sample_difference)
                    {
                        m_stats.av.skipped++;
                    }
                    else
                    {
                        math::vec3f value(x, y, z);
                        value = value * m_angular_velocity_sensor_scale;
                        value = math::transform(m_imu_rotation, value);
                        value = value - m_angular_velocity_bias;
                        m_angular_velocity->push_sample(value, true);
                    }

//                    if (math::length(m_samples.angular_velocity[i]) > 1.f)
//                    {
//                        LOG_ERR("XXX::: gyro: {}, acc: {}", m_samples.angular_velocity[i], m_samples.acceleration[i]);
//                    }
                }
            }
            else
            {
                m_stats.bus_failures++;
            }
        }
    }

    auto now = q::Clock::now();

    //handle and report the slow clock
    {
        constexpr size_t k_max_sample_difference = 8;
        {
            auto dt = m_acceleration->get_dt();
            if (m_acceleration->get_tp() <= now - dt * k_max_sample_difference)
            {
                while (m_acceleration->get_tp() <= now - dt)
                {
                    m_acceleration->push_last_sample(false);
                    m_stats.acc.added++;
                }
            }
        }
        {
            auto dt = m_angular_velocity->get_dt();
            if (m_angular_velocity->get_tp() <= now - dt * k_max_sample_difference)
            {
                while (m_angular_velocity->get_tp() <= now - dt)
                {
                    m_angular_velocity->push_sample(math::vec3f::zero, false);
                    m_stats.av.added++;
                }
            }
        }
    }

    process_thermometer(buses);
    process_magnetometer(buses);

    if (m_stats.last_report_tp + std::chrono::seconds(1) < now)
    {
        if (m_stats != Stats())
        {
            QLOGW("IMU stats: A:a{}s{}, AV:a{}s{}, MF:r{}a{}o{}b{}, T:a{}b{}, bus{}",
                        m_stats.acc.added, m_stats.acc.skipped,
                        m_stats.av.added, m_stats.av.skipped,
                        m_stats.mf.reset, m_stats.mf.added, m_stats.mf.overflow, m_stats.mf.bad_values,
                        m_stats.temp.added, m_stats.temp.bad_values,
                        m_stats.bus_failures);
        }
        m_stats = Stats();
        m_stats.last_report_tp = now;
    }

//    LOG_INFO("fc {} / {}", fifo_count, fc2);
}

void MPU9250::process_thermometer(Buses& buses)
{
    QLOG_TOPIC("mpu9250::process_thermometer");

    size_t samples_needed = m_temperature->compute_samples_needed();
    if (samples_needed == 0)
    {
        return;
    }

    uint16_t data = 0;
    if (mpu_read_u16(buses, MPU_REG_TEMP_OUT_H, data, SENSOR_REGISTER_SPEED))
    {
        int16_t itemp = static_cast<int16_t>(data);
        float temp = itemp / 333.87f + 21.f;
        if (temp > -55.f && temp < 90.f)
        {
            m_last_temperature_value = temp;
            m_last_temperature_tp = q::Clock::now();

            m_acceleration_scale = get_calibration_scale(m_config->get_calibration().get_acceleration(), temp);
            m_acceleration_bias = get_calibration_bias(m_config->get_calibration().get_acceleration(), temp) * m_acceleration_scale;
            m_acceleration_scale *= m_acceleration_sensor_scale;

            m_angular_velocity_bias = get_calibration_bias(m_config->get_calibration().get_angular_velocity(), temp);

            m_magnetic_field_scale = get_calibration_scale(m_config->get_calibration().get_magnetic_field(), temp);
            m_magnetic_field_bias = get_calibration_bias(m_config->get_calibration().get_magnetic_field(), temp) * m_magnetic_field_scale;
            m_magnetic_field_scale *= m_magnetic_field_sensor_scale;
        }
        else
        {
            m_stats.temp.bad_values += samples_needed;
        }
    }
    else
    {
        m_stats.bus_failures++;
    }

    constexpr size_t k_max_sample_difference = 5;
    bool is_healthy = q::Clock::now() - m_last_temperature_tp <= m_temperature->get_dt() * k_max_sample_difference;

    if (!is_healthy)
    {
        m_stats.temp.added += samples_needed;
    }

    while (samples_needed > 0)
    {
        m_temperature->push_sample(m_last_temperature_value, is_healthy);
        samples_needed--;
    }
}

void MPU9250::process_magnetometer(Buses& buses)
{
    QLOG_TOPIC("mpu9250::process_magnetometer");

#ifdef USE_AK8963
    auto now = q::Clock::now();

    auto dt = now - m_last_magnetic_field_tp;
    if (dt >= std::chrono::seconds(5))
    {
        m_stats.mf.reset++;
        setup_magnetometer(buses);
    }

    bool data_available = false;
    std::array<uint8_t, 8> data;
    if (dt >= m_magnetic_field->get_dt())
    {
        if (buses.i2c)
        {
            if (akm_read(buses, AKM_REG_ST1, data.data(), data.size(), SENSOR_REGISTER_SPEED))
            {
                data_available = true;
            }
            else
            {
                m_stats.bus_failures++;
            }
        }
        else //spi
        {
            //first read what is in the ext registers (so the previous reading)
            if (mpu_read(buses, MPU_REG_EXT_SENS_DATA_00, data.data(), data.size(), SENSOR_REGISTER_SPEED))
            {
                data_available = true;
            }
            else
            {
                m_stats.bus_failures++;
            }

    //        //now request the transfer again
    //        constexpr uint8_t READ_FLAG = 0x80;
    //        constexpr uint8_t akm_address = 0x0C | READ_FLAG;
    //        if (m_akm_address != akm_address)
    //        {
    //            mpu_write_u8(buses, MPU_REG_I2C_SLV0_ADDR, akm_address);
    //            m_akm_address = akm_address;
    //        }
    //        mpu_write_u8(buses, MPU_REG_I2C_SLV0_REG,  AKM_REG_ST1);
    //        mpu_write_u8(buses, MPU_REG_I2C_SLV0_CTRL, MPU_BIT_I2C_SLV0_EN + data.size());
        }
    }

    if (data_available && (data[0] & AKM_DATA_READY) != 0)
    {
        if (data[7] & AKM_OVERFLOW)
        {
            m_stats.mf.overflow++;
        }
        else
        {
            math::vec3f value(static_cast<int16_t>((data[2] << 8) | data[1]),
                            static_cast<int16_t>((data[4] << 8) | data[3]),
                            static_cast<int16_t>((data[6] << 8) | data[5]));

            float length_sq = math::length_sq(value);
            if (length_sq > math::square(1.f) && length_sq < math::square(32767.f))
            {
                //result is already in in micro teslas
                value = math::transform(m_magnetometer_rotation, value);
                m_last_magnetic_field_value = value * m_magnetic_field_scale - m_magnetic_field_bias;

                m_last_magnetic_field_tp = q::Clock::now();
            }
            else
            {
                m_stats.mf.bad_values++;
            }
        }
    }

    constexpr size_t k_max_sample_difference = 5;
    bool is_healthy = q::Clock::now() - m_last_magnetic_field_tp <= m_magnetic_field->get_dt() * k_max_sample_difference;

    size_t samples_needed = m_magnetic_field->compute_samples_needed();

    if (!is_healthy)
    {
        m_stats.mf.added += samples_needed;
    }

    while (samples_needed > 0)
    {
        m_magnetic_field->push_sample(m_last_magnetic_field_value, is_healthy);
        samples_needed--;
    }

#endif
}

auto MPU9250::set_config(hal::INode_Config const& config) -> bool
{
    QLOG_TOPIC("mpu9250::set_config");

    auto specialized = dynamic_cast<hal::MPU9250_Config const*>(&config);
    if (!specialized)
    {
        QLOGE("Wrong config type");
        return false;
    }
    *m_config = *specialized;

    hal::MPU9250_Config::Calibration calibration = m_config->get_calibration();

    //sort so we can lower_bound search the point quickly
    {
        auto points = calibration.get_acceleration();
        std::sort(points.begin(), points.end(), [](const hal::Acceleration_Calibration_Point& a, const hal::Acceleration_Calibration_Point& b)
        {
            return a.get_temperature() < b.get_temperature();
        });
        calibration.set_acceleration(points);
    }

    {
        auto points = calibration.get_angular_velocity();
        std::sort(points.begin(), points.end(), [](const hal::Angular_Velocity_Calibration_Point& a, const hal::Angular_Velocity_Calibration_Point& b)
        {
            return a.get_temperature() < b.get_temperature();
        });
        calibration.set_angular_velocity(points);
    }

    {
        auto points = calibration.get_magnetic_field();
        std::sort(points.begin(), points.end(), [](const hal::Magnetic_Field_Calibration_Point& a, const hal::Magnetic_Field_Calibration_Point& b)
        {
            return a.get_temperature() < b.get_temperature();
        });
        calibration.set_magnetic_field(points);
    }

    m_config->set_calibration(calibration);

    //compute chip rotation
    math::quatf imu_rot;
    imu_rot.set_from_euler_xyz(math::radians(m_config->get_rotation()));
    m_imu_rotation = imu_rot.get_as_mat3();

    //change of axis according to the specs. By default the magnetometer has front X, right Y and down Z
    static const math::quatf mag_rot = math::quatf::from_axis_y(math::radians(180.f)) *
                    math::quatf::from_axis_z(math::radians(90.f));

    m_magnetometer_rotation = (imu_rot * mag_rot).get_as_mat3();

    return true;
}
auto MPU9250::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto MPU9250::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

//auto MPU9250::send_message(rapidjson::Value const& /*json*/) -> rapidjson::Document
//{
//    return rapidjson::Document();
//}

}
}
