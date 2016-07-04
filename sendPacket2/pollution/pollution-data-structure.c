#ifndef POLLUTION_DATA_STRUCTURE_C 
#define POLLUTION_DATA_STRUCTURE_C 313

#include "DS1000.h"
#include "pollution_data_structure.h"
#include "confPollution.h"

static uint8_t //To be implemented
alarm(struct pollution_data *pollution_data)
{	
	struct pollution_data *pollution_data_old=pollution_data;
	return 1;
}

static void
pollution_sensing(struct pollution_data *pollution_data_sensed){
	pollution_data_sensed->co=ds1000.value(SENSOR_CO2);
	pollution_data_sensed->co2=ds1000.value(SENSOR_TEMP);
	pollution_data_sensed->temp=  ds1000.value(SENSOR_CO);
}


static void
print_pollution_values(struct pollution_data pollution_data_sensed){
	printf("CO2:%u,TEMPERATURE:%u,CO:%u\n", \
  pollution_data_sensed.co, \
  pollution_data_sensed.co2, \
  pollution_data_sensed.temp); 
}
#endif
