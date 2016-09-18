#ifndef ENERGY_DATA_STRUCTURE_C 
#define ENERGY_DATA_STRUCTURE_C 4332

#include "energy-data-structure.h"

static uint32_t 
tot_energy_cons(void)
{	
	struct energy_data_structure *my_struct;
	uint32_t total_energy;
	my_struct->energy_lpm = energest_type_time(ENERGEST_TYPE_LPM);
	my_struct->energy_cpu = energest_type_time(ENERGEST_TYPE_CPU);
	my_struct->energy_rx = energest_type_time(ENERGEST_TYPE_LISTEN);
	my_struct->energy_tx = energest_type_time(ENERGEST_TYPE_TRANSMIT);
	my_struct->energy_rled = energest_type_time(ENERGEST_TYPE_LED_RED);
	total_energy=my_struct->energy_lpm+my_struct->energy_cpu+my_struct->energy_rx+my_struct->energy_tx+my_struct->energy_rled;
	return total_energy;
}

#endif
