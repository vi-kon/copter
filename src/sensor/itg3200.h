#ifndef MULTIWII_ITG3200_H
#define MULTIWII_ITG3200_H

#include "../config.h"

#if defined(ITG_3200) || defined(ITG3205) || defined(ITG_3050) || defined(MPU3050)

#define ITG3200_ADDRESS 0x68

// Registers
#define ITG3200_PWR_MGM 0x3E
#define ITG3200_DLPF_FS 0x16
#define ITG3200_GYRO_XOUT_H 0x1D

#define ITG3200_H_RESET 0x01

// FS_SEL options
#define ITG3200_FS_SEL 0x03

// CLK_SEL options
#define ITG3200_CLK_SEL_INTERNAL_OSCILATOR 0x00
#define ITG3200_CLK_SEL_PLL_X_GYRO 0x01
#define ITG3200_CLK_SEL_PLL_Y_GYRO 0x02
#define ITG3200_CLK_SEL_PLL_Z_GYRO 0x03
#define ITG3200_CLK_SEL_PLL_32768kHz 0x04
#define ITG3200_CLK_SEL_PLL_192Mhz 0x05

void itg3205_init();

void itg3205_update();

#endif

#endif //MULTIWII_ITG3200_H