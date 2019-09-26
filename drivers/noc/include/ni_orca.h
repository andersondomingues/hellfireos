#define NI_PACKET_SIZE 64 /* 1 packet = 64 flits */

#define MAGIC_TILE_ID       (uint16_t*)0x403F1000  

//dma-ni program signals 0 1 2 (3)
#define SIGNAL_CPU_STALL    (int8_t*)(0x403F0000)
#define SIGNAL_CPU_INTR     (int8_t*)(0x403F0001)
#define SIGNAL_SEND_STATUS  (int8_t*)(0x403F0002)

//dma-ni recv status 4-7
#define SIGNAL_RECV_ADDR    (int32_t*)(0x403F0004)

//programming low 8 9 (10 11)
#define SIGNAL_PROG_SEND    (int8_t*)(0x403F0008)
#define SIGNAL_PROG_RECV    (int8_t*)(0x403F0009)

//programming addresses 12-15 16-19
#define SIGNAL_PROG_ADDR    (int32_t*)(0x403F000C)
#define SIGNAL_PROG_SIZE    (int32_t*)(0x403F0010)

//raise/lower macroses
#define RAISE(x) *x=0x1
#define LOWER(x) *x=0x0

int32_t ni_ready(void);
int32_t ni_flush(uint16_t pkt_size);
int32_t ni_read_packet(uint16_t *buf, uint16_t pkt_size);
int32_t ni_write_packet(uint16_t *buf, uint16_t pkt_size);
int32_t ni_get_next_size();