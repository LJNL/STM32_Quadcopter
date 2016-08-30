/***������***/
#include "MPU6050.h"
#include "kalman.h"
#include "i2cdev.h"
#include "usart1.h"
#include "ucos_ii.h"

/*�궨��------------------------------------------------------------------*/

#define	MPU6050_Addr   0xD0	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�
// ����MPU6050�ڲ���ַ
//****************************************
#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)

#define INT_PIN_CFG     0x37    //������·��Ч ��ֵ��0x42 AUX_DA�ĸ���I2C
#define USER_CTRL       0x6A    //�û����üĴ��� ��ֵ��0x40  AUX_DA�ĸ���I2C

#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40

#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define	WHO_AM_I		0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)

u8 TX_DATA[4];  	 //��ʾ�ݻ�����
u8 BUF[10];       //�������ݻ�����	
uint16_t GYRO_XOUT,GYRO_YOUT,GYRO_ZOUT,ACCEL_XOUT,ACCEL_YOUT,ACCEL_ZOUT,MP6050_Temperature;//X,Y,Z�ᣬ�¶�	
float Angle_X_Error=0.0f, Angle_Y_Error=0.0f,Angle_Z_Error=0.0f;

void Init_MPU6050(void)
{
//	int i;
  I2C_WriteByte(MPU6050_Addr,PWR_MGMT_1, 0x00);	//�������״̬
	
	
	I2C_WriteByte(MPU6050_Addr,SMPLRT_DIV, 0x07);    //�����ǲ�����
	I2C_WriteByte(MPU6050_Addr,CONFIG, 0x06);        //5Hz 
	
	I2C_WriteByte(MPU6050_Addr,INT_PIN_CFG, 0x42);   //ʹ����·I2C
	I2C_WriteByte(MPU6050_Addr,USER_CTRL, 0x40);     //ʹ����·I2C
	
	I2C_WriteByte(MPU6050_Addr,GYRO_CONFIG, 0x18);   
	I2C_WriteByte(MPU6050_Addr,ACCEL_CONFIG, 0x01);
	OSTimeDlyHMSM(0,0,0,200);
//	for(i=0;i<2000;i++){
//			READ_MPU6050_Gyro();
//			READ_MPU6050_Accel();	
//			
//			Angle_Calcu();
//		  
//			Angle_X_Error += Angle_X_Final;
//			Angle_Y_Error += Angle_Y_Final;
//			Angle_Z_Error	+= Angle_Z_Final;
//		}
//			Angle_X_Error/=2000;
//			Angle_Y_Error/=2000;
//			Angle_Z_Error/=2000;
}

//void MPU6050_WHO_AM_I(void)
//{
//	u8 dev=0;
//		if(dev=I2C_ReadByte(MPU6050_Addr,WHO_AM_I),dev==0x68)
//  { 
//    	printf("\r�豸MP6050ʶ��ɹ���id=0x%x\r\n\r",dev);
//  }
//	else{ printf("\r����!�޷�����豸MP6050��id=0x%x\r\n\r");}
//}

void READ_MPU6050_Gyro(void)
{
   BUF[0]=I2C_ReadByte(MPU6050_Addr,GYRO_XOUT_L); 
   BUF[1]=I2C_ReadByte(MPU6050_Addr,GYRO_XOUT_H);
   GYRO_XOUT=	(BUF[1]<<8)|BUF[0];
  // GYRO_XOUT/=16.4; 						   //��ȡ����X������

   BUF[2]=I2C_ReadByte(MPU6050_Addr,GYRO_YOUT_L);
   BUF[3]=I2C_ReadByte(MPU6050_Addr,GYRO_YOUT_H);
   GYRO_YOUT=	(BUF[3]<<8)|BUF[2];
  // GYRO_YOUT/=16.4; 						   //��ȡ����Y������
   BUF[4]=I2C_ReadByte(MPU6050_Addr,GYRO_ZOUT_L);
   BUF[5]=I2C_ReadByte(MPU6050_Addr,GYRO_ZOUT_H);
   GYRO_ZOUT=	(BUF[5]<<8)|BUF[4];
  // GYRO_ZOUT/=16.4; 					       //��ȡ����Z������

}

void READ_MPU6050_Accel(void)
{
	BUF[0]=I2C_ReadByte(MPU6050_Addr,ACCEL_XOUT_L); 
	BUF[1]=I2C_ReadByte(MPU6050_Addr,ACCEL_XOUT_H);
	ACCEL_XOUT=	(BUF[1]<<8)|BUF[0];
//	ACCEL_XOUT=(float)((float)ACCEL_XOUT/16384)*100; 		//����100��	       //��ȡ����X������

	BUF[2]=I2C_ReadByte(MPU6050_Addr,ACCEL_YOUT_L);
	BUF[3]=I2C_ReadByte(MPU6050_Addr,ACCEL_YOUT_H);
	ACCEL_YOUT=	(BUF[3]<<8)|BUF[2];
//	ACCEL_YOUT=(float)((float)ACCEL_YOUT/16384)*100; 						   //��ȡ����Y������
   
	BUF[4]=I2C_ReadByte(MPU6050_Addr,ACCEL_ZOUT_L);
	BUF[5]=I2C_ReadByte(MPU6050_Addr,ACCEL_ZOUT_H);
	ACCEL_ZOUT=	(BUF[5]<<8)|BUF[4];
//	ACCEL_ZOUT=(float)((float)ACCEL_ZOUT/16384)*100; 					       //��ȡ����Z������

	
	BUF[6]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_L); 
	BUF[7]=I2C_ReadByte(MPU6050_Addr,TEMP_OUT_H); 
	MP6050_Temperature=(BUF[7]<<8)|BUF[6];
//	MP6050_Temperature = 35+ ((double) (T_T + 13200)) / 280;// ��ȡ������¶�
	MP6050_Temperature = (((float) MP6050_Temperature )/340+36.53)*10 ;//+36.53;  // ��ȡ������¶�
}