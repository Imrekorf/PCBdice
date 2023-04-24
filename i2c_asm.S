#include <xc.inc>
#include "custom_def.inc"
    
GLOBAL _I2C_start, _I2C_stop, _I2C_write, _I2C_read

_I2C_REG EQU 100
    
#define C_BIT 0
#define Z_BIT 2
    
/**
 * @brief Start's I2C communication
 * Start I2C state:
 * - SCL: don't care
 * - SDA: don't care
 * End I2C state:
 * - SCL: low
 * - SDA: low
 */
_I2C_start:
    ; START entry conditions:
    ; After ACK assume ( SDA = x, SCL = 0 )
    ; After Stop/Init  ( SDA = 1, SCL = 1 )
    BANKSEL (LATA)		    ; select LATA bank
    BSF	    BANKMASK(LATA), _SCL		    ; if ACK was previous action, first set SDA line high while SCL is still low
    BSF	    BANKMASK(LATA), _SDA		    ; if ack was previous action, set SCL high
    BCF	    BANKMASK(LATA), _SDA		    ; actual H->L START condition
    BCF	    BANKMASK(LATA), _SCL		    ; end clock cycle
    RETURN

_I2C_stop:
    BANKSEL (LATA)		    ; select LATA bank
    BCF	    BANKMASK(LATA), _SDA		    ; make sure SDA is low
    BSF	    BANKMASK(LATA), _SCL		    ; make clock high for STOP condition
    BSF	    BANKMASK(LATA), _SDA		    ; actual L->H STOP condition
    RETURN

_I2C_write:
    BANKSEL (LATA)		    ; select LATA bank
    MOVWF   _I2C_REG		    ; load W into I2C_REG
__I2C_write_loop:
    BCF	    BANKMASK(LATA), _SCL		    ; end I2C clock cycle
    LSLF    _I2C_REG, f		    ; move bit to Carry
    BTFSS   BANKMASK(STATUS), C_BIT		    ; check bit
	BCF BANKMASK(LATA), _SDA		    ; if carry == 0, clear
    BSF	    BANKMASK(LATA), _SDA		    ; if carry == 1, set
    BSF	    BANKMASK(LATA), _SCL		    ; make slave read bit
    BTFSS   BANKMASK(STATUS), Z_BIT		    ; if last bit is sent, Z == 1
	GOTO __I2C_write_loop
    BCF	    BANKMASK(LATA), _SCL		    ; end I2C clock cycle
_I2C_write_end:
    RETURN
    
_I2C_read:
    BANKSEL (LATA)		    ; select LATA bank
    MOVLW   0x02		    ; load end bit indicator
    MOVWF   _I2C_REG		    ; put base value in I2C_REG
__I2C_read_loop:
    BSF	    BANKMASK(LATA), _SCL		    ; make slave load bit on data
    BTFSC   PORTA, _SDA		    ; read bus
	BSF _I2C_REG, 0		    ; if bus is set, set data
    BCF	    BANKMASK(LATA), _SCL		    ; end I2C clock cycle
    LSLF    _I2C_REG, f		    ; make space for next bit
    BTFSS   BANKMASK(STATUS), C_BIT ; if carry bit is set, last bit has been read
	goto    __I2C_read_loop	    ; execute next read
__I2C_read_end:
    MOVF    _I2C_REG, w		    ; load I2C_REG into W
    RETURN
