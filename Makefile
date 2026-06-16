# --- Config ---
MCU        = atmega328p
F_CPU      = 16000000UL
BAUD_RATE  = 9600
PORT       = /dev/ttyACM0
PROGRAMMER = arduino

# --- Toolchain ---
CC      = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# --- Flags ---
CFLAGS  = -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -Wall -std=c11
LDFLAGS = -mmcu=$(MCU)

# --- Files ---
TARGET = main
SRC    = main.c uart.c adc.c timer.c i2c.c mpu6050.c ds1307.c

# --- Rules ---
all: $(TARGET).hex

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b 115200 -U flash:w:$<

clean:
	rm -f *.elf *.hex

.PHONY: all flash clean