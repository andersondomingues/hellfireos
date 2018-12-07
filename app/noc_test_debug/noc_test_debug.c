#include <hellfire.h>
#include <noc.h>

unsigned int packet_counter = 0;
int max_val = 0;
int max_index = 0;

void receiver(void)
{
	int8_t buf[100];
	uint16_t cpu, tsk, size;
	int16_t val;

	if (hf_comm_create(hf_selfid(), 5000, 0))
		panic(0xff);

	while (1){
		
		//recv from outside
		val = hf_recv(&cpu, &tsk, buf, &size, 0);
		
		if (val){
			printf("hf_recv(): error %d\n", val);
						
		}else{
			
//			318 * 2 =  636 rays, only even ones are sent
			packet_counter = (packet_counter + 1) % 318;
			
			if(packet_counter == 0){
				
				//print best value obtained from the 
				//burst and reset counters
				
				//send best acquired value
				//through the network
//				val = hf_send(0, 5000, buf, sizeof(buf), 0);
//			
//				if (val)
//					printf("hf_send(): error %d\n", val);
				
				max_val = 0;
				max_index = 0;
			}else{
				
				//store the max obtained value
				//among the burst
				if(buf[5] > max_val){
					max_val = buf[5];
					max_index = buf[9];
				}
					
			}
			
			printf("max_val = %d, index = %d, counter is %d\n", max_val, max_index, packet_counter);
			
//			printf("%s\n", buf);			
			
//			printf("%s\n", buf);
			
//			char f = buf[5];
			
			
//			hexdump(buf, 100);			
			
			//answer back
//			val = hf_send(0, 5000, buf, sizeof(buf), 0);
//			
//			if (val)
//				printf("hf_send(): error %d\n", val);

//			delay_ms(1);		
		}
	}
}

void app_main(void)
{
	//spawn receivers for all PE (use hf_cpuid() to discriminate)
	hf_spawn(receiver, 0, 0, 0, "receiver", 4096);
}
