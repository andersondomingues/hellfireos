

#define NUM_SAMPLES 32
#define NUM_ODOMETRY 3
#define MPSOC_SIZE  16
#define RESOLUTION  100
#define MAP_W		1792
#define MAP_H		1700

struct Particle {
	int16_t w; //weight
	int16_t x;
	int16_t y;
	int16_t theta;
};

uint32_t angles[NUM_SAMPLES] = {
	fix_val(-2.268890),
	fix_val(-2.126864),
	fix_val(-1.984838),
	fix_val(-1.842809),
	fix_val(-1.700781),
	fix_val(-1.558753),
	fix_val(-1.416724),
	fix_val(-1.274696),
	fix_val(-1.132668),
	fix_val(-0.990639),
	fix_val(-0.848611),
	fix_val(-0.706583),
	fix_val(-0.564554),
	fix_val(-0.422526),
	fix_val(-0.280499),
	fix_val(-0.138471),
	fix_val(0.003557),
	fix_val(0.145585),
	fix_val(0.287613),
	fix_val(0.429640),
	fix_val(0.571668),
	fix_val(0.713697),
	fix_val(0.855725),
	fix_val(0.997753),
	fix_val(1.139782),
	fix_val(1.281810),
	fix_val(1.423838),
	fix_val(1.565867),
	fix_val(1.707895),
	fix_val(1.849923),
	fix_val(1.991952),
	fix_val(2.133978)
};