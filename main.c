/*
 * File:   main.c
 * Author: mcimr
 *
 * Created on February 19, 2023, 12:29 PM
 */

#include <xc.h>
#include "config_bits.h"
#include "version_git.h"

#include "custom_def.h"

#include "events.h"
#include "i2c.h"
#include "dice_pattern.h"

inline void initIO(void);
inline void initClock(void);

void ledsExecute(void);

// Table of time driven events.
static const struct stEVENT_INFO theEventsInfo [] = {
	//  Slot (us)				Event handler (void)
	{	EVENT_SLOT_MS(500),	    &ledsExecute },
};

#define evCOUNT (sizeof(theEventsInfo) / sizeof(theEventsInfo[0]))

static unsigned short theEvents [evCOUNT];

void main(void) {
    
    initClock();
    initIO();
    
    set_led_pattern(eLED_NONE, eSIDE_NONE);
    rLAT(SIDE_EN) = 1;
    
    for (;;) {
       int i = 0;
       
		for (i = 0; i < evCOUNT; i++) {
			if (eventExecute(&theEvents[i], theEventsInfo[i].Slot)) {
				theEventsInfo[i].Handler();
				break;
			}
		}
    }
    
    return;
}

void ledsExecute(void) {
    static unsigned char num = eLED_1;
    static unsigned char side = N_PATT_SIDES-1;
    if ( num >= N_PATT_LED) {
        side--;
        if (side >= N_PATT_SIDES)
            side = N_PATT_SIDES-1;
        num = eLED_1;
    }

    set_led_pattern(num++, 1 << (side+2));
}

inline void initIO(void) {
    ANSELA      = 0; // set all IO to digital
    
    LATA        = eLOW; // set pin values by default low
    rLAT(SCL)   = eHIGH; // make SCL high per I2C specifications
    rLAT(SDA)   = eHIGH; // make SDA high per I2C specifications
    
    TRISA       = 0; // init all bits as output, RA3 will be input as it is non-writable
}

inline void initClock(void) {
    OSCCONbits.IRCF = 0xB;   // 1MHz clock
    OSCCONbits.SCS = 0;      // Clock determined by FOSC<1:0> in configuration words (INTOSC)
    
    // setup timer 1 to keep track of time
    T1CONbits.TMR1CS = 0; // use FOSC/4
    T1CONbits.T1CKPS = 3; // 1:8 prescaler
    T1CONbits.TMR1ON = 1;  // enable timer 1    
}
