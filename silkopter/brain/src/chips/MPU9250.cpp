#include "BrainStdAfx.h"
#include "MPU9250.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#ifdef RASPBERRY_PI


#define USE_AK8963

using namespace silk;

static const uint8_t ADDR_MPU9250                       = 0x68;


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


auto MPU9250::init(const q::String& device, Gyroscope_Range gr, Accelerometer_Range ar) -> bool
{
    m_gyroscope_rate = gr;
    m_accelerometer_range = ar;

    SILK_INFO("initializing device: {}", device);

    if (!m_i2c.open(device.c_str()))
    {
        SILK_ERR("can't open {}: {}", device, strerror(errno));
        return false;
    }

    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_PWR_MGMT_1, MPU_BIT_H_RESET);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(120));

    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_PWR_MGMT_1, 0);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_PWR_MGMT_1, MPU_BIT_CLKSEL_AUTO);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

    auto who_am_i = m_i2c.read_u8(ADDR_MPU9250, MPU_REG_WHO_AM_I);
    SILK_INFO("device {} id: {x}", device, who_am_i);
    if (who_am_i != 0x71)
    {
        return false;
    }

    uint8_t gyro_range = MPU_BIT_GYRO_FS_SEL_1000_DPS;
    switch (gr)
    {
    case Gyroscope_Range::_250_DPS:
        gyro_range = MPU_BIT_GYRO_FS_SEL_250_DPS;
        m_gyroscope_scale_inv = math::radians(1.f) / (131.f);
        break;
    case Gyroscope_Range::_500_DPS:
        gyro_range = MPU_BIT_GYRO_FS_SEL_500_DPS;
        m_gyroscope_scale_inv = math::radians(1.f) / (131.f / 2.f);
        break;
    case Gyroscope_Range::_1000_DPS:
        gyro_range = MPU_BIT_GYRO_FS_SEL_1000_DPS;
        m_gyroscope_scale_inv = math::radians(1.f) / (131.f / 4.f);
        break;
    case Gyroscope_Range::_2000_DPS:
        gyro_range = MPU_BIT_GYRO_FS_SEL_2000_DPS;
        m_gyroscope_scale_inv = math::radians(1.f) / (131.f / 8.f);
        break;
    default:
        SILK_ERR("Invalid gyroscope range.");
        return false;
    }

    uint8_t accel_range = MPU_BIT_ACCEL_FS_SEL_8_G;
    switch (ar)
    {
    case Accelerometer_Range::_2_G:
        accel_range = MPU_BIT_ACCEL_FS_SEL_2_G;
        m_accelerometer_scale_inv = physics::constants::g / 16384.f;
        break;
    case Accelerometer_Range::_4_G:
        accel_range = MPU_BIT_ACCEL_FS_SEL_4_G;
        m_accelerometer_scale_inv = physics::constants::g / 8192.f;
        break;
    case Accelerometer_Range::_8_G:
        accel_range = MPU_BIT_ACCEL_FS_SEL_8_G;
        m_accelerometer_scale_inv = physics::constants::g / 4096.f;
        break;
    case Accelerometer_Range::_16_G:
        accel_range = MPU_BIT_ACCEL_FS_SEL_16_G;
        m_accelerometer_scale_inv = physics::constants::g / 2048.f;
        break;
    }

    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_GYRO_CONFIG, gyro_range);
    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_ACCEL_CONFIG, accel_range);

    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_CONFIG, MPU_BIT_DLPF_CFG_20_1);
    //m_i2c.write_u8(ADDR_MPU9250, MPU_REG_CONFIG, MPU_BIT_DLPF_CFG_184_1);


    //compute the rate
    m_sample_rate = 1000;
    QASSERT(m_sample_rate >= 100 && m_sample_rate <= 1000);
    uint8_t rate = 1000 / m_sample_rate - 1;
    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_SMPLRT_DIV, rate);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

    m_sample_time = std::chrono::milliseconds(1000 / m_sample_rate);

    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_PWR_MGMT_2, 0);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_ACCEL_CONFIG2, MPU_BIT_FIFO_SIZE_4096 | 0x8 | MPU_BIT_A_DLPF_CFG_20_1);
    //m_i2c.write_u8(ADDR_MPU9250, MPU_REG_ACCEL_CONFIG2, MPU_BIT_FIFO_SIZE_4096 | 0x8 | MPU_BIT_A_DLPF_CFG_460_1);

    boost::this_thread::sleep_for(boost::chrono::milliseconds(5));

    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_FIFO_EN, MPU_BIT_GYRO_XO_UT | MPU_BIT_GYRO_YO_UT | MPU_BIT_GYRO_ZO_UT | MPU_BIT_ACCEL);
    m_fifo_sample_size = 12;

    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_USER_CTRL, MPU_BIT_FIFO_EN | MPU_BIT_FIFO_RST/* | MPU_BIT_I2C_MST*/);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(100));

    setup_compass();

    SILK_INFO("using MPU9250 device: {}", device);

