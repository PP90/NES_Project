/*
 

 */

#include "contiki.h"
#include "contiki-lib.h"
#include "contiki-net.h"
#include "net/ip/uip.h"
#include "net/rpl/rpl.h"
#include "net/linkaddr.h"

#include "net/netstack.h"
#include "dev/button-sensor.h"
#include "dev/serial-line.h"
#if CONTIKI_TARGET_Z1
#include "dev/uart0.h"
#else
#include "dev/uart1.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "collect-common.h"
#include "collect-view.h"

#define DEBUG DEBUG_PRINT
#include "net/ip/uip-debug.h"
#include "power/pow_cons_sensor.c"
#include "pollution/pollution-data-structure.c"
#include "energy/energy_data_structure.c"
#define UIP_IP_BUF   ((struct uip_ip_hdr *)&uip_buf[UIP_LLH_LEN])

#define UDP_CLIENT_PORT 8775
#define UDP_SERVER_PORT 5688

static struct uip_udp_conn *server_conn;

PROCESS(udp_server_process, "UDP server process");
AUTOSTART_PROCESSES(&udp_server_process,&collect_common_process);
/*---------------------------------------------------------------------------*/
void
collect_common_set_sink(void)
{
}
/*---------------------------------------------------------------------------*/
void
collect_common_net_print(void)
{
  printf("I am sink!\n");
}
/*---------------------------------------------------------------------------*/
void
collect_common_send(void)
{
	//It should be implemented the polling request in order to receive the data in a different order than periodic
  /* Server never sends */
}
/*---------------------------------------------------------------------------*/
void
collect_common_net_init(void)
{
  serial_line_init();

  PRINTF("I am sink!\n");
}

/*---------------------------------------------------------------------------*/

/*
The field about power consumption are set. 
*/
void set_data_pow_cons(uint8_t i, unsigned long pow_cons_data, struct pow_tracking_info_actual *pow_info_actual){

	switch(i){
	case 0:
	pow_info_actual->cpu=pow_cons_data;
	break;

	case 1:
	pow_info_actual->lpm=pow_cons_data;
	break;

	case 2:
	pow_info_actual->idle_listen=pow_cons_data;
	break;

	case 3:
	pow_info_actual->listen=pow_cons_data;
	break;

	case 4:
	pow_info_actual->transmit=pow_cons_data;
	break;

	default:
	break;
	}
}


/*
The field about pollution are set. 
*/

void set_data_pollution(uint8_t i, uint16_t rec_pollution_data,struct pollution_data *pollution_info){

	switch(i){
	case 5:
	pollution_info->co=rec_pollution_data;
	break;

	case 6:
	pollution_info->co2=rec_pollution_data;
	break;

	case 7:
	pollution_info->temp=rec_pollution_data;
	break;

	case 8:
	pollution_info->time_sensing=rec_pollution_data;
	break;

	default:
	break;
	}
}

/*
The data coming from the pollution sensors are extracted and putted in the respective data structures.
*/


void extract_data2(uint8_t seqno, uint8_t *payload, uint16_t payload_len)
{
	uint8_t i=0;
	uint16_t rec_pollution_data;
	unsigned long pow_cons_data;
	struct pow_tracking_info_actual pow_info_actual;
	struct pollution_data pollution_data;
	struct energy_cons energy_cons_data;

	printf("(Packet #%u)\n",seqno);
	//In order to identify which node has sent the packet is necessary to get the MAC address.
	//FOR THE MAC ADDRESS SEE THIS LINK: https://sourceforge.net/p/contiki/mailman/message/28166309/
	//The read start from 2*8 byte, i.e. after 127 bit (16 byte). This 16 byte info are related to the WSN it self.
	payload += sizeof(rec_pollution_data)*22;

//printf("Power consumption data:\n");
while(i<5) {	
	memcpy(&pow_cons_data, payload, sizeof(pow_cons_data));
	
    payload += sizeof(pow_cons_data);//Shift of 32 bit (4 byte)
	set_data_pow_cons(i, pow_cons_data, &pow_info_actual);
	//printf(" %lu", pow_cons_data);
	i++;
  }
	//The duty cycle and the energy consumption of the radio is printed out
	set_energy_cons_radio(&energy_cons_data, pow_info_actual);
	radio_energy_cons_print(energy_cons_data);

	//The usage and the energy consumption of the CPU is printed out
	set_energy_cons_ucontr(&energy_cons_data,pow_info_actual);
	energy_cons_cpu_print(energy_cons_data);

//printf("Pollution data:\n");
while(i<9) {
	memcpy(&rec_pollution_data, payload, sizeof(rec_pollution_data));
    payload += sizeof(rec_pollution_data);//Shift of 16 bit
	set_data_pollution(i, rec_pollution_data,&pollution_data);
	//printf(" %u", pollution_data);
	i++;
  }
	//The energy consumption of the pollution sensor is printed out
	set_energy_pollution_sens(&energy_cons_data, pollution_data.time_sensing);
	energy_pollution_sens_print(energy_cons_data);

	//It is also printed out the CPU usage, duty cycle and a sum up of energy consumption
	print_cpu_usage(pow_info_actual);
	print_duty_cycle(pow_info_actual);	
	sum_up_energy_cons(energy_cons_data);

}


