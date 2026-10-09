; XC8: forward interrupts to the application without a C ISR prologue.
; Keep these destinations in sync with boot.h (checked in usbmmap.c).
; Reset at 0x0000 is provided by the XC8 C runtime.
    PSECT boot_high_vector,class=CODE,delta=1,abs,ovrld
    ORG 0x0008
    goto 0x08C8

    PSECT boot_low_vector,class=CODE,delta=1,abs,ovrld
    ORG 0x0018
    goto 0x08D8
