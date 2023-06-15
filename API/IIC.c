#include "IIC.h"

static void gpio_config(void)
{
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);

    gpio_init(GPIOA, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_5);// CLK
    gpio_init(GPIOA, GPIO_MODE_OUT_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6);// SDA
    i2c_Stop();

}



/**
 * @brief IIC ��ʼ��
 * 
 */
void I2C_Init(void)
{
    gpio_config();

}


void i2c_Delay(void)
{
    uint8_t i;
    uint8_t a;

    for ( i = 0; i < 20; i++)
    {
        a++;
    }
    

}


void i2c_Start(void)
{
    i2c_SDA_1();
    i2c_SCL_1();

    i2c_Delay();
    i2c_SDA_0();
    i2c_Delay();
    i2c_SCL_0();
    i2c_Delay();

}

void i2c_Stop()
{
    i2c_SDA_0();
    i2c_SCL_1();
    i2c_Delay();
    i2c_SDA_1();
}


void i2c_SendByte(uint8_t _ucByte)
{
    uint8_t i;

    for(i = 0; i < 8 ; i++)
    {
        if(_ucByte & 0x80)
        {
            i2c_SDA_1();
        }
        else
        {
            i2c_SDA_0();
        }
        i2c_Delay();
        i2c_SCL_1();
        i2c_Delay();
        i2c_SCL_0();
        if(i == 7)
        {
            i2c_SDA_1();
        }
        _ucByte <<= 1;
        i2c_Delay();
    }
}




uint8_t i2c_ReadByte()
{
    uint8_t i;
    uint8_t value;

    value = 0;
    for(i =0; i < 8; i++)
    {
        value <<= 1;
        i2c_SCL_1();
        i2c_Delay();
        if(i2c_SDA_READ())
        {
            value++;
        }
        i2c_SCL_0();
        i2c_Delay();


    }

    return value;
}





uint8_t i2c_WaitACK(void)
{
    uint8_t re;

    i2c_SDA_1();
    i2c_Delay();

    i2c_SCL_1();
    i2c_Delay();

    if(i2c_SDA_READ())
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    i2c_SCL_0();
    i2c_Delay();
    return  re;
}




void i2c_Ack()
{
    i2c_SDA_0();
    i2c_Delay();

    i2c_SCL_1();
    i2c_Delay();
    i2c_SCL_0();
    i2c_Delay();

    i2c_SDA_1();
}





void i2c_NACK()
{
    i2c_SDA_1();
    i2c_Delay();

    i2c_SCL_1();
    i2c_Delay();
    i2c_SCL_0();
    i2c_Delay();
}



/**
 * @brief ����豸���?
 * 
 * @param _Address 
 * @return uint8_t 
 */
uint8_t IIC_CheckDevice(uint8_t _Address)
{

    uint8_t i;
    i2c_Start();

    i2c_SendByte(_Address);
    i = i2c_WaitACK();
    i2c_Stop();

    return i;

}


/**
 * @brief ��Ĵ���д������?
 * 
 * @param addr 
 * @param reg 
 * @param len 
 * @param data 
 * @return uint8_t 
 */
uint8_t i2c_write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
    uint8_t i;

    i2c_Start();
    i2c_SendByte((addr << 1) | 0);
    if(i2c_WaitACK() == 1)
    {
        i2c_Stop();
        printf("IIC Send Slave Address error\n");
        return Light_EACK;
    }

    i2c_SendByte(reg);
    if(i2c_WaitACK() == 1)
    {
        i2c_Stop();
        printf("IIC Send Reg Address error\n");
        return Light_EACK;
    }

    for(i = 0; i<len ; i++)
    {
        i2c_SendByte(data[i]);
        if(i2c_WaitACK() == 1)
        {
            i2c_Stop();
            printf("IIC Send Data error in i = %d", i);
        }
    }

    i2c_Stop();
    return Light_EOK;

}


/**
 * @brief ��ȡ�Ĵ�������
 * 
 * @param addr 
 * @param reg 
 * @param len 
 * @param data 
 * @return uint8_t 
 */
uint8_t i2c_read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
    i2c_Start();
    i2c_SendByte((addr << 1) | 0);
    if(i2c_WaitACK() == 1)
    {
        i2c_Stop();
        printf("IIC Send Slave Address error\n");
        return Light_EACK;
    }

    i2c_SendByte(reg);
    if(i2c_WaitACK() == 1)
    {
        i2c_Stop();
        printf("IIC Send Reg Address error\n");
        return Light_EACK;
    }

    i2c_Start();
    i2c_SendByte((addr << 1) | 1);
    if(i2c_WaitACK() == 1)
    {
        i2c_Stop();
        printf("IIC Send Slave Address error\n");
        return Light_EACK;
    }

    while (len)
    {
        *data = i2c_ReadByte();
        len--;
        data++;
    }

    i2c_Stop();
    return Light_EOK;
    
    


}




/**
 * @brief ���մ�������ʼ��
 * 
 * @details ���п���
 * 
 */
void light_Sensor_Init(void)
{
    uint8_t err = 0;
    i2c_Start();
    i2c_SendByte(0x23 << 1);
    err = i2c_WaitACK();
    if(err)
    {
        printf("Call Device failed in light_Sensor_Init [1]\n");
    }
    i2c_SendByte(0x01);
    err = i2c_WaitACK();
    if(err)
    {
        printf("light sensor command failed in light_Sensor_Init [2]\n");
    }
    i2c_Stop();
}




/**
 * @brief ���մ��������ݶ�ȡ
 * 
 * @return uint16_t ����16λ����ǿ������
 */
uint16_t light_Sensor_read_data(void)
{


/**************** ����ָ�� ************************/
    uint8_t err = 0;
    i2c_Start();// ��ʼ�ź�
    i2c_SendByte( (0x23 << 1) | 0);// �����豸��ַ д����
    err = i2c_WaitACK();// �ȴ��豸�ظ�ACK
    if(err)// �������?
    {
        printf("Call Device failed in light_Sensor_read_data [1]\n");
    }
    i2c_SendByte(0x10);// �����豸����ָ��
    err = i2c_WaitACK();// �ȴ��豸�ظ�ACK
    if(err)// �������?
    {
        printf("light sensor command failed in light_Sensor_read_data [2]\n");
    }
    i2c_Stop();// ֹͣ�ź�
/************** End of ����ָ�� ********************/



/**************** ��ȡ���� ************************/
    uint16_t light_sensor_data = 0;// �������ݴ洢����
    i2c_Start();// ��ʼ�ź�
    i2c_SendByte( (0x23 << 1) | 1);// �����豸��ַ ������
    err = i2c_WaitACK();// �ȴ��豸�ظ�ACK
    if(err)// �������?
    {
        printf("Call Device failed in light_Sensor_read_data [3]\n");
    }

    light_sensor_data |= i2c_ReadByte();// ��ȡ�Ĵ������� ��8λ
    light_sensor_data <<= 8 ;// ����8λ
    i2c_Ack();// ���豸�ظ�ACK
    light_sensor_data |= i2c_ReadByte();// ��ȡ�Ĵ������� ��8λ
    i2c_Ack();// ���豸�ظ�ACK
/************* End of ��ȡ���� ********************/

    return light_sensor_data;

}



