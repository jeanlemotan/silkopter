// TWI.c
//
//  v1.2  3/18/2011
//
// Object provides TWI/I2C communication functions.
//
// There are some known problems with TWI on ATmel AVR's ...
//
//      http://www.robotroom.com/Atmel-AVR-TWI-I2C-Multi-Master-Problem.html
//

#ifdef __AVR_ATmega328P__
#  define SIG_OVERFLOW0 TIMER0_OVF_vect
#  define SIG_OVERFLOW1 TIMER1_OVF_vect
#  define SIG_OVERFLOW2 TIMER2_OVF_vect
#include "../libnerdkits/io_328p.h"
#endif

#include <avr/interrupt.h>
#include "I2C_slave.h"
#include <avr/io.h>

// buffers and variables
volatile uint16_t TWI_buffer_max;
volatile uint8_t*  p_TWI_buffer;
volatile uint16_t TWI_buffer_pos;
volatile uint16_t TWI_buffer_len;
volatile uint16_t TWI_read_bytes;
volatile uint16_t TWI_write_bytes;
volatile uint16_t TWI_bytes_returned;

volatile uint8_t TWI_target_slave_addr;
volatile uint8_t TWI_match_addr;

// keep track of current state
volatile uint8_t TWI_status;
#define TWI_WRITE_STATE 0x01
#define TWI_READ_STATE 0x02

// call types
volatile uint8_t TWI_master_state;
#define TWI_OP_WRITE_ONLY 0x01
#define TWI_OP_READ_ONLY 0x02
#define TWI_OP_WRITE_THEN_READ 0x03

// control variables
volatile uint8_t TWI_operation;
volatile uint8_t TWI_busy;
volatile uint8_t TWI_error;

// define callback function
void (*TWI_return_result)(volatile uint8_t TWI_match_addr, volatile uint8_t TWI_return_code);


// initialize the component
void TWI_init(long cpu_freq, long bit_rate, uint8_t* buffer, uint16_t max, void (*callback)(volatile uint8_t TWI_match_addr, volatile uint8_t TWI_return_code)){
    TWI_return_result = callback;
    p_TWI_buffer = buffer;
    TWI_buffer_max = max;
    TWBR = ((cpu_freq/bit_rate)-16)/2; // bit rate register
    TWSR = 0; // prescaler
    TWI_busy=0;
}

// master write to slave
void TWI_master_start_write(uint8_t slave_addr, uint16_t write_bytes){
    TWI_busy=1;
    if(write_bytes>TWI_buffer_max){
        TWI_write_bytes=TWI_buffer_max;
    }else{
        TWI_write_bytes=write_bytes;
    }
    TWI_operation=TWI_OP_WRITE_ONLY;
    TWI_master_state = TWI_WRITE_STATE;
    TWI_target_slave_addr = slave_addr;
    TWCR = TWI_START; // start TWI master mode
}

// master read from slave
void TWI_master_start_read(uint8_t slave_addr, uint16_t read_bytes){
    TWI_busy=1;
    if(read_bytes>TWI_buffer_max){
        TWI_read_bytes=TWI_buffer_max;
    }else{
        TWI_read_bytes=read_bytes;
    }
    TWI_operation=TWI_OP_READ_ONLY;
    TWI_master_state = TWI_READ_STATE;
    TWI_target_slave_addr = slave_addr;
    TWCR = TWI_START; // start TWI master mode
}

// master write then read without releasing buss between
void TWI_master_start_write_then_read(uint8_t slave_addr, uint16_t write_bytes, uint16_t read_bytes){
    TWI_busy=1;
    if(write_bytes>TWI_buffer_max){
        TWI_write_bytes=TWI_buffer_max;
    }else{
        TWI_write_bytes=write_bytes;
    }
    if(read_bytes>TWI_buffer_max){
        TWI_read_bytes=TWI_buffer_max;
    }else{
        TWI_read_bytes=read_bytes;
    }
    TWI_operation=TWI_OP_WRITE_THEN_READ;
    TWI_master_state = TWI_WRITE_STATE;
    TWI_target_slave_addr = slave_addr;
    TWCR = TWI_START; // start TWI master mode
}

// enable slave and start receiving messages
void TWI_enable_slave_mode(uint8_t my_slave_addr, uint8_t my_slave_addr_mask, uint8_t enable_general_call){
    TWAR = (my_slave_addr<<1);      // set my slave addr
    TWAMR = (my_slave_addr_mask<<1);    // set the addr mask for multi-device mode
    if(enable_general_call>0){
        TWAR |= _BV(TWGCE);             // enable general call receipts
    }
    TWCR = TWI_ACK;                     // enable ACK on SLA_W/SLA_R
}

