#define NI_PACKET_SIZE 64 /* 1 packet = 64 flits */

#define NI_RECV_ADDR 0x90000000 /* mem1 */
#define NI_SEND_ADDR 0x90000080 /* mem2*/

#define COMM_NOC_ACK    0x80000001
#define COMM_NOC_INTR   0x80000002
#define COMM_NOC_START  0x80000003
//#define COMM_NOC_STATUS 0x80000004

#define COMM_ACK   *((volatile uint8_t*)(COMM_NOC_ACK))
#define COMM_INTR  *((volatile uint8_t*)(COMM_NOC_INTR))
#define COMM_START *((volatile uint8_t*)(COMM_NOC_START))

#include <hellfire.h>
#include <ni_generic.h>

int32_t ni_read_packet(uint16_t *buf, uint16_t pkt_size)
{
	int i;
	uint16_t* recv_addr = (volatile uint16_t*)(NI_RECV_ADDR);
	
	//copy packet from aux memory to main memory
	for(i = 0; i < pkt_size; i++)
		buf[i] = recv_addr[i];
	
	//raise ACK
	COMM_ACK = 0x1;

	hexdump(buf, NI_PACKET_SIZE * 2);
	//wait INTR to down (unecessarily)
	while(COMM_INTR);
	
	return 0;
}

int32_t ni_write_packet(uint16_t *buf, uint16_t pkt_size)
{
	int i;
	
	//holds until previous operation finishes
	while(ni_ready());
	
	//populate buffer
	uint16_t* send_addr = (volatile uint16_t*)(NI_SEND_ADDR);

	//copy buf to the packet mem area
	for(i = 0; i < pkt_size; i++)
		send_addr[i] = buf[i];
	
	//printf("packet send...\n");
	//hexdump(send_addr, pkt_size);
	//printf("\n");
	
	//raise START so packets can be pushed to the router
	COMM_START = 0x1;
	
	return 0;
}

// 0 - means START is down and thus ni is ready.
// 1 - means START is up and packages cannot be sent yet.
int32_t ni_ready(void)
{
	return COMM_START;
}

int32_t ni_flush(uint16_t pkt_size)
{	
	//clean auxiliary ram mem2. Please note that mem1 is ready-only.
	//memset((uint16_t*)(NI_SEND_ADDR), 0, sizeof(uint16_t) * NI_PACKET_SIZE);
	
	//no reason for failing.
	ni_ready();
}
