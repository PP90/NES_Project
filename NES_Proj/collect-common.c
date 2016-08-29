/*
This source code is used in order to collect periodically data and send it to a specific destination.

 */

#include "contiki.h"
#include "lib/random.h"
#include "net/netstack.h"
#include "dev/serial-line.h"
#include "dev/leds.h"
#include "collect-common.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

static unsigned long time_offset;
static int send_active = 1;

#ifndef PERIOD
#define PERIOD 120
#endif

/*---------------------------------------------------------------------------*/
PROCESS(collect_common_process, "collect common process");
AUTOSTART_PROCESSES(&collect_common_process);
/*---------------------------------------------------------------------------*/
static unsigned long
get_time(void)
{
  return clock_seconds() + time_offset;
}
/*---------------------------------------------------------------------------*/
static unsigned long
strtolong(const char *data) {
  unsigned long value = 0;
  int i;
  for(i = 0; i < 10 && isdigit(data[i]); i++) {
    value = value * 10 + data[i] - '0';
  }
  return value;
}
/*---------------------------------------------------------------------------*/
void
collect_common_set_send_active(int active)
{
  send_active = active;
}
/*---------------------------------------------------------------------------*/
void
collect_common_recv(const linkaddr_t *originator, uint8_t seqno, uint8_t hops,
                    uint8_t *payload, uint16_t payload_len)
{
  unsigned long time;
  uint16_t data;
  int i;

  printf("%u", 8 + payload_len / 2);

  /* Timestamp. Ignore time synch for now. */
  time = get_time();
  printf(" %lu %lu 0", ((time >> 16) & 0xffff), time & 0xffff);
  /* Ignore latency for now */
  printf(" %u %u %u %u",
         originator->u8[0] + (originator->u8[1] << 8), seqno, hops, 0);

	printf("\nall Data:\n");
  for(i = 0; i < payload_len / 2; i++) {
    memcpy(&data, payload, sizeof(data));
    payload += sizeof(data);
    printf(" %u", data);
  }

  printf("\n");
  leds_blink();
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(collect_common_process, ev, data)
{
  static struct etimer period_timer, wait_timer;
  PROCESS_BEGIN();
	unsigned short seed=20;
  collect_common_net_init();
  random_init(seed);
  /* Send a packet every PERIOD + rand(0,1) seconds. */
  etimer_set(&period_timer, CLOCK_SECOND * PERIOD+random_rand()/512);//CLOCK SECOND IS EQUAL TO 128, i.e. one clock is equal to 7.8 ms
  while(1) {
    PROCESS_WAIT_EVENT();
    if(ev == PROCESS_EVENT_TIMER) {
      if(data == &period_timer) {
        etimer_reset(&period_timer);
        etimer_set(&wait_timer, PERIOD);
      } else if(data == &wait_timer) {
        if(send_active) {
          /* Time to send the data */
          collect_common_send();
        }
      }
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
