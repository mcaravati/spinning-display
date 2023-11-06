firmware = FIRM
filename = main
frq_cpu = 13000000

all : $(firmware)/$(filename).bin 

$(firmware)/$(filename).elf: $(firmware)/$(filename).c
	avr-gcc -mmcu=atmega328p -Os -o $(firmware)/$(filename).elf $(firmware)/$(filename).c -DF_CPU=$(frq_cpu)

$(firmware)/$(filename).bin: $(firmware)/$(filename).elf
	avr-objcopy -O binary $(firmware)/$(filename).elf $(firmware)/$(filename).bin

install: $(firmware)/$(filename).bin
	avrdude -p atmega328p -c usbasp -P /dev/ttyACM0 -U flash:w:$(firmware)/$(filename).bin