#include <hellfire.h>
#include <noc.h>

unsigned int packet_counter = 0;
int8_t max_val = 0;
int8_t max_index = 0;

void receiver(void)
{
	int8_t buf[100];
	uint16_t cpu, tsk, size;
	int16_t val;

	if (hf_comm_create(hf_selfid(), 5000, 0))
		panic(0xff);

	while (1){
		
		//recv from outside (PE-0)
		val = hf_recv(&cpu, &tsk, buf, &size, 0);
		
		//recv succefully
		if (!val){
			
			//318 * 2 =  636 rays, only even ones are sent
			packet_counter = (packet_counter + 1) % 318;
			
			//limit of packets reach, rr
			if(packet_counter == 0){
				
				//TODO: integer division not working
				//TODO: integer multiplication not working				
				printf("max_val = %d, index = %d\n",
					max_val, max_index);
				
				//reset obtained values and start over
				max_val = 0;
				max_index = 0;

			}else{

				//store the max obtained value
				//among the burst

//				hexdump(buf, 100);

				int16_t index = *(int16_t*)&buf[0];
				index = (index >> 8) | ((index & 0x0F )<< 8);
				
				uint16_t range = *(uint16_t*)&buf[2];
				range = (range >> 8) | ((range & 0x0F )<< 8);
				
//				printf("%d = %d\n", index, range);

				if(range > max_val){
					max_val = range;
					max_index = index;
				}
			}
			
		}else{
			printf("hf_recv(): error %d\n", val);
		}
		
		
	}
//			delay_ms(1);		
}

//
//void sender(void)
//{
//	int8_t buf[100];
//	int16_t val;
//	
//	if (hf_comm_create(hf_selfid(), 1000, 0))
//		panic(0xff);
//		
//	delay_ms(50);
//	
//	buf[0] = 0xed;
//		
//	while(1){
//	
//		val = hf_send(0, 5000, buf, sizeof(buf), 5000);
//		if (val)
//			printf("hf_send(): error %d\n", val);
//	}
//}


void app_main(void)
{
//	if(hf_cpuid() == 5)
//		hf_spawn(sender, 0, 0, 0, "sender", 4096);	
//	else
//		hf_spawn(lala, 0, 0, 0, "lala", 4096);	
		
	//spawn receivers for all PE (use hf_cpuid() to discriminate)
	hf_spawn(receiver, 0, 0, 0, "receiver", 4096);
}
