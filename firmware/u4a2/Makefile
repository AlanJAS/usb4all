# Standalone build of the active USB4all.X configuration for MPLAB XC8.
XC8 ?= /opt/microchip/xc8/v4.00/bin/xc8-cc
BUILD ?= build/xc8
PYTHON ?= python3
SOURCES := xc8/low_vector.S xc8/layout_checks.c \
   usb4all/proxys/T0Service.c \
  user/adminModule.c \
  user/descriptorManager.c \
  usb4all/dynamicISR/dynamicISR.c \
  user/handlerManager.c \
  user/loaderModule.c \
  main.c \
  user/pnp.c \
  user/port.c \
  user/usb4butia.c \
  system/usb/usb9/usb9.c \
  system/usb/usbctrltrf/usbctrltrf.c \
  system/usb/usbdrv/usbdrv.c \
  usb4all/usbconf/desc/usbdsc.c \
  usb4all/usbconf/usballoc/usbmmap.c \
  user/usr_ax.c \
  user/ax12.c \
  user/usr_button.c \
  user/usr_hackpoints.c \
  user/usr_motors.c \
  user/usr_butia.c \
  user/usr_modAct.c \
  user/usr_modSen.c
INCLUDES := -I. -Iuser -Iusb4all/proxys -Iusb4all/dynamicISR -Iusb4all/usbconf/usballoc -Iusb4all/usbconf/desc -Isystem -Isystem/usb -Iautofiles
DFP ?= /opt/microchip/mplabx/v6.35/packs/Microchip/PIC18Fxxxx_DFP/1.7.171/xc8
FLAGS := -mcpu=18F4550 -std=c99 -O2 -mstack=software -mcodeoffset=0x8C0 -mno-config -fsigned-char
ifneq ($(strip $(DFP)),)
FLAGS += -mdfp="$(DFP)"
endif
.DELETE_ON_ERROR:
.PHONY: all clean check
all: check
HEADERS := $(wildcard *.h system/*.h system/usb/*/*.h usb4all/*/*.h usb4all/usbconf/*/*.h user/*.h)
$(BUILD)/usb4all2.hex: $(SOURCES) $(HEADERS) $(MAKEFILE_LIST)
	mkdir -p $(BUILD)
	"$(XC8)" $(FLAGS) $(INCLUDES) $(SOURCES) -Wl,-Map=$(BUILD)/usb4all2.map -o $@
check: $(BUILD)/usb4all2.hex
	$(PYTHON) xc8/check_image.py $< $(BUILD)/usb4all2.sym
clean:
	rm -rf $(BUILD)
