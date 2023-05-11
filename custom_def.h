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

#include <stdbool.h>
    
#define xstr(s) str(s)
#define str(s) #s

#define CPU_SPEED 4000000
    
#define __rBIT(Type, Port, Nr)  Type##Port##bits.Type##Port##Nr
#define __rBITPORT(Type, Port, Nr)  Type##Port##bits.R##Port##Nr
#define rTRIS(Def)              __rBIT(TRIS, Def)
#define rPORT(Def)              __rBITPORT(PORT, Def)
#define rLAT(Def)               __rBIT(LAT, Def)
#define rANSEL(Def)             __rBIT(ANSEL, Def)

#define _SCL                2
#define _SDA                4
    
#define pSCL                 A, _SCL
#define pSDA                 A, _SDA
    
#define pSIDE_EN             A, 0
#define pSIDE_STR            A, 1
#define pPATT_STR            A, 5

#define pMMA_INT             A, 3

#define DICE_LED_EXEC_SIDE_I TMR1H
#define CURR_ACTIVE_LED_SIDE TMR1L
#define LED_DISPLAY_PATT     TMR0
#define LED_DISPLAY_SIDE     DICE_LED_EXEC_SIDE_I

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

#define _eSUCCESS 0
#define _eFAILURE 1

typedef enum {
    eSUCCESS = _eSUCCESS,
    eFAILURE = _eFAILURE,
} glbl_err_t;

#if CPU_SPEED == 16000000
#define OSCCON_IRCF 0xF
#elif CPU_SPEED == 8000000
#define OSCCON_IRCF 0xE
#elif CPU_SPEED == 4000000
#define OSCCON_IRCF 0xD
#elif CPU_SPEED == 2000000
#define OSCCON_IRCF 0xC
#elif CPU_SPEED == 1000000
#define OSCCON_IRCF 0xB
#elif CPU_SPEED ==  500000
#define OSCCON_IRCF 0xA
#elif CPU_SPEED ==  250000
#define OSCCON_IRCF 0x9
#elif CPU_SPEED ==  125000
#define OSCCON_IRCF 0x5
#elif CPU_SPEED ==   62500
#define OSCCON_IRCF 0x4
#elif CPU_SPEED ==   31250
#define OSCCON_IRCF 0x2
#elif CPU_SPEED ==   31000
#define OSCCON_IRCF 0x0
#else
#error INVALID CPU_SPEED SPECIFIED 
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* CUSTOM_DEF_H */

