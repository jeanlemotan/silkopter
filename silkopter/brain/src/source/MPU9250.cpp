#include "BrainStdAfx.h"
#include "MPU9250.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "sz_math.hpp"
#include "sz_MPU9250.hpp"

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


constexpr uint8_t USER_CTRL_VALUE = MPU_BIT_FIFO_EN | MPU_BIT_FIFO_RST/* | MPU_BIT_I2C_MST*/;


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

constexpr uint8_t AKM_REG_CNTL                      = 0x0A;
constexpr uint8_t AKM_REG_ASTC                      = 0x0C;
constexpr uint8_t AKM_REG_ASAX                      = 0x10;
constexpr uint8_t AKM_REG_ASAY                      = 0x11;
constexpr uint8_t AKM_REG_ASAZ                      = 0x12;

constexpr uint8_t AKM_DATA_READY                    = 0x01;
constexpr uint8_t AKM_DATA_OVERRUN                  = 0x02;
constexpr uint8_t AKM_OVERFLOW                      = 0x80;
constexpr uint8_t AKM_DATA_ERROR                    = 0x40;

constexpr uint8_t AKM_BIT_SELF_TEST                 = 0x40;

constexpr uint8_t AKM_POWER_DOWN                    = 0x00;
constexpr uint8_t AKM_SINGLE_MEASUREMENT            = 0x01;
constexpr uint8_t AKM_CONTINUOUS1_MEASUREMENT       = 0x02;
constexpr uint8_t AKM_CONTINUOUS2_MEASUREMENT       = 0x06;
constexpr uint8_t AKM_FUSE_ROM_ACCESS               = 0x0F;
constexpr uint8_t AKM_MODE_SELF_TEST                = 0x08;
constexpr uint8_t AKM_16_BIT_MODE                   = 1 << 4;

constexpr uint8_t AKM_WHOAMI                        = 0x48;

#endif

