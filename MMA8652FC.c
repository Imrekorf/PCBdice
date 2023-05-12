#include <xc.h>
#include "dice_pattern.h"
#include "MMA8652FC.h"
#include "i2c.h"
#include "custom_def.h"

glbl_err_t comm_MMA_start(volatile mma_reg_t addr) {
    static mma_reg_t _addr;
    asm("MOVWF  comm_MMA_start@_addr");                     // save addr for later
    asm("CALL   _i2c_start");                               // start i2c
    asm("MOVLW " xstr(MMA_DEVICE_ADDR | MODE_I2C_WRITE));   // load i2c slave addr
    asm("CALL   _i2c_write");                               // write i2c slave addr
    asm("CALL   _i2c_read_bit");                            // check ack_bit
    asm("XORLW  " xstr(I2C_NACK));                          // (ack_bit ^ I2C_NACK) == 0 -> stop
    asm("BTFSC  STATUS, " xstr(_STATUS_Z_POSN));            // status.z == 1 <= (ack_bit ^ I2C_NACK) == 0
    asm("GOTO   _comm_MMA_start_fail");                     // status.z == 1 -> failure
    asm("MOVF   comm_MMA_start@_addr, w");                  // load addr
    asm("CALL   _i2c_write");                               // write addr
    asm("CALL   _i2c_read_bit");                            // check ack_bit
    asm("XORLW  " xstr(I2C_NACK));                          // (ack_bit ^ I2C_NACK) == 0 -> stop
    asm("BTFSC  STATUS, " xstr(_STATUS_Z_POSN));            // status.z == 1 <= (ack_bit ^ I2C_NACK) == 0
    asm("GOTO   _comm_MMA_start_fail");                     // status.z == 1 -> failure
    asm("MOVLW  " xstr(_eSUCCESS));                         // status.z == 0 -> success
    asm("RETURN");                                          // early return on success
    asm("_comm_MMA_start_fail:");
    asm("CALL   _i2c_stop");                                // on failure stop i2c comm
    asm("MOVLW  " xstr(_eFAILURE));                         // load fail
    return WREG;
}

// IMPORTANT buff_len should not be 0!
glbl_err_t comm_MMA_read(const volatile mma_reg_t addr, volatile unsigned char* buff, volatile unsigned char buff_len) {        
    asm("CALL   _comm_MMA_start");
    asm("XORLW  " xstr(_eFAILURE));                     // check if start succeeded
    asm("BTFSC  STATUS, " xstr(_STATUS_Z_POSN));        // (ret ^ eFAILURE) == 0 -> stop
    asm("RETURN");                                      // status.z == 1 -> failure
    
    asm("CALL   _i2c_start");                           // i2c restart signal
    asm("MOVLW " xstr(MMA_DEVICE_ADDR | MODE_I2C_READ));// load device addr
    asm("CALL   _i2c_write");                           // resend device addr
    asm("CALL   _i2c_read_bit");                        // check response
    asm("XORLW " xstr(I2C_NACK));                       // (ack_bit ^ I2C_NACK) == 0 -> stop
    asm("BTFSC  STATUS, " xstr(_STATUS_Z_POSN));        // status.z == 1 <= (ack_bit ^ I2C_NACK) == 0
    asm("GOTO   _comm_mma_read_stop");                  // status.z == 1 -> failure
    
//    do { *buff++ = i2c_read(); i2c_write_bit(--buff_len); } while(buff_len);
    asm("_comm_mma_read_loop:");                        // normal loop: read byte 0..N-1 and send ACK
    asm("MOVF   comm_MMA_read@buff, w");                // load buff ptr
    asm("MOVWF  FSR1L");                                //
    asm("CLRF   FSR1H");                                //
    asm("CALL   _i2c_read");                            // read data
    asm("MOVWF  INDF1");                                // move byte to buff ptr
    asm("INCF   comm_MMA_read@buff, f");                // ++buff
    asm("DECF   comm_MMA_read@buff_len, f");            // --buff_len
    asm("BTFSC  STATUS, " xstr(_STATUS_Z_POSN));        // 
    asm("GOTO   _comm_mma_read_clean_stop");            // buff_len == 0 => last byte to send
    asm("MOVLW  " xstr(I2C_ACK));                       // 
    asm("CALL   _i2c_write_bit");                       // ack read
    asm("GOTO   _comm_mma_read_loop");                  //
    
    asm("_comm_mma_read_clean_stop:");                  // clean up routine: read byte N and send NACK
    asm("MOVLW " xstr(I2C_NACK));                       //
    asm("CALL   _i2c_write_bit");                       //
    asm("_comm_mma_read_stop:");                        //
    asm("CALL   _i2c_stop");                            //
    return !!buff_len;
}

