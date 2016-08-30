#include "kalman.h"
#include "MPU6050.h"
#include "math.h"

float Accel_x;	     //X����ٶ�ֵ�ݴ�
float Accel_y;	     //Y����ٶ�ֵ�ݴ�
float Accel_z;	     //Z����ٶ�ֵ�ݴ�

float Gyro_x;		 			//X�������������ݴ�
float Gyro_y;        //Y�������������ݴ�
float Gyro_z;		 		//Z�������������ݴ�

float magn_x;		 		//X��ų������ݴ�
float magn_y;       //Y��ų������ݴ�
float magn_z;       //Z��ų������ݴ�
float angle;				//�������̷�λ��

//float Angle_gy;    //�ɽ��ٶȼ������б�Ƕ�
float Angle_x_temp;  //�ɼ��ٶȼ����x��б�Ƕ�
float Angle_y_temp;  //�ɼ��ٶȼ����y��б�Ƕ�
float Angle_z_temp;  //�ɴų������Z��б�Ƕ�

float Angle_X_Final; //X������б�Ƕ�
float Angle_Y_Final; //Y������б�Ƕ�
float Angle_Z_Final; //Z������б�Ƕ�

extern uint16_t GYRO_XOUT,GYRO_YOUT,GYRO_ZOUT,ACCEL_XOUT,ACCEL_YOUT,ACCEL_ZOUT;
extern int Magn_x,Magn_y,Magn_z;

//�Ƕȼ���
void Angle_Calcu(void)	 
{
	//��ΧΪ2gʱ�������ϵ��16384 LSB/g
	//deg = rad*180/3.14
	float x,y,z,hx,hy;
	
	Accel_x = ACCEL_XOUT; //x����ٶ�ֵ�ݴ�
	Accel_y = ACCEL_YOUT; //y����ٶ�ֵ�ݴ�
	Accel_z = ACCEL_ZOUT; //z����ٶ�ֵ�ݴ�
	Gyro_x  = GYRO_XOUT;  //x��������ֵ�ݴ�
	Gyro_y  = GYRO_YOUT;  //y��������ֵ�ݴ�
	Gyro_z  = GYRO_ZOUT;  //z��������ֵ�ݴ�
	magn_x=Magn_x;
  magn_y=Magn_y;    
  magn_z=Magn_z;	


	//����x����ٶ�
	if(Accel_x<32764) x=Accel_x/16384;
	else              x=(32768-Accel_x)/16384;
	
	//����y����ٶ�
	if(Accel_y<32764) y=Accel_y/16384;
	else              y=(32768-Accel_y)/16384;//1-(Accel_y-49152)/16384;
	
	//����z����ٶ�
	if(Accel_z<32764) z=Accel_z/16384;
	else              z=(32768-Accel_z)/16384;
	
	
	//�ü��ٶȼ����������ˮƽ������ϵ֮��ļн�
	Angle_x_temp=(atan(y/z))*180/3.14f;//pitch
	Angle_y_temp=(atan(x/z))*180/3.14f;//roll
	
	//�ų�����
	if(magn_x<32764) x=magn_x/16384;
	else              x=(32768-magn_x)/16384;
		
	if(magn_y<32764) y=magn_y/16384;
	else              y=(32768-magn_y)/16384;//1-(Accel_y-49152)/16384;
	
	if(magn_z<32764) z=magn_z/16384;
	else              z=(32768-magn_z)/16384;
	
		//�Ƕȵ�������											
	if(Accel_x<32764) Angle_y_temp = +Angle_y_temp;
	if(Accel_x>32764) Angle_y_temp = -Angle_y_temp;
	if(Accel_y<32764) Angle_x_temp = +Angle_x_temp;
	if(Accel_y>32764) Angle_x_temp = -Angle_x_temp;
//	if(Accel_z<32764) {}
//	if(Accel_z>32764) {}
		
	//������ʽ a pitch b roll
	//Xr=Xcos��+Ysin��sin��-Zcos��sin��
	//Yr=Ycos��+Zsin��
	hx=x*cos(Angle_x_temp)+y*sin(Angle_x_temp)*sin(Angle_y_temp)-z*cos(Angle_y_temp)*sin(Angle_x_temp);
	hy=y*cos(Angle_y_temp)+z*sin(Angle_y_temp);
	
		//���ôų����� yaw���һ����
	Angle_z_temp= atan2(hy,hx) * (180 / 3.14159265);//+180
	Angle_z_temp-=angle;
	
	

	
	//���ٶ�
	//��ǰ�˶�
	if(Gyro_x<32768) Gyro_x=-(Gyro_x/16.4);//��ΧΪ1000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	//����˶�
	if(Gyro_x>32768) Gyro_x=+(65535-Gyro_x)/16.4;
	//��ǰ�˶�
	if(Gyro_y<32768) Gyro_y=-(Gyro_y/16.4);//��ΧΪ1000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	//����˶�
	if(Gyro_y>32768) Gyro_y=+(65535-Gyro_y)/16.4;
	//��ǰ�˶�
	if(Gyro_z<32768) Gyro_z=-(Gyro_z/16.4);//��ΧΪ1000deg/sʱ�������ϵ��16.4 LSB/(deg/s)
	//����˶�
	if(Gyro_z>32768) Gyro_z=+(65535-Gyro_z)/16.4;
	
	//Angle_gy = Angle_gy + Gyro_y*0.025;  //���ٶȻ��ֵõ���б�Ƕ�.Խ����ֳ����ĽǶ�Խ��
	Kalman_Filter_X(Angle_x_temp,Gyro_x);  //�������˲�����Y���
	Kalman_Filter_Y(Angle_y_temp,Gyro_y);  //�������˲�����Y���
	Kalman_Filter_Y(Angle_z_temp,Gyro_z);  //�������˲�����Z���													  
} 




