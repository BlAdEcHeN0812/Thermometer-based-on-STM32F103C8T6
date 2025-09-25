#include "stm32f1xx_hal.h"
#include "Delay.h" // 假设这是您的延时函数


// 软件模拟 I2C
// PB6 口为 SCL；PB7 为 SDA；

void MyI2C_W_SCL(uint8_t BitValue)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (GPIO_PinState)BitValue);
    Delay_us(10);
}
 
void MyI2C_W_SDA(uint8_t BitValue)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, (GPIO_PinState)BitValue);
    Delay_us(10);
}
 
uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);
    Delay_us(10);
    return BitValue;
}
 
void MyI2C_Init(void)
{
    // HAL 库的初始化通常通过 CubeMX 生成，但这里手动配置
    // 启用 GPIOB 时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();
 
    GPIO_InitTypeDef GPIO_InitStructure = {0};
 
    // 配置 SCL (PB6) 和 SDA (PB7)
    GPIO_InitStructure.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD; // 开漏输出模式
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
 
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
 
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_SET);
}
 
void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}
 
void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}
 
void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i ++)
    {
        MyI2C_W_SDA((Byte & (0x80 >> i)) ? 1 : 0);
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }
}
 
uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);
    for (i = 0; i < 8; i ++)
    {
        MyI2C_W_SCL(1);
        if (MyI2C_R_SDA() == 1) {
            Byte |= (0x80 >> i);
        }
        MyI2C_W_SCL(0);
    }
    return Byte;
}
 
void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}
 
uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);
    return AckBit;
}