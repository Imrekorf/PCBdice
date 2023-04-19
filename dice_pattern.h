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
    eSIDE_A = 1 << 2, // works!
    eSIDE_B = 1 << 3, // works!
    eSIDE_C = 1 << 4, // works!
    eSIDE_D = 1 << 5, // works!
    eSIDE_E = 1 << 6, // untested
    eSIDE_F = 1 << 7, // works!
    N_PATT_SIDES = 6,
} eSIDE_t;

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
    N_PATT_LED,
} ePATT_t;

void set_led_pattern(ePATT_t pattern, eSIDE_t side) ;

#ifdef	__cplusplus
}
#endif

#endif	/* DICE_PATTERN_H */

