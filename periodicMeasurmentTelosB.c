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

#define HOW_OFTEN 4

/*---------------------------------------------------------------------------*/
PROCESS(myProcess, "myProcess");
AUTOSTART_PROCESSES(&myProcess);
/*---------------------------------------------------------------------------*/
static struct etimer et;

PROCESS_THREAD(myProcess, ev, data)
{

  PROCESS_BEGIN();
  etimer_set(&et, CLOCK_SECOND*HOW_OFTEN);
  while(1) {
    PROCESS_WAIT_EVENT();
	if(etimer_expired(&et)){
		leds_toggle(LEDS_ALL);
		SENSORS_ACTIVATE(light_sensor);
		SENSORS_ACTIVATE(sht11_sensor);
		printf("Light: %d\n", light_sensor.value(0));
		printf("Temp: %d\n",sht11_sensor.value(SHT11_SENSOR_TEMP));
      		printf("Hum: %d\n",sht11_sensor.value(SHT11_SENSOR_HUMIDITY));
		SENSORS_DEACTIVATE(light_sensor);
		leds_toggle(LEDS_ALL);
		etimer_reset(&et);
		}
} 
PROCESS_END();   
  }
/*---------------------------------------------------------------------------*/
