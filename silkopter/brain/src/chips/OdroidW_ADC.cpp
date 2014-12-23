#include "BrainStdAfx.h"
#include "OdroidW_ADC.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#ifdef RASPBERRY_PI

extern "C"
{
    #include "pigpiod_if.h"
}


namespace silk
{

constexpr uint8_t ADDR = 0x32;

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


auto OdroidW_ADC::init(q::Clock::duration current_sample_time, q::Clock::duration voltage_sample_time) -> bool
{
    std::string device("/dev/i2c-0");
    SILK_INFO("initializing device: {}", device);

    if (!m_i2c.open(device))
    {
        SILK_ERR("can't open {}: {}", device, strerror(errno));
        return false;
    }

    m_adc_current.sample_time = math::max(current_sample_time, q::Clock::duration(std::chrono::milliseconds(20)));
    m_adc_current.last_time_point = q::Clock::now();

    m_adc_voltage.sample_time = math::max(voltage_sample_time, q::Clock::duration(std::chrono::milliseconds(20)));
    m_adc_voltage.last_time_point = q::Clock::now();

    return true;
}

void OdroidW_ADC::process()
{

}

auto OdroidW_ADC::read_sample(size_t idx) -> boost::optional<float>
{
    set_mode(28, PI_INPUT);
    set_mode(29, PI_INPUT);
    set_mode(0, PI_ALT0);
    set_mode(1, PI_ALT0);

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

    // Start AIN0/AIN1 pin single-mode & 1-time conversion mode for ADC
    m_i2c.write_u8(ADDR, RC5T619_ADC_CNT3, adc_sel);
    boost::this_thread::sleep_for(boost::chrono::microseconds(300));

    int rawdata_H = m_i2c.read_u8(ADDR, addr_H);
    int rawdata_L = m_i2c.read_u8(ADDR, addr_L);

    set_mode(0, PI_INPUT);
    set_mode(1, PI_INPUT);
    set_mode(28, PI_ALT0);
    set_mode(29, PI_ALT0);

    int result = (unsigned int)(rawdata_H << 4) | (rawdata_L&0xf);

    // Stop AD conversion */
    //m_i2c.write_u8(ADDR, RC5T619_ADC_CNT3, uint8_t(0x00));

    return math::clamp(static_cast<float>(result) / 4095.f, 0.f, 1.f);
}

auto OdroidW_ADC::read_current_sample() -> boost::optional<float>
{
    auto now = q::Clock::now();
    if (now - m_adc_current.last_time_point >= m_adc_current.sample_time)
    {
        m_adc_current.last_time_point = now;
        m_adc_current.data = read_sample(0);
        return m_adc_current.data;
    }
    return boost::none;
}
auto OdroidW_ADC::read_voltage_sample() -> boost::optional<float>
{
    auto now = q::Clock::now();
    if (now - m_adc_voltage.last_time_point >= m_adc_voltage.sample_time)
    {
        m_adc_voltage.last_time_point = now;
        m_adc_voltage.data = read_sample(1);
        return m_adc_voltage.data;
    }
    return boost::none;
}

auto OdroidW_ADC::get_current_sample_time() const -> q::Clock::duration
{
    return m_adc_current.sample_time;
}
auto OdroidW_ADC::get_voltage_sample_time() const -> q::Clock::duration
{
    return m_adc_voltage.sample_time;
}




}


#endif