void
extract_data
(uint8_t seqno, uint8_t *payload, uint16_t payload_len)
{

	uint16_t data;
	int i;

	payload += sizeof(data)*12;
	printf("\nData:\n");

	for(i = 12; i < payload_len / 2; i++) {
		memcpy(&data, payload, sizeof(data));
		payload += sizeof(data);
		printf(" %u", data);
  	}
	printf("\n");

}
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void
tcpip_handler(void)
{
	//sender and hops are not used at all.
	uint8_t *appdata;
	linkaddr_t sender;
	uint8_t seqno;
	uint8_t hops;

	if(uip_newdata()) {
		appdata = (uint8_t *)uip_appdata;
		sender.u8[0] = UIP_IP_BUF->srcipaddr.u8[15];
 		sender.u8[1] = UIP_IP_BUF->srcipaddr.u8[14];
		hops = uip_ds6_if.cur_hop_limit - UIP_IP_BUF->ttl + 1;
		seqno = *appdata;
		extract_data2(seqno, appdata + 2, uip_datalen() - 2);
		//collect_common_recv(&sender, seqno, hops, appdata + 2, uip_datalen() - 2);
  	}
}
/*---------------------------------------------------------------------------*/
static void
print_local_addresses(void)
{
  int i;
  uint8_t state;

  PRINTF("Server IPv6 addresses: ");
  for(i = 0; i < UIP_DS6_ADDR_NB; i++) {
    state = uip_ds6_if.addr_list[i].state;
    if(state == ADDR_TENTATIVE || state == ADDR_PREFERRED) {
      PRINT6ADDR(&uip_ds6_if.addr_list[i].ipaddr);
      PRINTF("\n");
      /* hack to make address "final" */
      if (state == ADDR_TENTATIVE) {
        uip_ds6_if.addr_list[i].state = ADDR_PREFERRED;
      }
    }
  }
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(udp_server_process, ev, data)
{
  uip_ipaddr_t ipaddr;
  struct uip_ds6_addr *root_if;

  PROCESS_BEGIN();

  PROCESS_PAUSE();

  SENSORS_ACTIVATE(button_sensor);

  PRINTF("UDP server started\n");

#if UIP_CONF_ROUTER
  uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 1);
  /* uip_ds6_set_addr_iid(&ipaddr, &uip_lladdr); */
  uip_ds6_addr_add(&ipaddr, 0, ADDR_MANUAL);
  root_if = uip_ds6_addr_lookup(&ipaddr);
  if(root_if != NULL) {
    rpl_dag_t *dag;
    dag = rpl_set_root(RPL_DEFAULT_INSTANCE,(uip_ip6addr_t *)&ipaddr);
    uip_ip6addr(&ipaddr, UIP_DS6_DEFAULT_PREFIX, 0, 0, 0, 0, 0, 0, 0);
    rpl_set_prefix(dag, &ipaddr, 64);
    PRINTF("created a new RPL dag\n");
  } else {
    PRINTF("failed to create a new RPL DAG\n");
  }
#endif /* UIP_CONF_ROUTER */

  print_local_addresses();

  /* The data sink runs with a 100% duty cycle in order to ensure high
     packet reception rates. */
  NETSTACK_RDC.off(1);

  server_conn = udp_new(NULL, UIP_HTONS(UDP_CLIENT_PORT), NULL);
  udp_bind(server_conn, UIP_HTONS(UDP_SERVER_PORT));

  PRINTF("Created a server connection with remote address ");
  PRINT6ADDR(&server_conn->ripaddr);
  PRINTF(" local/remote port %u/%u\n", UIP_HTONS(server_conn->lport),
         UIP_HTONS(server_conn->rport));

  while(1) {
    PROCESS_YIELD();
    if(ev == tcpip_event) {
      tcpip_handler();
    } else if (ev == sensors_event && data == &button_sensor) {
      PRINTF("Initiaing global repair\n");
      rpl_repair_root(RPL_DEFAULT_INSTANCE);
    }
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
