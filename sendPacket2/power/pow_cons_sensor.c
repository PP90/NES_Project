#include "contiki.h"
#include "contiki-lib.h"
#include "sys/compower.h"
#include "powertrace.h"
#include "net/rime/rime.h"
#include <stdio.h>
#include <string.h>
#include "pow_cons.h"

#define NUM_TICKS_IN_ONE_SECOND 32768

struct pow_tracking_info_actual{
//the sum is the total time
unsigned long cpu;
unsigned long lpm;

//the sum is the radio time
unsigned long transmit;
unsigned long listen;

unsigned long idle_transmit;//It should be always zero
unsigned long idle_listen;
};

struct pow_tracking_info_all{
unsigned long seqno;//Incremented at each hop

unsigned long all_cpu;//Get from pow function
unsigned long all_lpm;

unsigned long all_transmit;
unsigned long all_listen;

unsigned long all_idle_transmit;
unsigned long all_idle_listen;
};


void pow_tracking_info_actual_init(struct pow_tracking_info_all *pow_info_all){
pow_info_all->seqno=0;
pow_info_all->all_cpu=0;
pow_info_all->all_lpm=0;
pow_info_all->all_transmit=0;
pow_info_all->all_listen=0;
pow_info_all->all_idle_transmit=0;
pow_info_all->all_idle_listen=0;
}

//TO NOT DELETE CAUSE PRINT THE INFO
/*
void
print_powertracing
(struct power_tracking_info_actual pow_cons_all, struct power_tracking_info_actual pow_cons_actual){

  unsigned long time, all_time, radio, all_radio;
	
  all_time = pow_info_all.all_cpu + pow_info_all.all_lpm;
	time = pow_info_actual->cpu + pow_info_actual->lpm;
  all_radio = energest_type_time(ENERGEST_TYPE_LISTEN) + energest_type_time(ENERGEST_TYPE_TRANSMIT);
	radio = pow_info_actual->transmit + pow_info_actual->listen;	
printf("SeqNo%lu\nAllCPU: %lu\nAllLPM: %lu\nAllTrasmit: %lu\nAllListen: %lu\n\
AllIdleTransmit %lu\nAllIdleListen %lu\n\
CPU %lu\nLPM %lu\ntransmit %lu\nlisten %lu\nidleTransmit %lu\nidleListen %lu\n\
(radio %d.%02d%% / %d.%02d%% tx %d.%02d%% / %d.%02d%% listen %d.%02d%% / %d.%02d%%)\n",
         seqno, 
	pow_cons_all.all_cpu, pow_cons_all.all_lpm, pow_cons_all.all_transmit, pow_cons_all.all_listen,
	pow_cons_all.all_idle_transmit, pow_cons_all.all_idle_listen,
	pow_cons_actual.cpu, pow_cons_actual.lpm, pow_cons_actual.transmit, pow_cons_actual.listen,
	pow_cons_actual.idle_transmit, pow_cons_actual.idle_listen,

	(int)((100L* (pow_cons_all.all_transmit + pow_cons_all.all_listen)) / all_time),
	(int)((10000L*(pow_cons_all.all_transmit + pow_cons_all.all_listen) / all_time)-
	(100L * (pow_cons_all.all_transmit+pow_cons_all.all_listen) / all_time)*100),

	(int)((100L * (pow_cons_actual.transmit + pow_cons_actual.listen)) / time),
	(int)((10000L * (transmit + listen) / time) - (100L * (transmit + listen) / time) * 100),
	 (int)((100L * all_transmit) / all_time),
         (int)((10000L * all_transmit) / all_time - (100L * all_transmit / all_time) * 100),
         (int)((100L * transmit) / time),
         (int)((10000L * transmit) / time - (100L * transmit / time) * 100),
         (int)((100L * all_listen) / all_time),
         (int)((10000L * all_listen) / all_time - (100L * all_listen / all_time) * 100),
         (int)((100L * listen) / time),
         (int)((10000L * listen) / time - (100L * listen / time) * 100));
}
*/