MPU9250::MPU9250(HAL& hal)
    : m_hal(hal)
    , m_init_params(new sz::MPU9250::Init_Params())
    , m_config(new sz::MPU9250::Config())
{
    autojsoncxx::to_document(*m_init_params, m_init_paramsj);
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

auto MPU9250::mpu_read(Buses& buses, uint8_t reg, uint8_t* data, uint32_t size) -> bool
{
    return buses.i2c ? buses.i2c->read_register(ADDR_MPU9250, reg, data, size)
         : buses.spi ? buses.spi->read_register(reg, data, size)
         : false;
}
auto MPU9250::mpu_read_u8(Buses& buses, uint8_t reg, uint8_t& dst) -> bool
{
    return buses.i2c ? buses.i2c->read_register_u8(ADDR_MPU9250, reg, dst)
         : buses.spi ? buses.spi->read_register_u8(reg, dst)
         : false;
}
auto MPU9250::mpu_read_u16(Buses& buses, uint8_t reg, uint16_t& dst) -> bool
{
    return buses.i2c ? buses.i2c->read_register_u16(ADDR_MPU9250, reg, dst)
         : buses.spi ? buses.spi->read_register_u16(reg, dst)
         : false;
}
auto MPU9250::mpu_write_u8(Buses& buses, uint8_t reg, uint8_t t) -> bool
{
    return buses.i2c ? buses.i2c->write_register_u8(ADDR_MPU9250, reg, t)
         : buses.spi ? buses.spi->write_register_u8(reg, t)
         : false;
}
auto MPU9250::mpu_write_u16(Buses& buses, uint8_t reg, uint16_t t) -> bool
{
    return buses.i2c ? buses.i2c->write_register_u16(ADDR_MPU9250, reg, t)
         : buses.spi ? buses.spi->write_register_u16(reg, t)
         : false;
}
auto MPU9250::akm_read(Buses& buses, uint8_t reg, uint8_t* data, uint32_t size) -> bool
{
    return buses.i2c ? buses.i2c->read_register(m_magnetic_field->akm_address, reg, data, size)
         : buses.spi ? buses.spi->read_register(reg, data, size)
         : false;
}
auto MPU9250::akm_read_u8(Buses& buses, uint8_t reg, uint8_t& dst) -> bool
{
    return buses.i2c ? buses.i2c->read_register_u8(m_magnetic_field->akm_address, reg, dst)
         : buses.spi ? buses.spi->read_register_u8(reg, dst)
         : false;
}
auto MPU9250::akm_read_u16(Buses& buses, uint8_t reg, uint16_t& dst) -> bool
{
    return buses.i2c ? buses.i2c->read_register_u16(m_magnetic_field->akm_address, reg, dst)
         : buses.spi ? buses.spi->read_register_u16(reg, dst)
         : false;
}
auto MPU9250::akm_write_u8(Buses& buses, uint8_t reg, uint8_t t) -> bool
{
    return buses.i2c ? buses.i2c->write_register_u8(m_magnetic_field->akm_address, reg, t)
         : buses.spi ? buses.spi->write_register_u8(reg, t)
         : false;
}
auto MPU9250::akm_write_u16(Buses& buses, uint8_t reg, uint16_t t) -> bool
{
    return buses.i2c ? buses.i2c->write_register_u16(m_magnetic_field->akm_address, reg, t)
         : buses.spi ? buses.spi->write_register_u16(reg, t)
         : false;
}

auto MPU9250::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(4);
    outputs[0].class_id = q::rtti::get_class_id<stream::IAngular_Velocity>();
    outputs[0].name = "Angular Velocity";
    outputs[0].stream = m_angular_velocity;
    outputs[1].class_id = q::rtti::get_class_id<stream::IAcceleration>();
    outputs[1].name = "Acceleration";
    outputs[1].stream = m_acceleration;
    outputs[2].class_id = q::rtti::get_class_id<stream::IMagnetic_Field>();
    outputs[2].name = "Magnetic Field";
    outputs[2].stream = m_magnetic_field;
    outputs[3].class_id = q::rtti::get_class_id<stream::ITemperature>();
    outputs[3].name = "Temperature";
    outputs[3].stream = m_temperature;
    return outputs;
}
auto MPU9250::init(rapidjson::Value const& init_params) -> bool
{
    QLOG_TOPIC("mpu9250::init");

    sz::MPU9250::Init_Params sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, init_params, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize MPU9250 data: {}", ss.str());
        return false;
    }
    jsonutil::clone_value(m_init_paramsj, init_params, m_init_paramsj.GetAllocator());
    *m_init_params = sz;
    return init();
}


