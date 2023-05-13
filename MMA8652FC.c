#include <xc.h>
#include "dice_pattern.h"
#include "MMA8652FC.h"
#include "i2c.h"
#include "custom_def.h"

glbl_err_t comm_MMA_start(mma_reg_t addr) {
    i2c_start();
    i2c_write(MMA_DEVICE_ADDR | MODE_I2C_WRITE);
    if(i2c_read_bit() == I2C_NACK) {
        comm_MMA_stop();
        return eFAILURE;
    }
    i2c_write(addr);
    if(i2c_read_bit() == I2C_NACK) {
        comm_MMA_stop();
        return eFAILURE;
    }
    return eSUCCESS;
}

// IMPORTANT buff_len should not be 0!
glbl_err_t comm_MMA_read(const mma_reg_t addr, unsigned char* buff, unsigned char buff_len) {   
    if (eSUCCESS != comm_MMA_start(addr))
        return eFAILURE;
    
    i2c_start();
    i2c_write(MMA_DEVICE_ADDR | MODE_I2C_READ);
    if(i2c_read_bit() == I2C_NACK)
        goto COMM_STOP;
        
    do { 
        *buff = i2c_read();
        buff=buff+1; 
        buff_len = buff_len-1;
        i2c_write_bit(!buff_len); 
    } while(buff_len);
    
COMM_STOP:
    comm_MMA_stop();

    return !!buff_len;
}

// IMPORTANT buff_len should not be 0!
glbl_err_t comm_MMA_write(const mma_reg_t addr, const unsigned char* buff, unsigned char buff_len) {
    if (eSUCCESS != comm_MMA_start(addr))
        return eFAILURE;

    do {
        i2c_write(*buff); 
        buff=buff+1;
        buff_len = buff_len - 1;
    } while(!i2c_read_bit() && buff_len);
    
    comm_MMA_stop();
    
    return !!buff_len;
}

void mmaExecute(void) {
    
//    unsigned char V = 0;
//    comm_MMA_read(MMA_CTRL_REG1, (unsigned char*)&V, 1);
    leds_display_dbg(255);
    /*
    union {
        signed char buff[3];
        struct {
            signed char x;
            signed char y;
            signed char z;
        } g;
    } data;

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
    
    if (rPORT(pMMA_INT)) {
        leds_display(CURR_ACTIVE_LED_SIDE, 7);
    } else {
        leds_display(CURR_ACTIVE_LED_SIDE, 2);
    }
    */
}

