#include "BrainStdAfx.h"
#include "OdroidW_ADC.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#ifdef RASPBERRY_PI


namespace silk
{


constexpr uint8_t ADDR = 0x32;

/* Maximum number of main interrupts */
constexpr uint8_t MAX_INTERRUPT_MASKS = 13;
constexpr uint8_t MAX_MAIN_INTERRUPT = 7;
constexpr uint8_t MAX_GPEDGE_REG	 = 2;

/* Power control register */
constexpr uint8_t RC5T619_PWR_WD		 = 0x0B;
constexpr uint8_t RC5T619_PWR_WD_COUNT	 = 0x0C;
constexpr uint8_t RC5T619_PWR_FUNC	 = 0x0D;
constexpr uint8_t RC5T619_PWR_SLP_CNT	 = 0x0E;
constexpr uint8_t RC5T619_PWR_REP_CNT	 = 0x0F;
constexpr uint8_t RC5T619_PWR_ON_TIMSET	 = 0x10;
constexpr uint8_t RC5T619_PWR_NOE_TIMSET	 = 0x11;
//constexpr uint8_t RC5T619_PWR_IRSEL	 = 0x15;

/* Interrupt enable register */
constexpr uint8_t RC5T619_INT_EN_SYS	 = 0x12;
constexpr uint8_t RC5T619_INT_EN_DCDC	 = 0x40;
constexpr uint8_t RC5T619_INT_EN_RTC	 = 0xAE;
constexpr uint8_t RC5T619_INT_EN_ADC1	 = 0x88;
constexpr uint8_t RC5T619_INT_EN_ADC2	 = 0x89;
constexpr uint8_t RC5T619_INT_EN_ADC3	 = 0x8A;
constexpr uint8_t RC5T619_INT_EN_GPIO	 = 0x94;
constexpr uint8_t RC5T619_INT_EN_GPIO2	 = 0x94; // dummy
constexpr uint8_t RC5T619_INT_MSK_CHGCTR	 = 0xBE;
constexpr uint8_t RC5T619_INT_MSK_CHGSTS1 = 0xBF;
constexpr uint8_t RC5T619_INT_MSK_CHGSTS2 = 0xC0;
constexpr uint8_t RC5T619_INT_MSK_CHGERR	 = 0xC1;
constexpr uint8_t RC5T619_INT_MSK_CHGEXTIF = 0xD1;

/* Interrupt select register */
constexpr uint8_t RC5T619_PWR_IRSEL		 = 0x15;
constexpr uint8_t RC5T619_CHG_CTRL_DETMOD1 = 0xCA;
constexpr uint8_t RC5T619_CHG_CTRL_DETMOD2 = 0xCB;
constexpr uint8_t RC5T619_CHG_STAT_DETMOD1 = 0xCC;
constexpr uint8_t RC5T619_CHG_STAT_DETMOD2 = 0xCD;
constexpr uint8_t RC5T619_CHG_STAT_DETMOD3 = 0xCE;


/* interrupt status registers (monitor regs)*/
constexpr uint8_t RC5T619_INTC_INTPOL	 = 0x9C;
constexpr uint8_t RC5T619_INTC_INTEN	 = 0x9D;
constexpr uint8_t RC5T619_INTC_INTMON	 = 0x9E;

constexpr uint8_t RC5T619_INT_MON_SYS	 = 0x14;
constexpr uint8_t RC5T619_INT_MON_DCDC	 = 0x42;
constexpr uint8_t RC5T619_INT_MON_RTC	 = 0xAF;

constexpr uint8_t RC5T619_INT_MON_CHGCTR	 = 0xC6;
constexpr uint8_t RC5T619_INT_MON_CHGSTS1 = 0xC7;
constexpr uint8_t RC5T619_INT_MON_CHGSTS2 = 0xC8;
constexpr uint8_t RC5T619_INT_MON_CHGERR	 = 0xC9;
constexpr uint8_t RC5T619_INT_MON_CHGEXTIF = 0xD3;

/* interrupt clearing registers */
constexpr uint8_t RC5T619_INT_IR_SYS	 = 0x13;
constexpr uint8_t RC5T619_INT_IR_DCDC	 = 0x41;
constexpr uint8_t RC5T619_INT_IR_RTC	 = 0xAF;
constexpr uint8_t RC5T619_INT_IR_ADCL	 = 0x8C;
constexpr uint8_t RC5T619_INT_IR_ADCH	 = 0x8D;
constexpr uint8_t RC5T619_INT_IR_ADCEND	 = 0x8E;
constexpr uint8_t RC5T619_INT_IR_GPIOR	 = 0x95;
constexpr uint8_t RC5T619_INT_IR_GPIOF	 = 0x96;
constexpr uint8_t RC5T619_INT_IR_CHGCTR	 = 0xC2;
constexpr uint8_t RC5T619_INT_IR_CHGSTS1	 = 0xC3;
constexpr uint8_t RC5T619_INT_IR_CHGSTS2	 = 0xC4;
constexpr uint8_t RC5T619_INT_IR_CHGERR	 = 0xC5;
constexpr uint8_t RC5T619_INT_IR_CHGEXTIF = 0xD2;

/* GPIO register base address */
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

/* Charger Control register */
constexpr uint8_t RC5T619_CHG_CTL1	 = 0xB3;
constexpr uint8_t TIMSET_REG		 = 0xB9;

/* ADC Control register */
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

constexpr uint8_t RICOH_DC1_SLOT  = 0x16;
constexpr uint8_t RICOH_DC2_SLOT  = 0x17;
constexpr uint8_t RICOH_DC3_SLOT  = 0x18;
constexpr uint8_t RICOH_DC4_SLOT  = 0x19;
constexpr uint8_t RICOH_DC5_SLOT  = 0x1a;

constexpr uint8_t RICOH_LDO1_SLOT  = 0x1b;
constexpr uint8_t RICOH_LDO2_SLOT  = 0x1c;
constexpr uint8_t RICOH_LDO3_SLOT  = 0x1d;
constexpr uint8_t RICOH_LDO4_SLOT  = 0x1e;
constexpr uint8_t RICOH_LDO5_SLOT  = 0x1f;
constexpr uint8_t RICOH_LDO6_SLOT  = 0x20;
constexpr uint8_t RICOH_LDO7_SLOT  = 0x21;
constexpr uint8_t RICOH_LDO8_SLOT  = 0x22;
constexpr uint8_t RICOH_LDO9_SLOT  = 0x23;
constexpr uint8_t RICOH_LDO10_SLOT  = 0x24;



auto OdroidW_ADC::init(q::Clock::duration adc0_sample_time, q::Clock::duration adc1_sample_time) -> bool
{
    std::string device("/dev/i2c-1");
    SILK_INFO("initializing device: {}", device);

    if (!m_i2c.open(device))
    {
        SILK_ERR("can't open {}: {}", device, strerror(errno));
        return false;
    }

    m_adc0.sample_time = adc0_sample_time;
    m_adc1.sample_time = adc1_sample_time;

    return true;
}

void OdroidW_ADC::process()
{

}

auto OdroidW_ADC::read_sample(size_t idx) -> boost::optional<float>
{
    uint8_t adc_sel = 0;
    uint8_t addr_H = 0;
    uint8_t addr_L = 0;
    if (idx == 0)
    {
        adc_sel = 0x17;
        addr_H = RC5T619_AIN0_DATAH;
        addr_L = RC5T619_AIN0_DATAL;
    }
    else
    {
        adc_sel = 0x16;
        addr_H = RC5T619_AIN1_DATAH;
        addr_L = RC5T619_AIN1_DATAL;
    }

    /* Start AIN0/AIN1 pin single-mode & 1-time conversion mode for ADC */
    m_i2c.write_u8(ADDR, RC5T619_ADC_CNT3, adc_sel);
    boost::this_thread::sleep_for(boost::chrono::microseconds(300));

    int rawdata_H = m_i2c.read_u8(ADDR, addr_H);
    int rawdata_L = m_i2c.read_u8(ADDR, addr_L);

    int result = (unsigned int)(rawdata_H << 4) | (rawdata_L&0xf);

    /* Stop AD conversion */
    m_i2c.write_u8(ADDR, RC5T619_ADC_CNT3, uint8_t(0x00));

    return math::clamp(result / 4095.f, 0.f, 1.f);
}

auto OdroidW_ADC::read_adc0_sample() -> boost::optional<float>
{
    auto now = q::Clock::now();
    if (now - m_adc0.last_time_point >= m_adc0.sample_time)
    {
        m_adc0.last_time_point = now;
        m_adc0.data = read_sample(0);
    }
}
auto OdroidW_ADC::read_adc1_sample() -> boost::optional<float>
{
    auto now = q::Clock::now();
    if (now - m_adc0.last_time_point >= m_adc0.sample_time)
    {
        m_adc0.last_time_point = now;
        m_adc0.data = read_sample(1);
    }
}

auto OdroidW_ADC::get_adc0_sample_time() const -> q::Clock::duration
{
    return m_adc0.sample_time;
}
auto OdroidW_ADC::get_adc1_sample_time() const -> q::Clock::duration
{
    return m_adc1.sample_time;
}




}


#endif
