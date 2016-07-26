/*
Energy data_structure.c source code. This src code cointans all function for the related data structure. 

*/

#ifndef ENERGY_CONS_C
#define ENERGY_CONS_C 423111

#include "contiki.h"
#include <stdio.h>
#include "energy_data_structure.h"

/*
Return the overall energy consumption of the CPU. It is expressed in uJ.
*/
unsigned long energy_cons_cpu(struct energy_cons energy_cons_data){
	return energy_cons_data.cpu+energy_cons_data.lpm;
}

/*
Return the overall energy consumption of the radio. It is expressed in uJ.
*/
unsigned long energy_cons_radio(struct energy_cons energy_cons_data){
	return energy_cons_data.transmit+energy_cons_data.listen+energy_cons_data.sleep;
}

/*
Return the overall energy consumption of the pollution sensor (CO+CO2+temp) in low power mode. It is expressed in uJ.
*/
unsigned long energy_cons_pollution_low(struct energy_cons energy_cons_data){
	return energy_cons_data.co+energy_cons_data.co2_low+energy_cons_data.temp;
}

/*
Return the overall energy consumption of the pollution sensor (CO+CO2+temp) in peak power mode. It is expressed in uJ.
*/
unsigned long energy_cons_pollution_peak(struct energy_cons energy_cons_data){
	return energy_cons_data.co+energy_cons_data.co2_peak+energy_cons_data.temp;
}


/*
Return the overall energy consumption of the sensor node in low power mode. It is expressed in uJ.
*/
unsigned long total_energy_cons_low(struct energy_cons energy_cons_data){
	return energy_cons_cpu(energy_cons_data)+energy_cons_radio(energy_cons_data)+energy_cons_pollution_low(energy_cons_data);

}

/*
Return the overall energy consumption of the sensor node in peak power mode. It is expressed in uJ.
*/
unsigned long total_energy_cons_peak(struct energy_cons energy_cons_data){
	return energy_cons_cpu(energy_cons_data)+energy_cons_radio(energy_cons_data)+energy_cons_pollution_peak(energy_cons_data);

}


/*
Return the energy consumption computed by the product of the power (1st parameter) 
and the time, i.e. the ratio between the time_ticks (2nd parameter) and the number of ticks in one second (well-know parameter already defined).
*/
unsigned long energy_cons(uint16_t pow_cons, unsigned long time_ticks){
return pow_cons*time_ticks/NUM_TICKS_IN_ONE_SECOND;//W*s=J
}

/*
This function prints out all the information about the energy consumption of the node.
*/
void sum_up_energy_cons(struct energy_cons energy_cons_data){

	unsigned long tot_energy_cons_low=total_energy_cons_low(energy_cons_data);
	unsigned long tot_energy_cons_peak=total_energy_cons_peak(energy_cons_data);
	unsigned long tot_cpu_energy_cons=energy_cons_cpu(energy_cons_data);
	printf("\n");
	printf("Tot CPU:%lu uJ (Min:%lu%%)\t(Max%lu%%) \n",tot_cpu_energy_cons, tot_cpu_energy_cons*100/tot_energy_cons_peak, tot_cpu_energy_cons*100/tot_energy_cons_low);

	printf("Tot radio: %lu uJ.\t(Min %lu%%)\t(Max %lu%%)\n",energy_cons_radio(energy_cons_data), energy_cons_radio(energy_cons_data)*100/	tot_energy_cons_peak,  energy_cons_radio(energy_cons_data)*100/tot_energy_cons_low);	

	printf("Tot pollution: (low %lu uJ %lu%%)\t(Peak %lu uJ %lu%%)\n", energy_cons_pollution_low(energy_cons_data),energy_cons_pollution_low(energy_cons_data)*100/ tot_energy_cons_low,energy_cons_pollution_peak(energy_cons_data),energy_cons_pollution_peak(energy_cons_data)*100/ tot_energy_cons_peak);

	printf("Tot energy cons:(low %lu uJ)\t(Peak %lu uJ)\n",tot_energy_cons_low, tot_energy_cons_peak);
}