//����������		
float Q_angle = 0.001;  //Ԥ�����أ�������  ���� Э����  
float Q_gyro  = 0.003;	//Ԥ�����أ�������  ���� Э����

/*****************
[  cov(X,X)  cov(Y,X) ]
	 cov(X,Y)  cov(Y,Y)
���Խ���  �������������ֵ
�����  б�Խ���Ϊ 0 
*******************/

float R_angle = 0.5;		//�۲�ֵ ���� �ķ���

/*****************
Ҳ��Э���� �������ؾ�һ�������ټƣ�
�����˻���һ����
*******************/

float dt      = 0.01;//dtΪkalman�˲�������ʱ��;

char  C_0     = 1;// �൱�� H ��ά����{1,0}

float Q_bias=0, Angle_err;
/*****************
�����Ǿ����ֹʱ����������  Q_Bias��
���ټ��� �����ǽǶ����
*******************/


/*****************
�����������õģ����ݹ�ʽ�ʹ������� ����������Ϊ��ά���� ���� �����K1��K2 ��
���Ǽٶ�H�̶���ά����{1,0}���ݹ�ʽ
����Щ�м����
�����ά����
�����ĸ
*******************/
float PCt_0, PCt_1, E;
float K_0, K_1, t_0, t_1;

/********
t1 t2 ����һ���ù۲�ֵ��X��k|k��ʱ�õ����м��������ΪҲ�ǽ��о���˷���
*********/

/*****************
����Ķ�ά������ǹ���Э�������
Pdot��4�����Ǳ������˷����̵��м���� �����ḳֵ������� 
*******************/
float Pdot[4] ={0,0,0,0};
float PP[2][2] = { { 1, 0 },{ 0, 1 } };

