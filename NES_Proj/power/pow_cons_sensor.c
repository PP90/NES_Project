#include "pow_cons.h"

#define NUM_TICKS_IN_ONE_SECOND 32768

/*
This function prints out the duty cycle. 
*/



void print_duty_cycle(struct pow_tracking_info_actual pow_info_actual){
	unsigned long time=pow_info_actual.cpu + pow_info_actual.lpm;
	unsigned long radio=pow_info_actual.transmit + pow_info_actual.listen;
	printf("Duty cycle:  %d.%02d%%\n", (int)((100L * radio) / time),
	(int)((10000L * radio / time) - (100L *radio/ time) * 100));
}



//This function prints out the CPU usage. 


void print_cpu_usage(struct pow_tracking_info_actual pow_info_actual){
	unsigned long time=pow_info_actual.cpu + pow_info_actual.lpm;
	printf("Cpu usage:  %d.%02d%%\n", (int)((100L * pow_info_actual.cpu) / time),
	(int)((10000L * pow_info_actual.cpu / time) - (100L *pow_info_actual.cpu/ time) * 100));
}


/*
This function prints out the time ticks of the CPU (when it is active and not) and of the radio.
*/


void print_actual_pow(struct pow_tracking_info_actual pow_info_actual){
	printf("(CPU) (LPM) Time:(%lu)(%lu) %lu\n",pow_info_actual.cpu, pow_info_actual.lpm, pow_info_actual.cpu+pow_info_actual.lpm);
	printf("(IDLE_RX) (TOT_RX) (TX) radio_time:(%lu) (%lu) (%lu) %lu\n",pow_info_actual.idle_listen, pow_info_actual.listen, pow_info_actual.transmit, pow_info_actual.listen+pow_info_actual.transmit);

}


/*
This function computes through the power trace the time of the various components of the sensor node.
*/

void
power_tracing(struct pow_tracking_info_all *pow_info_all,  struct pow_tracking_info_actual *pow_info_actual)
{
  static unsigned long last_cpu, last_lpm, last_transmit, last_listen;
  static unsigned long last_idle_transmit, last_idle_listen;
  static unsigned long seqno;
	
  energest_flush();
	
  pow_info_all->all_cpu = energest_type_time(ENERGEST_TYPE_CPU);
  pow_info_all->all_lpm = energest_type_time(ENERGEST_TYPE_LPM);
  pow_info_all->all_transmit = energest_type_time(ENERGEST_TYPE_TRANSMIT);
  pow_info_all->all_listen = energest_type_time(ENERGEST_TYPE_LISTEN);
  pow_info_all->all_idle_transmit = compower_idle_activity.transmit;
  pow_info_all->all_idle_listen = compower_idle_activity.listen;

  pow_info_actual->cpu = pow_info_all->all_cpu - last_cpu;
  pow_info_actual->lpm = pow_info_all->all_lpm - last_lpm;
  pow_info_actual->transmit = pow_info_all->all_transmit - last_transmit;
  pow_info_actual->listen = pow_info_all->all_listen - last_listen;
  pow_info_actual->idle_transmit = compower_idle_activity.transmit - last_idle_transmit;
  pow_info_actual->idle_listen = compower_idle_activity.listen - last_idle_listen;

  last_cpu = energest_type_time(ENERGEST_TYPE_CPU);
  last_lpm = energest_type_time(ENERGEST_TYPE_LPM);
  last_transmit = energest_type_time(ENERGEST_TYPE_TRANSMIT);
  last_listen = energest_type_time(ENERGEST_TYPE_LISTEN);
  last_idle_listen = compower_idle_activity.listen;
  last_idle_transmit = compower_idle_activity.transmit;

  pow_info_all->seqno=seqno++;
}	
