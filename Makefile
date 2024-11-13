CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
PORT = /dev/tty.usbserial-110

MCU = atmega328p
F_CPU = 16000000UL
PROGRAMMER = arduino

CFLAGS = -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -Wall

FILENAME = main
TARGET = $(FILENAME)
SRCS = $(FILENAME).c

$(TARGET).hex: $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET).o $(SRCS)
	$(OBJCOPY) -O ihex $(TARGET).o $@
	rm -f $(TARGET).o

upload: $(TARGET).hex
	$(AVRDUDE) -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b 115200 -D -U flash:w:$<:i

clean:
	rm -f $(TARGET).hex