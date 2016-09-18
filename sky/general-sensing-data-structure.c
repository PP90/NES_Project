#ifndef GENERAL_SENSE_DATA_STRUCT_C
#define GENERAL_SENSE_DATA_STRUCT_C 121212

#include "general-sensing-data-structure.h"

static void
sense_data(struct sensed_data *data_acquired){

		unsigned long mVolt_lux=light_sensor.value(0)*15*1000/40960;
		unsigned long uAmpere_lux=mVolt_lux/100;//mV/Kohm=uA
		data_acquired->light=6250*uAmpere_lux;
	
		data_acquired->temp=-40+sht11_sensor.value(SHT11_SENSOR_TEMP)/100;
		data_acquired->hum=sht11_sensor.value(SHT11_SENSOR_HUMIDITY);
		data_acquired->hum=-4+405*data_acquired->hum/10000-28*data_acquired->hum*data_acquired->hum/10000000;
}


void print_data(struct sensed_data data_acquired){
  printf("Temp:%lu °C\nHum:%lu %%\nLight:%lu Lux\n", \
  data_acquired.temp, \
  data_acquired.hum, \
  data_acquired.light); 
}

#endif
