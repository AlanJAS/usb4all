; The bootloader forwards low interrupts here. IPEN remains zero in u4a2.
; A bare RETFIE keeps this unused vector defined without allocating a third
; software stack (which a C low-priority ISR would do).
    PSECT u4a_low_vector,class=CODE,delta=1,abs,ovrld
    ORG 0x8D8
    retfie 0
