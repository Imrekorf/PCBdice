#include <xc.h>
#include "dice_pattern.h"
#include "custom_def.h"
#include "i2c.h"

static inline void _set_led_pattern(ePATT_t pattern, eSIDE_t side) ;

const unsigned char pattern_LED[N_PATT_LEDS] = {
    /* 0b 0000000 */ [eLED_NONE] = eLED_NONE,
    /* 0b 0001000 */ [eLED_PATT_1] = (PATT_LED(eLED_D)), 
    /* 0b 1000001 */ [eLED_PATT_2] = (PATT_LED(eLED_G) | PATT_LED(eLED_A)),
    /* 0b 1001001 */ [eLED_PATT_3] = (PATT_LED(eLED_G) | PATT_LED(eLED_D) | PATT_LED(eLED_A)),
    /* 0b 1010101 */ [eLED_PATT_4] = (PATT_LED(eLED_G) | PATT_LED(eLED_E) | PATT_LED(eLED_C) | PATT_LED(eLED_A)),
    /* 0b 1011101 */ [eLED_PATT_5] = (PATT_LED(eLED_G) | PATT_LED(eLED_E) | PATT_LED(eLED_D) | PATT_LED(eLED_C) | PATT_LED(eLED_A)),
    /* 0b 1110111 */ [eLED_PATT_6] = (PATT_LED(eLED_G) | PATT_LED(eLED_F) | PATT_LED(eLED_E) | PATT_LED(eLED_C) | PATT_LED(eLED_B) | PATT_LED(eLED_A)),
    /* 0b 1111111 */ [eLED_PATT_7] = (PATT_LED(eLED_G) | PATT_LED(eLED_F) | PATT_LED(eLED_E) | PATT_LED(eLED_D) | PATT_LED(eLED_C) | PATT_LED(eLED_B) | PATT_LED(eLED_A)),
};

static inline void _set_led_pattern(ePATT_t pattern, eSIDE_t side) {
    rLAT(SIDE_EN)  = 0;
    // set pattern
    rLAT(PATT_STR) = 1;
    I2C_write_byte(pattern_LED[pattern]);
    unsigned char b = pattern_LED[pattern];
    for (signed char i = 7; i >= 0; i--) {
        rLAT(SDA) = (b >> i) & 1; 		// change data
        rLAT(SCL) = eHIGH; 		    // pull clock high
        rLAT(SCL) = eLOW; 			// make sure SCL is low
    }
    rLAT(PATT_STR) = 0;
    // set side
    rLAT(SIDE_STR) = 1;
    for (signed char i = 7; i >= 0; i--) {
        rLAT(SDA) = (side >> i);    // change data
        rLAT(SCL) = eHIGH; 		    // pull clock high
        rLAT(SCL) = eLOW; 			// make sure SCL is low
    }
    rLAT(SIDE_STR) = 0;
    
    rLAT(SIDE_EN)  = 1;
}

static ePATT_t _display[N_PATT_SIDES] = {0};

void ledsExecute(void) {
    static unsigned char i = 0;
    if (i >= N_PATT_SIDES)
        i = 0;
    _set_led_pattern(_display[i], PATT_SIDE(i));
    i++;
}

void leds_display(unsigned short val) {
    // can display values < 4095
    _display[eSIDE_A] = val & 7;
    val >>= 3;
    _display[eSIDE_C] = val & 7;
    val >>= 3;
    _display[eSIDE_D] = val & 7;
    val >>= 3;
    _display[eSIDE_F] = val & 7;
}
