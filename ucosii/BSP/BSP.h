#ifndef __BSP_H
#define __BSP_H

#include "stm32f10x_rcc.h"
#include "bsp_led.h" 				//LED ��������
#include "BlueTooth.h" 			//����ͨ�ź���
#include "GPS.h"						//gps
#include "I2cdev.h"					//i2c
#include "motor.h"					//���
#include "kalman.h"					//�˲��㷨
#include "MPU6050.h"				//mpu6050
#include "HMC5883L.h"				//hmc5883l
#include "Rev.h"						//������
#include "tim7.h"						//TIM7��ʱ��
#include "usart1.h"					//����1

void SysTick_init(void);
void BSP_Init(void);        //��������ϵͳʱ�ӣ���ʼ��Ӳ��

#endif // __BSP_H

