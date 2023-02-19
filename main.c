/*
 * File:   main.c
 * Author: mcimr
 *
 * Created on February 19, 2023, 12:29 PM
 */


#include <xc.h>
#include "config_bits.h"
#include "version_git.h"

void main(void) {
    
    while(1) {
        
    }
    
    return;
}


const char _GIT_VERSION[] __at(1020) = 
{
	((GIT_VERSION >> 24) & 0xFF),
	((GIT_VERSION >> 16) & 0xFF),
	((GIT_VERSION >>  8) & 0xFF),
	((GIT_VERSION      ) & 0xFF),
};
