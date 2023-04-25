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

/**
 * @brief Functions should always return the SCL state back to low after function execution, except for the I2C.stop() function
 */

static volatile unsigned char _i2c_reg = 0; //__at(I2C_REG_ADDR);

/**
 * @brief Start's I2C communication
 * Start I2C state:
 * - SCL: don't care
 * - SDA: don't care
 * End I2C state:
 * - SCL: low
 * - SDA: low
 */
void i2c_start(void) {
	// START entry conditions:
    // After ACK assume ( SDA = x, SCL = 0 )
    // After Stop/Init  ( SDA = 1, SCL = 1 )
	asm("BANKSEL (LATA)");							  // select LATA bank
    asm("BSF " xstr(BANKMASK(LATA)) ", " xstr(_SCL)); // if ACK was previous action, first set SDA line high while SCL is still low
    asm("BSF " xstr(BANKMASK(LATA)) ", " xstr(_SDA)); // if ack was previous action, set SCL high
    asm("NOP");
	asm("NOP");
	asm("BCF " xstr(BANKMASK(LATA)) ", " xstr(_SDA)); // actual H->L START condition
    asm("BCF " xstr(BANKMASK(LATA)) ", " xstr(_SCL)); // end clock cycle
	return;
}

/**
 * @brief Stop's I2C communication
 * Start I2C state:
 * - SCL: low
 * - SDA: don't care
 * 
 * End I2C state:
 * - SCL: high
 * - SDA: high
 */
void i2c_stop(void) {
	asm("BANKSEL (LATA)");							  // select LATA bank
    asm("BCF " xstr(BANKMASK(LATA)) ", " xstr(_SDA)); // make sure SDA is low
    asm("BSF " xstr(BANKMASK(LATA)) ", " xstr(_SCL)); // make clock high for STOP condition
	asm("NOP");
	asm("NOP");
    asm("BSF " xstr(BANKMASK(LATA)) ", " xstr(_SDA)); // actual L->H STOP condition
}

void i2c_write(unsigned char b) {
	// extremely dirty way to optimise not having to set _i2c_reg
	// as b is set in W register on function call
	asm("BANKSEL (LATA)");
	asm("MOVWF   __i2c_reg	 ");							// load W into I2C_REG
	asm("LSLF    __i2c_reg, f");							// make room in i2c_reg for end delimiter
	asm("BSF	 __i2c_reg, 0");							// set first bit, so that a byte of 0x00 can also be send
	asm("__i2c_write_loop:");
	asm("BCF	  " xstr(BANKMASK(LATA)) ", " xstr(_SCL));	// end I2C clock cycle
	asm("BSF	  " xstr(BANKMASK(LATA)) ", " xstr(_SDA));	// if carry == 1, set
	asm("BTFSS   STATUS, " xstr(_STATUS_C_POSN));			// check bit
	asm("	BCF   " xstr(BANKMASK(LATA)) ", " xstr(_SDA));	// if carry == 0, clear
	asm("BSF	  " xstr(BANKMASK(LATA)) ", " xstr(_SCL));	// make slave read bit
	asm("LSLF    __i2c_reg, f");							// move bit to carry
	asm("BTFSS   STATUS, " xstr(_STATUS_Z_POSN));			// if last bit is sent, Z==1
	asm("  GOTO __i2c_write_loop");
	asm("BCF	  " xstr(BANKMASK(LATA)) ", " xstr(_SCL));	// end I2C clock cycle
}

unsigned char i2c_read(void) {
	asm("MOVLW	 0x01");									// load end bit indicator
	asm("MOVWF   __i2c_reg");								// put base value in I2C_REG
	asm("BANKSEL (TRISA)");
	asm("BSF	  " xstr(BANKMASK(TRISA)) ", " xstr(_SDA));
	asm("BANKSEL (LATA)");
	asm("__i2c_read_loop:");
	asm("LSLF    __i2c_reg, f");							// make place for next bit to read
	asm("BSF	  " xstr(BANKMASK(LATA)) ", " xstr(_SCL));	// make slave load bit on data
	asm("BANKSEL (PORTA)");
	asm("BTFSC    PORTA, " xstr(_SDA));						// read bus
		asm("BSF	 __i2c_reg, 0");						// if bus is set, set data
	asm("BANKSEL (LATA)");
	asm("BCF	  " xstr(BANKMASK(LATA)) ", " xstr(_SCL));	// end I2C clock cycle
	asm("BTFSS   STATUS, " xstr(_STATUS_C_POSN));			// if carry bit is set, last bit has been read
		asm("GOTO __i2c_read_loop");
	asm("BANKSEL (TRISA)");
	asm("BCF	  " xstr(BANKMASK(TRISA)) ", " xstr(_SDA));
	return _i2c_reg;
}

/**
 * @brief Read 1 bit over I2C bus
 * @return the bit read on the I2C bus
 * 
 * Start I2C state:
 * - SDA: don't care
 * - SCL: low
 * 
 * End I2C state:
 * - SDA: bit
 * - SCL: low
 */
unsigned char i2c_read_bit(void) {
	asm("BANKSEL (TRISA)");
	asm("BSF	  " xstr(BANKMASK(TRISA)) ", " xstr(_SDA));	// release data line
	asm("BSF	  " xstr(BANKMASK(TRISA)) ", " xstr(_SCL));	// make slave load data on bus (allow clock stretching), pulled high
	asm("BANKSEL (PORTA)");
	asm("NOP");
	asm("MOVF	 PORTA, w");	// read data
	asm("ANDLW	 " xstr(1 << _SDA)); // mask SDA bit
	asm("MOVLW	 0x0");
	asm("BTFSC   STATUS, " xstr(_STATUS_Z_POSN));
	asm("ADDLW   0x1");
	asm("BANKSEL (TRISA)");
	asm("BCF	  " xstr(BANKMASK(TRISA)) ", " xstr(_SCL));	// reclaim clock line
	asm("BCF	  " xstr(BANKMASK(TRISA)) ", " xstr(_SDA));	// reclaim data line	
	asm("MOVWF	 __i2c_reg");
    return _i2c_reg;
}

/**
 * @brief Write 1 bit over I2C bus
 * @param bit the value to write on the I2C bus
 * 
 * Start I2C state:
 * - SDA: don't care
 * - SCL: low
 * 
 * End I2C state:
 * - SDA: bit
 * - SCL: low
 */
void i2c_write_bit(unsigned char b) {
	// extremely dirty way to optimise not having to set _i2c_reg
	// as b is set in W register on function call
	asm("BANKSEL (LATA)");
	asm("BCF " xstr(BANKMASK(LATA)) ", " xstr(_SDA));	// clear SDA
	asm("ADDLW 0");
	asm("BTFSS STATUS, " xstr(_STATUS_Z_POSN));			// check if SDA should be set
	asm("BSF " xstr(BANKMASK(LATA)) ", " xstr(_SDA));	// if so set SDA
	asm("BSF " xstr(BANKMASK(LATA)) ", " xstr(_SCL));	// pull clock high
	asm("NOP");											// 4MHz/4 / 4 = 250Khz
	asm("NOP");
	asm("BCF " xstr(BANKMASK(LATA)) ", " xstr(_SCL));	// end clock cycle
}
