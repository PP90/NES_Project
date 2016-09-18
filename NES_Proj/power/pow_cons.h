#include "contiki.h"
#include "contiki-lib.h"
#include "sys/compower.h"
#include "powertrace.h"
#include "net/rime/rime.h"
#include <stdio.h>
#include <string.h>

/*
This file containts all power consumption values used for the energy consumption profiling.
*/
#ifndef POW_CONS_VALUES_H 
#define POW_CONS_VALUES_H 7654


//MICROCONTROLLER MSP430F1611 POWER CONSUMPTION VALUES
#define MSP430F1611_SUPPLY 2200//mV
#define MSP430F1611_ACTIVE_MODE 330*MSP430F1611_SUPPLY//uA*mV=nW
#define MSP430F1611_STANBY_MODE 1.1*MSP430F1611_SUPPLY//uA*mV=nW


//RADIO MODULE CC2420 802.15.4 POWER CONSUMPTION VALUES (TAKEN FROM SLIDES)
#define CC2420_SUPPLY 3//V
#define CC2420_TX 19*CC2420_SUPPLY//mA*V=mW
#define CC2420_RX 17*CC2420_SUPPLY//mA*V=mW
#define CC2420_SLEEP 20*CC2420_SUPPLY//uA*V=uW 


//DS-100 POWER CONSUMPTION VALUES
#define POLLUTION_SENSOR_SUPPLY 9//V

//CO SENSOR
#define CO_SENSOR_OP_CONDITION 83//mW

//CO2 SENSOR
#define SUPPLY_POLLUTION 5 //vOLT
#define CO2_SENSOR_LOW 25*SUPPLY_POLLUTION//(mA*V)=mW
#define CO2_SENSOR_PEAK 125*SUPPLY_POLLUTION//(mA*V)=mW

//TEMP SENSOR
#define HEAT_DISSIPATION_CONSTANT 35//(mW/°C) to be multiplied by the °C sensed
#define OPERATIONAL_CONSUME 45//(mW/°C) to be multiplied by the °C sensed



//This data structure takes into account all times of the cpu, lpm, trasmit and so on in a given cycle. 
struct pow_tracking_info_actual{
//the sum is the total time
unsigned long cpu;
unsigned long lpm;

//the sum is the radio time
unsigned long transmit;
unsigned long listen;

unsigned long idle_transmit;//It should be always zero. Todo: to remove ??
unsigned long idle_listen;
};

//This data structure takes into account all times of the cpu, lpm, trasmit and so on in the overall time.
struct pow_tracking_info_all{
unsigned long seqno;//maybe not useful 'cause already used.

unsigned long all_cpu;
unsigned long all_lpm;

unsigned long all_transmit;
unsigned long all_listen;

unsigned long all_idle_transmit;
unsigned long all_idle_listen;
};

void print_duty_cycle(struct pow_tracking_info_actual pow_info_actual);
void print_cpu_usage(struct pow_tracking_info_actual pow_info_actual);
void print_actual_pow(struct pow_tracking_info_actual pow_info_actual);
void power_tracing(struct pow_tracking_info_all *pow_info_all,  struct pow_tracking_info_actual *pow_info_actual);


#endif
