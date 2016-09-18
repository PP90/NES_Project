#ifndef POLLUTION_DATA_STRUCT_H
#define POLLUTION_DATA_STRUCT_H 4212

#define THRESHOLD_CO 100
#define THRESHOLD_CO2 300
#define THRESHOLD_TEMP 5

#define NO_THRESHOLD_EXCEED 0
#define CO2_EXCEED 1
#define CO_EXCEED 2
#define TEMP_EXCEED 4


struct pollution_data {
	uint16_t time_sensing;
	uint16_t co;
	uint16_t co2;
	uint16_t temp;
};

void pollution_sensing(struct pollution_data *pollution_data_sensed);
void print_pollution_values_raw(struct pollution_data poll_data_acquired);

#endif
