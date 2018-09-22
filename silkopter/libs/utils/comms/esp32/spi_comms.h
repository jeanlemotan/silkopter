#pragma once

static constexpr size_t MAX_SPI_BUFFER_SIZE = 1600; //has to be multiple of 16

enum class SPI_Req : uint8_t
{
    PACKET = 1,
    SET_RATE = 2,
    GET_RATE = 3,
    SETUP_FEC_CODEC = 4,
    SET_CHANNEL = 5,
    GET_CHANNEL = 6,
    SET_POWER = 7,
    GET_POWER = 8,
    SETUP_ADC = 9,
    GET_ADC = 10,
};

enum class SPI_Res : uint8_t
{
    PACKET = 1,
    SET_RATE = 2,
    GET_RATE = 3,
    SETUP_FEC_CODEC = 4,
    SET_CHANNEL = 5,
    GET_CHANNEL = 6,
    SET_POWER = 7,
    GET_POWER = 8,
    SETUP_ADC = 9,
    GET_ADC = 10,
};

#pragma pack(push, 1) // exact fit - no padding

///////////////////////////////////////////////////////////////////////////////////////

struct SPI_Req_Base_Header
{
    uint32_t crc : 8; //crc of the entire header
    uint32_t seq : 7; //incrementing seq number that should be matched with the response
    uint32_t req : 5; //request
};

struct SPI_Res_Base_Header
{
    uint32_t crc : 8; //crc of the entire header
    uint32_t seq : 7; //seq of the previous command
    uint32_t res : 5; //response

    //rx
    uint32_t pending_packets : 5;
    uint32_t next_packet_size : 11;
    
    //... data follows
};

///////////////////////////////////////////////////////////////////////////////////////

struct SPI_Req_Packet_Header : public SPI_Req_Base_Header
{
    uint16_t packet_size : 11;
    uint16_t use_fec : 1;
    //... data follows
};

///////////////////////////////////////////////////////////////////////////////////////

struct SPI_Res_Packet_Header : public SPI_Res_Base_Header
{
    int16_t rssi;
    uint16_t packet_id : 5; //it repeats every 32
    uint16_t packet_size : 11;
    //... data follows
};

///////////////////////////////////////////////////////////////////////////////////////

struct SPI_Req_Setup_Fec_Codec_Header : public SPI_Req_Base_Header
{
    uint32_t fec_coding_k : 5;
    uint32_t fec_coding_n : 5;
    uint32_t fec_mtu : 11;
};

struct SPI_Res_Setup_Fec_Codec_Header : public SPI_Res_Base_Header
{
    uint32_t fec_coding_k : 5;
    uint32_t fec_coding_n : 5;
    uint32_t fec_mtu : 11;
};

///////////////////////////////////////////////////////////////////////////////////////

struct SPI_Req_Set_Channel_Header : public SPI_Req_Base_Header
{
    uint8_t channel;
};

struct SPI_Res_Set_Channel_Header : public SPI_Res_Base_Header
{
    uint8_t channel;
};

///////////////////////////////////////////////////////////////////////////////////////

struct SPI_Req_Get_Channel_Header : public SPI_Req_Base_Header
{
};

struct SPI_Res_Get_Channel_Header : public SPI_Res_Base_Header
{
    uint8_t channel;
};

///////////////////////////////////////////////////////////////////////////////////////

struct SPI_Req_Set_Power_Header : public SPI_Req_Base_Header
{
    int16_t power; // power * 10 dbm
};

struct SPI_Res_Set_Power_Header : public SPI_Res_Base_Header
{
    int16_t power; // power * 10 dbm
};

///////////////////////////////////////////////////////////////////////////////////////

struct SPI_Req_Get_Power_Header : public SPI_Req_Base_Header
{
};

struct SPI_Res_Get_Power_Header : public SPI_Res_Base_Header
{
    int16_t power; // power * 10 dbm
};

///////////////////////////////////////////////////////////////////////////////////////

struct SPI_Req_Set_Rate_Header : public SPI_Req_Base_Header
{
    uint8_t rate;
};

struct SPI_Res_Set_Rate_Header : public SPI_Res_Base_Header
{
    uint8_t rate;
};

///////////////////////////////////////////////////////////////////////////////////////

struct SPI_Req_Get_Rate_Header : public SPI_Req_Base_Header
{
};

struct SPI_Res_Get_Rate_Header : public SPI_Res_Base_Header
{
    uint8_t rate;
};

///////////////////////////////////////////////////////////////////////////////////////

struct SPI_Req_Setup_ADC_Header : public SPI_Req_Base_Header
{
    //ADC1
    //Channel 0 - GPIO36
    //Channel 1 - GPIO37 *** not exposed on most boards
    //Channel 2 - GPIO38 *** not exposed on most boards
    //Channel 3 - GPIO39
    //Channel 4 - GPIO32
    //Channel 5 - GPIO33
    //Channel 6 - GPIO34
    //Channel 7 - GPIO35
    uint32_t adc_enabled : 8;

    //0 - 9 bits
    //1 - 10 bits
    //2 - 11 bits
    //3 - 12 bits
    uint32_t adc_width : 2;

    //max rate: 100
    uint32_t adc_rate : 7;

    //0 - 0 dB (Full scale: 1.1V)
    //1 - 2.5 dB (Full scale: 1.5V)
    //2 - 6 dB (Full scale: 2.2V)
    //3 - 11 dB (Full scale: 3.9V)
    uint32_t adc_attenuation : 2; 
};

struct SPI_Res_Setup_ADC_Header : public SPI_Res_Base_Header
{
};

///////////////////////////////////////////////////////////////////////////////////////

struct SPI_Req_Get_ADC_Header : public SPI_Req_Base_Header
{
};

struct SPI_Res_Get_ADC_Header : public SPI_Res_Base_Header
{
    uint32_t adc0_sample_count : 16;
    uint32_t adc0_average : 16;

    uint32_t adc1_sample_count : 16;
    uint32_t adc1_average : 16;

    uint32_t adc2_sample_count : 16;
    uint32_t adc2_average : 16;

    uint32_t adc3_sample_count : 16;
    uint32_t adc3_average : 16;

    uint32_t adc4_sample_count : 16;
    uint32_t adc4_average : 16;

    uint32_t adc5_sample_count : 16;
    uint32_t adc5_average : 16;

    uint32_t adc6_sample_count : 16;
    uint32_t adc6_average : 16;

    uint32_t adc7_sample_count : 16;
    uint32_t adc7_average : 16;
};

///////////////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

