#ifndef U4A_XC8_EEPROM_H
#define U4A_XC8_EEPROM_H
#include <xc.h>
/* Blocking write: the boot request must survive the immediately following reset.
 * Preserve GIE; only the unlock sequence needs interrupts disabled. */
static inline void u4a_eeprom_write(unsigned char address, unsigned char value)
{
    unsigned char gie;
    while (EECON1bits.WR) { }
    EEADR = address;
    EEDATA = value;
    EECON1bits.EEPGD = 0;
    EECON1bits.CFGS = 0;
    EECON1bits.WREN = 1;
    gie = INTCONbits.GIE;
    INTCONbits.GIE = 0;
    EECON2 = 0x55;
    EECON2 = 0xAA;
    EECON1bits.WR = 1;
    INTCONbits.GIE = gie;
    while (EECON1bits.WR) { }
    EECON1bits.WREN = 0;
}
#endif
