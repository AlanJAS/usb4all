# Standalone XC8 build. The legacy MCHPUSB.X project remains C18.
XC8 ?= /opt/microchip/xc8/v4.00/bin/xc8-cc
DFP ?= /opt/microchip/mplabx/v6.35/packs/Microchip/PIC18Fxxxx_DFP/1.7.171/xc8
BUILD ?= build/xc8

SOURCES := main.c boot.c usb9.c usbctrltrf.c usbdrv.c usbdsc.c usbmmap.c boot_vectors.S
HEADERS := $(wildcard *.h)
FLAGS := -mcpu=18F4550 -std=c99 -Os -mstack=compiled -I.
# Reserve the two assembly trampolines and the entire application region.
# Absolute assembly psects supply the trampolines, outside C allocation.
FLAGS += -mrom=default,-8-b,-18-1b,-8c0-7fff
ifneq ($(strip $(DFP)),)
FLAGS += -mdfp="$(DFP)"
endif

.DEFAULT_GOAL := all
.DELETE_ON_ERROR:
.PHONY: all
all: $(BUILD)/bootloader.hex

$(BUILD)/bootloader.hex: $(SOURCES) $(HEADERS) Makefile
	mkdir -p "$(BUILD)"
	"$(XC8)" $(FLAGS) $(SOURCES) -Wl,-Map=$(BUILD)/bootloader.map -o "$@"
