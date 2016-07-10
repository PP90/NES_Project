#ifndef ENERGY_CONS_C
#define ENERGY_CONS_C 423111

#include "contiki.h"
#include <stdio.h>
#include "energy_data_structure.h"

unsigned long energy_cons_cpu(struct energy_cons energy_cons_data){
	return energy_cons_data.cpu+energy_cons_data.lpm;
}

unsigned long energy_cons_radio(struct energy_cons energy_cons_data){
	return energy_cons_data.transmit+energy_cons_data.listen;
}


unsigned long energy_cons_pollution_low(struct energy_cons energy_cons_data){
	return energy_cons_data.co+energy_cons_data.co2_low+energy_cons_data.temp;
}

unsigned long energy_cons_pollution_peak(struct energy_cons energy_cons_data){
	return energy_cons_data.co+energy_cons_data.co2_peak+energy_cons_data.temp;
}


unsigned long total_energy_cons_low(struct energy_cons energy_cons_data){
	return energy_cons_cpu(energy_cons_data)/1000+energy_cons_radio(energy_cons_data)+energy_cons_pollution_low(energy_cons_data);

}

//The CPU power consumption is expressed in uJ
unsigned long total_energy_cons_peak(struct energy_cons energy_cons_data){
	return energy_cons_cpu(energy_cons_data)/1000+energy_cons_radio(energy_cons_data)+energy_cons_pollution_peak(energy_cons_data);

}


uint16_t energy_cons(uint16_t pow_cons, unsigned long time_ticks){
return pow_cons*time_ticks/NUM_TICKS_IN_ONE_SECOND;//W*s=J
}


void sum_up_energy_cons(struct energy_cons energy_cons_data){

unsigned long tot_energy_cons_low=total_energy_cons_low(energy_cons_data);
unsigned long tot_energy_cons_peak=total_energy_cons_peak(energy_cons_data);
printf("Total cpu:%lu uJ\n",energy_cons_cpu(energy_cons_data));
printf("Total radio: %lu mJ (MAX %lu%) (MIN %lu)%\n",energy_cons_radio(energy_cons_data), energy_cons_radio(energy_cons_data)*100/tot_energy_cons_low,  energy_cons_radio(energy_cons_data)*100/tot_energy_cons_peak);
printf("Total pollution (low): %lu mJ (%lu%)\n", energy_cons_pollution_low(energy_cons_data),energy_cons_pollution_low(energy_cons_data)*100/ tot_energy_cons_low);
printf("Total pollution (peak): %lu mJ (%lu%)\n", energy_cons_pollution_peak(energy_cons_data),energy_cons_pollution_peak(energy_cons_data)*100/ tot_energy_cons_peak);
printf("Total energy cons (low): %lu mJ\n",tot_energy_cons_low);
printf("Total energy cons (peak): %lu mJ\n",tot_energy_cons_peak);
}

/*
Set and print energy consumption data about pollution
*/
//all power consumption are expressed in mW, thus the energy consumption is expressed in mJ
void set_energy_pollution_sens(struct energy_cons *energy_cons_data, uint16_t time_sensing){
	
	energy_cons_data->time_sensing=time_sensing;
	energy_cons_data->co=CO_SENSOR_OP_CONDITION*time_sensing;
	energy_cons_data->co2_low=CO2_SENSOR_LOW*time_sensing;
	energy_cons_data->co2_peak=CO2_SENSOR_PEAK*time_sensing;
	energy_cons_data->temp=OPERATIONAL_CONSUME*time_sensing;
}


void energy_pollution_sens_print(struct energy_cons energy_cons_data){
	printf("\nPollution sensor energy consumption info:\n");
	printf("Time sensing: %u s\n",energy_cons_data.time_sensing);//Expressed in seconds
	printf("(co_sensor) %lu mJ\n", energy_cons_data.co);//mW*s=mJ
	printf("(co2_sensor (MIN) %lu mJ (peak) %lu mJ)\n",energy_cons_data.co2_low, energy_cons_data.co2_peak);//mW*s=mJ
	printf("(temp) %lu mJ\n",energy_cons_data.temp);
}

/*
Set and print energy consumption data about CPU
*/
void set_energy_cons_ucontr(struct energy_cons *energy_cons_data, struct pow_tracking_info_actual pow_info_actual){
	energy_cons_data->cpu=energy_cons(MSP430F1611_ACTIVE_MODE, pow_info_actual.cpu/1000);
	energy_cons_data->lpm=energy_cons(MSP430F1611_STANBY_MODE, pow_info_actual.lpm/1000);
}


void energy_cons_cpu_print(struct energy_cons energy_cons_data){
	printf("(CPU_ACTIVE) %u uJ\n",energy_cons_data.cpu);//nW*s/1000=mJ
	printf("(CPU_IDLE) %u uJ\n",energy_cons_data.lpm);//nW*s/1000=mJ
}



/*
Set and print energy consumption data about radio
*/
void set_energy_cons_radio(struct energy_cons *energy_cons_data, struct pow_tracking_info_actual pow_info_actual){
        
	unsigned long idle_radio=pow_info_actual.cpu+pow_info_actual.lpm-pow_info_actual.listen-pow_info_actual.transmit;
	energy_cons_data->listen=energy_cons(CC2420_RX, pow_info_actual.listen*1000);
	energy_cons_data->idle_listen=energy_cons(CC2420_RX, pow_info_actual.idle_listen*1000);
	energy_cons_data->transmit=energy_cons(CC2420_RX, pow_info_actual.transmit*1000);
	energy_cons_data->lpm=energy_cons(CC2420_IDLE, idle_radio);
}

void radio_energy_cons_print(struct energy_cons energy_cons_data){
	printf("Radio energy consumption:\n");
	printf("(rx_idle) %u uJ\n",energy_cons_data.idle_listen);//mV*s*1000=uJ
	printf("(rx) %u uJ\n",energy_cons_data.listen);//mV*s*1000=uJ
	printf("(tx)%u uJ\n",energy_cons_data.transmit);//mV*s*1000=uJ
	printf("(Idle)%u uJ\n",energy_cons_data.lpm);//Not multiplied per 1000 'cause the CC2420_IDLE is expressed in uW. uW*s=uJ;
}

#endif

