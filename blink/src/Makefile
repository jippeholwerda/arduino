TARGET = acceler

CC = avr-g++
# CPP = avr-g++
OBJCOPY = avr-objcopy
NM = avr-nm
AVR_ARCH = atmega2560
AVRDUDE_ARCH = m2560
AVRDUDE_PROGRAMMER = avrispmkII
F_CPU = 16000000
PORT = /dev/tty.usbmodem*
FORMAT = ihex
# CSTANDARD = -std=c89
# CSTANDARD = -std=gnu99


# AVRDUDE_VERBOSE = -v -v
AVRDUDE_FLAGS = -D -F -p $(AVRDUDE_ARCH) -P $(PORT) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS += $(AVRDUDE_NO_VERIFY)
AVRDUDE_FLAGS += $(AVRDUDE_VERBOSE)
AVRDUDE_FLAGS += $(AVRDUDE_ERASE_COUNTER)

AVRDUDE_WRITE_FLASH = -U flash:w:$(TARGET).hex
# AVRDUDE_WRITE_EEPROM = -U eeprom:w:$(TARGET).eep

CURDIR := $(dir $(lastword $(MAKEFILE_LIST)))
AVR_BASE_DIR  ?= $(abspath $(CURDIR)..)

# SRC = $(TARGET).c
ARDUINO_CORE = .
SRC = \
	$(ARDUINO_CORE)/$(TARGET).c \
	$(ARDUINO_CORE)/hooks.c \
	$(ARDUINO_CORE)/wiring.c \
	$(ARDUINO_CORE)/wiring_analog.c \
	$(ARDUINO_CORE)/wiring_digital.c \
	$(ARDUINO_CORE)/wiring_pulse.c \
	$(ARDUINO_CORE)/wiring_shift.c \
	$(ARDUINO_CORE)/WInterrupts.c \

CPPSRC = $(ARDUINO_CORE)/HardwareSerial.cpp

INC_DIR = $(AVR_BASE_DIR)/include
OBJ = $(SRC:.c=.o)
 # $(CPPSRC:.cpp=.o)
# CPPOBJ = $(SRC:.cpp=.o)
LST = $(SRC:.c=.lst)

CFLAGS = -DF_CPU=$(F_CPU)UL -D_MEGA_BOARD_ -DUSBCON
CFLAGS += -Os
CFLAGS += -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -Wall
CFLAGS += -Wa,-adhlns=$(<:.c=.lst)
CFLAGS += $(patsubst %,-I%,$(INC_DIR))
# CFLAGS +=  -Wstrict-prototypes $(CSTANDARD)

LDFLAGS = -lm -mmcu=$(AVR_ARCH) -Wl,--gc-sections

LDFLAGS = -Wl,-Map=$(TARGET).map,--cref
LDFLAGS += -lm

GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d
ALL_CFLAGS = -mmcu=$(AVR_ARCH) -I. $(CFLAGS) $(GENDEPFLAGS)

all: build

build: elf hex

elf: $(TARGET).elf
hex: $(TARGET).hex
eep: $(TARGET).eep
sym: $(TARGET).sym

%.hex: %.elf
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

%.eep: %.elf
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 -O $(FORMAT) $< $@

%.lss: %.elf
	$(OBJDUMP) -h -S $< > $@

%.sym: %.elf
	$(NM) -n $< > $@

%.o : %.c
	$(CC) -c $(ALL_CFLAGS) $< -o $@

# %.o : %.cpp
# 	$(CPP) -c $(ALL_CFLAGS) $< -o $@

$(TARGET).elf: $(OBJ) $(TARGET).o
	$(CC) $(ALL_CFLAGS) $^ new.o WString.o Print.o Stream.o HardwareSerial.o --output $@ $(LDFLAGS)

test:
	echo $(OBJ)

clean:
	rm -f $(TARGET).hex $(TARGET).eep $(TARGET).elf $(TARGET).map $(TARGET).lst $(TARGET).sym $(LST) $(OBJ) $(TARGET).o

upload: $(TARGET).hex $(TARGET).eep
	avrdude $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) $(AVRDUDE_WRITE_EEPROM)

-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)
