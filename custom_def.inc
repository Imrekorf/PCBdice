/* 
 * File:   custom_def.h
 * Author: mcimr
 *
 * Created on February 19, 2023, 4:11 PM
 */

#ifndef CUSTOM_DEF_INC
#define	CUSTOM_DEF_INC
    
#define xstr(s) str(s)
#define str(s) #s

#define CPU_SPEED 4000000
    
#define __rBIT(Type, Port, Nr)  Type##Port##bits.Type##Port##Nr
#define __rBITPORT(Type, Port, Nr)  Type##Port##bits.R##Port##Nr
#define rTRIS(Def)              __rBIT(TRIS, Def)
#define rPORT(Def)              __rBITPORT(PORT, Def)
#define rLAT(Def)               __rBIT(LAT, Def)
#define rANSEL(Def)             __rBIT(ANSEL, Def)

#define _SCL		     2
#define _SDA		     4
    
#define pSCL                 A, _SCL
#define pSDA                 A, _SDA
    
#define pSIDE_EN             A, 0
#define pSIDE_STR            A, 1
#define pPATT_STR            A, 5

#define pMMA_INT             A, 3



#endif	/* CUSTOM_DEF_INC */

