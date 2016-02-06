#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "avr_stdio.h"
#include <deque>
#include "Chrono.h"

//////////////////////////////////////////////////////////////////////////////////////////
//COMPILER STUFF

__extension__ typedef int __guard __attribute__((mode (__DI__)));

extern "C" int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);};
extern "C" void __cxa_guard_release (__guard *g) {*(char *)g = 1;};
extern "C" void __cxa_guard_abort (__guard *) {};
//extern "C" void __cxa_pure_virtual() { while (1); }

#include <stdlib.h>
inline void* operator new(size_t size) { return malloc(size); }
inline void* operator new[](size_t size) { return malloc(size); }
inline void operator delete(void* p) { free(p); }
inline void operator delete[](void* p) { free(p); }
inline void* operator new(size_t size_, void *ptr_) { return ptr_; }

class Scope_Sync
{
public:
    Scope_Sync()
    {
        m_old_sreg = SREG;
        cli();
    }
    ~Scope_Sync()
    {
        SREG = m_old_sreg;
    }
private:
    uint8_t m_old_sreg;
};

//////////////////////////////////////////////////////////////////////////////////////////
// INIT

ISR(BADISR_vect)
{
}

void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

void wdt_init(void)
{
    wdt_disable();
    return;
}


//////////////////////////////////////////////////////////////////////////////////////////
// ADC

constexpr uint8_t s_adc_ports[] = { 6, 7 };
static uint8_t s_adc_port_idx = 0;
static int16_t s_adc_values[sizeof(s_adc_ports)] = {0};
static uint8_t s_adc_interrupt_count = 0;

void init_adc()
{
    ADMUX  = ((0<<REFS1)|	// Reference Selection Bits
              (1<<REFS0)|	// 2.56V Internal
              (0<<ADLAR)|	// ADC Left Adjust Result
              (0<< MUX4)|
              (0<< MUX3)|
              (0<< MUX2)|
              (0<< MUX1)|
              (0<< MUX0));

    ADCSRA = ((1<< ADEN)|	// 1 = ADC Enable
              (0<< ADSC)|	// 1 = ADC Start Conversion
              (0<<ADATE)|	// 1 = ADC Auto Trigger Enable
              (0<< ADIF)|	// ADC Interrupt Flag
              (1<< ADIE)|	// ADC Interrupt Enable
              (1<<ADPS2)|
              (1<<ADPS1)|	// ADC Prescaler Selects adc sample freq.
              (1<<ADPS0));

    ADCSRB = ((1<<ADHSM)|	// High Speed mode select
              (0<< MUX5));

    DIDR0  = ((1<<ADC0D)|   // Turn Off Digital Input Buffer.
              (1<<ADC1D)|
              (1<<ADC4D)|
              (1<<ADC5D)|
              (1<<ADC6D)|
              (1<<ADC7D));
}

void start_adc()
{
    ADCSRA |= (1<< ADSC);
}

ISR(ADC_vect)
{
    s_adc_interrupt_count++;
    s_adc_values[s_adc_port_idx] = ADCW;
    s_adc_port_idx++;
    if (s_adc_port_idx >= sizeof(s_adc_ports))
    {
        s_adc_port_idx = 0;
    }

    ADMUX  = (ADMUX & 0xF8) | (s_adc_ports[s_adc_port_idx]);
}


//////////////////////////////////////////////////////////////////////////////////////////
//STDIO

template<typename... Args>
void log(PGM_P fmt, Args... args)
{
    printf_P(fmt, args...);
}


FILE s_uart_output;

#define LOG log
//#define LOG(...)

void soft_reset()
{
    LOG(PSTR("resetting..."));
    //cli();
    //wdt_enable(WDTO_15MS);
    //while(1);
    void (*resetptr)( void ) = 0x0000;
    resetptr();
}

//////////////////////////////////////////////////////////////////////////////////////////
//TIME

namespace chrono
{
    time_ms s_time_point = time_ms(0);
}

#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( ((a) * 1000L) / (F_CPU / 1000L) )
#define microsecondsToClockCycles(a) ( ((a) * (F_CPU / 1000L)) / 1000L )

// the prescaler is set so that timer0 ticks every 64 clock cycles, and the
// the overflow handler is called every 256 ticks.
#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

volatile unsigned long s_timer0_overflow_count = 0;
volatile unsigned long s_timer0_millis = 0;
static unsigned char s_timer0_fract = 0;

