PORT:=/dev/ttyUSB0

CC = xtensa-lx106-elf-gcc
CFLAGS = -Os -Iinclude -mlongcalls

LDFLAGS = \
	-nostdlib \
	-Teagle.app.v6.ld \
	-Wl,--no-check-sections \
	-u call_user_start \
	-Wl,-static \
	-Wl,--start-group \
	-lc \
	-lcirom \
	-lgcc \
	-lhal \
	-ljson \
	-lmain \
	-lupgrade \
	-lnet80211 \
	-llwip \
	-lphy \
	-lpp \
	-lpwm \
	-lsmartconfig \
	-lssl \
	-lwpa \
	-Wl,--end-group

OBJS = \
	app.o \
	uart.o

all : app-0x00000.bin

app-0x00000.bin: app
	esptool.py elf2image $^

app: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	
flash: app-0x00000.bin
	esptool.py write_flash 0 app-0x00000.bin 0x40000 app-0x40000.bin

clean:
	rm -rf *.o app app-0x00000.bin app-0x40000.bin
