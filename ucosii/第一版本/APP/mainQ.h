#ifndef __MAINQ_H__
#define __MAINQ_H__
#include "ucos_ii.h"

/******************��Ϣ����*******************************/
//����������




OS_EVENT * MainQ;
void *QMessageMain[QSIZE];
INT8U errMainQ;

//������̬����
OS_EVENT * PIDQ;
void *QMessagePID[QSIZE];
INT8U errPIDQ;

//����ͨ���߳� ->��λ��
OS_EVENT * PCQ;
void *QMessagePC[QSIZE];
INT8U errPCQ;

#endif