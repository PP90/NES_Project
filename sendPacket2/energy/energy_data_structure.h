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

//pollution energy cons
unsigned long co;
unsigned long co2_low;
unsigned long co2_peak;
unsigned long temp;
};
#endif
