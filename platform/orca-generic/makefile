APP = noc_test4
ARCH = riscv/hf-riscv

CPU_ARCH = \"$(ARCH)\"
MAX_TASKS = 30
MUTEX_TYPE = 0
MEM_ALLOC = 3
HEAP_SIZE = 500000
FLOATING_POINT = 0
KERNEL_LOG = 0

SRC_DIR = $(CURDIR)/../..

include $(SRC_DIR)/arch/$(ARCH)/arch.mak
include $(SRC_DIR)/lib/lib.mak
include $(SRC_DIR)/drivers/noc.mak
include $(SRC_DIR)/sys/kernel.mak
#include $(SRC_DIR)/$(APP)/app.mak
include $(SRC_DIR)/../applications/$(APP)/app.mak

INC_DIRS += -I $(SRC_DIR)/lib/include -I $(SRC_DIR)/sys/include -I $(SRC_DIR)/drivers/noc/include
CFLAGS += -DCPU_ID=$(CORE) -DCPU_ARCH=$(CPU_ARCH) -DMAX_TASKS=$(MAX_TASKS) -DMEM_ALLOC=$(MEM_ALLOC) -DHEAP_SIZE=$(HEAP_SIZE) -DMUTEX_TYPE=$(MUTEX_TYPE) -DFLOATING_POINT=$(FLOATING_POINT) -DKERNEL_LOG=$(KERNEL_LOG) $(NOC_FLAGS) -DDEBUG_PORT

CORE := 0
CORE_LIST = 0
NOC_FLAGS = -DNOC_INTERCONNECT -DNOC_WIDTH=4 -DNOC_HEIGHT=4 -DNOC_PACKET_SIZE=64 -DNOC_PACKET_SLOTS=64

images: 
	make hal
	make libc
	make noc
	make kernel
	make app
	$(LD) $(LDFLAGS) -T$(LINKER_SCRIPT) -o code0.elf *.o
	$(DUMP) --disassemble --reloc code0.elf > code0.lst
	$(DUMP) -h code0.elf > code0.sec
	$(DUMP) -s code0.elf > code0.cnt
	$(OBJ) -O binary code0.elf code0.bin
	$(SIZE) code0.elf
	hexdump -v -e '4/1 "%02x" "\n"' code0.bin > code0.txt
clean:
	rm -rf *.o *~ *.elf *.bin *.cnt *.lst *.sec *.txt

