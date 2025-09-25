#include "stdint.h"
#ifndef __MLX90614_H
#define __MLX90614_H
 
extern float a_temp;    //环境温度
extern float o_temp;    //物体温度
 
uint32_t MLX90614_ReadReg(uint8_t RegAddress);
void MLX90614_Init(void);
void MLX90614_TO(void);
void MLX90614_TA(void);
 
#endif