auto MPU9250::init() -> bool
{
    m_i2c = m_hal.get_buses().find_by_name<bus::II2C>(m_init_params->bus);
    m_spi = m_hal.get_buses().find_by_name<bus::ISPI>(m_init_params->bus);

    Buses buses = { m_i2c.lock(), m_spi.lock() };
    if (!buses.i2c && !buses.spi)
    {
        QLOGE("No bus configured");
        return false;
    }

    m_acceleration = std::make_shared<Acceleration>();
    m_angular_velocity = std::make_shared<Angular_Velocity>();
    m_magnetic_field = std::make_shared<Magnetic_Field>();
    m_temperature = std::make_shared<Temperature>();

    std::vector<size_t> g_ranges = { 250, 500, 1000, 2000 };
    std::vector<size_t> a_ranges = { 2, 4, 8, 16 };
    std::vector<size_t> imu_rates = { 250, 500, 1000 }; //, 2000, 4000, 8000 };

    auto req_params = *m_init_params;

    if (buses.i2c)
    {
        //max supported on a 400Khz i2c bus
        m_init_params->acceleration_angular_velocity_rate = math::min<size_t>(m_init_params->acceleration_angular_velocity_rate, 1000);
    }
    m_init_params->magnetic_field_rate = math::clamp<size_t>(m_init_params->magnetic_field_rate, 10, 100);
    m_init_params->temperature_rate = math::clamp<size_t>(m_init_params->temperature_rate, 10, 50);

    std::nth_element(g_ranges.begin(), g_ranges.begin(), g_ranges.end(), [&](size_t a, size_t b)
    {
        return math::abs(a - m_init_params->angular_velocity_range) < math::abs(b - m_init_params->angular_velocity_range);
    });
    m_init_params->angular_velocity_range = g_ranges.front();

    std::nth_element(a_ranges.begin(), a_ranges.begin(), a_ranges.end(), [&](size_t a, size_t b)
    {
        return math::abs(a - m_init_params->acceleration_range) < math::abs(b - m_init_params->acceleration_range);
    });
    m_init_params->acceleration_range = a_ranges.front();

    std::nth_element(imu_rates.begin(), imu_rates.begin(), imu_rates.end(), [&](size_t a, size_t b)
    {
        return math::abs(a - m_init_params->acceleration_angular_velocity_rate) < math::abs(b - m_init_params->acceleration_angular_velocity_rate);
    });
    m_init_params->acceleration_angular_velocity_rate = imu_rates.front();

    QLOGI("Probing MPU9250 on {}", m_init_params->bus);
    QLOGI("Angular Velocity range {} DPS (requested {} DPS)", m_init_params->angular_velocity_range, req_params.angular_velocity_range);
    QLOGI("Acceleration range {}G (requested {}G)", m_init_params->acceleration_range, req_params.acceleration_range);
    QLOGI("Acceleration & Angular Velocity Rate {}Hz (requested {}Hz)", m_init_params->acceleration_angular_velocity_rate, req_params.acceleration_angular_velocity_rate);
    QLOGI("Magnetic Field Rate {}Hz (requested {}Hz)", m_init_params->magnetic_field_rate, req_params.magnetic_field_rate);
    QLOGI("Temperature Rate {}Hz (requested {}Hz)", m_init_params->temperature_rate, req_params.temperature_rate);

    m_acceleration->rate = m_init_params->acceleration_angular_velocity_rate;
    m_angular_velocity->rate = m_acceleration->rate;
    m_magnetic_field->rate = m_init_params->magnetic_field_rate;
    m_temperature->rate = m_init_params->temperature_rate;

    uint8_t gyro_range = MPU_BIT_GYRO_FS_SEL_1000_DPS;
    switch (m_init_params->angular_velocity_range)
    {
    case 250:
        gyro_range = MPU_BIT_GYRO_FS_SEL_250_DPS;
        m_angular_velocity->scale_inv = math::radians(1.f) / (131.f);
        break;
    case 500:
        gyro_range = MPU_BIT_GYRO_FS_SEL_500_DPS;
        m_angular_velocity->scale_inv = math::radians(1.f) / (131.f / 2.f);
        break;
    case 1000:
        gyro_range = MPU_BIT_GYRO_FS_SEL_1000_DPS;
        m_angular_velocity->scale_inv = math::radians(1.f) / (131.f / 4.f);
        break;
    case 2000:
        gyro_range = MPU_BIT_GYRO_FS_SEL_2000_DPS;
        m_angular_velocity->scale_inv = math::radians(1.f) / (131.f / 8.f);
        break;
    default:
        QLOGE("Invalid angular velocity range: {}", m_init_params->angular_velocity_range);
        return false;
    }

    uint8_t accel_range = MPU_BIT_ACCEL_FS_SEL_8_G;
    switch (m_init_params->acceleration_range)
    {
    case 2:
        accel_range = MPU_BIT_ACCEL_FS_SEL_2_G;
        m_acceleration->scale_inv = physics::constants::g / 16384.f;
        break;
    case 4:
        accel_range = MPU_BIT_ACCEL_FS_SEL_4_G;
        m_acceleration->scale_inv = physics::constants::g / 8192.f;
        break;
    case 8:
        accel_range = MPU_BIT_ACCEL_FS_SEL_8_G;
        m_acceleration->scale_inv = physics::constants::g / 4096.f;
        break;
    case 16:
        accel_range = MPU_BIT_ACCEL_FS_SEL_16_G;
        m_acceleration->scale_inv = physics::constants::g / 2048.f;
        break;
    default:
        QLOGE("Invalid acceleration range: {}", m_init_params->acceleration_range);
        return false;
    }

    boost::this_thread::sleep_for(boost::chrono::milliseconds(120));

    auto res = mpu_write_u8(buses, MPU_REG_PWR_MGMT_1, MPU_BIT_H_RESET);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(120));

    res &= mpu_write_u8(buses, MPU_REG_PWR_MGMT_1, 0);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

    res &= mpu_write_u8(buses, MPU_REG_PWR_MGMT_1, MPU_BIT_CLKSEL_AUTO);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

    uint8_t who_am_i;
    res &= mpu_read_u8(buses, MPU_REG_WHO_AM_I, who_am_i);
    if (!res || who_am_i != 0x71)
    {
        QLOGE("Cannot find mpu9250");
#ifdef RASPBERRY_PI
        return false;
#endif
    }
    QLOGI("Found MPU9250 id: {x}", who_am_i);

    res &= mpu_write_u8(buses, MPU_REG_GYRO_CONFIG, gyro_range);
    res &= mpu_write_u8(buses, MPU_REG_ACCEL_CONFIG, accel_range);

    //res &= mpu_write_u8(MPU_REG_CONFIG, MPU_BIT_DLPF_CFG_20_1);
    res &= mpu_write_u8(buses, MPU_REG_CONFIG, MPU_BIT_DLPF_CFG_184_1);


    //compute the rate
    uint8_t div = 1000 / m_init_params->acceleration_angular_velocity_rate - 1;
    res &= mpu_write_u8(buses, MPU_REG_SMPLRT_DIV, div);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

    m_acceleration->dt = m_angular_velocity->dt = std::chrono::milliseconds(1000 / m_init_params->acceleration_angular_velocity_rate);
    m_magnetic_field->dt = std::chrono::milliseconds(1000 / m_init_params->magnetic_field_rate);
    m_temperature->dt = std::chrono::milliseconds(1000 / m_init_params->temperature_rate);

    res &= mpu_write_u8(buses, MPU_REG_PWR_MGMT_2, 0);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

    //res &= mpu_write_u8(MPU_REG_ACCEL_CONFIG2, MPU_BIT_FIFO_SIZE_4096 | 0x8 | MPU_BIT_A_DLPF_CFG_20_1);
    mpu_write_u8(buses, MPU_REG_ACCEL_CONFIG2, MPU_BIT_FIFO_SIZE_4096 | 0x8 | MPU_BIT_A_DLPF_CFG_460_1);

    boost::this_thread::sleep_for(boost::chrono::milliseconds(5));

    res &= mpu_write_u8(buses, MPU_REG_FIFO_EN, MPU_BIT_GYRO_XO_UT | MPU_BIT_GYRO_YO_UT | MPU_BIT_GYRO_ZO_UT | MPU_BIT_ACCEL);
    m_fifo_sample_size = 12;

    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
    res &= mpu_write_u8(buses, MPU_REG_USER_CTRL, USER_CTRL_VALUE);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));

    m_last_fifo_tp = q::Clock::now();

    res &= setup_compass(buses);
    if (!res)
    {
        QLOGE("Failed to setup mpu9250");
#ifdef RASPBERRY_PI
        return false;
#endif
    }

    return true;
}

