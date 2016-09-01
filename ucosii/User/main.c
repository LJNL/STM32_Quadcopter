#include "includes.h"
#include "MainQ.h"
static OS_STK startup_task_stk[STARTUP_TASK_STK_SIZE]; //����������ջ

int main(void)
{
	BSP_Init(); //�弶֧�ְ���ʼ��
	OSInit();   //����ϵͳ��ʼ��
	
	//��Ϣ����
	MainQ=OSQCreate(QMessageMain,QSIZE);
	PIDQ=OSQCreate(QMessagePID,QSIZE);
	PCQ=OSQCreate(QMessagePC,QSIZE);
	
	OSTaskCreate(Task_Start,(void *)0,&startup_task_stk[STARTUP_TASK_STK_SIZE-1], STARTUP_TASK_PRIO);//��������

	OSStart();  //��������
	
	return 0;
}
