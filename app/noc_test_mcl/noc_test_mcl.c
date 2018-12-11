#include <hellfire.h>
#include <noc.h>
#include <fixed.h>

#include "noc_test_mcl.h"

unsigned int packet_counter = 0;
int max_val = 0;
int max_index = 0;

#define itofix(V)		((V)*FIX_ONE/RESOLUTION)
#define itoprob(V)		((V)*FIX_ONE/255)
#define fixtoi(V)		((V)/FIX_ONE)

void mcl_slave(void)
{
	int8_t buf[120];
	uint16_t cpu, tsk, size;
	int16_t val;
	int i;
	int16_t distance[NUM_SAMPLES];
	struct Particle p;
	unsigned char map_val[NUM_SAMPLES];
	int32_t prob;

	if (hf_comm_create(hf_selfid(), 5000, 0))
		panic(0xff);

	while (1){

		//recv from outside
		val = hf_recv(&cpu, &tsk, buf, &size, 0);
		
		if (val){
			printf("hf_recv(): error %d\n", val);
		}else{

			for(i = 0; i < NUM_SAMPLES; i++){
				distance[i] = buf[4+i*2] << 8 | buf[5+i*2];
			}

			p.x 	= buf[4+i*2] << 8 | buf[5+i*2]; i++;
			p.y 	= buf[4+i*2] << 8 | buf[5+i*2]; i++;
			p.theta = buf[4+i*2] << 8 | buf[5+i*2];

			int32_t ptheta = itofix(p.theta);
			int32_t px = MAP_W/2 + p.x;
			int32_t py = MAP_H/2 + p.y;
			
			for(i = 0; i < NUM_SAMPLES/2; i++){
				int32_t sx = fixtoi(fix_cos(ptheta + angles[i]) * distance[i] + px*FIX_ONE);
				int32_t sy = fixtoi(fix_sin(ptheta + angles[i]) * distance[i] + py*FIX_ONE);

				buf[i*4]   = sx & 0x00ff;
				buf[i*4+1] = (sx & 0xff00) >> 8;
				buf[i*4+2] = sy & 0x00ff;
				buf[i*4+3] = (sy & 0xff00) >> 8;
			}
			val = hf_send(0, 5000, buf, 64, 0);

			val = hf_recv(&cpu, &tsk, buf, &size, 0);
			for(i = 0; i < NUM_SAMPLES/2; i++)
				map_val[i] = buf[i];

			for(i = 0; i < NUM_SAMPLES/2; i++){
				int32_t sx = fixtoi(fix_cos(ptheta + angles[i+NUM_SAMPLES/2]) * distance[i+NUM_SAMPLES/2] + px*FIX_ONE);
				int32_t sy = fixtoi(fix_sin(ptheta + angles[i+NUM_SAMPLES/2]) * distance[i+NUM_SAMPLES/2] + py*FIX_ONE);

				buf[i*4]   = sx & 0x00ff;
				buf[i*4+1] = (sx & 0xff00) >> 8;
				buf[i*4+2] = sy & 0x00ff;
				buf[i*4+3] = (sy & 0xff00) >> 8;
			}
			val = hf_send(0, 5000, buf, 64, 0);

			val = hf_recv(&cpu, &tsk, buf, &size, 0);
			for(i = 0; i < NUM_SAMPLES/2; i++)
				map_val[i+16] = buf[i];

			prob = FIX_ONE;
			for(i = 0; i < NUM_SAMPLES; i++)
				prob = fix_mul(itofix(256 - map_val[i]), prob);

			printf("prob = %d\n", prob);
		}
	}
}

void app_main(void)
{
	//spawn slaves for all PE (use hf_cpuid() to discriminate)
	hf_spawn(mcl_slave, 0, 0, 0, "mcl_slave", 4096);
}
