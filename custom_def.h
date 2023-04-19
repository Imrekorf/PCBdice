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

#define xstr(s) str(s)
#define str(s) #s


#define CPU_SPEED 1000000
    
#define __rBIT(Type, Port, Nr)  Type##Port##bits.Type##Port##Nr
#define __rBITPORT(Type, Port, Nr)  Type##Port##bits.R##Port##Nr
#define rTRIS(Def)              __rBIT(TRIS, Def)
#define rPORT(Def)              __rBITPORT(PORT, Def)
#define rLAT(Def)               __rBIT(LAT, Def)
#define rANSEL(Def)             __rBIT(ANSEL, Def)
    
#define SCL                 A, 2
#define SDA                 A, 4
    
#define SIDE_EN             A, 0
#define SIDE_STR            A, 1
#define PATT_STR            A, 5

#define MMA_INT             A, 3

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