ISR(TIMER0_OVF_vect)
{
   // copy these to local variables so they can be stored in registers
   // (volatile variables must be read from memory on every access)
   unsigned long m = s_timer0_millis;
   unsigned char f = s_timer0_fract;

   m += MILLIS_INC;
   f += FRACT_INC;
   if (f >= FRACT_MAX)
   {
      f -= FRACT_MAX;
      m += 1;
   }

   s_timer0_fract = f;
   s_timer0_millis = m;
   s_timer0_overflow_count++;
}

unsigned long millis()
{
   unsigned long m;
   Scope_Sync ss;

   // disable interrupts while we read timer0_millis or we might get an
   // inconsistent value (e.g. in the middle of a write to timer0_millis)
   m = s_timer0_millis;

   return m;
}


#define sbi(PORT, BIT) (_SFR_BYTE(PORT) |= _BV(BIT)) // set bit
#define tbi(PORT, BIT) (_SFR_BYTE(PORT) ^= _BV(BIT)) // toggle bit

void init_timer()
{
    sbi(TCCR0A, WGM01);
    sbi(TCCR0A, WGM00);
    // set timer 0 prescale factor to 64
    // this combination is for the standard 168/328/1280/2560
    sbi(TCCR0B, CS01);
    sbi(TCCR0B, CS00);
    // enable timer 0 overflow interrupt
    sbi(TIMSK0, TOIE0);
}

//////////////////////////////////////////////////////////////////////////////////////////
// UART

void init_uart(uint32_t baud)
{
    uint16_t ubrr = F_CPU/16/baud - 1;

    // Set baud rate
    UBRR1H = (uint8_t)(ubrr>>8);
    UBRR1L = (uint8_t)ubrr;
    // Enable receiver and transmitter
    UCSR1B = (1<<RXEN1)|(1<<TXEN1);
    // Set frame format: 8data, 1stop bit
    UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);
}

//////////////////////////////////////////////////////////////////////////////////////////
// PINS

enum class Pin_Mode
{
    INPUT,
    OUTPUT
};

enum class Port
{
    NONE,
    B,
    C,
    D,
    E,
    F
};

struct Port_Bit
{
    Port port;
    uint8_t bit;
};

constexpr Port_Bit k_pin_to_port_bit[] =
{
    { Port::NONE, 0 },//nonexistant

    { Port::E, 6 },//1
    { Port::NONE, 0 }, //UVcc
    { Port::NONE, 0 }, //D-
    { Port::NONE, 0 }, //D+
    { Port::NONE, 0 }, //UGnd
    { Port::NONE, 0 }, //UCap
    { Port::NONE, 0 }, //VBus
    { Port::B, 0 }, //8
    { Port::B, 1 }, //9
    { Port::B, 2 }, //10
    { Port::B, 3 }, //11
    { Port::B, 4 }, //12
    { Port::NONE, 0 }, //Reset
    { Port::NONE, 0 }, //Vcc
    { Port::NONE, 0 }, //GND
    { Port::NONE, 0 }, //XTal2
    { Port::NONE, 0 }, //XTal2
    { Port::D, 0 }, //18
    { Port::D, 1 }, //19
    { Port::D, 2 }, //20
    { Port::D, 3 }, //21
    { Port::D, 5 }, //22
    { Port::NONE, 0 }, //GND
    { Port::NONE, 0 }, //AVcc
    { Port::D, 4 }, //25
    { Port::D, 6 }, //26
    { Port::D, 7 }, //27
    { Port::B, 4 }, //28
    { Port::B, 5 }, //29
    { Port::B, 6 }, //30
    { Port::C, 6 }, //31
    { Port::C, 7 }, //32
    { Port::E, 2 }, //33
    { Port::NONE, 0 }, //Vcc
    { Port::NONE, 0 }, //GND
    { Port::F, 7 }, //36
    { Port::F, 6 }, //37
    { Port::F, 5 }, //38
    { Port::F, 4 }, //39
    { Port::F, 1 }, //40
    { Port::F, 0 }, //41
    { Port::NONE, 0 }, //ARef
    { Port::NONE, 0 }, //GND
    { Port::NONE, 0 } //AVcc
};

constexpr volatile uint8_t* k_port_to_mode[] =
{
    nullptr,
    &DDRB,
    &DDRC,
    &DDRD,
    &DDRE,
    &DDRF
};

constexpr volatile uint8_t* k_port_to_output[] =
{
    nullptr,
    &PORTB,
    &PORTC,
    &PORTD,
    &PORTE,
    &PORTF
};

