#include <hellfire.h>
#include <ni_orca.h>
#include <noc.h>

//cpu-specific signals
volatile int8_t* sig_stall = SIGNAL_CPU_STALL;
volatile int8_t* sig_intr  = SIGNAL_CPU_INTR;

//signals to start the ni
volatile int8_t* sig_send  = SIGNAL_PROG_SEND;
volatile int8_t* sig_recv  = SIGNAL_PROG_RECV;

//signals to check on ni statuses
volatile int8_t*  sig_send_status = SIGNAL_SEND_STATUS;
volatile int32_t* sig_recv_status = SIGNAL_RECV_ADDR;

//signals to ni programming
volatile int32_t* sig_addr = SIGNAL_PROG_ADDR;
volatile int32_t* sig_size = SIGNAL_PROG_SIZE;

//memory place to flush unused data
uint8_t ni_flush_dummy[NI_PACKET_SIZE];

//get the size of the next packet 
int32_t ni_get_next_size(){
	return *sig_recv_status;
}

//reads one packet from the noc 
int32_t ni_read_packet(uint16_t *buf, uint16_t pkt_size)
{	
	//uint32_t im = _di();
	//printf("ni read %d flits\n", pkt_size);

	//configure dma 
	*sig_size = pkt_size;
	*sig_addr = (int32_t)buf;
	
	//if(*sig_addr <= 0x4000675c){
	//	printf("WTF >> %d\n", *sig_addr);
	//}

	//printf("ni prog size %d, addr 0x%x\n", *sig_size, *sig_addr);
	//printf("trg: 0x%x\n", buf[PKT_TARGET_CPU]);
	
	//stall and recv
	*sig_recv = 0x1;
	
	//CPU is stalled here, nothing to do
	//delay_ms(1);
	
	//flag off 
	*sig_recv = 0x0;
	
	//printf("----\n");
	//hexdump((int8_t*)buf, pkt_size * 2);
	//printf("\n");
	
	//_ei(im);
	
	//printf("out: %d\n", packet_counter_driver_lala++);
	
	return 0; //<<- no reason to fail
}

//writes one packet to the noc
int32_t ni_write_packet(uint16_t *buf, uint16_t pkt_size)
{
	//uint32_t im = _di();
	
	//printf("ni write %d flits\n", pkt_size);
	
	//if(*sig_addr <= 0x4000675c){
	//	printf("WTF >> %d\n", *sig_addr);
	//}
	
	//wait until previous send to finish
	while(*sig_send_status == 0x1);

	//hexdump((int8_t*)buf, pkt_size * 2);
	//printf("\n");
	//printf("numbytes = %d\n", pkt_size * 2);

	//configure dma 
	*sig_size = pkt_size;
	*sig_addr = (int32_t)buf;
	
	// printf("ni prog size %d, addr 0x%x\n", *sig_size, *sig_addr);

	//stall and send
	*sig_send = 0x1;
	
	//CPU is stalled here, nothing to do
	//delay_ms(1);
	
	//flag off 
	*sig_send = 0x0;
	
	//_ei(im);

	return 0; //<<- no reason to fail
}

int32_t ni_ready(void)
{
	//printf("ni ready\n");

	//ready when start is down, other is sending already
	return (*sig_send_status);
}

int32_t ni_flush(uint16_t pkt_size)
{
	//uint32_t im = _di();

	//printf("ni flush %d flits\n", pkt_size);

	//configure dma 
	*sig_size = pkt_size;
	*sig_addr = (int32_t)ni_flush_dummy;

	//*sig_size = 0;

	//printf("ni flush %d, addr 0x%x\n", *sig_size, *sig_addr);

	//stall and recv
	*sig_recv = 0x1;

	//delay_ms(1);
	
	//<---------CPU is stalled here, nothing to do
	
	//flag off 
	*sig_recv = 0x0;

	//_ei(im);

	//no reason to fail...
	return pkt_size;
}
