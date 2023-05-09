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

void comm_MMA_stop(void) {
    i2c_stop();
}

glbl_err_t comm_MMA_read(mma_reg_t addr, unsigned char* buff, unsigned char buff_len) {
    if (eSUCCESS != comm_MMA_start(addr))
        return eFAILURE;
    
    i2c_start();
    i2c_write(MMA_DEVICE_ADDR | MODE_I2C_READ);
    if(i2c_read_bit() == I2C_NACK)
        goto COMM_STOP;
        
    while(buff_len--) {        
        *buff++ = i2c_read();
        if (buff_len)
            i2c_write_bit(I2C_ACK);
    }
    
COMM_STOP:
    i2c_write_bit(I2C_NACK);
    comm_MMA_stop();

    return !!buff_len;
}

glbl_err_t comm_MMA_write(mma_reg_t addr, unsigned char* buff, unsigned char buff_len) {
    if (eSUCCESS != comm_MMA_start(addr))
        return eFAILURE;
    
    while(buff_len--) {
        i2c_write(*buff++);
        if(i2c_read_bit() == I2C_NACK)
            break;
    }
    
COMM_STOP:
    comm_MMA_stop();

    return !!buff_len;
}
