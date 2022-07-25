/*
ϵͳ·���㷨ģ�����õ����ݽṹ
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#ifndef _RISKANA_H_
#define _RISKANA_H_

//��ͨ���߱��
#ifndef FLIGHT
#define FLIGHT 1
#endif

#ifndef CAR
#define CAR 2
#endif

#ifndef TRAIN
#define TRAIN 3
#endif

//�ÿ�״̬
#ifndef WAIT
#define WAIT 1
#endif

#ifndef TRAVEL 
#define TRAVEL 2
#endif

#ifndef ARRIVE
#define ARRIVE 3
#endif

#ifndef SWITCH
#define SWITCH 4//ת��
#endif

//��ͨ���߷���ֵ
#ifndef CAR_RISK
#define CAR_RISK 2
#endif

#ifndef TRAIN_RISK
#define TRAIN_RISK 5
#endif

#ifndef FLIGHT_RISK
#define FLIGHT_RISK 9
#endif

//���������ʱ�䡢���롢���գ�
#ifndef MAXTIME
#define MAXTIME 10000
#endif

#ifndef MAXLINE
#define MAXLINE 1024
#endif

#ifndef MAXRISK
#define MAXRISK 1000
#endif

struct City//������Ϣ
{
	char NAME[50];//���е�����
	int seq;//���е����
	float risk;//���еķ���ֵ
};

struct Route//ֱ��·��
{
	int p0, p1;//��ʼ���к͵������
	int t0, t1;//��ʼʱ�̺͵���ʱ��
	char day='0';//�Ƿ����
	char traf = 0;//��ͨ����
	int seq;//��������ͨ���ߵİ�κ�
};

struct schedule//���ʱ�̱�
{
	struct Route *route;//��ε�·�߼�ʱ��
	int shift;//�����
	int time;//����ʱ��
};

struct seq//���Ǽ��κ�����
{
	int seq;//��ͨ���߰�κ�
	struct seq *next=NULL;
};

typedef struct Routes//·��������ȫ��ֱ�
{
	struct Route route;//�����ĵ���ֱ��·��
	struct Routes *next = NULL;
}ROUTES;

typedef struct traf_cost//�ó̴��۽ṹ�壬����������·��
{
	float risk;
	ROUTES *routes = NULL;
}Cost;

typedef struct Traveller//�ÿ�����ṹ��
{
	int state=0;
	ROUTES *routes=NULL;
	struct Traveller *next=NULL;
}TRA;

extern int city_number;//���е�����
extern int flight_number;//���������
extern int train_number;//�𳵵�����
extern struct seq ***flight_seq;//�洢����֮��ĺ����,�����������б�ţ���ȡ�����ĺ����
extern struct seq ***train_seq;//�洢����֮��Ļ𳵺�,�洢����֮��ĺ����,�����������б�ţ���ȡ�������г���
extern int **car_t;//����֮������������̵�ʱ��
extern struct schedule *flight;//����ʱ�̱�
extern struct schedule *train;//�г�ʱ�̱�
extern struct City *city;//��������
extern Cost **RISK;//������������
extern TRA *traveller;//�ÿ�����
extern int Traf_risk;//��־�Ƿ��ǽ�ͨ���߷���

extern void initialize();//��ȡ�����ļ�����ʼ����������
extern ROUTES *addNodeRou(ROUTES *rou1, ROUTES *rou2);//����·��
extern Cost** cons_RISK(int T0);//�����������
extern Cost** Floyd(int T0, Cost **Risk,int loop);//������������С����·���㷨
extern Cost** init_cost();//��ʼ����������
extern Cost minWait(int t0, int p0, int p1);//ѡ��ȴ�ʱ����̵Ĳ���
extern Cost** cons_RISK_time(int T0, int limit_time);//������ʱ��������
extern Cost** Floyd_time(int T0, Cost **Risk, int limit_time, int loop);//��ʱ������������С����·���㷨
extern int routeTime(int t0, ROUTES *rou);//���ش���ʼʱ�俪ʼ��·�߽����ܹ���ʱ
extern void outputFile(int t0, ROUTES *rou, int tra_seq);//���ÿ�״̬������ļ�����Ļ��
extern void travel_GUI(int t, ROUTES *p, ROUTES *rou, int state, int day);//�ڵ�ͼ�����·��
extern void delete_Risk(Cost **Risk);//�ͷ�����ռ�
extern void delete_Risk_xy(Cost **Risk, int x, int y);//�ͷ�Risk[i][j]����Ŀռ�
extern void draw_Route(int T, ROUTES *p, ROUTES *rou, int state);//�ڵ�ͼ������Tʱ�̵�·��
#endif // !RISKANA H
