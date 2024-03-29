#include "FCStdAfx.h"
#include "RC5T619.h"
#include "physics/constants.h"
#include "utils/Timed_Scope.h"

#include "hal.def.h"
//#include "sz_RC5T619.hpp"

#if defined RASPBERRY_PI

extern "C"
{
    #include "utils/hw/pigpio.h"
}

#endif

namespace silk
{
namespace node
{



struct Mode_Guard
{
    Mode_Guard()
    {
#if defined RASPBERRY_PI
        gpioSetMode(28, PI_INPUT);
        gpioSetMode(29, PI_INPUT);
        gpioSetMode(0, PI_ALT0);
        gpioSetMode(1, PI_ALT0);
#endif
    }
    ~Mode_Guard()
    {
#if defined RASPBERRY_PI
        gpioSetMode(28, PI_ALT0);
        gpioSetMode(29, PI_ALT0);
        gpioSetMode(0, PI_INPUT);
        gpioSetMode(1, PI_INPUT);
#endif
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

RC5T619::RC5T619(HAL& hal)
    : m_hal(hal)
    , m_descriptor(new hal::RC5T619_Descriptor())
    , m_config(new hal::RC5T619_Config())
{
    m_adc[0] = std::make_shared<Stream>();
    m_adc[1] = std::make_shared<Stream>();
}

auto RC5T619::get_outputs() const -> std::vector<Output>
{
    std::vector<Output> outputs(2);
    outputs[0].name = "adc_0";
    outputs[0].stream = m_adc[0];
    outputs[1].name = "adc_1";
    outputs[1].stream = m_adc[1];
    return outputs;
}
ts::Result<void> RC5T619::init(hal::INode_Descriptor const& descriptor)
{
    QLOG_TOPIC("rc5t619::init");

    auto specialized = dynamic_cast<hal::RC5T619_Descriptor const*>(&descriptor);
    if (!specialized)
    {
        return make_error("Wrong descriptor type");
    }
    *m_descriptor = *specialized;

    return init();
}

ts::Result<void> RC5T619::init()
{
    m_i2c_bus = m_hal.get_bus_registry().find_by_name<bus::II2C_Bus>(m_descriptor->get_bus());

    auto i2c_bus = m_i2c_bus.lock();
    if (!i2c_bus)
    {
        return make_error("No bus configured");
    }

    util::hw::II2C& i2c = i2c_bus->get_i2c();

    //m_descriptor->adc0_rate = math::clamp<size_t>(m_descriptor->adc0_rate, 1, 50);
    //m_descriptor->adc1_rate_ratio = math::clamp<size_t>(m_descriptor->adc1_rate_ratio, 1, 100);

    m_adc[0]->rate = m_descriptor->get_adc0_rate();
    m_adc[1]->rate = m_descriptor->get_adc1_rate();

    m_dt = std::chrono::milliseconds(1000 / m_descriptor->get_adc0_rate());

    Mode_Guard mg;

    uint8_t control;
    auto res = i2c.read_register_u8(ADDR, 0x36, control);
    if (!res || control == 0xff)
    {
        return make_error("rc5t619 not found");
    }
    QLOGI("rc5t619 found: {}", control);

    // Set ADRQ=00 to stop ADC
    res &= i2c.write_register_u8(ADDR, RC5T619_ADC_CNT3, 0x0);

    // Set ADC auto conversion interval 250ms
    res &= i2c.write_register_u8(ADDR, RC5T619_ADC_CNT2, 0x0);

    // Enable AIN0, AIN1 pin conversion in auto-ADC
    res &= i2c.write_register_u8(ADDR, RC5T619_ADC_CNT1, 0xC0);

    // Start auto-mode & average 4-time conversion mode for ADC
    res &= i2c.write_register_u8(ADDR, RC5T619_ADC_CNT3, 0x17);

    //start by converting voltage first
    res &= i2c.write_register_u8(ADDR, RC5T619_ADC_CNT3, CONVERT_ADC1);
    if (!res)
    {
        return make_error("Failed to init rc5t619");
    }

    return ts::success;
}

ts::Result<void> RC5T619::start(Clock::time_point tp)
{
    m_last_tp = tp;
    return ts::success;
}

void RC5T619::process()
{
    QLOG_TOPIC("rc5t619::process");

    m_adc[0]->samples.clear();
    m_adc[1]->samples.clear();

    auto i2c_bus = m_i2c_bus.lock();
    if (!i2c_bus)
    {
        return;
    }

    util::hw::II2C& i2c = i2c_bus->get_i2c();

    auto now = Clock::now();
    if (now - m_last_tp < m_dt)
    {
        return;
    }

    m_last_tp = now;

//    LOG_INFO("ADC{} : {}:{} -> {}", idx, buf[0], buf[1], result);

    // Stop AD conversion */
    //i2c.write_u8(ADDR, RC5T619_ADC_CNT3, uint8_t(0x00));

    Mode_Guard mg;

    std::array<uint8_t, 2> buf;
    if (m_stage == 0)
    {
        //read coltage

        if (i2c.read_register(ADDR, RC5T619_AIN1_DATAH, buf.data(), buf.size()))
        {
            int r = (unsigned int)(buf[0] << 4) | (buf[1]&0xf);
            auto result =  math::clamp(static_cast<float>(r) / 4095.f, 0.f, 1.f);

            Stream::Sample& sample = m_adc[1]->last_sample;
            sample.value = result;
            m_adc[1]->samples.push_back(sample);

            m_adc[1]->last_tp = now;
        }

        //next
        if (i2c.write_register_u8(ADDR, RC5T619_ADC_CNT3, CONVERT_ADC0)) //read current next
        {
            m_stage++;
        }
    }
    else
    {
        //read current

        if (i2c.read_register(ADDR, RC5T619_AIN0_DATAH, buf.data(), buf.size()))
        {
            int r = (unsigned int)(buf[0] << 4) | (buf[1]&0xf);
            auto result =  math::clamp(static_cast<float>(r) / 4095.f, 0.f, 1.f);

            Stream::Sample& sample = m_adc[0]->last_sample;
            sample.value = result;
            m_adc[0]->samples.push_back(sample);

            m_adc[0]->last_tp = now;
        }

        //next
        //todo - fix this
//        if (m_stage >= m_descriptor->adc1_rate_ratio)
//        {
//            if (i2c->write_register_u8(ADDR, RC5T619_ADC_CNT3, CONVERT_ADC1)) //read voltage next
//            {
//                m_stage = 0;
//            }
//        }
//        else if (i2c->write_register_u8(ADDR, RC5T619_ADC_CNT3, CONVERT_ADC0)) //read current next
//        {
//            m_stage++;
//        }
    }
}

ts::Result<void> RC5T619::set_config(hal::INode_Config const& config)
{
    QLOG_TOPIC("rc5t619::set_config");

    auto specialized = dynamic_cast<hal::RC5T619_Config const*>(&config);
    if (!specialized)
    {
        return make_error("Wrong config type");
    }
    *m_config = *specialized;

    return ts::success;
}
auto RC5T619::get_config() const -> std::shared_ptr<const hal::INode_Config>
{
    return m_config;
}

auto RC5T619::get_descriptor() const -> std::shared_ptr<const hal::INode_Descriptor>
{
    return m_descriptor;
}

ts::Result<std::shared_ptr<messages::INode_Message>> RC5T619::send_message(messages::INode_Message const& message)
{
    return make_error("Unknown message");
}

}
}
