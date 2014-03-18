#include "Config.h"
#include "board/sonar.h"

#if SONAR_TYPE == SONAR_NONE

void init() 
{

}
bool is_enabled()
{
	return false;
}
float get_distance()
{
	return 0;
}
		
#endif
