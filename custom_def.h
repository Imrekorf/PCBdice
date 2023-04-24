/* 
 * File:   custom_def.h
 * Author: mcimr
 *
 * Created on February 19, 2023, 4:11 PM
 */

#ifndef CUSTOM_DEF_H
#define	CUSTOM_DEF_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "custom_def.inc"

enum {
    eHIGH = 1,
    eLOW  = 0,
    
    eON   = 1,
    eOFF  = 0,
    
    eDIGITAL = 0,
    eANALOG  = 1,
    
    eOUTPUT  = 0,
    eINPUT   = 1,
};

#ifdef	__cplusplus
}
#endif

#endif	/* CUSTOM_DEF_H */

