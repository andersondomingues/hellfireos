#include <hellfire.h>
#include <ni_orca.h>

//cpu-specific signals
volatile int8_t* sig_stall = SIGNAL_CPU_STALL;
volatile int8_t* sig_intr  = SIGNAL_CPU_INTR;

//signals to start the ni
volatile int8_t* sig_send  = SIGNAL_PROG_SEND;
volatile int8_t* sig_recv  = SIGNAL_PROG_RECV;

//signals to check on ni statuses
volatile int8_t* sig_send_status = SIGNAL_SEND_STATUS;
volatile int32_t* sig_recv_addr = SIGNAL_RECV_ADDR;

//signals to ni programming
volatile int32_t* sig_addr = SIGNAL_PROG_ADDR;
volatile int32_t* sig_size = SIGNAL_PROG_SIZE;


int32_t ni_get_next_size(){
	return *sig_recv_addr;
}

int32_t ni_read_packet(uint16_t *buf, uint16_t pkt_size)
{	
	printf("ni read size=%d\n", pkt_size);

	//configure dma 
	*sig_size = pkt_size;
	*sig_addr = (int32_t)buf;
	
	//stall and recv
	*sig_recv = 0x1;
	
	//CPU is stalled here, nothing to do
	
	//flag off 
	*sig_recv = 0x0;
	
	hexdump(buf, pkt_size);
		
	return 0; //<<- no reason to fail
}

int32_t ni_write_packet(uint16_t *buf, uint16_t pkt_size)
{
	printf("ni write size=%d\n", pkt_size);
	
	//wait until previous send to finish
	while(*sig_send_status == 0x1);
	
	//configure dma 
	*sig_size = pkt_size;
	*sig_addr = (int32_t)buf;
	
	//stall and send
	*sig_send = 0x1;
	
	//CPU is stalled here, nothing to do
	
	//flag off 
	*sig_send = 0x0;
	
	hexdump(buf, pkt_size);
	
	return 0; //<<- no reason to fail
}

int32_t ni_ready(void)
{
	printf("ni ready\n");

	//ready when start is down, other is sending already
	return (*sig_send);
}

int32_t ni_flush(uint16_t pkt_size)
{
	printf("ni flush\n");

	//raise ACK
	//*comm_noc_ack = 0x1;
	
	//wait for interruption to go low
	//while(*comm_noc_intr == 0x1); 
	
	//lowers ack and proceed
	//*comm_noc_ack = 0x0; 

	//no reason to fail...
	return 1;
}
