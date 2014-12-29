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

struct Mode_Guard
{
    Mode_Guard()
    {
        set_mode(28, PI_INPUT);
        set_mode(29, PI_INPUT);
        set_mode(0, PI_ALT0);
        set_mode(1, PI_ALT0);
    }
    ~Mode_Guard()
    {
        set_mode(28, PI_ALT0);
        set_mode(29, PI_ALT0);
        set_mode(0, PI_INPUT);
        set_mode(1, PI_INPUT);
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

//static inline int set_bank_rc5t619(i2c& dev, int bank)
//{
//    int ret;

//    if (bank != (bank & 1))
//        return -EINVAL;
////    if (bank == rc5t619->bank_num)
////        return 0;
//    ret = dev.write_u8(ADDR, RC5T619_REG_BANKSEL, bank);
//    return ret;
//}

//static int rc5t619_set_bits(i2c& dev, uint8_t reg, uint8_t bit_mask)
//{
//    uint8_t reg_val;
//    int ret = 0;

//    ret = set_bank_rc5t619(dev, 0);
//    if (!ret)
//    {
//        dev.read_u8(ADDR, reg, reg_val);
//        if ((reg_val & bit_mask) != bit_mask)
//        {
//            reg_val |= bit_mask;
//            dev.write_u8(ADDR, reg, reg_val);
//        }
//    }
//    return ret;
//}

//int rc5t619_clr_bits(i2c& dev, uint8_t reg, uint8_t bit_mask)
//{
//    uint8_t reg_val;
//    int ret = 0;

//    ret = set_bank_rc5t619(dev, 0);
//    if( !ret )
//    {
//        dev.read_u8(ADDR, reg, reg_val);
//        if (reg_val & bit_mask)
//        {
//            reg_val &= ~bit_mask;
//            ret = dev.write_u8(ADDR, reg, reg_val);
//        }
//    }
//    return ret;
//}

//static void print_regs(const char *header, i2c& dev, int start_offset, int end_offset)
//{
//    q::quick_logf("{}", header);
//    for (int i = start_offset; i <= end_offset; ++i)
//    {
//        uint8_t reg_val;
//        dev.read_u8(ADDR, i, reg_val);
//        q::quick_logf("Reg {} Value {}", i, reg_val);
//    }
//    q::quick_logf("------------------");
//}

//static void print_regs(i2c& dev)
//{
//    q::quick_logf("RC5T619 Registers");
//    q::quick_logf("------------------");

//    print_regs("System Regs",		dev, 0x0, 0x05);
//    print_regs("Power Control Regs",dev, 0x07, 0x2B);
//    print_regs("DCDC  Regs",		dev, 0x2C, 0x43);
//    print_regs("LDO   Regs",		dev, 0x44, 0x5C);
//    print_regs("ADC   Regs",		dev, 0x64, 0x8F);
//    print_regs("GPIO  Regs",		dev, 0x90, 0x9B);
//    print_regs("INTC  Regs",		dev, 0x9C, 0x9E);
//    print_regs("OPT   Regs",		dev, 0xB0, 0xB1);
//    print_regs("CHG   Regs",		dev, 0xB2, 0xDF);
//    print_regs("FUEL  Regs",		dev, 0xE0, 0xFC);
//}


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

//    set_bank_rc5t619(m_i2c, 0);

//    uint8_t control;
//    auto res = m_i2c.read_u8(ADDR, 0x36, control);
//    if (!res || control == 0xff)
//    {
//        SILK_ERR("rc5t619 not found");
//        return false;
//    }
//    SILK_INFO("rc5t619 found: {}", control);

//    print_regs(m_i2c);


//    /***************set noe time 128ms**************/
//    rc5t619_set_bits(m_i2c, 0x11, (0x1 <<3));
//    rc5t619_clr_bits(m_i2c, 0x11, (0x7 <<0));
//    rc5t619_clr_bits(m_i2c, 0x11, (0x1 <<3));
//    /**********************************************/

//    /***************set PKEY long press time 0sec*******/
//    rc5t619_set_bits(m_i2c, 0x10, (0x1 <<7));
//    rc5t619_clr_bits(m_i2c, 0x10, (0x1 <<3));
//    rc5t619_clr_bits(m_i2c, 0x10, (0x1 <<7));
//    /**********************************************/

//    print_regs(m_i2c);

//    /* Set ADRQ=00 to stop ADC */
//    m_i2c.write_u8(ADDR, RC5T619_ADC_CNT3, 0x0);

//    /* Enable AIN0L, AIN1L threshold Low interrupt */
//    m_i2c.write_u8(ADDR, RC5T619_INT_EN_ADC1, 0xC0);
//    /* Enable AIN0H, AIN1H threshold High interrupt */
//    m_i2c.write_u8(ADDR, RC5T619_INT_EN_ADC2, 0xC0);
//    /* Enable Single-mode interrupt */
//    m_i2c.write_u8(ADDR, RC5T619_INT_EN_ADC2, 0x01);

//    /* Set ADC auto conversion interval 250ms */
//    m_i2c.write_u8(ADDR, RC5T619_ADC_CNT2, 0x0);

//    /* Enable AIN0, AIN1 pin conversion in auto-ADC */
//    m_i2c.write_u8(ADDR, RC5T619_ADC_CNT1, 0xC0);

//    /* Start auto-mode & average 4-time conversion mode for ADC */
//    m_i2c.write_u8(ADDR, RC5T619_ADC_CNT3, 0x17);
//    /* Enable master ADC INT */
//    rc5t619_set_bits(m_i2c, RC5T619_INTC_INTEN, 0x08);

//    print_regs(m_i2c);


//    m_i2c.write_u8(ADDR, RC5T619_PWR_NOE_TIMSET, 0x0); //N_OE timer setting to 128mS
//    m_i2c.write_u8(ADDR, RC5T619_PWR_REP_CNT, 0x0); //Repeat power ON after reset (Power Off/N_OE)

//    rc5t619_set_bits(m_i2c, RC5T619_PWR_REP_CNT, (0 << 0));  //Disable restart when power off


//    rc5t619_clr_bits(m_i2c, 0xb1, (7<<0));  //set vbatdec voltage  3.0v
//    rc5t619_set_bits(m_i2c, 0xb1, (3<<0));  //set vbatdec voltage 3.0v



//    print_regs(m_i2c);


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

//    m_adc_current.sample_time = math::max(current_sample_time, q::Clock::duration(std::chrono::milliseconds(50)));
//    m_adc_current.last_time_point = q::Clock::now();

//    m_adc_voltage.sample_time = math::max(voltage_sample_time, q::Clock::duration(std::chrono::milliseconds(50)));
//    m_adc_voltage.last_time_point = q::Clock::now();

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
            int result = (unsigned int)(buf[0] << 4) | (buf[1]&0xf);
            m_adc_voltage.data =  math::clamp(static_cast<float>(result) / 4095.f, 0.f, 1.f);
            m_adc_voltage.sample_time = now - m_adc_voltage.last_time_point;
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
            int result = (unsigned int)(buf[0] << 4) | (buf[1]&0xf);
            m_adc_current.data =  math::clamp(static_cast<float>(result) / 4095.f, 0.f, 1.f);
            m_adc_current.sample_time = now - m_adc_current.last_time_point;
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

auto OdroidW_ADC::read_current() -> boost::optional<float>
{
    auto res = m_adc_current.data;
    m_adc_current.data.reset();
    return res;
}
auto OdroidW_ADC::read_voltage() -> boost::optional<float>
{
    auto res = m_adc_voltage.data;
    m_adc_voltage.data.reset();
    return res;
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
