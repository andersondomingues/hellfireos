#define NI_PACKET_SIZE 64 /* 1 packet = 64 flits */

#define NI_RECV_ADDR (uint16_t*)(0x90000000) /* mem1 */
#define NI_SEND_ADDR (uint16_t*)(0x90000080) /* mem2*/

#define COMM_NOC_ACK    (int8_t*)(0x80000001)
#define COMM_NOC_INTR   (int8_t*)(0x80000002)
#define COMM_NOC_START  (int8_t*)(0x80000003)

#include <hellfire.h>
#include <ni_generic.h>

int32_t ni_read_packet(uint16_t *buf, uint16_t pkt_size)
{	
	int i;
	volatile uint16_t* recv_addr;
	recv_addr = NI_RECV_ADDR;
	
	//printf("recv drvier\n");

	//copy packet from aux memory to main memory at given addr
	for(i = 0; i < pkt_size; i++)
		buf[i] = recv_addr[i];
	
	//raise ACK
	*COMM_NOC_ACK = 0x1;
	
	//hexdump(buf, NOC_PACKET_SIZE);
	
	//while(COMM_ACK);
	//wait for netif to ack (why is it necessary?)
	
	return 0;
}

int32_t ni_write_packet(uint16_t *buf, uint16_t pkt_size)
{
	int i;

	volatile uint16_t* send_addr;
	send_addr = NI_SEND_ADDR;

	//holds until previous operation finishes
	while(ni_ready());
	
	//copy buf to the packet mem area
	for(i = 0; i < pkt_size; i++)
		send_addr[i] = buf[i];
	
	//hexdump(send_addr, NI_PACKET_SIZE);
	
	//raise START so packets can be pushed to the router
	*COMM_NOC_START = 0x1;
	
	return 0;
}

// 0 - means START is down and thus ni is ready.
// 1 - means START is up and packages cannot be sent yet.
int32_t ni_ready(void)
{
	return *COMM_NOC_START;
}

int32_t ni_flush(uint16_t pkt_size)
{	
	//clean auxiliary ram mem2. Please note that mem1 is ready-only.
	//memset((uint16_t*)(NI_SEND_ADDR), 0, sizeof(uint16_t) * NI_PACKET_SIZE);
	
	//no reason for failing.
	//ni_ready();
	return 1;
}
