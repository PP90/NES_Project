/*
 *
 * Simple application to indicate connectivity between two nodes:
 *
 * - Red led indicates a packet sent via radio (one packet sent each second)
 * - Green led indicates that this node can hear the other node but not
 *   necessary vice versa (unidirectional communication).
 * - Blue led indicates that both nodes can communicate with each
 *   other (bidirectional communication)
 */

#include "contiki.h"
#include "net/rime/rime.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "cc2420.h"
#include <stdio.h>
#include <string.h>

PROCESS(radio_test_process, "Radio test");
AUTOSTART_PROCESSES(&radio_test_process);

#define ON  1
#define OFF 0

#define HEADER "RTST"
#define PACKET_SIZE 20
#define PORT 9345

struct indicator {
  int onoff;
  int led;
  clock_time_t interval;
  struct etimer timer;
};
static struct etimer send_timer;
static struct indicator recv, other, flash;

/*---------------------------------------------------------------------*/
static void
set(struct indicator *indicator, int onoff) {
//printf("The led value is:%d\n",indicator->onoff);
  if(indicator->onoff ^ onoff) {
    indicator->onoff = onoff;
    if(onoff) {
      leds_on(indicator->led);
	
    } else {
      leds_off(indicator->led);
    }
  }
  if(onoff) {
    etimer_set(&indicator->timer, indicator->interval);
  }
}
/*---------------------------------------------------------------------------*/
static void
abc_recv(struct abc_conn *c)
{
  /* packet received */
	char buff[PACKET_SIZE];
	packetbuf_copyto(buff);
	printf("payload: %s\n",buff);

  if(packetbuf_datalen() < PACKET_SIZE
     || strncmp((char *)packetbuf_dataptr(), HEADER, sizeof(HEADER))) {
	printf("Invalid message because size is %d\n",packetbuf_datalen());
    /* invalid message */

  } else {
    PROCESS_CONTEXT_BEGIN(&radio_test_process);
    set(&recv, ON);
    set(&other, ((char *)packetbuf_dataptr())[sizeof(HEADER)] ? ON : OFF);

    /* synchronize the sending to keep the nodes from sending
       simultaneously */

    etimer_set(&send_timer, CLOCK_SECOND*3);
    etimer_adjust(&send_timer, - (int) (CLOCK_SECOND >> 1));
    PROCESS_CONTEXT_END(&radio_test_process);
  }
}
static const struct abc_callbacks abc_call = {abc_recv};
static struct abc_conn abc;
/*---------------------------------------------------------------------*/
PROCESS_THREAD(radio_test_process, ev, data)
{
  static uint8_t txpower;
  PROCESS_BEGIN();

  txpower = CC2420_TXPOWER_MAX;
	printf("Tx power is:%d\n",txpower);
  /* Initialize the indicators */
  recv.onoff = other.onoff = flash.onoff = OFF;
  recv.interval = other.interval = CLOCK_SECOND;
  flash.interval = 1;

  flash.led = LEDS_RED;
  recv.led = LEDS_GREEN;
  other.led = LEDS_BLUE;

  abc_open(&abc, PORT, &abc_call);
  etimer_set(&send_timer, CLOCK_SECOND*3);
  SENSORS_ACTIVATE(button_sensor);//Why do we activate the button ?

  while(1) {
    PROCESS_WAIT_EVENT();
    if (ev == PROCESS_EVENT_TIMER) {
      if(data == &send_timer) {
	etimer_reset(&send_timer);

	/* send packet */
	packetbuf_copyfrom(HEADER, sizeof(HEADER));
	//printf("Write something on the buffer: %d\n",packetbuf_copyfrom("aaa",4));
	((char *)packetbuf_dataptr())[sizeof(HEADER)] = recv.onoff;
	printf("Packet payload %c\n",packetbuf_dataptr());
	/* send arbitrary data to fill the packet size */
	packetbuf_set_datalen(PACKET_SIZE);
	set(&flash, ON);
	abc_send(&abc);


      } else if(data == &other.timer) {
	set(&other, OFF);

      } else if(data == &recv.timer) {
	set(&recv, OFF);

      } else if(data == &flash.timer) {
	set(&flash, OFF);
      }
    } else if(ev == sensors_event && data == &button_sensor) {
      if(txpower > 5) {
	txpower -= 5;
      } else {
	txpower = CC2420_TXPOWER_MAX;
	leds_blink();
      }
      cc2420_set_txpower(txpower);
      printf("txpower set to %u\n", txpower);
    }
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------*/
