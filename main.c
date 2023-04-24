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
//	{	0, EVENT_INTERVAL_US(1000),	&ledsExecute },
    {   0, EVENT_INTERVAL_US(125000), &i2cExecute },
};

#define EVENT_COUNT (sizeof(event_info)/sizeof(event_info[0]))

void i2cExecute(void) {
//    unsigned char i2c_msg[2] = {MMA_DEVICE_ADDR | I2C_WRITE_BIT, MMA_WHO_AM_I};
//    if (I2C_write(i2c_msg, 2, 1)) {
//        i2c_msg[0] |= MMA_DEVICE_ADDR | I2C_READ_BIT;
//        if(I2C_write(i2c_msg, 1, 1)) {
//            I2C_read(i2c_msg, 2);
//            leds_display(i2c_msg[0]);
//        }
//    }
    
    I2C_write(MMA_DEVICE_ADDR | I2C_WRITE_BIT);
}

void main(void) {
    
    initClock();
    initIO();
    
    leds_display(02345);

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
    OSCCONbits.IRCF = 0xD;   // 4MHz clock
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
