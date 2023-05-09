/* 
 * File:   i2c.h
 * Author: mcimr
 *
 * Created on April 20, 2023, 9:44 PM
 */

#ifndef MMA8652FC_H
#define	MMA8652FC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "custom_def.h"
    
#define MMA_DEVICE_ADDR         0x3A

#define MMA_G_SCALE             (2)
#define MMA_TRIP_mG             (850L)
#define MMA_TRIP_mG_BIN         (MMA_mG_TO_BIN(MMA_TRIP_mG))
#define MMA_mG_TO_BIN(x)        ((x / MMA_G_SCALE * 127) / 1000)
    
typedef enum {
    MMA_STATUS      		= 0x00,
    MMA_F_STATUS    		= 0x00,
    MMA_OUT_X_MSB   		= 0x01,
    MMA_OUT_X_LSB   		= 0x02,
    MMA_OUT_Y_MSB   		= 0x03,
    MMA_OUT_Y_LSB   		= 0x04,
    MMA_OUT_Z_MSB   		= 0x05,
    MMA_OUT_Z_LSB   		= 0x06,

	MMA_F_SETUP     		= 0x09,
	MMA_TRIG_CFG    		= 0x0A,
	MMA_SYSMOD      		= 0x0B,
	MMA_INT_SOURCE			= 0x0C,
	MMA_WHO_AM_I			= 0x0D,
	MMA_XYZ_DATA_CFG		= 0x0E,
	MMA_HP_FILTER_CUTOFF	= 0x0F,
	MMA_PL_STATUS			= 0x10,
	MMA_PL_CFG				= 0x11,
	MMA_PL_COUNT			= 0x12,
	MMA_PL_BF_ZCOMP			= 0x13,
	MMA_P_L_THS_REG			= 0x14,
	MMA_FF_MT_CFG			= 0x15,
	MMA_FF_MT_SRC			= 0x16,
	MMA_FF_MT_THS			= 0x17,
	MMA_FF_MT_COUNT			= 0x18,

	MMA_TRANSIENT_CFG		= 0x1D,
	MMA_TRANSIENT_SRC		= 0x1E,
	MMA_TRANSIENT_THS		= 0x1F,
	MMA_TRANSIENT_COUNT		= 0x20,
	MMA_PULSE_CFG			= 0x21,
	MMA_PULSE_SRC			= 0x22,
	MMA_PULSE_THSX			= 0x23,
	MMA_PULSE_THSY			= 0x24,
	MMA_PULSE_THSZ			= 0x25,
	MMA_PULSE_TMLT			= 0x26,
	MMA_PULSE_LTCY			= 0x27,
	MMA_PULSE_WIND			= 0x28,
	MMA_ASLP_COUNT			= 0x29,
	MMA_CTRL_REG1			= 0x2A,
	MMA_CTRL_REG2			= 0x2B,
	MMA_CTRL_REG3			= 0x2C,
	MMA_CTRL_REG4			= 0x2D,
	MMA_CTRL_REG5			= 0x2E,
	MMA_OFF_X				= 0x2F,
	MMA_OFF_Y				= 0x30,
	MMA_OFF_Z				= 0x31,
} mma_reg_t;

glbl_err_t comm_MMA_start(mma_reg_t addr);
void comm_MMA_stop(void);

#define comm_MMA_write_byte(byte) i2c_write(byte), i2c_read_bit();

glbl_err_t comm_MMA_read(mma_reg_t addr, unsigned char* buff, unsigned char buff_len);
glbl_err_t comm_MMA_write(mma_reg_t addr, unsigned char* buff, unsigned char buff_len);

#ifdef	__cplusplus
}
#endif

#endif	/* MMA8652FC_H */
