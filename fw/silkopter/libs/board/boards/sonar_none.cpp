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
void get_data(Data& data)
{
	data.is_valid = false;
	data.altitude = 0;
}
		
#endif
