#define NI_PACKET_SIZE 64 /* 1 packet = 64 flits */

//#define NI_WRITE_ADDR 0x3FFFFF80
//#define NI_READ_ADDR  0x3FFFFF00
#define NI_READ_ADDR  0x90000000
#define NI_WRITE_ADDR 0x90000080


#define COMM_NOC_ACK    0x80000001
#define COMM_NOC_INTR   0x80000002
#define COMM_NOC_START  0x80000003
#define COMM_NOC_STATUS 0x80000004

#include <hellfire.h>
#include <ni_generic.h>

// 0 - means INTR is down and thus ni is ready.
// 1 - means INTR is up and packages cannot be sent yet.
int32_t ni_ready(void)
{
	uint8_t* comm_intr_ptr = (uint8_t*)COMM_NOC_INTR;
	return *comm_intr_ptr;
}

int32_t ni_flush(uint16_t pkt_size)
{	
	//clean auxiliary ram mem2. Please note that mem1 is ready-only.
	memset((uint16_t*)(NI_WRITE_ADDR), 0, sizeof(uint16_t) * NI_PACKET_SIZE);
	
	//no reason for failing.
	return 1;
}

int32_t ni_read_packet(uint16_t *buf, uint16_t pkt_size)
{
	uint16_t* recv_addr = (uint16_t*)(NI_READ_ADDR);
	int i = 0;
	
	for(; i < pkt_size; i++)
		buf[i] = recv_addr[i];
	
	//raise ACK
	uint8_t* comm_ack_ptr = (uint8_t*)(COMM_NOC_ACK);
	*comm_ack_ptr = 0x1;
	
	//no reason for failing	
	return 1;
}

int32_t ni_write_packet(uint16_t *buf, uint16_t pkt_size)
{
	int i;
	
	//holds until previous operation finishes
	while(ni_ready());
	
	//populate buffer
	uint16_t* send_addr = (uint16_t*)(NI_WRITE_ADDR);

	//copy buf to the packet mem area
	for(i = 0; i < pkt_size; i++) 
		send_addr[i] = buf[i];
	
	hexdump(send_addr, pkt_size);

	printf("\n");

	//raise INTR
	uint8_t* comm_intr_ptr = (uint8_t*)COMM_NOC_START;
	*comm_intr_ptr = 0x1;
	
	printf("ni_write_packet\n");
	return 1;
}