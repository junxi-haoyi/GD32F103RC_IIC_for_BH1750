#ifndef __IIC_H__
#define __IIC_H__


#include "gd32f10x.h"
#include <stdio.h>

#define light_Sensor_Address 0x68


/**********************  SCL SDA 引脚操作  *****************************/
// GPIOA_5 CLK GPIOA_6 SDA
#define i2c_SCL_0()     gpio_bit_reset(GPIOA, GPIO_PIN_5)
#define i2c_SCL_1()     gpio_bit_set(GPIOA, GPIO_PIN_5)
#define i2c_SDA_0()     gpio_bit_reset(GPIOA, GPIO_PIN_6)
#define i2c_SDA_1()     gpio_bit_set(GPIOA, GPIO_PIN_6)

#define i2c_SCL_READ()  gpio_input_bit_get(GPIOA, GPIO_PIN_5)
#define i2c_SDA_READ()  gpio_input_bit_get(GPIOA, GPIO_PIN_6)
/*****************  END of SCL SDA 引脚操作  **************************/


/***********************  IIC 基本函数 ********************************/
void I2C_Init(void);
void i2c_Delay(void);
void i2c_Start(void);
void i2c_Stop();
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte();
uint8_t i2c_WaitACK(void);
void i2c_Ack();
void i2c_NACK();
uint8_t IIC_CheckDevice(uint8_t _Address);
/********************  END of IIC 基本函数 ***************************/


uint8_t i2c_write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
uint8_t i2c_read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);

void light_Sensor_Init(void);
uint16_t light_Sensor_read_data(void);

#define Light_EOK      0
#define Light_EID      1
#define Light_EACK     2


#endif