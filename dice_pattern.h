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
    eSIDE_A = 5, // works!
    eSIDE_B = 4, // works!
    eSIDE_C = 3, // works!
    eSIDE_D = 2, // works!
    eSIDE_E = 1, // untested
    eSIDE_F = 0, // works!
    N_PATT_SIDES = 6,
} eSIDE_t;

#define PATT_MASK        ((1 << N_PATT_SIDES) - 1)
#define PATT_SIDE(x)     (1 << (x))

/**
 * E2      A6
 * 
 * F1  D3  B5
 * 
 * G0      C4
 */
    
typedef enum {
    __eLED_NONE = 0,
    eLED_A = 6,
    eLED_B = 5,
    eLED_C = 4,
    eLED_D = 3,
    eLED_E = 2,
    eLED_F = 1,
    eLED_G = 0,
    N_LEDS = 7,
} eLED_t;

#define PATT_LED_MASK   ((1 << N_LEDS)-1)
#define PATT_LED(x)     (1 << (x))
#define LED_NONE        (__eLED_NONE)

typedef enum {
    eLED_NONE = __eLED_NONE,
    eLED_PATT_1 = 1,
    eLED_PATT_2,
    eLED_PATT_3,
    eLED_PATT_4,
    eLED_PATT_5,
    eLED_PATT_6,
    eLED_PATT_7,
    N_PATT_LEDS,
} ePATT_t;

void ledsExecute(void);
void leds_display(unsigned short val);

#ifdef	__cplusplus
}
#endif

#endif	/* DICE_PATTERN_H */

