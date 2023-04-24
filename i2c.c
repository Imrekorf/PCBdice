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

//static void _I2C_start(void);
//static void _I2C_stop(void);
//static void _I2C_write_bit(unsigned char bit);
//static unsigned char _I2C_read_bit(void);
//
//// TODO: double sample input ?
//	
//#define I2C_ACTION_DELAY()		//__I2C_ACTION_DELAY(1)
///**
// * @brief Functions should always return the SCL state back to low after function execution, except for the I2C.stop() function
// */
//
//void __I2C_ACTION_DELAY(signed char x) {
////    for(char _nop_i = 0; _nop_i < (x); _nop_i++) { __nop(); }
//}
//
///**
// * @brief Start's I2C communication
// * Start I2C state:
// * - SCL: don't care
// * - SDA: don't care
// * End I2C state:
// * - SCL: low
// * - SDA: low
// */
//static void _I2C_start(void) {
//	// START entry conditions:
//	// After ACK assume ( SDA = x, SCL = 0 )
//	// After Stop/Init  ( SDA = 1, SCL = 1 )
//	rLAT(SDA) = eHIGH;		// if ACK was previous action, first set SDA line high while SCL is still low
//	rLAT(SCL) = eHIGH;		// if ack was previous action, set SCL high
//	__I2C_ACTION_DELAY(2);	// allow bus to settle
//	rLAT(SDA) = eLOW; 	    // actual H->L START condition
//	__I2C_ACTION_DELAY(3);	// allow bus to settle
//	rLAT(SCL) = eLOW; 		// end clock cycle
//}
//
///**
// * @brief Stop's I2C communication
// * Start I2C state:
// * - SCL: low
// * - SDA: don't care
// * 
// * End I2C state:
// * - SCL: high
// * - SDA: high
// */
//static void _I2C_stop(void) {
//	rLAT(SDA) = eLOW; 		// make sure SDA is low
//	rLAT(SCL) = eHIGH; 		// make clock high for STOP condition
//	__I2C_ACTION_DELAY(2);	// allow bus to settle
//	rLAT(SDA) = eHIGH; 		// actual L->H STOP condition
//	// don't end clock cycle
//}
//
///**
// * @brief Write 1 bit over I2C bus
// * @param bit the value to write on the I2C bus
// * 
// * Start I2C state:
// * - SDA: don't care
// * - SCL: low
// * 
// * End I2C state:
// * - SDA: bit
// * - SCL: low
// */
//static void _I2C_write_bit(unsigned char bit) {
//	rLAT(SDA) = bit; 		    // change data
//	rLAT(SCL) = eHIGH; 		    // pull clock high
//	I2C_ACTION_DELAY();			// allow slave to read bit
//	rLAT(SCL) = eLOW; 			// make sure SCL is low
//}
//
///**
// * @brief Read 1 bit over I2C bus
// * @return the bit read on the I2C bus
// * 
// * Start I2C state:
// * - SDA: don't care
// * - SCL: low
// * 
// * End I2C state:
// * - SDA: bit
// * - SCL: low
// */
//static unsigned char _I2C_read_bit(void) {
//	rTRIS(SDA) = eINPUT; 		// release data line
//	rTRIS(SCL) = eINPUT;		// make slave load data on bus (allow clock stretching), pulled high
//	I2C_ACTION_DELAY();			// allow device to send bit
//	unsigned char val = rPORT(SDA); 	// read data
//	rTRIS(SCL) = eOUTPUT; 		// end clock cycle, set back to low
//	rTRIS(SDA) = eOUTPUT;		// reclaim data line
//    return val;
//}
//
//void I2C_write_byte(unsigned char b) {
//    I2C_ACTION_DELAY();             // allow bus to settle before sending byte
//	for (signed char i = 7; i >= 0; i--) {
//		_I2C_write_bit((b >> i) & 1);
//        I2C_ACTION_DELAY();			// allow bus to settle
//    }
//}
//
//unsigned char I2C_read_byte(void) {
//	unsigned char read_byte = 0;
//	// make sure SDA is not analog on init
//	for (signed char i = 7; i >= 0; i--)
//		read_byte |= _I2C_read_bit() << i; 	// read data
//    return read_byte;
//}
//
//unsigned char I2C_write(unsigned char* write, unsigned char write_len, unsigned char repeated_start) {
//	_I2C_start();
//    
//	for(unsigned char i = 0; i < write_len; i++) {
//        I2C_write_byte(write[i]);
//		if (_I2C_read_bit()) { // read (n)ack
//			// NACK received
//			_I2C_stop();
//            return 0;
//		}
//	}
//    
//    if (!repeated_start)
//        _I2C_stop();
//
//    return 1;
//}
//
//void I2C_read(unsigned char* read, unsigned char read_len) {
//    for(unsigned char i = 0; i < read_len; i++) {
//		read[i] = I2C_read_byte();
//        _I2C_write_bit(i+1 >= read_len); // Send ACK
//    }
//    
//    _I2C_stop();
//}
