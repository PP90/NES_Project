#ifndef POLLUTION_DATA_STRUCTURE_C 
#define POLLUTION_DATA_STRUCTURE_C 313


#include "DS1000.h"
#include "pollution_data_structure.h"
#include "confPollution.h"
#include "random.h"

static uint8_t //To be implemented
alarm(struct pollution_data *pollution_data)
{	
	struct pollution_data *pollution_data_old=pollution_data;
	return 1;
}

static void
pollution_sensing(struct pollution_data *pollution_data_sensed){
	clock_time_t t1=clock_time();	
	SENSORS_ACTIVATE(ds1000);	
	pollution_data_sensed->co=ds1000.value(SENSOR_CO2);
	pollution_data_sensed->co2=ds1000.value(SENSOR_TEMP);
	pollution_data_sensed->temp=ds1000.value(SENSOR_CO);
	SENSORS_DEACTIVATE(ds1000);
	clock_time_t t2=clock_time();
	//pollution_data_sensed->time_sensing=t2-t1;
	pollution_data_sensed->time_sensing=2+random_rand()/30000;//RANDOM BETWEEN 2 AND 4
	//printf("delta_t: %lu\n",pollution_data_sensed->time_sensing);

}


static void
print_pollution_values(struct pollution_data pollution_data_sensed){
	printf("CO2:%u,TEMPERATURE:%u,CO:%u\n", \
  pollution_data_sensed.co, \
  pollution_data_sensed.co2, \
  pollution_data_sensed.temp); 
}
#endif
