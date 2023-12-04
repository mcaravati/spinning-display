FREQ_CPU := 13000000
BAUDRATE := 115200

CC := avr-gcc
# https://gcc.gnu.org/bugzilla//show_bug.cgi?id=105523
CFLAGS := -Os -DF_CPU=$(FREQ_CPU) -DUSING_ATMEGA -DBAUD=$(BAUDRATE) -mmcu=atmega328p -Wall -Wextra -Werror --param=min-pagesize=0

all:
	@echo "ATMega compiler"

%.bin: %.elf
	avr-objcopy -j .text -j .data -O binary $^ $@

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

%.elf: spi_utils.o uart_utils.o ring_buffer.o main.c magnetic_sensor.o timer.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.elf *.bin *.o *.gch

install: main.bin
	avrdude -B 1 -p atmega328p -c usbasp -V -U flash:w:$<:r