#pragma once

//////////////////////////////////////////////////////////////////////
//boards
#define BOARD_SIMULATOR     0
#define BOARD_CRIUS_AIOP_2  1

//////////////////////////////////////////////////////////////////////
//CPU types
#define CPU_UNKNOWN         0
#define CPU_ATMEGA_2560		1

//////////////////////////////////////////////////////////////////////

#ifdef __AVR__
#   if !defined F_CPU
#       define F_CPU 16000000L
#   endif

#   define BOARD_TYPE BOARD_CRIUS_AIOP_2 //change this to your board
#	define CPU_TYPE CPU_ATMEGA_2560
#else
#   define BOARD_TYPE BOARD_SIMULATOR
#	define CPU_TYPE CPU_UNKNOWN
#endif



#define IO_BOARD_DBG(fmt, ...)  QLOG_DBG("io_board", fmt, ##__VA_ARGS__)
#define IO_BOARD_INFO(fmt, ...)  QLOG_INFO("io_board", fmt, ##__VA_ARGS__)
#define IO_BOARD_WARNING(fmt, ...)  QLOG_WARNING("io_board", fmt, ##__VA_ARGS__)
#define IO_BOARD_ERR(fmt, ...)  QLOG_ERR("io_board", fmt, ##__VA_ARGS__)
