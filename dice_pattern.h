/*
 * File:   dice_pattern.h
 * Author: mcimr
 *
 * Created on April 15, 2023, 8:07 PM
 */

#ifndef DICE_PATTERN_H
#define	DICE_PATTERN_H

#ifdef	__cplusplus
extern "C" {
#endif

/**
 *     D2
 * 
 * F0  A5  B4  E1
 * 
 *     C3
 */
    
typedef enum {
    eSIDE_NONE = 0,
    eSIDE_A = 0, // works!
    eSIDE_B = 1, // works!
    eSIDE_C = 2, // works!
    eSIDE_D = 3, // works!
    eSIDE_E = 4, // untested
    eSIDE_F = 5, // works!
    N_PATT_SIDES = 6,
} eSIDE_t;

// add +1 to x for i2c offset (LSB is send first)
#define PATT_SIDE(x)     (1 << (x+2))

/**
 * E2      A6
 * 
 * F1  D3  B5
 * 
 * G0      C4
 */
    
typedef enum {
    __eLED_NONE = 0,
    eLED_A = 0,
    eLED_B = 1,
    eLED_C = 2,
    eLED_D = 3,
    eLED_E = 4,
    eLED_F = 5,
    eLED_G = 6,
} eLED_t;

// add +1 to x for i2c offset (LSB is send first)
#define PATT_LED(x)     (1 << (x+1))
#define LED_NONE        (__eLED_NONE)

typedef enum {
    eLED_NONE = __eLED_NONE,
    eLED_1 = 1,
    eLED_2,
    eLED_3,
    eLED_4,
    eLED_5,
    eLED_6,
    eLED_7,
    N_PATT_LED,
} ePATT_t;

void ledsExecute(void);
void leds_display(unsigned short val);

#ifdef	__cplusplus
}
#endif

#endif	/* DICE_PATTERN_H */

