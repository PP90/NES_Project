#ifndef ENERGY_CONS_H
#define ENERGY_CONS_H 3424


struct energy_cons{

unsigned long time_sensing;
//cpu energy cons
unsigned long cpu;
unsigned long lpm;

////radio energy cons
unsigned long transmit;
unsigned long listen;
unsigned long idle_listen;
unsigned long sleep;

//pollution energy cons
unsigned long co;
unsigned long co2_low;
unsigned long co2_peak;
unsigned long temp;
};

/*Functions prototypes*/
unsigned long energy_cons_cpu(struct energy_cons energy_cons_data);
unsigned long energy_cons_radio(struct energy_cons energy_cons_data);
unsigned long energy_cons_pollution_low(struct energy_cons energy_cons_data);
unsigned long energy_cons_pollution_peak(struct energy_cons energy_cons_data);
unsigned long total_energy_cons_low(struct energy_cons energy_cons_data);
unsigned long total_energy_cons_peak(struct energy_cons energy_cons_data);
unsigned long energy_cons(uint16_t pow_cons, unsigned long time_ticks);
void sum_up_energy_cons(struct energy_cons energy_cons_data);
void set_energy_pollution_sens(struct energy_cons *energy_cons_data, unsigned long time_sensing);
void energy_pollution_sens_print(struct energy_cons energy_cons_data);
void set_energy_cons_ucontr(struct energy_cons *energy_cons_data, struct pow_tracking_info_actual pow_info_actual);
void energy_cons_cpu_print(struct energy_cons energy_cons_data);
void set_energy_cons_radio(struct energy_cons *energy_cons_data, struct pow_tracking_info_actual pow_info_actual);
void radio_energy_cons_print(struct energy_cons energy_cons_data);

#endif
