#include "contiki.h"
#include "contiki-lib.h"
#include "sys/compower.h"
#include "powertrace.h"
#include "net/rime/rime.h"
#include <stdio.h>
#include <string.h>

#define NUM_TICKS_IN_ONE_SECOND 32768


struct pow_tracking_info_actual{
//the sum is the total time
unsigned long cpu;
unsigned long lpm;

//the sum is the radio time
unsigned long transmit;
unsigned long listen;

unsigned long idle_transmit;
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


void print_actual_pow(struct pow_tracking_info_actual pow_info_actual){
	printf("(CPU) (LPM) Time:(%lu)(%lu) %lu\n",pow_info_actual.cpu, pow_info_actual.lpm, pow_info_actual.cpu+pow_info_actual.lpm);
	printf("(LISTEN) (TX) radio:(%lu)(%lu) %lu\n",pow_info_actual.listen, pow_info_actual.transmit, pow_info_actual.listen+pow_info_actual.transmit);

}
void send_data_pow_cons(struct pow_tracking_info_actual *pow_info_actual){
		

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
	//send_data_pow_cons(pow_info_actual);
}	