// IMPORTANT buff_len should not be 0!
glbl_err_t comm_MMA_write(const volatile mma_reg_t addr, volatile unsigned char* buff, volatile unsigned char buff_len) {
    asm("CALL   _comm_MMA_start");
    asm("XORLW  " xstr(eFAILURE));
    asm("BTFSC  STATUS, " xstr(_STATUS_Z_POSN));        // (ret ^ eFAILURE) == 0 -> stop
    asm("RETURN");                                      // status.z == 1 -> failure
    
    // do {i2c_write(*buff++); if (i2c_read_bit() == I2C_NACK) break;} while(--buff_len);
    asm("_comm_MMA_write_loop:");
    asm("MOVF   comm_MMA_write@buff, w");               // deref buff ptr
    asm("MOVWF  FSR1L");
    asm("CLRF   FSR1H");
    asm("MOVF   INDF1, w");                             // load *buff into W
    asm("CALL   _i2c_write");                           // write *buff
    asm("CALL   _i2c_read_bit");                        // read ack bit
    asm("XORLW " xstr(I2C_NACK));                       // (ack_bit ^ I2C_NACK) == 0 -> stop
    asm("BTFSC  STATUS, " xstr(_STATUS_Z_POSN));        // (status.z == 1 <= (ack_bit ^ I2C_NACK) == 0
    asm("GOTO   _comm_MMA_write_stop");                 // break
    asm("INCF   comm_MMA_write@buff");                  // ++buff
    asm("DECF   comm_MMA_write@buff_len");              // --buff_len;
    asm("BTFSS  STATUS, " xstr(_STATUS_Z_POSN));        // buff_len == 0 => stop sending
    asm("GOTO   _comm_MMA_write_loop");                 // buff_len > 0
    asm("_comm_MMA_write_stop:");                       
    asm("CALL _i2c_stop");

    return !!buff_len;
}

void mmaExecute(void) {
    union {
        signed char buff[3];
        struct {
            signed char x;
            signed char y;
            signed char z;
        } g;
    } data;
    comm_MMA_read(MMA_OUT_X_MSB, (unsigned char*)&data.buff, 3);
    
    leds_display(CURR_ACTIVE_LED_SIDE, 0);
    
    if ( data.g.x <= -MMA_TRIP_mG_BIN )
        CURR_ACTIVE_LED_SIDE = eSIDE_E;
    if ( data.g.x >=  MMA_TRIP_mG_BIN )
        CURR_ACTIVE_LED_SIDE = eSIDE_A;
    if ( data.g.y <= -MMA_TRIP_mG_BIN )
        CURR_ACTIVE_LED_SIDE = eSIDE_B;
    if ( data.g.y >=  MMA_TRIP_mG_BIN )
        CURR_ACTIVE_LED_SIDE = eSIDE_F;
    if ( data.g.z <= -MMA_TRIP_mG_BIN )
        CURR_ACTIVE_LED_SIDE = eSIDE_D;
    if ( data.g.z >=  MMA_TRIP_mG_BIN )
        CURR_ACTIVE_LED_SIDE = eSIDE_C;
    
    if (rPORT(pMMA_INT))
        leds_display(CURR_ACTIVE_LED_SIDE, 7);        
}

