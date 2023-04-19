/**
 * @file i2c.c
 * @author Imre Korf (I.korf@outlook.com)
 * @brief 
 * @version 0.1
 * @date 2023-02-14
 * 
 * @copyright Copyright (c) 2023
 * 
 * I2C protocol specifications summary:
 * 
 * -> SDA can only change when SCL is low
 * -> when SCL is high SDA should not change[1]
 * 
 * -> each I2C command initiated by master starts with a START condition
 * -> each I2C command initiated by master end with a STOP condition
 * 
 * -> [1] START and STOP conditions have SCL high and SDA transition
 * -> START condition: 	SCL : High | SDA 	High -> Low
 * -> STOP condition: 	SCL : High | SDA 	Low  -> High
 * 
 * -> ACK signal is the 9th bit of transmission from the receiving side of the data.
 * -> ACK signal: 		release SDA line during ACK clock pulse (9th clock pulse). SDA LOW during SCL HIGH period -> ACK, SDA HIGH NACK
 * 
 * -> first byte => Target address(7 bits) followed by 8th R(1)/W(0) bit
 * 
 * SDA --| . |-~-~-~-~-|-------|       |-|-~-~-~-~-|-------|       |-|-~-~-~-~-|-------|         . |--
 * 	   . | . |         X       |       | X         X       X       | X         X       X         . | .
 * 	   . |---|-~-~-~-~-|-------|-------| |-~-~-~-~-|-------|-------| |-~-~-~-~-|-------|-----------| .
 * 	   .   .                                                                                     .   .
 * SCL ----|   |-~-~-|   |---|   |---|     |-~-~-|   |---|   |---|     |-~-~-|   |---|   |---|   |----
 * 	   .   |   | 1-7 |   | 8 |   | 9 |     | 1-7 |   | 8 |   | 9 |     | 1-7 |   | 8 |   | 9 |   |   .
 * 	   .   |---|     |---|   |---|   |-----|     |---|   |---|   |-----|     |---|   |---|   |---|   .
 *     .   .   .     .   .  _.   .   .     .             .   .   .     .             .   .   .   .   .
 *     . S .   .ADDR .   .R/W.   .ACK.     \     DATA    /   .ACK.     \     DATA    /   .ACK.   . P .
 *     Start                                                                                     Stop 
 * 
 * -> note: LSbit is send first
 */

#include <xc.h>
#include "custom_def.h"
#include "i2c.h"

static void _I2C_start(void);
static void _I2C_stop(void);
static void _I2C_write_bit(unsigned char bit);
static unsigned char _I2C_read_bit(void);

// TODO: double sample input ?

static void _I2C_start(void) {
	// START entry conditions:
	// After ACK assume ( SDA = 0, SCL = 0 )
	// After Stop/Init  ( SDA = 1, SCL = 1 )
	rLAT(SDA) = eHIGH; 		// make sure SDA is high
	rLAT(SCL) = eHIGH; 		// make sure SCL is high
	rLAT(SDA) = eLOW; 	    // actual H->L START condition
	rLAT(SCL) = eLOW; 		// end clock cycle
}

static void _I2C_stop(void) {
	rLAT(SCL) = eLOW; 		// make sure SCL is low
	rLAT(SDA) = eLOW; 		// make sure SDA is low
	rLAT(SCL) = eHIGH; 		// make clock high for STOP condition
	rLAT(SDA) = eHIGH; 		// actual L->H STOP condition
	// don't end clock cycle
}

static void _I2C_write_bit(unsigned char bit) {
	rLAT(SCL) = eLOW; 			// make sure SCL is low
	rLAT(SDA) = bit; 		    // change data
	rLAT(SCL) = eHIGH; 		    // pull clock high
}

static unsigned char _I2C_read_bit(void) {
	// make sure SDA is not analog on init
	rTRIS(SDA) = eINPUT; 		// release data line
	rLAT(SCL)  = eHIGH;			// make slave load data on bus
	unsigned char val = rPORT(SDA); 	// read data
	rLAT(SCL)  = eLOW; 			// end clock cycle
	rTRIS(SDA) = eOUTPUT;		// reclaim data line
    return val;
}

unsigned char I2C_write_byte(unsigned char b) {
	for (unsigned char i = 0; i < 8; i++)
		_I2C_write_bit((b >> i) & 1);
	return !_I2C_read_bit(); // read ack
}

unsigned char I2C_read_byte(void) {
	unsigned char read_byte = 0;
	// make sure SDA is not analog on init
	rTRIS(SDA) = eINPUT; 		// release data line
	for (int i = 0; i < 8; i++) {
		rLAT(SCL) = eHIGH;			// make slave load data on bus
		read_byte |= rPORT(SDA) << i; 	// read data
		rLAT(SCL) = eLOW; 			// end clock cycle
	}
	rTRIS(SDA) = eOUTPUT;		// claim data line
	_I2C_write_bit(0); 		// Send ACK
    return read_byte;
}

unsigned char I2C_transfer(unsigned char* write, unsigned char* write_len, unsigned char* read, unsigned char* read_len) {
	_I2C_start();

    unsigned char i = 0;
	for(; i < *write_len; i++) {
		if (!I2C_write_byte(write[i])) {
			// NACK received
			*write_len = i;
            return 0;
		}
	}
    *write_len = i;

	for(i = 0; i < *read_len; i++) {
		read[i] = I2C_read_byte();
		_I2C_start(); // repeated start
	}
    *read_len = i;

	_I2C_stop();
    return 1;
}
