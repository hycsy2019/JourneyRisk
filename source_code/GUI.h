/*
ͼ�ν���ģ����ʹ�õ�����
*/

#include<graphics.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
#include <ege.h>
#include<time.h>
#include <ege/sys_edit.h>
#include<stdio.h>

#ifndef _GUI_H_
#define _GUI_H_

#define for if (0); else for

//�Ƿ�ֵ
#ifndef NOV
#define NOV -1
#endif

//��������״̬
#ifndef NOTRAVEL
#define NOTRAVEL 0
#endif

#ifndef ROUTE 
#define ROUTE 1
#endif

#ifndef TRAVELLING
#define TRAVELLING 2
#endif

typedef struct city_gui//��������
{
	int x, y;
}CITY_GUI;

extern int STATE;//ͼ�ν���ģʽ״̬
extern int LIMIT_TIME;//�Ƿ���ʱ
extern int t_limit;//��ʱʱ��
extern CITY_GUI C[13];//����������е�����
extern int Depa;//������
extern int Dest;//Ŀ�ĵ�
extern int T0;//����ʱ��
extern int CON_RISK;//��־RISK�����Ƿ������
extern int tra_seq;//��־�ÿ͵�����
extern int inquire;//�Ƿ��ѯ
extern PIMAGE map_img;//��ͼ����
extern PIMAGE inquire_img;//�Ŵ�ͼƬ

extern void background();//��ͼ��ʱ��
extern void notravel_GUI();//δ��ʼ����ģʽ
extern void init_C();//�����е�ͼ�ϵ����긳ֵ
extern void draw_clock(int t, int day);//ʱ��
extern void route_GUI();//ѡ��·��ģʽ
extern void Inquire();//��ѯ����

#endif // !_GUI_H_