/*
This function sets the energy consumption values about pollution sensor.
All power consumption are expressed in mW, thus the energy consumption is expressed in uJ, since that the time sensing in in ms
*/

void set_energy_pollution_sens(struct energy_cons *energy_cons_data, unsigned long time_sensing){
	energy_cons_data->time_sensing=time_sensing;
	energy_cons_data->co=CO_SENSOR_OP_CONDITION*time_sensing;
	energy_cons_data->co2_low=CO2_SENSOR_LOW*time_sensing;
	energy_cons_data->co2_peak=CO2_SENSOR_PEAK*time_sensing;
	energy_cons_data->temp=OPERATIONAL_CONSUME*time_sensing;
}

/*
This function prints the energy consumption values about pollution sensor.
*/
void energy_pollution_sens_print(struct energy_cons energy_cons_data){
	printf("Pollution sensor energy consumption:\n");
	printf("Time sensing: %u ms\n",energy_cons_data.time_sensing);//Expressed in ms
	printf("Co_sensor: %lu uJ\n", energy_cons_data.co);//mW*ms=uJ
	printf("Co2_sensor: (min %lu uJ) -> (peak %lu uJ)\n",energy_cons_data.co2_low, energy_cons_data.co2_peak);//mW*ms=uJ
	printf("Temp: %lu uJ\n",energy_cons_data.temp);
}

/*
This function sets and print energy consumption data about CPU.
The energy consumption of the cpu is expressed in mJ.
Dimensional analysis:
CPU active/stanby mode: nW/1000=uW
uW*s=uJ
*/

void set_energy_cons_ucontr(struct energy_cons *energy_cons_data, struct pow_tracking_info_actual pow_info_actual){
	energy_cons_data->cpu=energy_cons(MSP430F1611_ACTIVE_MODE, pow_info_actual.cpu/1000);//warning here.
	energy_cons_data->lpm=energy_cons(MSP430F1611_STANBY_MODE, pow_info_actual.lpm/1000);
}

/*
This function prints the energy consumption values about CPU.
*/
void energy_cons_cpu_print(struct energy_cons energy_cons_data){
	printf("[Cpu_active] %lu uJ\n",energy_cons_data.cpu);
	printf("[Cpu_idle] %lu uJ\n",energy_cons_data.lpm);
}


/*
This function sets the energy consumption values about the radio module.
//Listen energy consumption: mV*1000*s=uJ
//Idle listen energy consumption: mV*1000*s=uJ
//Transmit energy consumption: mV*1000*s=uJ
//Sleep energy consumption: uV*s=uJ
*/
void set_energy_cons_radio(struct energy_cons *energy_cons_data, struct pow_tracking_info_actual pow_info_actual){
        //Total time - listen_time - transmit time = sleep time of the radio
	unsigned long sleep_radio=pow_info_actual.cpu+pow_info_actual.lpm-pow_info_actual.listen-pow_info_actual.transmit;

	energy_cons_data->listen=energy_cons(CC2420_RX, pow_info_actual.listen*1000);
	energy_cons_data->idle_listen=energy_cons(CC2420_RX, pow_info_actual.idle_listen*1000);
	energy_cons_data->transmit=energy_cons(CC2420_TX, pow_info_actual.transmit*1000);
	energy_cons_data->sleep=energy_cons(CC2420_SLEEP, sleep_radio);
}

/*
This function prints the energy consumption values about the radio module.
*/
void radio_energy_cons_print(struct energy_cons energy_cons_data){
	printf("\nRadio energy consumption:\n");
	printf("[Rx_idle] %lu uJ\n",energy_cons_data.idle_listen);
	printf("[Rx] %lu uJ\n",energy_cons_data.listen);
	printf("[Rx wasted:%lu%%]\n",energy_cons_data.idle_listen*100/energy_cons_data.listen);
	printf("[Tx]%lu uJ\n",energy_cons_data.transmit);
	printf("[Sleep]%lu uJ\n",energy_cons_data.sleep);
}

#endif

