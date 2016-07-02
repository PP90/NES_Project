#ifndef GENERAL_SENSE_DATA_STRUCT_C
#define GENERAL_SENSE_DATA_STRUCT_C 121212

#include "general-sensing-data-structure.h"


static void
sense_data(struct sensed_data *data_acquired){
		data_acquired->light=light_sensor.value(0);
		data_acquired->temp=sht11_sensor.value(SHT11_SENSOR_TEMP);
		data_acquired->hum=sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
}


void print_data(struct sensed_data data_acquired){
  printf("Temp:%u,Hum:%u,Light:%u\n", \
  data_acquired.temp, \
  data_acquired.hum, \
  data_acquired.light); 
}

#endif
