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

enum {
    leds_event = 0,
    mma_event  = 1,
    n_events
};

static unsigned short event_timer[n_events] = {0};

#define LEDS_EVENT_INTERVAL EVENT_INTERVAL_US(1000)
#define MMA_EVENT_INTERVAL  EVENT_INTERVAL_US(10000)

#define EVENT_COUNT (sizeof(event_info)/sizeof(event_info[0]))

void main(void) {
    
    initClock();
    initIO();
    
    // const mmaCTRL_REG1bits_t ctrl_reg1_val = {
    //     .ACTIVE     = 1,        // turn on the device
    //     .F_READ     = 1,        // make sure fast read is on (only MSB bits of G data are read on consecutive readings)
    //     /* X */
    //     .DR         = 0b100,    // Data sampling rate of 50.0Hz during wake
    //     .ASLP_RATE  = 0b11      // Data sampling rate of 1.56Hz during sleep
    // }; -> 11 100 x 1 1
    // const mmaCTRL_REG2bits_t ctrl_reg2_val = {
    //     .MODS       = 0b11,     // wake low power: OS ratio 2
    //     .SLPE       = 0,        // disable auto sleep          
    //     .SMODS      = 0b11,     // sleep low power: OS ratio 16
    //     /* X */
    //     .RST        = 0,        // no reset
    //     .ST         = 0,        // no self test
    // }; -> 0 0 X 11 0 11
    // const mmaCTRL_REG3bits_t ctrl_reg3_val = {
    //     .PP_OD      = 0,        // Int pin is Push-Pull
    //     .IPOL       = 1,        // make interrupts active high
    //     /* X */
    //     .WAKE_FF_MT = 0,        // disable wake from Free fall / motion interrupt
    //     .WAKE_PULSE = 0,        // disable wake from pulse interrupt
    //     .WAKE_LNDPRT = 0,       // disable orientation interrupt
    //     .WAKE_TRANS = 0,        // disable transient interrupt
    //     .FIFO_GATE  = 0,        // Fifo is flushed upon the system mode transitioning
    // }; -> 0 0 0 0 0 x 1 0
    const unsigned char ctrl_cfg[3] = {0b11100010, 0b00011011, 0b00000010};
    comm_MMA_write(MMA_CTRL_REG1, ctrl_cfg, 3);

    CURR_ACTIVE_LED_SIDE = 0; // we use TMR1L here to keep track of which SIDE is currently being displayed on
    DICE_LED_EXEC_SIDE_I = 0;
    
    leds_display_dbg(02345);
    
    for (;;) {
        eventExecute(LEDS_EVENT_INTERVAL, event_timer[leds_event], ledsExecute);
        eventExecute(MMA_EVENT_INTERVAL,  event_timer[mma_event],  mmaExecute);
    }
    
    return;
}

inline void initIO(void) {
    ANSELA      = 0; // set all IO to digital
    
    // load default pin states
    LATA        = 0
                    | 1 << _SDA
                    | 1 << _SCL;
    
    TRISA       = 1 << _MMA_INT; // init all bits as output except RA3
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
