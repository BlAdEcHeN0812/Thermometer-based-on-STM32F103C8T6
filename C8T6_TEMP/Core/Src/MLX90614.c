#include "MyIIC.h"
#include "MLX90614_Address.h" 
 
float a_temp=0;    //环境温度
float o_temp=0;    //物体温度
 
uint32_t MLX90614_ReadReg(uint8_t RegAddress)
{
	uint8_t Data[2]={0};
	uint16_t Data_all=0;
	
	MyI2C_Start();
	MyI2C_SendByte(MLX90614_ADDRESS);
	MyI2C_ReceiveAck();
	MyI2C_SendByte(RegAddress);
	MyI2C_ReceiveAck();
	
	MyI2C_Start();
	MyI2C_SendByte(MLX90614_ADDRESS | 0x01);
	MyI2C_ReceiveAck();
	Data[0] = MyI2C_ReceiveByte();
	MyI2C_ReceiveAck();
	Data[1] = MyI2C_ReceiveByte();
	MyI2C_SendAck(1);
	MyI2C_Stop();
	
	Data_all=(Data[1]<<8)+Data[0];
	return Data_all;
}
 
void MLX90614_Init(void)
{
	MyI2C_Init();
}
void MLX90614_TO(void)
{
	int i=MLX90614_ReadReg(MLX90614_RAM_TOBJ1);
	i=(i*2)-27315;
	o_temp=i*0.01;
}
void MLX90614_TA(void)
{
	int i=MLX90614_ReadReg(MLX90614_RAM_TA);
	i=(i*2)-27315;
	a_temp=i*0.01;
}
 