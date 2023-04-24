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

#define I2C_READ_BIT  1
#define I2C_WRITE_BIT 0

//unsigned char I2C_write(unsigned char* write, unsigned char write_len, unsigned char repeated_start);
//void I2C_read(unsigned char* read, unsigned char read_len);
//void I2C_write_byte(unsigned char b);
//unsigned char I2C_read_byte(void);

extern void I2C_write(unsigned char byte);
extern unsigned char I2C_read(void);
    
#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

