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

#define EVENT_TIME_UNIT 100 // us
    
#define EVENT_INTERVAL_MS(x) ((x * 1000) / EVENT_TIME_UNIT)
#define EVENT_INTERVAL_US(x) (x / EVENT_TIME_UNIT)

extern volatile unsigned short __T;
#define eventExecute(event_interval, event_timer, event) {if (event_interval < (__T - event_timer)) {event_timer += __T - event_timer /* event_interval */; event();}}

#ifdef	__cplusplus
}
#endif

#endif	/* EVENTS_H */

