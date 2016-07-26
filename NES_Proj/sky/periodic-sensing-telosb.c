/*

 */

#include <stdio.h>
#include "contiki.h"
#include "dev/button-sensor.h"
#include "dev/light-sensor.h"
#include "dev/leds.h"
#include "dev/sht11/sht11-sensor.h"
#include "sys/etimer.h"
#include "conf.h"
#include "energy-data-structure.c"
#include "general-sensing-data-structure.c"

/*---------------------------------------------------------------------------*/
PROCESS(myProcess, "myProcess");
AUTOSTART_PROCESSES(&myProcess);
/*---------------------------------------------------------------------------*/
static struct etimer et;
static struct sensed_data data_acquired;
static uint8_t i;
PROCESS_THREAD(myProcess, ev, data)
{
	
	PROCESS_BEGIN();
	etimer_set(&et, CLOCK_SECOND*HOW_OFTEN);
	i=0;
	while(1) {
		PROCESS_WAIT_EVENT();
		if(etimer_expired(&et)){
			leds_toggle(LEDS_ALL);
			SENSORS_ACTIVATE(light_sensor);
			SENSORS_ACTIVATE(sht11_sensor);
			sense_data(&data_acquired);
			i+=i;
			printf("#%u\n",i);
			print_data(data_acquired);

		//printf("Total consumed: %u\n",  total_energy_consumed());
		SENSORS_DEACTIVATE(sht11_sensor);
		SENSORS_DEACTIVATE(light_sensor);
		leds_toggle(LEDS_ALL);
		etimer_reset(&et);
		}
} 
PROCESS_END();   
  }
/*---------------------------------------------------------------------------*/
