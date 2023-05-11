#include <xc.h>
#include "dice_pattern.h"
#include "custom_def.h"
#include "i2c.h"

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

#ifdef _DEBUG
ePATT_t _display[N_PATT_SIDES] = {0};
#endif

void ledsExecute(void) {
#ifdef _DEBUG
    if (DICE_LED_EXEC_SIDE_I >= N_PATT_SIDES)
        DICE_LED_EXEC_SIDE_I = 0;
#endif
    
    rLAT(pSIDE_EN)  = 0;
    // set pattern
    rLAT(pPATT_STR) = 1;
#ifdef _DEBUG
    i2c_write(pattern_LED[_display[DICE_LED_EXEC_SIDE_I]]);
#else
    i2c_write(pattern_LED[LED_DISPLAY_PATT]);
#endif
    rLAT(pPATT_STR) = 0;
    // set side
    rLAT(pSIDE_STR) = 1;
#ifdef _DEBUG
    i2c_write((unsigned char)PATT_SIDE(DICE_LED_EXEC_SIDE_I));
#else
    i2c_write((unsigned char)PATT_SIDE(LED_DISPLAY_SIDE));
#endif
    rLAT(pSIDE_STR) = 0;
    
    rLAT(pSIDE_EN)  = 1;
    
#ifdef _DEBUG
    DICE_LED_EXEC_SIDE_I++;
#endif
}

#ifdef _DEBUG
void leds_display_dbg(unsigned short val) {
    // can display values < 4095
    _display[eSIDE_A] = val & 7;
    val >>= 3;
    _display[eSIDE_C] = val & 7;
    val >>= 3;
    _display[eSIDE_D] = val & 7;
    val >>= 3;
    _display[eSIDE_F] = val & 7;
}

void leds_display_dbg_signed(signed char val) {
    _display[eSIDE_F] = 0;
    if (val < 0) {
        _display[eSIDE_F] = 1;
        val = ~val + 1;
    }
    _display[eSIDE_A] = val & 7;
    val >>= 3;
    _display[eSIDE_C] = val & 7;
    val >>= 3;
    _display[eSIDE_D] = val & 7;
    val >>= 3;
}
#endif
