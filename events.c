#include <xc.h>
#include "events.h"

static unsigned short __T = 0;
void __interrupt() isr() {
    if (PIR1bits.TMR2IF) {
        __T++;
        PIR1bits.TMR2IF = 0;
    }
}

void eventExecute (event_info_t* event_info, char event_count)
{
    for (event_info_t* _event = event_info; _event < (event_info + event_count); _event++){
        unsigned short tmr_diff = __T - _event->EventTimer;
        if ((_event->EventInterval) < tmr_diff) {
            _event->EventTimer += tmr_diff; // _event->EventInterval;
            _event->Handler();
        }
    }
}
