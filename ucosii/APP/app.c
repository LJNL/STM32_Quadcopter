#include "includes.h"

/********************��������ջ****************************/
OS_STK task_bluetooth_stk[TASK_STK_SIZE];
OS_STK task_atttitude_computation_stk[TASK_STK_SIZE];
OS_STK task_atttitude_pid_stk[TASK_STK_SIZE];
OS_STK task_gps_stk[TASK_STK_SIZE];

OS_STK task_led2_stk[TASK_LED2_STK_SIZE];
OS_STK task_led3_stk[TASK_LED3_STK_SIZE]; 


/*************************����******************************/
extern int pwmout1, pwmout2, pwmout3, pwmout4; 				//���ռ�ձ�
extern u16 USART3_RX_STA;
extern u16 USART2_RX_STA;

extern uint16_t GYRO_XOUT,GYRO_YOUT,GYRO_ZOUT,ACCEL_XOUT,ACCEL_YOUT,ACCEL_ZOUT,MP6050_Temperature;//X,Y,Z�ᣬ�¶�
extern float Angle_X_Error, Angle_Y_Error,Angle_Z_Error;
extern int Magn_x,Magn_y,Magn_z;
extern float angle;	//��������(Լ�ش��ϼ�)Ϊ��׼ ��λ��

//����������

extern OS_EVENT * MainQ;
extern void *QMessageMain[QSIZE];
extern INT8U errMainQ;

//������̬����

extern OS_EVENT * PIDQ;
extern void *QMessagePID[QSIZE];
extern INT8U errPIDQ;

//����ͨ���߳� ->��λ��

extern OS_EVENT * PCQ;
extern void *QMessagePC[QSIZE];
extern INT8U errPCQ;


 //������20HZ
void Task_Start(void *p_arg)
{
 (void)p_arg; // 'p_arg' ��û���õ�����ֹ��������ʾ����

	 
	//��ȡflash��Ԥ�����@@@
 
 
	
	//������������@@@
	OSTaskCreate(Task_BlueTooth,(void *)0, //��������ͨ������
	&task_bluetooth_stk[TASK_STK_SIZE-1], TASK_BLUETOOTH_PRIO);
	
	OSTaskCreate(Task_atttitude_computation,(void *)0, //������̬��������
	&task_atttitude_computation_stk[TASK_STK_SIZE-1], TASK_ATTITUDE_COMPUTATION_PRIO);
	
	OSTaskCreate(Task_atttitude_pid,(void *)0, //������̬��������
	&task_atttitude_pid_stk[TASK_STK_SIZE-1], TASK_ATTITUDE_PID_PRIO);
	
	OSTaskCreate(Task_atttitude_pid,(void *)0, //����gps����
	&task_gps_stk[TASK_STK_SIZE-1], TASK_GPS_PRIO);
	
	
	//DEBUG
	OSTaskCreate(Task_LED2,(void *)0, //�������� 2
	&task_led2_stk[TASK_LED2_STK_SIZE-1], TASK_LED2_PRIO);

	OSTaskCreate(Task_LED3,(void *)0, //�������� 3
	&task_led3_stk[TASK_LED3_STK_SIZE-1], TASK_LED3_PRIO);
 //��ȡflash��Ԥ�����@@@
 
 


 while (1)
 {
	 
	 
	 //����Ϣ�����л�ȡ��λ������������Ӧ,���ݷ�����Ϣ���в����͸���λ��
	 //������Ϣ ��Ҫ��PID�����Ľ���
	 OSQAccept(MainQ,&errMainQ);	 
	 
	 if(errMainQ==OS_NO_ERR)
	 {
		  //���ܵ���Ϣ����@@@
		 
	 }
	 else
	 {
	 //δ���յ���Ϣ@@@
	 }
	 
	 //����λ��������Ϣ@@@ 
	 //Ŀǰ��֪��Ҫ����ʲô��Ϣ@��@
	 //OSQPost(PCQ,  void * );
	 
	 
	 //����̬���Ʒ�����Ϣ@@@
	 //OSQPost(PIDQ,  void * );
	 
	 
	 //
	LED1( ON );
	OSTimeDlyHMSM(0, 0,0,50);
 }
}



// ����ͨ������50HZ 
// ���ȼ�5
void Task_BlueTooth(void*p_arg)
{
	(void)p_arg;
	
	//������ʼ�� ������LEDָʾ��
	BLUETOOTH_GPIO_Config();
	USART3_NVIC_Config();
	u3_printf("\r\n Bluetooth init succeed!\r\n");
	
	while(1)
	{
		//��Ϣ���в���
		//�ж���ʵ��---������λ����Ϣ�������͵���Ϣ������

		
		//���߳�Ҳ�ӷ�����Ϣ�����л�ȡ���ݣ�@@@
		//���͸���λ��
	 OSQAccept(PCQ,&errPCQ);
		if(errPCQ==OS_NO_ERR)
		{
		
			//printf();
		
		}
		else
		{
		
		
		}		
		OSTimeDlyHMSM(0,0,0,20);
	}
}
 //��̬��������250HZ
