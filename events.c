#include <xc.h>
#include "events.h"

volatile unsigned short __T = 0;
void __interrupt() isr() {
    if (PIR1bits.TMR2IF) {
        __T++;
        PIR1bits.TMR2IF = 0;
    }
}