auto MPU9250::setup_compass(Buses& buses) -> bool
{
    QLOG_TOPIC("mpu9250::setup_compass");

#ifdef USE_AK8963
    set_bypass(buses, 1);

    // Find compass. Possible addresses range from 0x0C to 0x0F.
    for (m_magnetic_field->akm_address = 0x0C; m_magnetic_field->akm_address <= 0x0F; m_magnetic_field->akm_address++)
    {
        uint8_t data;
        auto res = akm_read_u8(buses, AKM_REG_WHOAMI, data);
        if (res && data == AKM_WHOAMI)
        {
            break;
        }
    }

    if (m_magnetic_field->akm_address > 0x0F)
    {
        QLOGE("Compass not found.");
        return false;
    }

    QLOGI("Compass found at 0x{X}", m_magnetic_field->akm_address);

    akm_write_u8(buses, AKM_REG_CNTL, AKM_POWER_DOWN);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));

    akm_write_u8(buses, AKM_REG_CNTL, AKM_FUSE_ROM_ACCESS);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));

    // Get sensitivity adjustment data from fuse ROM.
    uint8_t data[4] = {0};
    akm_read(buses, AKM_REG_ASAX, data, 3);
    m_magnetic_field->magnetic_adj[0] = (long)(data[0] - 128)*0.5f / 128.f + 1.f;
    m_magnetic_field->magnetic_adj[1] = (long)(data[1] - 128)*0.5f / 128.f + 1.f;
    m_magnetic_field->magnetic_adj[2] = (long)(data[2] - 128)*0.5f / 128.f + 1.f;

    akm_write_u8(buses, AKM_REG_CNTL, AKM_POWER_DOWN);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));

    akm_write_u8(buses, AKM_REG_CNTL, AKM_CONTINUOUS2_MEASUREMENT | AKM_16_BIT_MODE);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));

