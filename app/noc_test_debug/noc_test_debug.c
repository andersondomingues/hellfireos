#include <hellfire.h>
#include <noc.h>

void idle(void){

	while(1);
}

void sender(void)
{
	int32_t msg = 0;
	int8_t buf[120];
	int16_t val;

	if (hf_comm_create(hf_selfid(), 1000, 0))
		panic(0xff);

	delay_ms(50);

	while (1){
		
		sprintf(buf, "i am cpu %d, thread %d: msg %d size: %d\n", hf_cpuid(), hf_selfid(), msg++, sizeof(buf));
		
		printf("app: sent message #%d\n", msg);
		
		//send buf data to core 8
//		val = hf_send(0, 5000, buf, sizeof(buf), 0);
//		if (val)
//			printf("hf_send(): error %d\n", val);
			
//		val = hf_send(13, 5000, buf, sizeof(buf), 0);
//		if (val)
//			printf("hf_send(): error %d\n", val);
		
		val = hf_send(0, 5000, buf, sizeof(buf), 0);
		if (val)
			printf("hf_send(): error %d\n", val);
			
		delay_ms(1);
	}
}

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
			
			printf("%s\n", buf);
			
			int32_t x = *(int32_t*)(&buf[0]);
			int32_t y = *(int32_t*)(&buf[4]);
			int32_t z = *(int32_t*)(&buf[8]);
			
			printf("x = %d, y = %d, z = %d\n", x, y, z);
			
			//answer back
			val = hf_send(0, 5000, buf, sizeof(buf), 0);
			
			if (val)
				printf("hf_send(): error %d\n", val);

			delay_ms(1);		
		}
	}
}

void app_main(void)
{
//	if (hf_cpuid() == 2){
//		hf_spawn(sender, 0, 0, 0, "sender", 4096);
//	}else{ // if (hf_cpuid() == 0 || hf_cpuid() == 3 || hf_cpuid() == 5){
	hf_spawn(receiver, 0, 0, 0, "receiver", 4096);
//	}
}