//    while (true)
//    {
//        process();
//        boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
//    }

    return true;
}

auto MPU9250::setup_compass() -> bool
{
#ifdef USE_AK8963
    mpu_set_bypass(1);

    // Find compass. Possible addresses range from 0x0C to 0x0F.
    uint8_t akm_addr = 0;
    for (akm_addr = 0x0C; akm_addr <= 0x0F; akm_addr++)
    {
        auto data = m_i2c.read_u8(akm_addr, AKM_REG_WHOAMI);
        if (data == AKM_WHOAMI)
        {
            break;
        }
    }

    if (akm_addr > 0x0F)
    {
        SILK_ERR("Compass not found.");
        return false;
    }

    m_compass_addr = akm_addr;
    SILK_INFO("Compass found at 0x{X}", m_compass_addr);

    m_i2c.write_u8(m_compass_addr, AKM_REG_CNTL, AKM_POWER_DOWN);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));

    m_i2c.write_u8(m_compass_addr, AKM_REG_CNTL, AKM_FUSE_ROM_ACCESS);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));

    // Get sensitivity adjustment data from fuse ROM.
    uint8_t data[4] = {0};
    m_i2c.read(m_compass_addr, AKM_REG_ASAX, data, 3);
    m_magnetic_adj[0] = (long)(data[0] - 128)*0.5f / 128.f + 1.f;
    m_magnetic_adj[1] = (long)(data[1] - 128)*0.5f / 128.f + 1.f;
    m_magnetic_adj[2] = (long)(data[2] - 128)*0.5f / 128.f + 1.f;

    m_i2c.write_u8(m_compass_addr, AKM_REG_CNTL, AKM_POWER_DOWN);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));

    m_i2c.write_u8(m_compass_addr, AKM_REG_CNTL, AKM_CONTINUOUS2_MEASUREMENT | AKM_16_BIT_MODE);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(1));

//    mpu_set_bypass(0);

//    // Set up master mode, master clock, and ES bit.
//    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_I2C_MST_CTRL, MPU_BIT_WAIT_FOR_ES);

//    // Slave 0 reads from AKM data registers.
//    constexpr uint8_t MPU_BIT_I2C_READ = 0x80;
//    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_I2C_SLV0_ADDR, MPU_BIT_I2C_READ | m_compass_addr);

//    // Compass reads start at this register.
//    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_I2C_SLV0_REG, AKM_REG_ST1);

//    // Enable slave 0, 8-byte reads.
//    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_I2C_SLV0_CTRL, MPU_BIT_I2C_SLV0_EN | 8);

//    // Slave 1 changes AKM measurement mode.
//    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_I2C_SLV1_ADDR, m_compass_addr);

//    // AKM measurement mode register.
//    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_I2C_SLV1_REG, AKM_REG_CNTL);

//    // Enable slave 1, 1-byte writes.
//    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_I2C_SLV1_CTRL, MPU_BIT_I2C_SLV1_EN | 1);

//    // Set slave 1 data.
//    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_I2C_SLV1_DO, AKM_SINGLE_MEASUREMENT);

//    // Trigger slave 0 and slave 1 actions at each sample.
//    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_I2C_MST_DELAY_CTRL, MPU_BIT_I2C_SLV0_DLY_EN | MPU_BIT_I2C_SLV1_DLY_EN);

//    {
//        uint8_t div = m_sample_rate / 10 - 1;
//        m_i2c.write_u8(ADDR_MPU9250, MPU_REG_I2C_SLV4_CTRL, div);
//    }

//    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_I2C_SLV1_DO, AKM_SINGLE_MEASUREMENT);

    return true;
#else
    return true;
#endif
}

void MPU9250::mpu_set_bypass(bool on)
{
    if (on)
    {
        auto tmp = m_i2c.read_u8(ADDR_MPU9250, MPU_REG_USER_CTRL);
        tmp &= ~MPU_BIT_I2C_MST;
        m_i2c.write_u8(ADDR_MPU9250, MPU_REG_USER_CTRL, tmp);
        boost::this_thread::sleep_for(boost::chrono::milliseconds(3));
        tmp = MPU_BIT_BYPASS_EN;
        m_i2c.write_u8(ADDR_MPU9250, MPU_REG_INT_PIN_CFG, tmp);
    }
    else
    {
        // Enable I2C master mode if compass is being used.
        auto tmp = m_i2c.read_u8(ADDR_MPU9250, MPU_REG_USER_CTRL);
        tmp |= MPU_BIT_I2C_MST;
        m_i2c.write_u8(ADDR_MPU9250, MPU_REG_USER_CTRL, tmp);
        boost::this_thread::sleep_for(boost::chrono::milliseconds(3));
        tmp = 0;
        m_i2c.write_u8(ADDR_MPU9250, MPU_REG_INT_PIN_CFG, tmp);
    }
}


