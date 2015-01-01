#include "BrainStdAfx.h"
#include "OdroidW_ADC.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#ifdef RASPBERRY_PI

extern "C"
{
    #include "pigpio.h"
}


namespace silk
{

struct Mode_Guard
{
    Mode_Guard()
    {
        gpioSetMode(28, PI_INPUT);
        gpioSetMode(29, PI_INPUT);
        gpioSetMode(0, PI_ALT0);
        gpioSetMode(1, PI_ALT0);
    }
    ~Mode_Guard()
    {
        gpioSetMode(28, PI_ALT0);
        gpioSetMode(29, PI_ALT0);
        gpioSetMode(0, PI_INPUT);
        gpioSetMode(1, PI_INPUT);
    }
};


constexpr uint8_t ADDR = 0x32;

constexpr uint8_t RC5T619_PWR_WD			= 0x0B;
constexpr uint8_t RC5T619_PWR_WD_COUNT		= 0x0C;
constexpr uint8_t RC5T619_PWR_FUNC          = 0x0D;
constexpr uint8_t RC5T619_PWR_SLP_CNT		= 0x0E;
constexpr uint8_t RC5T619_PWR_REP_CNT		= 0x0F;
constexpr uint8_t RC5T619_PWR_ON_TIMSET		= 0x10;
constexpr uint8_t RC5T619_PWR_NOE_TIMSET	= 0x11;
constexpr uint8_t RC5T619_PWR_IRSEL         = 0x15;

/* Interrupt enable register */
constexpr uint8_t RC5T619_INT_EN_SYS		= 0x12;
constexpr uint8_t RC5T619_INT_EN_DCDC		= 0x40;
constexpr uint8_t RC5T619_INT_EN_RTC		= 0xAE;
constexpr uint8_t RC5T619_INT_EN_ADC1		= 0x88;
constexpr uint8_t RC5T619_INT_EN_ADC2		= 0x89;
constexpr uint8_t RC5T619_INT_EN_ADC3		= 0x8A;
constexpr uint8_t RC5T619_INT_EN_GPIO		= 0x94;
constexpr uint8_t RC5T619_INT_EN_GPIO2		= 0x94; // dummy
constexpr uint8_t RC5T619_INT_MSK_CHGCTR	= 0xBE;
constexpr uint8_t RC5T619_INT_MSK_CHGSTS1	= 0xBF;
constexpr uint8_t RC5T619_INT_MSK_CHGSTS2	= 0xC0;
constexpr uint8_t RC5T619_INT_MSK_CHGERR	= 0xC1;
constexpr uint8_t RC5T619_INT_MSK_CHGEXTIF	= 0xD1;

/* interrupt status registers (monitor regs)*/
constexpr uint8_t RC5T619_INTC_INTPOL		 = 0x9C;
constexpr uint8_t RC5T619_INTC_INTEN		 = 0x9D;
constexpr uint8_t RC5T619_INTC_INTMON		 = 0x9E;

constexpr uint8_t RC5T619_INT_MON_SYS		 = 0x14;
constexpr uint8_t RC5T619_INT_MON_DCDC		 = 0x42;
constexpr uint8_t RC5T619_INT_MON_RTC		 = 0xAF;

constexpr uint8_t RC5T619_INT_MON_CHGCTR	 = 0xC6;
constexpr uint8_t RC5T619_INT_MON_CHGSTS1	 = 0xC7;
constexpr uint8_t RC5T619_INT_MON_CHGSTS2	 = 0xC8;
constexpr uint8_t RC5T619_INT_MON_CHGERR	 = 0xC9;
constexpr uint8_t RC5T619_INT_MON_CHGEXTIF	 = 0xD3;


// GPIO register base address
constexpr uint8_t RC5T619_GPIO_IOSEL	 = 0x90;
constexpr uint8_t RC5T619_GPIO_IOOUT	 = 0x91;
constexpr uint8_t RC5T619_GPIO_GPEDGE1	 = 0x92;
constexpr uint8_t RC5T619_GPIO_GPEDGE2	 = 0x93;
//constexpr uint8_t RC5T619_GPIO_EN_GPIR	 = 0x94;
//constexpr uint8_t RC5T619_GPIO_IR_GPR	 = 0x95;
//constexpr uint8_t RC5T619_GPIO_IR_GPF	 = 0x96;
constexpr uint8_t RC5T619_GPIO_MON_IOIN	 = 0x97;
constexpr uint8_t RC5T619_GPIO_LED_FUNC	 = 0x98;

constexpr uint8_t RC5T619_REG_BANKSEL	 = 0xFF;

// Charger Control register
constexpr uint8_t RC5T619_CHG_CTL1	 = 0xB3;
constexpr uint8_t TIMSET_REG		 = 0xB9;

// ADC Control register
constexpr uint8_t RC5T619_ADC_CNT1	 = 0x64;
constexpr uint8_t RC5T619_ADC_CNT2	 = 0x65;
constexpr uint8_t RC5T619_ADC_CNT3	 = 0x66;
constexpr uint8_t RC5T619_ADC_VADP_THL	 = 0x7C;
constexpr uint8_t RC5T619_ADC_VSYS_THL	 = 0x80;

constexpr uint8_t RC5T619_FG_CTRL	 = 0xE0;
constexpr uint8_t RC5T619_PSWR		 = 0x07;

constexpr uint8_t RC5T619_AIN1_DATAH	 = 0x74;
constexpr uint8_t RC5T619_AIN1_DATAL	 = 0x75;
constexpr uint8_t RC5T619_AIN0_DATAH	 = 0x76;
constexpr uint8_t RC5T619_AIN0_DATAL	 = 0x77;


constexpr uint8_t CONVERT_ADC0           = 0x17;
constexpr uint8_t CONVERT_ADC1           = 0x16;

auto OdroidW_ADC::init() -> bool
{
    std::string device("/dev/i2c-0");
    SILK_INFO("initializing device: {}", device);

    if (!m_i2c.open(device))
    {
        SILK_ERR("can't open {}: {}", device, strerror(errno));
        return false;
    }

    Mode_Guard mg;

    uint8_t control;
    auto res = m_i2c.read_u8(ADDR, 0x36, control);
    if (!res || control == 0xff)
    {
        SILK_ERR("rc5t619 not found");
        return false;
    }
    SILK_INFO("rc5t619 found: {}", control);

    // Set ADRQ=00 to stop ADC
    res &= m_i2c.write_u8(ADDR, RC5T619_ADC_CNT3, 0x0);

    // Set ADC auto conversion interval 250ms
    res &= m_i2c.write_u8(ADDR, RC5T619_ADC_CNT2, 0x0);

    // Enable AIN0, AIN1 pin conversion in auto-ADC
    res &= m_i2c.write_u8(ADDR, RC5T619_ADC_CNT1, 0xC0);

    // Start auto-mode & average 4-time conversion mode for ADC
    res &= m_i2c.write_u8(ADDR, RC5T619_ADC_CNT3, 0x17);

    //start by converting voltage first
    res &= m_i2c.write_u8(ADDR, RC5T619_ADC_CNT3, CONVERT_ADC1);
    if (!res)
    {
        SILK_INFO("Failed to init rc5t619");
        return false;
    }

    return true;
}


void OdroidW_ADC::process()
{
    auto now = q::Clock::now();
    if (now - m_last_time_point < std::chrono::milliseconds(20))
    {
        return;
    }

    m_last_time_point = now;

    constexpr size_t CURRENT_TO_VOLTAGE_RATIO = 10;


//    SILK_INFO("ADC{} : {}:{} -> {}", idx, buf[0], buf[1], result);

    // Stop AD conversion */
    //m_i2c.write_u8(ADDR, RC5T619_ADC_CNT3, uint8_t(0x00));

    Mode_Guard mg;

    std::array<uint8_t, 2> buf;
    if (m_stage == 0)
    {
        //read coltage

        if (m_i2c.read(ADDR, RC5T619_AIN1_DATAH, buf.data(), buf.size()))
        {
            int r = (unsigned int)(buf[0] << 4) | (buf[1]&0xf);
            auto result =  math::clamp(static_cast<float>(r) / 4095.f, 0.f, 1.f);
            m_adc_voltage.data = { result, now - m_adc_voltage.last_time_point };
            m_adc_voltage.last_time_point = now;
        }

        //next
        if (m_i2c.write_u8(ADDR, RC5T619_ADC_CNT3, CONVERT_ADC0)) //read current next
        {
            m_stage++;
        }
    }
    else
    {
        //read current

        if (m_i2c.read(ADDR, RC5T619_AIN0_DATAH, buf.data(), buf.size()))
        {
            int r = (unsigned int)(buf[0] << 4) | (buf[1]&0xf);
            auto result =  math::clamp(static_cast<float>(r) / 4095.f, 0.f, 1.f);
            m_adc_current.data = { result, now - m_adc_current.last_time_point };
            m_adc_current.last_time_point = now;
        }

        //next
        if (m_stage >= CURRENT_TO_VOLTAGE_RATIO)
        {
            if (m_i2c.write_u8(ADDR, RC5T619_ADC_CNT3, CONVERT_ADC1)) //read voltage next
            {
                m_stage = 0;
            }
        }
        else if (m_i2c.write_u8(ADDR, RC5T619_ADC_CNT3, CONVERT_ADC0)) //read current next
        {
            m_stage++;
        }
    }
}

auto OdroidW_ADC::get_current_data() -> boost::optional<Data>
{
    auto res = m_adc_current.data;
    m_adc_current.data.reset();
    return res;
}
auto OdroidW_ADC::get_voltage_data() -> boost::optional<Data>
{
    auto res = m_adc_voltage.data;
    m_adc_voltage.data.reset();
    return res;
}




}


#endif
