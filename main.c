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

#include "MMA8652FC.h"

inline void initIO(void);
inline void initClock(void);

void i2cExecute(void);

// Table of time driven events.
static event_info_t event_info [] = {
	//  Slot (us)				Event handler (void)
	{	0, EVENT_INTERVAL_US(1000),	&ledsExecute },
    {   0, EVENT_INTERVAL_US(10000), &i2cExecute },
};

#define EVENT_COUNT (sizeof(event_info)/sizeof(event_info[0]))

void i2cExecute(void) {
    union {
        signed char buff[3];
        struct {
            signed char x;
            signed char y;
            signed char z;
        } g;
    } data;
    comm_MMA_read(MMA_OUT_X_MSB, (unsigned char*)&data.buff, 3);
    
    static eSIDE_t side = eSIDE_D;
    leds_display(side, 0);
    
    if ( data.g.x <= -MMA_TRIP_mG_BIN )
        side = eSIDE_E;
    if ( data.g.x >=  MMA_TRIP_mG_BIN )
        side = eSIDE_A;
    if ( data.g.y <= -MMA_TRIP_mG_BIN )
        side = eSIDE_B;
    if ( data.g.y >=  MMA_TRIP_mG_BIN )
        side = eSIDE_F;
    if ( data.g.z <= -MMA_TRIP_mG_BIN )
        side = eSIDE_D;
    if ( data.g.z >=  MMA_TRIP_mG_BIN )
        side = eSIDE_C;
    
    leds_display(side, 7);
}

void main(void) {
    
    initClock();
    initIO();
    
//    writeMMA(MMA_XYZ_DATA_CFG, 0b01);
    comm_MMA_start(MMA_CTRL_REG1);
    comm_MMA_write_byte(0b11100011);
    comm_MMA_stop();
    
    for (;;) {
        eventExecute(event_info, EVENT_COUNT);
    }
    
    return;
}

inline void initIO(void) {
    ANSELA      = 0; // set all IO to digital
    
    // load default pin states
    LATA        = 0
                    | 1 << _SDA
                    | 1 << _SCL;
    
    TRISA       = 0; // init all bits as output, RA3 will be input as it is non-writable
}

inline void initClock(void) {
    OSCCONbits.IRCF = OSCCON_IRCF;
    OSCCONbits.SCS = 0;      // Clock determined by FOSC<1:0> in configuration words (INTOSC)
    
    T2CONbits.T2CKPS    = 0; // use FOSC/4 1:1
    T2CONbits.T2OUTPS   = 0; // no post scaler
    PR2 = (EVENT_TIME_UNIT / ((CPU_SPEED / 4) / 1000000)) - 1;
    // enable interrupt
    PIE1bits.TMR2IE     = 1;
    INTCONbits.PEIE     = 1;
    INTCONbits.GIE      = 1;
    T2CONbits.TMR2ON    = 1;
}
