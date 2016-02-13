//
// TWI.h
//
//  v1.2  3/18/2011
//
// Define status codes, buffers, and various variables used
// by TWI.c to communicate with other devices via the TWI/I2c buss.
//
#define TWI_TWSR_status_mask 0xF8

// Status codes for TWI Master Mode (TWSR)
#define TWI_MM_start_sent_x08 0x08
#define TWI_MM_repeated_start_sent_x10 0x10
#define TWI_MM_arbitration_lost_x38 0x38

// Status codes for TWI Master Transmitter Mode
#define TWI_MT_SLA_W_sent_ack_received_x18 0x18
#define TWI_MT_SLA_W_sent_nack_received_x20 0x20
#define TWI_MT_data_sent_ack_received_x28 0x28
#define TWI_MT_data_sent_nack_received_x30 0x30

// Status codes for TWI Master Receiver Mode
#define TWI_MR_SLA_R_sent_ack_received_x40 0x40
#define TWI_MR_SLA_R_sent_nack_received_x48 0x48
#define TWI_MR_data_received_ack_returned_x50 0x50
#define TWI_MR_data_received_nack_returned_x58 0x58

// Status codes for TWI Slave Receiver Mode
#define TWI_SR_SLA_W_received_ack_sent_x60 0x60
#define TWI_SR_SLA_W_received_after_arbitration_lost_ack_sent_x68 0x68
#define TWI_SR_general_call_received_ack_sent_x70 0x70
#define TWI_SR_general_call_received_after_arbitration_lost_ack_sent_x78 0x78
#define TWI_SR_SLA_W_data_received_ack_sent_x80 0x80
#define TWI_SR_SLA_W_data_received_nack_sent_x88 0x88
#define TWI_SR_general_call_data_received_ack_sent_x90 0x90
#define TWI_SR_general_call_data_received_nack_sent_x98 0x98
#define TWI_SR_stop_or_repeated_start_received_xA0 0xA0

// Status codes for TWI Slave Transmitter Mode
#define TWI_ST_SLA_R_received_ack_sent_xA8 0xA8
#define TWI_ST_SLA_R_received_after_arbitration_lost_ack_sent_x80 0xB0
#define TWI_ST_byte_sent_ack_received_x88 0xB8
#define TWI_ST_byte_sent_nack_received_xC0 0xC0
#define TWI_ST_last_byte_sent_ack_received_xC8 0xC8

// successful return code
#define TWI_success 0x00

// buffers and variables
extern volatile uint16_t TWI_buffer_max;
extern volatile uint8_t*  p_TWI_buffer;
extern volatile uint16_t TWI_buffer_pos;
extern volatile uint16_t TWI_buffer_len;
extern volatile uint16_t TWI_read_bytes;
extern volatile uint16_t TWI_write_bytes;
extern volatile uint16_t TWI_bytes_returned;

extern volatile uint8_t TWI_target_slave_addr;
extern volatile uint8_t TWI_match_addr;

// keep track of current state
extern volatile uint8_t TWI_status;
#define TWI_WRITE_STATE 0x01
#define TWI_READ_STATE 0x02

// call types
extern volatile uint8_t TWI_master_state;
#define TWI_OP_WRITE_ONLY 0x01
#define TWI_OP_READ_ONLY 0x02
#define TWI_OP_WRITE_THEN_READ 0x03

// control variables
extern volatile uint8_t TWI_operation;
extern volatile uint8_t TWI_busy;
extern volatile uint8_t TWI_error;

// various states of hardware that will be set in response to interrupts
#define TWI_ENABLE  _BV(TWEN) | _BV(TWINT) | _BV(TWIE)
//
#define TWI_ACK     _BV(TWEA)  |    TWI_ENABLE
#define TWI_NACK                    TWI_ENABLE
#define TWI_START   _BV(TWSTA) |    TWI_ENABLE
#define TWI_STOP    _BV(TWSTO) |    TWI_ENABLE


// define supported funcitons
void TWI_init(long cpu_freq, long bit_rate, uint8_t* buffer, uint16_t max, void (*callback)(volatile uint8_t TWI_match_addr, volatile uint8_t TWI_return_code));
void TWI_master_start_write(uint8_t slave_addr, uint16_t write_bytes);
void TWI_master_start_read(uint8_t slave_addr, uint16_t read_bytes);
void TWI_master_start_write_then_read(uint8_t slave_addr, uint16_t write_bytes, uint16_t read_bytes);
void TWI_enable_slave_mode(uint8_t my_slave_addr, uint8_t my_slave_addr_mask, uint8_t enable_general_call);
