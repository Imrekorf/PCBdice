/* 
 * File:   events.h
 * Author: mcimr
 *
 * Created on April 15, 2023, 5:28 PM
 */

#ifndef EVENTS_H
#define	EVENTS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#include "custom_def.h"

#define TIMER_SPEED CPU_SPEED

#define PRESCALER 8

#define MS_PER_TICK  ((4*PRESCALER) / (TIMER_SPEED / 1000000))
#define TICKS_PER_MS ((TIMER_SPEED / 1000000) / (4*PRESCALER))

#if MS_PER_TICK
#   define EVENT_SLOT_MS(x) ((x * 1000UL) / MS_PER_TICK)
#   define EVENT_SLOT_US(x) (x / MS_PER_TICK)
#else
#   define EVENT_SLOT_MS(x) ((x * 1000) * TICKS_PER_MS)
#   define EVENT_SLOT_US(x) (x * TICKS_PER_MS)
#endif
    
struct stEVENT_INFO {
    unsigned short Slot;
    void (*Handler)(void);
};

unsigned char eventExecute (unsigned short* event, unsigned short eventtime);

#ifdef	__cplusplus
}
#endif

#endif	/* EVENTS_H */

