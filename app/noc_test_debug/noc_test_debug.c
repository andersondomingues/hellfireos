#include <hellfire.h>
#include <noc.h>

void idle(void){

	while(1);
}

void sender(void)
{
	int32_t msg = 0;
	int8_t buf[100];
	int16_t val;

	if (hf_comm_create(hf_selfid(), 1000, 0))
		panic(0xff);

	delay_ms(50);

	while (1){
		sprintf(buf, "i am cpu %d, thread %d: msg %d size: %d\n", hf_cpuid(), hf_selfid(), msg++, sizeof(buf));
		
		//send buf data to core 8
		val = hf_send(8, 5000, buf, sizeof(buf), 0);
		if (val)
			printf("hf_send(): error %d\n", val);
	}
}

void receiver(void)
{
	int8_t buf[2000];
	uint16_t cpu, tsk, size;
	int16_t val;

	if (hf_comm_create(hf_selfid(), 5000, 0))
		panic(0xff);

	while (1){
		val = hf_recv(&cpu, &tsk, buf, &size, 0);
		if (val)
			printf("hf_recv(): error %d\n", val);
		else
			printf("%s", buf);
	}
}

void app_main(void)
{
	if (hf_cpuid() == 0){
		hf_spawn(sender, 0, 0, 0, "xsender", 4096);
	}else if (hf_cpuid() == 8){
		hf_spawn(receiver, 0, 0, 0, "xreceiver", 4096);
	}//else{
	//	hf_spawn(idle, 0, 0, 0, "xidle", 4096);
	//}
}
