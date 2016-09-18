#ifndef POLLUTION_DATA_STRUCTURE_C 
#define POLLUTION_DATA_STRUCTURE_C 313

#include "DS1000.h"
#include "pollution_data_structure.h"

void
pollution_sensing(struct pollution_data *pollution_data_sensed){
	
	uint16_t t1=RTIMER_NOW();
	
	SENSORS_ACTIVATE(ds1000);
	/*It must be wait 90 sec due to warming up time TODO*/
	printf ("SENSORS_ACTIVATE: %d\n",ds1000.status(SENSORS_ACTIVE));
	printf ("SENSORS_READY: %d\n",ds1000.status(SENSORS_READY));	
	pollution_data_sensed->co=ds1000.value(SENSOR_CO);
	pollution_data_sensed->co2=ds1000.value(SENSOR_CO2);
	pollution_data_sensed->temp=ds1000.value(SENSOR_TEMP);
	SENSORS_DEACTIVATE(ds1000);
	uint16_t t2=RTIMER_NOW();
	printf ("SENSORS_READY: %d\n",ds1000.status(SENSORS_READY));	
	pollution_data_sensed->time_sensing=t2-t1;	

}

void print_pollution_values_raw(struct pollution_data poll_data_acquired){
	printf("Not calibrated values\n");
	printf("CO2:%u ppm\n",poll_data_acquired.co2);
	printf("CO:%u ppm\n",poll_data_acquired.co);
	printf("Time sensing:%u Ticks of Rtimer\n",poll_data_acquired.time_sensing);
}


#endif
