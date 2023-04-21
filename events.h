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

typedef struct {
    unsigned short EventTimer;
    const unsigned short EventInterval;
    void (* const Handler)(void);
} event_info_t;

extern void eventExecute (event_info_t* event_info, char event_count);

#ifdef	__cplusplus
}
#endif

#endif	/* EVENTS_H */

