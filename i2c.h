/* 
 * File:   i2c.h
 * Author: mcimr
 *
 * Created on April 15, 2023, 8:45 PM
 */

#ifndef I2C_H
#define	I2C_H

#ifdef	__cplusplus
extern "C" {
#endif

unsigned char I2C_write_byte(unsigned char b);
unsigned char I2C_read_byte(void);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

