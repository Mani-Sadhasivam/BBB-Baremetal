TOOLCHAIN 	= arm-none-eabi

AS 		= $(TOOLCHAIN)-as
CC 		= $(TOOLCHAIN)-gcc
LD 		= $(TOOLCHAIN)-ld
OBJCOPY 	= $(TOOLCHAIN)-objcopy
CFLAGS 		= -Wall -mcpu=cortex-a8 -mtune=cortex-a8 -march=armv7-a -mfpu=neon -mfloat-abi=softfp -nostartfiles -ffreestanding -nostdlib \
		  -O0 -g
ASFLAGS 	= -mcpu=cortex-a8	

