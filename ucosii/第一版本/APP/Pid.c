#include "includes.h"

//������̬����

extern OS_EVENT * PIDQ;
extern void *QMessagePID[QSIZE];
extern INT8U errPIDQ;

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