//    mpu_set_bypass(0);

//    // Set up master mode, master clock, and ES bit.
//    mpu_write_u8(MPU_REG_I2C_MST_CTRL, MPU_BIT_WAIT_FOR_ES);

//    // Slave 0 reads from AKM data registers.
//    constexpr uint8_t MPU_BIT_I2C_READ = 0x80;
//    mpu_write_u8(MPU_REG_I2C_SLV0_ADDR, MPU_BIT_I2C_READ | m_compass_addr);

//    // Compass reads start at this register.
//    mpu_write_u8(MPU_REG_I2C_SLV0_REG, AKM_REG_ST1);

//    // Enable slave 0, 8-byte reads.
//    mpu_write_u8(MPU_REG_I2C_SLV0_CTRL, MPU_BIT_I2C_SLV0_EN | 8);

//    // Slave 1 changes AKM measurement mode.
//    mpu_write_u8(MPU_REG_I2C_SLV1_ADDR, m_compass_addr);

//    // AKM measurement mode register.
//    mpu_write_u8(MPU_REG_I2C_SLV1_REG, AKM_REG_CNTL);

//    // Enable slave 1, 1-byte writes.
//    mpu_write_u8(MPU_REG_I2C_SLV1_CTRL, MPU_BIT_I2C_SLV1_EN | 1);

//    // Set slave 1 data.
//    mpu_write_u8(MPU_REG_I2C_SLV1_DO, AKM_SINGLE_MEASUREMENT);

//    // Trigger slave 0 and slave 1 actions at each sample.
//    mpu_write_u8(MPU_REG_I2C_MST_DELAY_CTRL, MPU_BIT_I2C_SLV0_DLY_EN | MPU_BIT_I2C_SLV1_DLY_EN);

//    {
//        uint8_t div = m_sample_rate / 10 - 1;
//        mpu_write_u8(MPU_REG_I2C_SLV4_CTRL, div);
//    }

//    mpu_write_u8(MPU_REG_I2C_SLV1_DO, AKM_SINGLE_MEASUREMENT);

#endif

    m_magnetic_field->last_tp = q::Clock::now();
    return true;
}

void MPU9250::set_bypass(Buses& buses, bool on)
{
    QLOG_TOPIC("mpu9250::set_bypass");

    if (on)
    {
        uint8_t tmp;
        mpu_read_u8(buses, MPU_REG_USER_CTRL, tmp);
        tmp &= ~MPU_BIT_I2C_MST;
        mpu_write_u8(buses, MPU_REG_USER_CTRL, tmp);
        boost::this_thread::sleep_for(boost::chrono::milliseconds(3));
        tmp = MPU_BIT_BYPASS_EN;
        mpu_write_u8(buses, MPU_REG_INT_PIN_CFG, tmp);
    }
    else
    {
        // Enable I2C master mode if compass is being used.
        uint8_t tmp;
        mpu_read_u8(buses, MPU_REG_USER_CTRL, tmp);
        tmp |= MPU_BIT_I2C_MST;
        mpu_write_u8(buses, MPU_REG_USER_CTRL, tmp);
        boost::this_thread::sleep_for(boost::chrono::milliseconds(3));
        tmp = 0;
        mpu_write_u8(buses, MPU_REG_INT_PIN_CFG, tmp);
    }
}

