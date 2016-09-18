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
	rtimer_init ();
  	
	/*In one second the are 32768 ticks of rtimer*/
	while (1){
		etimer_set(&et, CLOCK_SECOND * 30);
		PROCESS_WAIT_UNTIL(etimer_expired(&et));
		//printf("RTIMER_SECOND:%d\n",RTIMER_SECOND);

		pollution_sensing(&poll_data_acquired);
		 print_pollution_values_raw(poll_data_acquired);
		}

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
