#ifndef POLLUTION_DATA_STRUCTURE_C 
#define POLLUTION_DATA_STRUCTURE_C 313

#include "DS1000.h"
#include "pollution_data_structure.h"

#define THRESHOLD_CO 100
#define THRESHOLD_CO2 300
#define THRESHOLD_TEMP 5

#define NO_THRESHOLD_EXCEED 0
#define CO2_EXCEED 1
#define CO_EXCEED 2
#define TEMP_EXCEED 4


#ifdef POLLUTION_SENSORS
static void
pollution_sensing(struct pollution_data *pollution_data_sensed){
	
	//clock_time_t t1=clock_time();	
	//leds_on (unsigned char leds);
	SENSORS_ACTIVATE(ds1000);	
	pollution_data_sensed->co=ds1000.value(SENSOR_CO);
	pollution_data_sensed->co2=ds1000.value(SENSOR_CO2);
	pollution_data_sensed->temp=ds1000.value(SENSOR_TEMP);//TO BE CONVERTED IN CELSIUS DEGREE
	SENSORS_DEACTIVATE(ds1000);
	//clock_time_t t2=clock_time();
	
	pollution_data_sensed->time_sensing=20;//Expressed in ms
	//pollution_data_sensed->time_sensing=t2-t1;	
	//printf("delta_t: %lu\n",pollution_data_sensed->time_sensing);

}

#endif

#endif
