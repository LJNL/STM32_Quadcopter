#include "includes.h"

void BSP_Init()
{
	/* ����ϵͳʱ��Ϊ72M  3.5���������ļ��ѵ���*/
	
	SysTick_init(); /* ��ʼ����ʹ�� SysTick ��ʱ�� */
	//LED_GPIO_Config(); /* LED �˿ڳ�ʼ�� ָʾ����*/
}

void SysTick_init(void)
{
	SysTick_Config(SystemCoreClock/OS_TICKS_PER_SEC);//��ʼ����ʹ�� SysTick ��ʱ�� 3.5��궨�岻ͬ
}