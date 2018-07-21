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

#pragma pack(pop)

