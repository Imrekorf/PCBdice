#include <xc.h>
#include "dice_pattern.h"
#include "custom_def.h"
#include "i2c.h"

const unsigned char pattern_LED[N_PATT_LED] = {
    /* 0b 0000000 */ [eLED_NONE] = eLED_NONE,
    /* 0b 0001000 */ [eLED_1] = (PATT_LED(eLED_D)), 
    /* 0b 1000001 */ [eLED_2] = (PATT_LED(eLED_G) | PATT_LED(eLED_A)),
    /* 0b 1001001 */ [eLED_3] = (PATT_LED(eLED_G) | PATT_LED(eLED_D) | PATT_LED(eLED_A)),
    /* 0b 1010101 */ [eLED_4] = (PATT_LED(eLED_G) | PATT_LED(eLED_E) | PATT_LED(eLED_C) | PATT_LED(eLED_A)),
    /* 0b 1011101 */ [eLED_5] = (PATT_LED(eLED_G) | PATT_LED(eLED_E) | PATT_LED(eLED_D) | PATT_LED(eLED_C) | PATT_LED(eLED_A)),
    /* 0b 1110111 */ [eLED_6] = (PATT_LED(eLED_G) | PATT_LED(eLED_F) | PATT_LED(eLED_E) | PATT_LED(eLED_C) | PATT_LED(eLED_B) | PATT_LED(eLED_A)),
};

void set_led_pattern(ePATT_t pattern, eSIDE_t side) {
    rLAT(SIDE_EN)  = 0;
    // set pattern
    rLAT(PATT_STR) = 1;
    I2C_write_byte(pattern_LED[pattern]);
    rLAT(PATT_STR) = 0;
    // set side
    rLAT(SIDE_STR) = 1;
    I2C_write_byte(side & 0xFC);
    rLAT(SIDE_STR) = 0;
    
    rLAT(SIDE_EN)  = 1;
}
