#include "timestamp.h"
#include <time.h>
#include <stdio.h>


char * date_type_string(char * buff){
	time_t the_time; 
	struct tm *timeInfo;
	time (&the_time);
	timeInfo = localtime(&the_time); // local time 
	strftime(buff, 80, "%d_%m_%Y", timeInfo); // style the time
	return buff; // return 
}