auto MPU9250::read_compass() -> boost::optional<math::vec3f>
{
    auto r = m_samples.compass;
    m_samples.compass.reset();
    return r;
}

void MPU9250::reset_fifo()
{
    m_i2c.write_u8(ADDR_MPU9250, MPU_REG_USER_CTRL, MPU_BIT_FIFO_EN | MPU_BIT_FIFO_RST/* | MPU_BIT_I2C_MST*/);
}

void MPU9250::process()
{
    m_samples.gyroscope.clear();
    m_samples.accelerometer.clear();

//    i2c_acquire(ADDR_MPU9250);

    auto fifo_count = m_i2c.read_u16(ADDR_MPU9250, MPU_REG_FIFO_COUNTH);
    uint16_t fc2 = 0;
    if (fifo_count >= m_fifo_sample_size)
    {
        if (fifo_count >= 2048)
        {
            SILK_WARNING("resetting fifo");
            reset_fifo();
            fifo_count = 0;
        }
        else
        {
            auto sample_count = fifo_count / m_fifo_sample_size;
            auto to_read = sample_count * m_fifo_sample_size;

            m_fifo_buffer.resize(to_read);
            auto* data = m_fifo_buffer.data();

            m_i2c.read(ADDR_MPU9250, MPU_REG_FIFO_R_W, data, to_read);

            //reset_fifo();
            //fc2 = m_i2c.read_u16(ADDR_MPU9250, MPU_REG_FIFO_COUNTH);

            QASSERT(sample_count >= 1);
            m_samples.gyroscope.resize(sample_count);
            m_samples.accelerometer.resize(sample_count);
            size_t off = 0;
            for (size_t i = 0; i < sample_count; i++)
            {
                {
                    short x = (data[off] << 8) | data[off + 1]; off += 2;
                    short y = (data[off] << 8) | data[off + 1]; off += 2;
                    short z = (data[off] << 8) | data[off + 1]; off += 2;
                    m_samples.accelerometer[i].set(x * m_accelerometer_scale_inv, y * m_accelerometer_scale_inv, z * m_accelerometer_scale_inv);
                }

                {
                    short x = (data[off] << 8) | data[off + 1]; off += 2;
                    short y = (data[off] << 8) | data[off + 1]; off += 2;
                    short z = (data[off] << 8) | data[off + 1]; off += 2;
                    m_samples.gyroscope[i].set(x * m_gyroscope_scale_inv, y * m_gyroscope_scale_inv, z * m_gyroscope_scale_inv);
                }
            }
        }
    }

    process_compass();

//    SILK_INFO("fc {} / {}", fifo_count, fc2);
}

void MPU9250::process_compass()
{
#ifdef USE_AK8963
    auto now = q::Clock::now();
    if (now - m_last_compass_timestamp < std::chrono::milliseconds(10))
    {
        return;
    }

    uint8_t tmp[8];
    m_i2c.read(m_compass_addr, AKM_REG_ST1, tmp, 8);

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

    m_last_compass_timestamp = now;

    short data[3];
    data[0] = (tmp[2] << 8) | tmp[1];
    data[1] = (tmp[4] << 8) | tmp[3];
    data[2] = (tmp[6] << 8) | tmp[5];

    data[0] = data[0] * m_magnetic_adj[0];
    data[1] = data[1] * m_magnetic_adj[1];
    data[2] = data[2] * m_magnetic_adj[2];

    //change of axis according to the specs. By default the compass has front X, right Y and down Z
    static const math::quatf rot = math::quatf::from_axis_y(math::radians(180.f)) *
            math::quatf::from_axis_z(math::radians(90.f));
    math::vec3f c(data[0], data[1], data[2]);
    c *= 0.15f;//16 bit mode
    m_samples.compass = math::rotate(rot, c);
//    SILK_INFO("c: {}", *m_samples.compass);
#endif
}

auto MPU9250::get_gyroscope_samples() const -> std::vector<math::vec3f> const&
{
    return m_samples.gyroscope;
}
auto MPU9250::get_accelerometer_samples() const -> std::vector<math::vec3f> const&
{
    return m_samples.accelerometer;
}

q::Clock::duration MPU9250::get_sample_time() const
{
    return m_sample_time;
}

#endif