void MPU9250::reset_fifo(Buses& buses)
{
    QLOG_TOPIC("mpu9250::reset_fifo");

    mpu_write_u8(buses, MPU_REG_USER_CTRL, USER_CTRL_VALUE);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
    mpu_write_u8(buses, MPU_REG_FIFO_EN, 0);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
    mpu_write_u8(buses, MPU_REG_USER_CTRL, USER_CTRL_VALUE);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
    mpu_write_u8(buses, MPU_REG_FIFO_EN, MPU_BIT_GYRO_XO_UT | MPU_BIT_GYRO_YO_UT | MPU_BIT_GYRO_ZO_UT | MPU_BIT_ACCEL);
}

void MPU9250::process()
{
    QLOG_TOPIC("mpu9250::process");

    Buses buses = { m_i2c.lock(), m_spi.lock() };
    if (!buses.i2c && !buses.spi)
    {
        return;
    }

    m_acceleration->samples.clear();
    m_angular_velocity->samples.clear();
    m_magnetic_field->samples.clear();
    m_temperature->samples.clear();

    //auto now = q::Clock::now();
    //static q::Clock::time_point last_timestamp = q::Clock::now();
   // auto dt = now - last_timestamp;
   // last_timestamp = now;

    uint16_t fifo_count;
    auto res = mpu_read_u16(buses, MPU_REG_FIFO_COUNTH, fifo_count);

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
            if (mpu_read(buses, MPU_REG_FIFO_R_W, m_fifo_buffer.data(), m_fifo_buffer.size()))
            {
                auto now = q::Clock::now();
                auto total_dt = now - m_last_fifo_tp;
                auto dt = total_dt / sample_count;
                auto tp = m_last_fifo_tp + dt;
                m_last_fifo_tp = now;

                m_angular_velocity->samples.resize(sample_count);
                m_acceleration->samples.resize(sample_count);
                auto* data = m_fifo_buffer.data();
                for (size_t i = 0; i < sample_count; i++)
                {
                    short x = (data[0] << 8) | data[1]; data += 2;
                    short y = (data[0] << 8) | data[1]; data += 2;
                    short z = (data[0] << 8) | data[1]; data += 2;
                    auto& asample = m_acceleration->samples[i];
                    asample.value.set(x * m_acceleration->scale_inv, y * m_acceleration->scale_inv, z * m_acceleration->scale_inv);
                    asample.sample_idx = ++m_acceleration->last_sample.sample_idx;
                    asample.tp = tp;
                    asample.dt = dt;

                    x = (data[0] << 8) | data[1]; data += 2;
                    y = (data[0] << 8) | data[1]; data += 2;
                    z = (data[0] << 8) | data[1]; data += 2;

                    auto& gsample = m_angular_velocity->samples[i];
                    gsample.value.set(x * m_angular_velocity->scale_inv, y * m_angular_velocity->scale_inv, z * m_angular_velocity->scale_inv);
                    gsample.sample_idx = ++m_angular_velocity->last_sample.sample_idx;
                    asample.tp = tp;
                    gsample.dt = dt;

                    tp += dt;

//                    if (math::length(m_samples.angular_velocity[i]) > 1.f)
//                    {
//                        LOG_ERR("XXX::: gyro: {}, acc: {}", m_samples.angular_velocity[i], m_samples.acceleration[i]);
//                    }
                }
            }

            if (!m_acceleration->samples.empty())
            {
                m_acceleration->last_sample = m_acceleration->samples.back();
            }
            if (!m_angular_velocity->samples.empty())
            {
                m_angular_velocity->last_sample = m_angular_velocity->samples.back();
            }
        }
    }

    process_compass(buses);

//    LOG_INFO("fc {} / {}", fifo_count, fc2);
}

