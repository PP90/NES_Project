#include "contiki.h"
#include "DS1000.h"

#include <stdio.h> 

void print_values();

/*---------------------------------------------------------------------------*/
PROCESS(ds1000_test, "DS1000 Test");
AUTOSTART_PROCESSES(&ds1000_test);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ds1000_test, ev, data)
{
  PROCESS_BEGIN();

  static struct etimer et;
static struct etimer st;	
  static int i=0;
  while (1){
	printf("I wait 30 sec\n");
	etimer_set(&et, CLOCK_SECOND * 30);
	PROCESS_WAIT_UNTIL(etimer_expired(&st));
	printf("End wait 30 sec\n");

	SENSORS_ACTIVATE(ds1000);
	printf("I wait 90 sec (warming up time)\n");
	etimer_set(&et, CLOCK_SECOND * 90);	
	PROCESS_WAIT_UNTIL(etimer_expired(&et));
	printf("End wait 90 sec\n");
	print_values(i);
	SENSORS_DEACTIVATE(ds1000);	
	i++;	
	}

  PROCESS_END();
}

void print_values(int i){
  printf("[%d] CO2:%u,TEMPERATURE:%u,CO:%u\n", \
  i,ds1000.value(SENSOR_CO2), \
  ds1000.value(SENSOR_TEMP), \
  ds1000.value(SENSOR_CO)); 
}
/*---------------------------------------------------------------------------*/
