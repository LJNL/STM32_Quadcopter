
#define Kp 100.0f                        // �������������Ǿ���
#define Ki 0.002f                // ���������Ƕȵľ���
#define halfT 0.0005f                // �������ڣ��´ν��������ʱ����

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;          // ��Ԫ����Ԫ�أ�������Ʒ���
float exInt = 0, eyInt = 0, ezInt = 0;        // ��������С�������

float Yaw,Pitch,Roll;  //ƫ���ǣ������ǣ�������

//ֻ�е�λ��Ԫ���ſ��Ա�ʾ��ת������Ϊʲô����Ϊ�������Ԫ����ʾ��ת��Լ��������
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az) {
float norm;
float vx, vy, vz;
float ex, ey, ez;      
   
//��λ�����ٶȼ�
norm = sqrt(ax*ax + ay*ay + az*az);      
ax = ax / norm;
ay = ay / norm;
az = az / norm;

/****
�������ϴμ���õ�����̬����Ԫ��������ɡ��������Ҿ����еĵ����е�����Ԫ�ء�
�������Ҿ����ŷ���ǵĶ��壬��������ϵ������������ת����������ϵ��������������Ԫ�ء�
���������halfvx��halfvy��halfvz��
��ʵ��������һ�η�����������̬����Ԫ��������������ڵ�ǰ�Ļ�������ϵ�ϵ�������λ������
����Ĵ�С�����ݻ����������ȣ����������������ݡ�
***/


// ���Ǹ��ݷ������Ҿ���ĳ����ģ�
//vxyz�����ݻ��ֺ����̬�����������������,
//�ʼ�Ĺ��Ʒ�������� ���Ƶ� q=��1 0 0 0 ������  ����ûת��OK
vx = 2*(q1*q3 - q0*q2);
vy = 2*(q0*q1 + q2*q3);
vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

//�������������  ���������ǲ���������� �� ���ټƲ��������������
ex = (ay*vz - az*vy);
ey = (az*vx - ax*vz);
ez = (ax*vy - ay*vx);

// integral error scaled integral gain �ò���������PI����������ƫ
//����Ĵ�С�����ݻ����������ȣ����������������ݡ�
exInt = exInt + ex*Ki;
eyInt = eyInt + ey*Ki;
ezInt = ezInt + ez*Ki;


//PI��������  KP����+����Ļ������� 
gx = gx + Kp*ex + exInt;
gy = gy + Kp*ey + eyInt;
gz = gz + Kp*ez + ezInt;

//��Ԫ����΢�ַ���
//һ�����������������Ԫ����halfT�����ݲ����ļ�� 
q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  

// ��λ����Ԫ�� ֻ�е�λ�����ܱ�����תOK
norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
q0 = q0 / norm;
q1 = q1 / norm;
q2 = q2 / norm;
q3 = q3 / norm;

//��λ���� �������ڵ�λ����Ԫ���ڿռ���תʱ�ǲ�������ģ�
//��ʽ ���� ����������������� �� �������Ҿ��� �����ģ�����Ԫ����ʾ���£�
Pitch  = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch 
Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
Yaw = atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;  //YAW
}
