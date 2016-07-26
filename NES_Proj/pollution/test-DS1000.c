#include "contiki.h"
#include <stdio.h> 
#include "DS1000.h"
#include "pollution-data-structure.c"

void print_values();

/*---------------------------------------------------------------------------*/
PROCESS(ds1000_test, "DS1000 Test");
AUTOSTART_PROCESSES(&ds1000_test);
/*----------------------------------1-----------------------------------------*/
PROCESS_THREAD(ds1000_test, ev, data)
{
  PROCESS_BEGIN();

	static struct etimer et;
	static struct pollution_data poll_data_acquired;

	SENSORS_ACTIVATE(ds1000);
  	printf ("SENSORS_ACTIVATE: %d\n",ds1000.status(SENSORS_ACTIVE));
	printf ("SENSORS_READY: %d\n",ds1000.status(SENSORS_READY));
	
	while (1){
		
		etimer_set(&et, CLOCK_SECOND * 3);
		PROCESS_WAIT_UNTIL(etimer_expired(&et));
		pollution_sensing(&poll_data_acquired);
		print_pollution_values(poll_data_acquired);
		}

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
