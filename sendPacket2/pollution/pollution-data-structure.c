#ifndef POLLUTION_DATA_STRUCTURE_C 
#define POLLUTION_DATA_STRUCTURE_C 313


#include "DS1000.h"
#include "pollution_data_structure.h"
#include "random.h"
#include "dev/leds.h"

#define THRESHOLD_CO 100
#define THRESHOLD_CO2 300
#define THRESHOLD_TEMP 5

/*Alarm function: This function can return one and only one of these following values: 0,1,2,3,4,5,6,7*/
static uint8_t
alarm(struct pollution_data pollution_data_old, struct pollution_data pollution_data_new)
{	
	uint8_t alarm_value=0;

	if((pollution_data_new.co2-pollution_data_old.co2)>=THRESHOLD_CO2) alarm_value+=1;
	if((pollution_data_new.co-pollution_data_old.co)>=THRESHOLD_CO) alarm_value+=2;
	if((pollution_data_new.temp-pollution_data_old.temp)>=THRESHOLD_TEMP) alarm_value+=4;

	return alarm_value;
}


void
print_alarm(uint8_t alarm_value){
	switch(alarm_value){
	case 0:
	printf("No alarm\n");
	break;

	case 1:
	printf("CO2 beyond the threshold\n");
	break;

	case 2:
	printf("CO beyond the threshold\n");
	break;

	case 3:
	printf("CO2 and CO beyond the thresholds\n");
	break;

	case 4:
	printf("TEMP beyond the threshold\n");
	break;

	case 5:
	printf("CO2 and temp the thresholds\n");
	break;

	case 6:
	printf("CO and temp the thresholds\n");
	break;

	case 7:
	printf("CO, CO2 and temp the thresholds\n");
	break;

	default:
	printf("Not predicted value\n");
	break;
}

}
static void
pollution_sensing(struct pollution_data *pollution_data_sensed){
	
	clock_time_t t1=clock_time();	
	//leds_on (unsigned char leds);
	SENSORS_ACTIVATE(ds1000);	
	pollution_data_sensed->co=ds1000.value(SENSOR_CO);
	pollution_data_sensed->co2=ds1000.value(SENSOR_CO2);
	pollution_data_sensed->temp=ds1000.value(SENSOR_TEMP);//TO BE CONVERTED IN CELSIUS DEGREE
	SENSORS_DEACTIVATE(ds1000);
	clock_time_t t2=clock_time();
	//pollution_data_sensed->time_sensing=t2-t1;
	//pollution_data_sensed->time_sensing=2+random_rand()/30000;//RANDOM BETWEEN 2 AND 4
	pollution_data_sensed->time_sensing=2;//I assume, for now, that is 2 sec fixed, but it can be change

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
