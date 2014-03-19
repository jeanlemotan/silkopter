//////////////////////////////////////////////////////////////////////
//boards
#define CRIUS_AIOP_2 0

#define BOARD_TYPE CRIUS_AIOP_2


//////////////////////////////////////////////////////////////////////
//sonar types
#define SONAR_NONE			0
#define SONAR_SR04			1

#if BOARD_TYPE == CRIUS_AIOP_2
#	define SONAR_TYPE SONAR_SR04
#else
#	error "Specify your Sonar"
#endif

//////////////////////////////////////////////////////////////////////
//CPU types
#define ATMEGA_2560			0

#if BOARD_TYPE == CRIUS_AIOP_2
#	define CPU_TYPE ATMEGA_2560
#else
#	error "Specify your CPU"
#endif


