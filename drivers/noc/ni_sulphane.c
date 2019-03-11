#define NI_PACKET_SIZE 64 /* 1 packet = 64 flits */

#define NI_RECV_ADDR (uint16_t*)(0x90000000) /* mem1 */
#define NI_SEND_ADDR (uint16_t*)(0x90000080) /* mem2*/

//raises when the CPU acknowldge receving a packet. NI lowers.
#define COMM_NOC_ACK (int8_t*)(0x80000001)

//raises when the NI interrupts the CPU. CPU lowers.
#define COMM_NOC_INTR (int8_t*)(0x80000002)

//raises when the CPU starts the NI. NI lowers.
#define COMM_NOC_START (int8_t*)(0x80000003)

#include <hellfire.h>
#include <ni_generic.h>

int32_t ni_read_packet(uint16_t *buf, uint16_t pkt_size)
{	
	int i;
	volatile uint16_t* recv_addr;
	recv_addr = NI_RECV_ADDR;
	
	//copy packet from aux memory to main memory at given addr
	for(i = 0; i < pkt_size; i++)
		buf[i] = recv_addr[i];
	
	//raise ACK and lower intr
	*COMM_NOC_ACK  = 0x1;
	
	//hexdump(buf, NOC_PACKET_SIZE);
	//printf("\n");
	
	//*COMM_NOC_ACK = 0x0;
	return 0;
}

int32_t ni_write_packet(uint16_t *buf, uint16_t pkt_size)
{
	int i;

	volatile uint16_t* send_addr;
	send_addr = NI_SEND_ADDR;

	//holds until previous operation finishes
	while(*COMM_NOC_START == 0x1);
	
	//copy buf to the packet mem area
	for(i = 0; i < pkt_size; i++)
		send_addr[i] = buf[i];
	
	hexdump(buf, NI_PACKET_SIZE);
	printf("\n");
	
	//raise START so packets can be pushed to the router
	*COMM_NOC_START = 0x1;
	
	return 0;
}

int32_t ni_ready(void)
{
	//ready when start is down, other is sending already
	return (*COMM_NOC_START);
}

int32_t ni_flush(uint16_t pkt_size)
{	
	//no reason to flush
	return 1;
}
