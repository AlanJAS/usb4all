#ifndef U4A_XC8_CLOCK_H
#define U4A_XC8_CLOCK_H
/* Repository bootloader: 20 MHz crystal, FOSC=HS, CPUDIV=OSC1_PLL2.
 * USB uses the separate 96 MHz PLL / 2. Do not assume a 48 MHz CPU. */
#define _XTAL_FREQ 20000000UL
#endif