//���ȼ�6
void Task_atttitude_computation(void *p_arg)
{
 int i;
 float x,y,z,magn_x,magn_y,magn_z;
	(void)p_arg;	
		
	//I2C��ʼ��
	I2C_GPIO_Configuration();
	OSTimeDlyHMSM(0,0,0,500);
	Init_MPU6050();
	//u3_printf("\r\n MPU6050 init succeed!\r\n");		
		//I2C��ʼ��
	I2C_GPIO_Configuration();
	OSTimeDlyHMSM(0,0,0,500);
	Init_MPU6050();
	//u3_printf("\r\n MPU6050 init succeed!\r\n");
		
	//���ջ���ʼ��
	Rev_GPIO_init();
	Rev_NVIC_TIM_init();
	//u3_printf("\r\n Receiver init succeed!\r\n");
		
	//e-switcher��ʼ��
	Motor_PWM_ALL(100);
	OSTimeDlyHMSM(0,0,0,2000);
	Motor_PWM_ALL(0);
	OSTimeDlyHMSM(0,0,0,2000);
	//u3_printf("\r\n e-switcher init succeed!\r\n");
		
	//�����ջ�
	if((pwmout1<1500)&&(pwmout1>1480)&&(pwmout2<990)&&(pwmout2>1010)&&(pwmout4<1500)&&(pwmout4>1480)&&(pwmout3<1500)&&(pwmout3>1480))
	{
	//	u3_printf("\r\n Receiver works good!\r\n");
	}
	else
	{
	//	u3_printf("\r\n Receiver doesn't work!\r\n");
	}
	
	Init_MPU6050();
	Init_HMC5883L();
	
	/********�������̱���ˮƽ���Է�λ�����***************/
		for(i=0;i<2000;i++){
			Read_HMC5883L();
			magn_x=Magn_x;
			magn_y=Magn_y;    
			magn_z=Magn_z;
			if(magn_x<32764) x=magn_x/16384;
			else              x=(32768-magn_x)/16384;
				
			if(magn_y<32764) y=magn_y/16384;
			else              y=(32768-magn_y)/16384;//1-(Accel_y-49152)/16384;	
			angle+=atan2(y,x) * (180 / 3.14159265);
		}
		angle=angle/2000;
		
		/********/
	
		for(i=0;i<2000;i++){
			READ_MPU6050_Gyro();
			READ_MPU6050_Accel();	
			Read_HMC5883L();
			Angle_Calcu();
		  
			Angle_X_Error += Angle_X_Final;
			Angle_Y_Error += Angle_Y_Final;
			Angle_Z_Error	+= Angle_Z_Final;
		}
//			Angle_X_Error/=2000;
//			Angle_Y_Error/=2000;
//			Angle_Z_Error/=2000;
	while (1)
 {
	//��ȡ����
	READ_MPU6050_Gyro();
	READ_MPU6050_Accel();	
	Read_HMC5883L();
	//�������˲�
	 
	 Angle_Calcu();
	 
	 //����̬���Ʒ�����Ϣ@@@
	 //OSQPost(PIDQ,  void * );
	 
	 
  OSTimeDlyHMSM(0, 0,0,4);
 }
}






//��̬����PID����100HZ
//���ȼ�7
void Task_atttitude_pid(void *p_arg)
{
 (void)p_arg;	
 while (1)
 {
	 OSQAccept(PIDQ,&errPIDQ);
	 if(errPIDQ==OS_NO_ERR)
	 {
		 
	//��Ԫ��ת��Ϊŷ����@@@
	 
	//�ж��д���õĽ��ջ�PPM����ת��Ϊң����ֵ@@@
	 
	//Ȼ�����ң��������ֵ��Ŀ��λ�ú���λ����������ĸ����ת��@@@
	 }
	 else
	 {
	 
	 }	 
 OSTimeDlyHMSM(0, 0,0,10);

 }
}

//GPS����50HZ(50hz ����)
//���ȼ�8
void Task_GPS(void *p_arg)
{
 (void)p_arg;	
 while (1)
 {
	//��ȡGPS@@@
	
	 //Ŀǰ��̫���@@@    @��@
	 
	 
 OSTimeDlyHMSM(0, 0,0,20);

 }
}




//DEBUG
//���� 2
void Task_LED2(void *p_arg)
{
 (void)p_arg;

 while (1)
 {
 LED2( ON );
 OSTimeDlyHMSM(0, 0,0,200);
 LED2( OFF);
 OSTimeDlyHMSM(0, 0,0,200);
 }
}

//���� 3
void Task_LED3(void *p_arg)
{
 (void)p_arg;

 while (1)
 {
 LED3( ON );
 OSTimeDlyHMSM(0, 0,0,300);
 LED3( OFF);
 OSTimeDlyHMSM(0, 0,0,300);
 }
}