static void set_pin_mode(uint8_t pin, Pin_Mode mode)
{
    Port_Bit port_bit = k_pin_to_port_bit[pin];
    if (port_bit.port == Port::NONE)
    {
        return;
    }

    volatile uint8_t* reg = k_port_to_mode[static_cast<int>(port_bit.port)];
    if (reg != nullptr)
    {
        Scope_Sync ss;
        if (mode == Pin_Mode::INPUT)
        {
            *reg &= ~(1 << port_bit.bit);
        }
        else
        {
            *reg |= (1 << port_bit.bit);
        }
    }
}

static void set_pin(uint8_t pin, bool value)
{
    Port_Bit port_bit = k_pin_to_port_bit[pin];
    if (port_bit.port == Port::NONE)
    {
        return;
    }

    volatile uint8_t* reg = k_port_to_output[static_cast<int>(port_bit.port)];
    if (reg != nullptr)
    {
        Scope_Sync ss;
        if (value)
        {
            *reg |= (1 << port_bit.bit);
        }
        else
        {
            *reg &= ~(1 << port_bit.bit);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
//PWM

static void init_pwm()
{
    //D3  -> 18 -> PD0 / OC0B
    //D5  -> 31 -> PC6 / OC3A
    //D6  -> 27 -> PD7 / OC4D
    //D9  -> 29 -> PB5 / OC1A
    //D10 -> 30 -> PB6 / OC1B
    //D11 -> 12 -> PB7 / OC0A / OC1C

    set_pin_mode(31, Pin_Mode::OUTPUT);
    TCCR3A = (1<<WGM31);
    TCCR3B = (1<<WGM33) | (1<<WGM32) | (1<<CS31);
    OCR3A = 2000; // Init OCR registers to nil output signal
    ICR3 = 10000; // 0.5us tick => 50hz freq

    TCCR3A |= (1<<COM3A1);
    //------------

    set_pin_mode(29, Pin_Mode::OUTPUT);
    set_pin_mode(30, Pin_Mode::OUTPUT);
    TCCR1A = (1<<WGM11);
    TCCR1B = (1<<WGM13) | (1<<WGM12) | (1<<CS11);
    ICR1 = 10000; // 0.5us tick => 50hz freq

    OCR1A = 3000; // Init OCR registers to nil output signal
    OCR1B = 5000; // Init OCR registers to nil output signal

    TCCR1A |= (1<<COM1A1);
    TCCR1A |= (1<<COM1B1);
    //-----------


//    set_pin_mode(27, Pin_Mode::OUTPUT);

//    TCCR4A = (1<<WGM41);
//    TCCR4B = (1<<WGM43) | (1<<WGM42) | (1<<CS41);
//    OCR4A = 2000; // Init OCR registers to nil output signal
//    ICR4 = 40000; // 0.5us tick => 50hz freq

//    TCCR4A |= (1<<COM4A1);

}

//////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
    int mcusr_value = MCUSR;
    MCUSR = 0;

    //setup serial. 1.2Khz seems to be the best for 8Mhz
    //Serial.begin(9600);
    //delay(200);
    init_uart(9600);

    fdev_setup_stream(&s_uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);
    stdout = &s_uart_output;

    if (mcusr_value & (1<<PORF )) LOG(PSTR("Power-on reset.\n"));
    if (mcusr_value & (1<<EXTRF)) LOG(PSTR("External reset!\n"));
    if (mcusr_value & (1<<BORF )) LOG(PSTR("Brownout reset!\n"));
    if (mcusr_value & (1<<WDRF )) LOG(PSTR("Watchdog reset!\n"));

    init_timer();

    ///////////////////////////////////////////////
    //seed the RNG
    {
//        float vcc = read_vcc();
//        float s = vcc;
//        uint32_t seed = reinterpret_cast<uint32_t&>(s);
//        srandom(seed);
//        LOG(PSTR("Random seed: %lu\n"), seed);
    }

    ///////////////////////////////////////////////

    LOG(PSTR("Starting rfm22b setup..."));
    LOG(PSTR("done\n"));

//    set_pin_mode(31, Pin_Mode::OUTPUT);
//    while (true)
//    {
//        set_pin(31, true);
//        chrono::delay(chrono::millis(1));
//        set_pin(31, false);
//        chrono::delay(chrono::millis(1));
//    }

    init_pwm();
    init_adc();
    sei();
}

int main()
{
    setup();

    start_adc();

    chrono::time_ms last_adc = chrono::now();

    while (true)
    {
        printf("ADC: %d, %d / %d\n", (int)s_adc_values[0], (int)s_adc_values[1], (int)s_adc_interrupt_count);

        auto now = chrono::now();
        if (now - last_adc > chrono::millis(10))
        {
            last_adc = now;
            start_adc();
        }

        OCR1A++;
    }
}