void MPU9250::process_compass(Buses& buses)
{
    QLOG_TOPIC("mpu9250::process_compass");

#ifdef USE_AK8963
    auto now = q::Clock::now();
    if (now - m_magnetic_field->last_tp < m_magnetic_field->dt)
    {
        return;
    }

    {
        math::vec3f xxx_output = math::vec3f::zero;
        {
            static q::Clock::duration s_dt(0);
            s_dt += m_magnetic_field->dt;
            static const float noise = 0.1f;
            std::vector<std::pair<float, float>> freq =
            {{
                 { 10.f, 1.f },
                 { 17.f, 0.7f },
                 { 33.f, 0.5f }
             }};
            static std::uniform_real_distribution<float> distribution(-noise, noise); //Values between 0 and 2
            static std::mt19937 engine; // Mersenne twister MT19937
            auto generator = std::bind(distribution, engine);
            {
                xxx_output = math::vec3f::zero;
                float a = q::Seconds(s_dt).count() * math::anglef::_2pi;
                xxx_output.x += math::sin(a * freq[0].first) * freq[0].second + generator();
                xxx_output.y += math::sin(a * freq[1].first) * freq[1].second + generator();
                xxx_output.z += math::sin(a * freq[2].first) * freq[2].second + generator();
            }
        }

        Magnetic_Field::Sample& sample = m_magnetic_field->last_sample;
        sample.value = xxx_output; //REMOVE ME
        sample.sample_idx++;
        sample.tp = now;
        sample.dt = m_magnetic_field->dt;

        m_magnetic_field->samples.push_back(sample);
    }

    std::array<uint8_t, 8> tmp;
    if (!akm_read(buses, AKM_REG_ST1, tmp.data(), tmp.size()))
    {
        return;
    }
    if (!(tmp[0] & AKM_DATA_READY))
    {
        return;
    }
    if ((tmp[0] & AKM_DATA_OVERRUN))
    {
        return;
    }
    if (tmp[7] & AKM_OVERFLOW)
    {
        return;
    }

    auto dt = now - m_magnetic_field->last_tp;
    m_magnetic_field->last_tp = now;

    short data[3];
    data[0] = (tmp[2] << 8) | tmp[1];
    data[1] = (tmp[4] << 8) | tmp[3];
    data[2] = (tmp[6] << 8) | tmp[5];

    data[0] = data[0] * m_magnetic_field->magnetic_adj[0];
    data[1] = data[1] * m_magnetic_field->magnetic_adj[1];
    data[2] = data[2] * m_magnetic_field->magnetic_adj[2];

    //change of axis according to the specs. By default the compass has front X, right Y and down Z
    static const math::quatf rot = math::quatf::from_axis_y(math::radians(180.f)) *
            math::quatf::from_axis_z(math::radians(90.f));
    math::vec3f c(data[0], data[1], data[2]);
    c *= 0.15f;//16 bit mode

    Magnetic_Field::Sample& sample = m_magnetic_field->last_sample;
    sample.value = math::rotate(rot, c);
    sample.sample_idx++;
    sample.tp = now;
    sample.dt = dt;

    m_magnetic_field->samples.push_back(sample);
//    LOG_INFO("c: {}", *m_samples.compass);
#endif
}

auto MPU9250::set_config(rapidjson::Value const& json) -> bool
{
    QLOG_TOPIC("mpu9250::set_config");

    sz::MPU9250::Config sz;
    autojsoncxx::error::ErrorStack result;
    if (!autojsoncxx::from_value(sz, json, result))
    {
        std::ostringstream ss;
        ss << result;
        QLOGE("Cannot deserialize MPU9250 config data: {}", ss.str());
        return false;
    }

    *m_config = sz;
    return true;
}
auto MPU9250::get_config() const -> rapidjson::Document
{
    rapidjson::Document json;
    autojsoncxx::to_document(*m_config, json);
    return std::move(json);
}

auto MPU9250::get_init_params() const -> rapidjson::Document const&
{
    return m_init_paramsj;
}


}
}
