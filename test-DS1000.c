 /**
 *  \file
 *       Test program and usage example of the DS1000 sensorboard contiki driver.
 *
 *  \author	 
 *       Eloy Díaz <eldial@gmail.com>
 */
#include "contiki.h"
#include "DS1000.h"
#include "conf.h"
#include <stdio.h> 

void print_values();

/*---------------------------------------------------------------------------*/
PROCESS(ds1000_test, "DS1000 Test");
AUTOSTART_PROCESSES(&ds1000_test);
/*----------------------------------1-----------------------------------------*/
PROCESS_THREAD(ds1000_test, ev, data)
{
  PROCESS_BEGIN();

  static struct etimer et;
  static int i=0;
	
  printf ("\n\nrunning SENSORS_ACTIVATE(ds1000). Values should slightly change\n");
  SENSORS_ACTIVATE(ds1000);
  printf ("SENSORS_ACTIVATE: %d\n",ds1000.status(SENSORS_ACTIVE));
  printf ("SENSORS_READY: %d\n",ds1000.status(SENSORS_READY));

  while (i<20){
    etimer_set(&et, CLOCK_SECOND * HOW_OFTEN_POLLUTION);
    PROCESS_WAIT_UNTIL(etimer_expired(&et));
    print_values();
    i++;
  }
  i=0;

  printf ("\n\nrunning SENSORS_DEACTIVATE(ds1000). Values should be fixed now\n");
  SENSORS_DEACTIVATE(ds1000);
  
  while (i<10){
    etimer_set(&et, CLOCK_SECOND * 1);
    PROCESS_WAIT_UNTIL(etimer_expired(&et));
    print_values();
    i++;
  }
  i=0;

  printf ("\n\nrunning SENSORS_ACTIVATE(ds1000). Values should slightly change\n");
  SENSORS_ACTIVATE(ds1000);

  while (i<20){
    etimer_set(&et, CLOCK_SECOND * 1);
    PROCESS_WAIT_UNTIL(etimer_expired(&et));
    print_values();
    i++;
  }
  i=0;

  printf ("\nTest complete\n");
  PROCESS_END();
}

void print_values(){
  printf("CO2:%u,TEMPERATURE:%u,CO:%u\n", \
  ds1000.value(SENSOR_CO2), \
  ds1000.value(SENSOR_TEMP), \
  ds1000.value(SENSOR_CO)); 
}
/*---------------------------------------------------------------------------*/
