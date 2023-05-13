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
#include "i2c.h"

#define MMA_DEVICE_ADDR         0x3A
#define MMA_WHO_AM_I_ID         0x4A

#define MMA_G_SCALE             (2)
#define MMA_TRIP_mG             (850L)
#define MMA_TRIP_mG_BIN         (MMA_mG_TO_BIN(MMA_TRIP_mG))
#define MMA_mG_TO_BIN(x)        ((x / MMA_G_SCALE * 127) / 1000)
    
typedef enum {
    MMA_STATUS      		= 0x00,
    MMA_F_STATUS    		= 0x00,
    MMA_OUT_X_MSB   		= 0x01,
    MMA_OUT_X_LSB   		= 0x02,
    MMA_F_OUT_XYZ           = MMA_OUT_X_MSB,
    MMA_OUT_XYZ             = MMA_OUT_X_MSB,
    MMA_OUT_Y_MSB   		= 0x03,
    MMA_F_OUT_YZ            = MMA_OUT_Y_MSB,
    MMA_OUT_YZ              = MMA_OUT_Y_MSB,
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wc11-extensions"

// Register: STATUS/F_STATUS
// bitfield definitions
typedef union {
    struct {
        unsigned XDR           :1;
        unsigned YDR           :1;
        unsigned ZDR           :1;
        unsigned ZYXDR         :1;
        unsigned XOW           :1;
        unsigned YOW           :1;
        unsigned ZOW           :1;
        unsigned ZYXOW         :1;
    };
    struct {
        unsigned F_CNT         :6;
        unsigned F_WMRK_FLAG   :1;
        unsigned F_OCF         :1;
    };
} mmaSTATUSbits_t;
typedef mmaSTATUSbits_t mmaF_STATUSbits_t;

// Register: OUT_X
// bitfield definitions
typedef union {
    struct {
        unsigned x_msb         :8;
        unsigned               :4;
        unsigned x_lsb         :4;
    };
    unsigned short _data;
} mmaOUT_Xbits_t;

// Register: OUT_X_MSB
// bitfield definitions
typedef union {
    struct {
        unsigned x_msb         :8;
    };
    unsigned char _data;
} mmaOUT_X_MSBbits_t;

// Register: OUT_X_LSB
// bitfield definitions
typedef union {
    struct {
        unsigned               :4;
        unsigned x_lsb         :4;
    };
    unsigned char _data;
} mmaOUT_X_LSBbits_t;

// Register: OUT_Y
// bitfield definitions
typedef union {
    struct {
        unsigned y_msb         :8;
        unsigned               :4;
        unsigned y_lsb         :4;
    };
    unsigned short _data;
} mmaOUT_Ybits_t;

// Register: OUT_Y_MSB
// bitfield definitions
typedef union {
    struct {
        unsigned y_msb         :8;
    };
    unsigned char _data;
} mmaOUT_Y_MSBbits_t;

// Register: OUT_Y_LSB
// bitfield definitions
typedef union {
    struct {
        unsigned               :4;
        unsigned y_lsb         :4;
    };
    unsigned char _data;
} mmaOUT_Y_LSBbits_t;

// Register: OUT_Z
// bitfield definitions
typedef union {
    struct {
        unsigned z_msb         :8;
        unsigned               :4;
        unsigned z_lsb         :4;
    };
    unsigned short _data;
} mmaOUT_Zbits_t;

// Register: OUT_Z_MSB
// bitfield definitions
typedef union {
    struct {
        unsigned z_msb         :8;
    };
    unsigned char _data;
} mmaOUT_Z_MSBbits_t;

// Register: OUT_Z_LSB
// bitfield definitions
typedef union {
    struct {
        unsigned z_lsb         :8;
    };
    unsigned char _data;
} mmaOUT_Z_LSBbits_t;

// Register: MMA_F_OUT_XYZ
// bitfield definitions
typedef union {
    struct {
        unsigned x             :8;
        unsigned y             :8;
        unsigned z             :8;
    };
    unsigned char _data[3];
} mmaF_OUT_XYZbits_t;

// Register: MMA_F_OUT_YZ
// bitfield definitions
typedef union {
    struct {
        unsigned y             :8;
        unsigned z             :8;
    };
    unsigned char _data[2];
} mmaF_OUT_YZbits_t;

// Register: MMA_F_OUT_XYZ
// bitfield definitions
typedef union {
    struct {
        unsigned x_msb        :8;
        unsigned              :4;
        unsigned x_lsb        :4;
        unsigned y_msb        :8;
        unsigned              :4;
        unsigned y_lsb        :4;
        unsigned z_msb        :8;
        unsigned              :4;
        unsigned z_lsb        :4;
    };
    struct {
        unsigned _x;
        unsigned _y;
        unsigned _z;
    };
    unsigned short _data[3];
} mmaOUT_XYZbits_t;

// Register: MMA_F_OUT_YZ
// bitfield definitions
typedef union {
    struct {
        unsigned              :4;
        unsigned y_lsb        :4;
        unsigned z_msb        :8;
        unsigned              :4;
        unsigned z_lsb        :4;
    };
    struct {
        unsigned _y;
        unsigned _z;
    };
    unsigned short _data[2];
} mmaOUT_YZbits_t;

// Register: MMA_F_SETUP
// bitfield definitions
typedef union {
    struct {
        unsigned F_WMRK        :6;
        unsigned F_MODE        :2;
    };
    unsigned char _data;
} mmaF_SETUPbits_t;

// Register: MMA_TRIG_CFG
// bitfield definitions
typedef union {
    struct {
        unsigned               :2;
        unsigned Trig_FF_MT    :1;
        unsigned Trig_PULSE    :1;
        unsigned Trig_LNDPRT   :1;
        unsigned Trig_TRANS    :1;
        unsigned               :2;
    };
    unsigned char _data;
} mmaTRIG_CFGbits_t;

// Register: MMA_SYSMOD
// bitfield definitions
typedef union {
    struct {
        unsigned SYSMOD0       :2;
        unsigned FGT           :5;
        unsigned FGERR         :1;
    };
    unsigned char _data;
} mmaSYSMODbits_t;

// Register: MMA_INT_SOURCE
// bitfield definitions
typedef union {
    struct {
        unsigned SRC_DRDY      :1;
        unsigned               :1;
        unsigned SRC_FF_MT     :1;
        unsigned SRC_PULSE     :1;
        unsigned SRC_LNDPRT    :1;
        unsigned SRC_TRANS     :1;
        unsigned SRC_FIFO      :1;
        unsigned SRC_ASLP      :1;
    };
    unsigned char _data;
} mmaINT_SOURCEbits_t;

// Register: MMA_WHO_AM_I
// bitfield definitions
typedef union {
    struct {
        unsigned WHO_AM_I      :8;
    };
    unsigned char _data;
} mmaWHO_AM_Ibits_t;

// Register: MMA_XYZ_DATA_CFG
// bitfield definitions
typedef union {
    struct {
        unsigned FS            :2;
        unsigned               :2;
        unsigned HPF_OUT       :1;
        unsigned               :3;
    };
    unsigned char _data;
} mmaXYZ_DATA_CFGbits_t;

// Register: MMA_HP_FILTER_CUTOFF
// bitfield definitions
typedef union {
    struct {
        unsigned SEL           :2;
        unsigned               :2;
        unsigned Pulse_LPF_EN  :1;
        unsigned Pulse_HPF_BYP :1;
        unsigned               :2;
    };
    unsigned char _data;
} mmaHP_FILTER_CUTOFFbits_t;

// Register: MMA_PL_STATUS
// bitfield definitions
typedef union {
    struct {
        unsigned BAFRO         :1;
        unsigned LAPO          :2;
        unsigned               :3;
        unsigned LO            :1;
        unsigned NEWLP         :1;
    };
    unsigned char _data;
} mmaPL_STATUSbits_t;

// Register: MMA_PL_CFG
// bitfield definitions
typedef union {
    struct {
        unsigned               :6;
        unsigned PL_EN         :1;
        unsigned DBCNTM        :1;
    };
    unsigned char _data;
} mmaPL_CFGbits_t;

// Register: MMA_PL_COUNT
// bitfield definitions
typedef union {
    struct  {
        unsigned DBCNE         :8;
    };
    unsigned char _data;
} mmaPL_COUNTbits_t;

// Register: MMA_PL_BF_ZCOMP
// bitfield definitions
typedef union {
    struct {
        unsigned ZLOCK         :3;
        unsigned               :3;
        unsigned BKFR          :2;
    };
    unsigned char _data;
} mmaPL_BF_ZCOMPbits_t;

// Register: MMA_P_L_THS_REG
// bitfield definitions
typedef union {
    struct {
        unsigned HYS           :3;
        unsigned P_L_THS       :5;
    };
    unsigned char _data;
} mmaP_L_THS_REGbits_t;

// Register: MMA_FF_MT_CFG
// bitfield definitions
typedef union {
    struct {
        unsigned               :3;
        unsigned XEFE          :1;
        unsigned YEFE          :1;
        unsigned ZEFE          :1;
        unsigned OAE           :1;
        unsigned ELE           :1;
    };
    unsigned char _data;
} mmaFF_MT_CFGbits_t;

// Register: MMA_FF_MT_SRC
// bitfield definitions
typedef union {
    struct {
        unsigned XHP           :1;
        unsigned XHE           :1;
        unsigned YHP           :1;
        unsigned YHE           :1;
        unsigned ZHP           :1;
        unsigned ZHE           :1;
        unsigned               :1;
        unsigned EA            :1;
    };
    unsigned char _data;
} mmaFF_MT_SRCbits_t;

// Register: MMA_FF_MT_THS
// bitfield definitions
typedef union {
    struct {
        unsigned THS           :7;
        unsigned DBCNTM        :1;
    };
    unsigned char _data;
} mmaFF_MT_THSbits_t;

// Register: MMA_FF_MT_COUNT
// bitfield definitions
typedef union {
    struct {
        unsigned D             :8;
    };
    unsigned char _data;
} mmaFF_MT_COUNTbits_t;

// Register: MMA_TRANSIENT_CFG
// bitfield definitions
typedef union {
    struct {
        unsigned HPF_BYP       :1;
        unsigned XTEFE         :1;
        unsigned YTEFE         :1;
        unsigned ZTEFE         :1;
        unsigned ELE           :1;
        unsigned               :3;
    };
    unsigned char _data;
} mmaTRANSIENT_CFGbits_t;

// Register: MMA_TRANSIENT_SRC
// bitfield definitions
typedef union {
    struct {
        unsigned X_Trans_Pol   :1;
        unsigned XTRANSE       :1;
        unsigned Y_Trans_Pol   :1;
        unsigned YTRANSE       :1;
        unsigned Z_TRANS_Pol   :1;
        unsigned ZTRANSE       :1;
        unsigned EA            :1;
        unsigned               :1;
    };
    unsigned char _data;
} mmaTRANSIENT_SRCbits_t;

// Register: MMA_TRANSIENT_THS
// bitfield definitions
typedef union {
    struct {
        unsigned THS           :7;
        unsigned DBCNTM        :1;
    };
    unsigned char _data;
} mmaTRANSIENT_THSbits_t;

// Register: MMA_TRANSIENT_COUNT
// bitfield definitions
typedef union {
    struct {
        unsigned D             :1;
    };
    unsigned char _data;
} mmaTRANSIENT_COUNTbits_t;

// Register: MMA_PULSE_CFG
// bitfield definitions
typedef union {
    struct {
        unsigned XSPEFE        :1;
        unsigned XDPEFE        :1;
        unsigned YSPEFE        :1;
        unsigned YDPEFE        :1;
        unsigned ZSPEFE        :1;
        unsigned ZDPEFE        :1;
        unsigned ELE           :1;
        unsigned DPA           :1;
    };
    unsigned char _data;
} mmaPULSE_CFGbits_t;

// Register: MMA_PULSE_SRC
// bitfield definitions
typedef union {
    struct {
        unsigned PolX          :1;
        unsigned PolY          :1;
        unsigned PolZ          :1;
        unsigned DPE           :1;
        unsigned AxX           :1;
        unsigned AxY           :1;
        unsigned AxZ           :1;
        unsigned EA            :1;
    };
    unsigned char _data;
} mmaPULSE_SRCbits_t;

// Register: MMA_PULSE_THSX
// bitfield definitions
typedef union {
    struct {
        unsigned THSX          :7;
        unsigned               :1;
    };
    unsigned char _data;
} mmaPULSE_THSXbits_t;

// Register: MMA_PULSE_THSY
// bitfield definitions
typedef union {
    struct {
        unsigned THSY          :7;
        unsigned               :1;
    };
    unsigned char _data;
} mmaPULSE_THSYbits_t;

// Register: MMA_THSZ
// bitfield definitions
typedef union {
    struct {
        unsigned THSZ          :7;
        unsigned               :1;
    };
    unsigned char _data;
} mmaTHSZbits_t;

// Register: MMA_PULSE_TMLT
// bitfield definitions
typedef union {
    struct {
        unsigned TMLT          :8;
    };
    unsigned char _data;
} mmaPULSE_TMLTbits_t;

// Register: MMA_PULSE_LTCY
// bitfield definitions
typedef union {
    struct {
        unsigned LTCY          :8;
    };
    unsigned char _data;
} mmaPULSE_LTCYbits_t;

// Register: MMA_PULSE_WIND
// bitfield definitions
typedef union {
    struct {
        unsigned WIND          :8;
    };
    unsigned char _data;
} mmaPULSE_WINDbits_t;

// Register: MMA_ASLP_COUNT
// bitfield definitions
typedef union {
    struct {
        unsigned D             :8;
    };
    unsigned char _data;
} mmaASLP_COUNTbits_t;

// Register: MMA_CTRL_REG1
// bitfield definitions
typedef union {
    struct {
        unsigned ACTIVE        :1;
        unsigned F_READ        :1;
        unsigned               :1;
        unsigned DR            :3;
        unsigned ASLP_RATE     :2;
    };
    unsigned char _data;
} mmaCTRL_REG1bits_t;

// Register: MMA_CTRL_REG2
// bitfield definitions
typedef union {
    struct {
        unsigned MODS          :2;
        unsigned SLPE          :1;
        unsigned SMODS         :2;
        unsigned               :1;
        unsigned RST           :1;
        unsigned ST            :1;
    };
    unsigned char _data;
} mmaCTRL_REG2bits_t;

// Register: MMA_CTRL_REG3
// bitfield definitions
typedef union {
    struct {
        unsigned PP_OD         :1;
        unsigned IPOL          :1;
        unsigned               :1;
        unsigned WAKE_FF_MT    :1;
        unsigned WAKE_PULSE    :1;
        unsigned WAKE_LNDPRT   :1;
        unsigned WAKE_TRANS    :1;
        unsigned FIFO_GATE     :1;
    };
    unsigned char _data;
} mmaCTRL_REG3bits_t;

// Register: MMA_CTRL_REG4
// bitfield definitions
typedef union {
    struct {
        unsigned INT_EN_DRDY   :1;
        unsigned               :1;
        unsigned INT_EN_FF_MT  :1;
        unsigned INT_EN_PULSE  :1;
        unsigned INT_EN_LNDPRT :1;
        unsigned INT_EN_TRANS  :1;
        unsigned INT_EN_FIFO   :1;
        unsigned INT_EN_ASLP   :1;
    };
    unsigned char _data;
} mmaCTRL_REG4bits_t;

// Register: MMA_CTRL_REG5
// bitfield definitions
typedef union {
    struct {
        unsigned INT_CFG_DRDY  :1;
        unsigned               :1;
        unsigned INT_CFG_FF_FMT:1;
        unsigned INT_CFG_PULSE :1;
        unsigned INT_CFG_LNDPRT:1;
        unsigned INT_CFG_TRANS :1;
        unsigned INT_CFG_FIFO  :1;
        unsigned INT_CFG_ASLP  :1;
    };
    unsigned char _data;
} mmaCTRL_REG5bits_t;

// Register: MMA_OFF_X
// bitfield definitions
typedef union {
    struct {
        unsigned D             :8;
    };
    unsigned char _data;
} mmaOFF_Xbits_t;

// Register: MMA_OFF_Y
// bitfield definitions
typedef union {
    struct {
        unsigned D             :8;
    };
    unsigned char _data;
} mmaOFF_Ybits_t;

// Register: MMA_OFF_Z
// bitfield definitions
typedef union {
    struct {
        unsigned D             :8;
    };
    unsigned char _data;
} mmaOFF_Zbits_t;

#pragma GCC diagnostic pop

glbl_err_t comm_MMA_start(volatile mma_reg_t addr);
#define comm_MMA_stop() i2c_stop();

#define comm_MMA_write_byte(reg) i2c_write(reg); i2c_read_bit()

glbl_err_t comm_MMA_read(const volatile mma_reg_t addr, volatile unsigned char* buff, volatile unsigned char buff_len);
glbl_err_t comm_MMA_write(const volatile mma_reg_t addr, volatile unsigned char* buff, volatile unsigned char buff_len);

void mmaExecute(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MMA8652FC_H */