void Kalman_Filter_X(float Accel,float Gyro) //����������		
{
	Angle_X_Final += (Gyro - Q_bias) * dt; //�������
	
	Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��

	Pdot[1]= -PP[1][1];
	Pdot[2]= -PP[1][1];
	Pdot[3]= Q_gyro;
	
	PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PP[0][1] += Pdot[1] * dt;   // =����������Э����
	PP[1][0] += Pdot[2] * dt;
	PP[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - Angle_X_Final;	//zk-�������
	
	PCt_0 = C_0 * PP[0][0]; 
	PCt_1 = C_0 * PP[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	//��������Ϊ����������
	
	t_0 = PCt_0;
	t_1 = C_0 * PP[0][1];


	PP[0][0] -= K_0 * t_0;		 //����������Э�������Э���� Ϊ�´ε�����׼��
	PP[0][1] -= K_0 * t_1;
	PP[1][0] -= K_1 * t_0;
	PP[1][1] -= K_1 * t_1;
		
	//���Ǿ�����㣬�õ������ X��K|K�����Ź���ֵ
		
	Angle_X_Final += K_0 * Angle_err;	 //�������
	Q_bias        += K_1 * Angle_err;	 //�������
	//Ϊ��PID���� ������ٶ�
	Gyro_x         = Gyro - Q_bias;	 //���ֵ(�������)��΢��=���ٶ�
}
/****����***/
float PPY[2][2] = { { 1, 0 },{ 0, 1 } };
float Q_biasy=0;
void Kalman_Filter_Y(float Accel,float Gyro) //����������		
{
	Angle_Y_Final += (Gyro - Q_biasy) * dt; //�������
	
	Pdot[0]=Q_angle - PPY[0][1] - PPY[1][0]; // Pk-����������Э�����΢��

	Pdot[1]= - PPY[1][1];
	Pdot[2]= - PPY[1][1];
	Pdot[3]=Q_gyro;
	
	PPY[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PPY[0][1] += Pdot[1] * dt;   // =����������Э����
	PPY[1][0] += Pdot[2] * dt;
	PPY[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - Angle_Y_Final;	//zk-�������
	
	PCt_0 = C_0 * PPY[0][0];
	PCt_1 = C_0 * PPY[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PPY[0][1];


	PPY[0][0] -= K_0 * t_0;		 //����������Э����
	PPY[0][1] -= K_0 * t_1;
	PPY[1][0] -= K_1 * t_0;
	PPY[1][1] -= K_1 * t_1;
		
	Angle_Y_Final	+= K_0 * Angle_err;	 //�������
	Q_biasy	+= K_1 * Angle_err;	 //�������
	Gyro_y   = Gyro - Q_biasy;	 //���ֵ(�������)��΢��=���ٶ�
}


float PPZ[2][2] = { { 1, 0 },{ 0, 1 } };
float Q_biasz=0;
void Kalman_Filter_Z(float Accel,float Gyro) //����������		
{
	Angle_Z_Final += (Gyro - Q_biasz) * dt; //�������
	
	Pdot[0]=Q_angle - PPZ[0][1] - PPZ[1][0]; // Pk-����������Э�����΢��

	Pdot[1]= - PPZ[1][1];
	Pdot[2]= - PPZ[1][1];
	Pdot[3]=Q_gyro;
	
	PPZ[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���
	PPZ[0][1] += Pdot[1] * dt;   // =����������Э����
	PPZ[1][0] += Pdot[2] * dt;
	PPZ[1][1] += Pdot[3] * dt;
		
	Angle_err = Accel - Angle_Z_Final;	//zk-�������
	
	PCt_0 = C_0 * PPZ[0][0];
	PCt_1 = C_0 * PPZ[1][0];
	
	E = R_angle + C_0 * PCt_0;
	
	K_0 = PCt_0 / E;
	K_1 = PCt_1 / E;
	
	t_0 = PCt_0;
	t_1 = C_0 * PPZ[0][1];


	PPZ[0][0] -= K_0 * t_0;		 //����������Э����
	PPZ[0][1] -= K_0 * t_1;
	PPZ[1][0] -= K_1 * t_0;
	PPZ[1][1] -= K_1 * t_1;
		
	Angle_Z_Final	+= K_0 * Angle_err;	 //�������
	Q_biasz	+= K_1 * Angle_err;	 //�������
	Gyro_z   = Gyro - Q_biasz;	 //���ֵ(�������)��΢��=���ٶ�
}