void print_duty_cycle(struct pow_tracking_info_actual pow_info_actual){
	unsigned long time=pow_info_actual.cpu + pow_info_actual.lpm;
	unsigned long radio=pow_info_actual.transmit + pow_info_actual.listen;
printf("\nDuty cycle:  %d.%02d%% (Period:%u ms)\n", (int)((100L * radio) / time),
	(int)((10000L * radio / time) - (100L *radio/ time) * 100),1000*time/NUM_TICKS_IN_ONE_SECOND);
}

void print_cpu_usage(struct pow_tracking_info_actual pow_info_actual){
	unsigned long time=pow_info_actual.cpu + pow_info_actual.lpm;
printf("\nCpu usage:  %d.%02d%%\n", (int)((100L * pow_info_actual.cpu) / time),
	(int)((10000L * pow_info_actual.cpu / time) - (100L *pow_info_actual.cpu/ time) * 100));
}


//Time is expressed in ticks

uint16_t energy_cons(uint16_t pow_cons, unsigned long time_ticks){
return pow_cons*time_ticks/NUM_TICKS_IN_ONE_SECOND;//W*s=J
}


void print_energy_pollution_sens(uint16_t time_sensing){
printf("\nPollution sensor energy consumption info:\n");
printf("Time sensing:%u s\n",time_sensing);//Expressed in seconds
printf("(co_sensor) %u mJ\n", CO_SENSOR_OP_CONDITION*time_sensing);//mW*s=mJ
printf("(co2_sensor (MIN) %u mJ (peak) %u mJ)\n",CO2_SENSOR_LOW*time_sensing, CO2_SENSOR_PEAK*time_sensing);//mW*s=mJ
printf("(temp) %u mJ\n",OPERATIONAL_CONSUME*time_sensing);//mW*s=mJ
}


void print_energy_cons_ucontr(struct pow_tracking_info_actual pow_info_actual){
printf("(CPU_ACTIVE) %u uJ\n",energy_cons(MSP430F1611_ACTIVE_MODE, pow_info_actual.cpu)/1000);//nW*s/1000=mJ
printf("(CPU_IDLE) %u uJ\n",energy_cons(MSP430F1611_STANBY_MODE, pow_info_actual.lpm)/1000);//nW*s/1000=mJ
}


void print_energy_cons_radio(struct pow_tracking_info_actual pow_info_actual){
        
	unsigned long idle_radio=pow_info_actual.cpu+pow_info_actual.lpm-pow_info_actual.listen-pow_info_actual.transmit;

	printf("Radio energy consumption:\n");
	printf("(rx_idle) %u uJ\n",energy_cons(CC2420_RX, pow_info_actual.idle_listen*1000));//mV*s*1000=uJ
	printf("(rx) %u uJ\n",energy_cons(CC2420_RX, pow_info_actual.listen*1000));//mV*s*1000=uJ
	printf("(tx)%u uJ\n",energy_cons(CC2420_TX, pow_info_actual.transmit*1000));//mV*s*1000=uJ
	printf("(Idle)%u uJ\n",energy_cons(CC2420_IDLE, idle_radio));//Not multiplied per 1000 'cause the CC2420_IDLE is expressed in uW
}

void print_actual_pow(struct pow_tracking_info_actual pow_info_actual){
	printf("(CPU) (LPM) Time:(%lu)(%lu) %lu\n",pow_info_actual.cpu, pow_info_actual.lpm, pow_info_actual.cpu+pow_info_actual.lpm);
	printf("(IDLE_LISTEN) (TOT_LISTEN) (TX) radio_time:(%lu) (%lu) (%lu) %lu\n",pow_info_actual.idle_listen, pow_info_actual.listen, pow_info_actual.transmit, pow_info_actual.listen+pow_info_actual.transmit);

}

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
