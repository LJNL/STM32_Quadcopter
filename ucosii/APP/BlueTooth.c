#include "includes.h"

//����ͨ���߳� ->��λ��

extern OS_EVENT * PCQ;
extern void *QMessagePC[QSIZE];
extern INT8U errPCQ;

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