// Routine to service interrupts from the TWI hardware.
// The most important thing is that this routine runs fast and returns control
// to the hardware asap. So, when results are ready and the callback is made to
// your application, be sure you return as quickly as possible. Remove significant
// work from the callback and instead perform that work in your main execution loop.
//
// See pages 229, 232, 235, and 238 of the ATmega328 datasheed for detailed
// explaination of the logic below.
SIGNAL(TWI_vect){
    TWI_status = TWSR & TWI_TWSR_status_mask;
    switch(TWI_status){

    case TWI_MM_repeated_start_sent_x10:
    case TWI_MM_start_sent_x08:
        switch(TWI_master_state){
        case TWI_WRITE_STATE:
            TWI_buffer_pos=0; // point to 1st byte
            TWDR = (TWI_target_slave_addr<<1) | 0x00; // set SLA_W
            break;
        case TWI_READ_STATE:
            TWI_buffer_pos=0; // point to first byte
            TWDR = (TWI_target_slave_addr<<1) | 0x01; // set SLA_R
            break;
        }
        TWCR = TWI_ACK; // transmit
        break;

    case TWI_MT_SLA_W_sent_ack_received_x18:
    case TWI_MT_data_sent_ack_received_x28:
        if(TWI_buffer_pos==TWI_write_bytes){
            if(TWI_operation==TWI_OP_WRITE_THEN_READ){
                TWI_master_state=TWI_READ_STATE; // now read from slave
                TWCR = TWI_START; // transmit repeated start
            }else{
                if(TWI_return_result){
                    (*TWI_return_result)(0, TWI_success);// callback with results
                }
                TWCR = TWI_STOP; // release the buss
                while(TWCR & (1<<TWSTO)); // wait for it
                TWI_busy=0;
            }
        }else{
            TWDR = p_TWI_buffer[TWI_buffer_pos++]; // load data
            TWCR = TWI_ENABLE; // transmit
        }
        break;

    case TWI_MR_data_received_ack_returned_x50:
        p_TWI_buffer[TWI_buffer_pos++]=TWDR; // save byte
    case TWI_MR_SLA_R_sent_ack_received_x40:
        if(TWI_buffer_pos==(TWI_read_bytes-1)){
            TWCR = TWI_NACK; // get last byte then nack
        }else{
            TWCR = TWI_ACK; // get next byte then ack
        }
        break;

    case TWI_MR_data_received_nack_returned_x58:
        p_TWI_buffer[TWI_buffer_pos++]=TWDR; // save byte
        if(TWI_return_result){
            (*TWI_return_result)(0, TWI_success);// callback with results
        }
        TWCR = TWI_STOP; // release the buss
        while(TWCR & (1<<TWSTO)); // wait for it
        TWI_busy=0;
        break;

    case TWI_SR_SLA_W_received_ack_sent_x60:
    case TWI_SR_SLA_W_received_after_arbitration_lost_ack_sent_x68:
    case TWI_SR_general_call_received_ack_sent_x70:
    case TWI_SR_general_call_received_after_arbitration_lost_ack_sent_x78:
        TWI_match_addr=TWDR>>1; // save the match address
        TWI_buffer_pos=0; // point to start of input buffer
        TWCR = TWI_ACK;
        break;

    case TWI_SR_SLA_W_data_received_ack_sent_x80:
    case TWI_SR_general_call_data_received_ack_sent_x90:
        if(TWI_buffer_pos<TWI_buffer_max){
            p_TWI_buffer[TWI_buffer_pos++]=TWDR; // store data
        }
        TWCR = TWI_ACK;
        break;

    case TWI_SR_stop_or_repeated_start_received_xA0:
        TWI_buffer_len=TWI_buffer_pos; // bytes returned
        if(TWI_return_result){
            (*TWI_return_result)(TWI_match_addr, TWI_success); // callback with results
        }
        TWCR = TWI_ACK;
        break;

    case TWI_ST_SLA_R_received_after_arbitration_lost_ack_sent_x80:
    case TWI_ST_SLA_R_received_ack_sent_xA8:
        TWI_match_addr=TWDR>>1; // save the match address
        TWI_buffer_pos=0; // point to start of input buffer
    case TWI_ST_byte_sent_ack_received_x88:
        if(TWI_buffer_pos<TWI_buffer_max){
            TWDR = p_TWI_buffer[TWI_buffer_pos++]; // load data
        }
    case TWI_SR_SLA_W_data_received_nack_sent_x88:
    case TWI_SR_general_call_data_received_nack_sent_x98:
    case TWI_ST_byte_sent_nack_received_xC0:
    case TWI_ST_last_byte_sent_ack_received_xC8:
        TWCR = TWI_ACK;
        break;

    case TWI_MT_SLA_W_sent_nack_received_x20:
    case TWI_MT_data_sent_nack_received_x30:
    case TWI_MR_SLA_R_sent_nack_received_x48:
        if(TWI_return_result){
            (*TWI_return_result)(TWI_match_addr, TWI_status);// callback with status
        }
    case TWI_MM_arbitration_lost_x38:
    default:
        TWCR=TWI_STOP;
        while(TWCR & (1<<TWSTO)); // wait for it
        TWCR=TWI_START; // try again
        break;
    }

}
