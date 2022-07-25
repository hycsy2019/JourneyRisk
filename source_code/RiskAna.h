/*
系统路线算法模块所用的数据结构
*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#ifndef _RISKANA_H_
#define _RISKANA_H_

//交通工具标号
#ifndef FLIGHT
#define FLIGHT 1
#endif

#ifndef CAR
#define CAR 2
#endif

#ifndef TRAIN
#define TRAIN 3
#endif

//旅客状态
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
#define SWITCH 4//转乘
#endif

//交通工具风险值
#ifndef CAR_RISK
#define CAR_RISK 2
#endif

#ifndef TRAIN_RISK
#define TRAIN_RISK 5
#endif

#ifndef FLIGHT_RISK
#define FLIGHT_RISK 9
#endif

//定义无穷大（时间、距离、风险）
#ifndef MAXTIME
#define MAXTIME 10000
#endif

#ifndef MAXLINE
#define MAXLINE 1024
#endif

#ifndef MAXRISK
#define MAXRISK 1000
#endif

struct City//城市信息
{
	char NAME[50];//城市的名称
	int seq;//城市的序号
	float risk;//城市的风险值
};

struct Route//直达路线
{
	int p0, p1;//起始城市和到达城市
	int t0, t1;//起始时刻和到达时刻
	char day='0';//是否跨天
	char traf = 0;//交通工具
	int seq;//所乘坐交通工具的班次号
};

struct schedule//班次时刻表
{
	struct Route *route;//班次的路线及时间
	int shift;//班次数
	int time;//单程时长
};

struct seq//两城间班次号链表
{
	int seq;//交通工具班次号
	struct seq *next=NULL;
};

typedef struct Routes//路线链表（非全部直达）
{
	struct Route route;//包含的单条直达路线
	struct Routes *next = NULL;
}ROUTES;

typedef struct traf_cost//旅程代价结构体，包含风险与路线
{
	float risk;
	ROUTES *routes = NULL;
}Cost;

typedef struct Traveller//旅客链表结构体
{
	int state=0;
	ROUTES *routes=NULL;
	struct Traveller *next=NULL;
}TRA;

extern int city_number;//城市的数量
extern int flight_number;//航班的数量
extern int train_number;//火车的数量
extern struct seq ***flight_seq;//存储各地之间的航班号,给定两个城市编号，能取出其间的航班号
extern struct seq ***train_seq;//存储各地之间的火车号,存储各地之间的航班号,给定两个城市编号，能取出其间的列车号
extern int **car_t;//各地之间乘坐汽车单程的时间
extern struct schedule *flight;//航班时刻表
extern struct schedule *train;//列车时刻表
extern struct City *city;//城市数组
extern Cost **RISK;//风险量化数组
extern TRA *traveller;//旅客链表
extern int Traf_risk;//标志是否考虑交通工具风险

extern void initialize();//读取数据文件，初始化各个变量
extern ROUTES *addNodeRou(ROUTES *rou1, ROUTES *rou2);//更新路线
extern Cost** cons_RISK(int T0);//构造风险数组
extern Cost** Floyd(int T0, Cost **Risk,int loop);//风险数组求最小风险路径算法
extern Cost** init_cost();//初始化风险数组
extern Cost minWait(int t0, int p0, int p1);//选择等待时间最短的策略
extern Cost** cons_RISK_time(int T0, int limit_time);//构造限时风险数组
extern Cost** Floyd_time(int T0, Cost **Risk, int limit_time, int loop);//限时风险数组求最小风险路径算法
extern int routeTime(int t0, ROUTES *rou);//返回从起始时间开始到路线结束总共耗时
extern void outputFile(int t0, ROUTES *rou, int tra_seq);//将旅客状态输出到文件和屏幕上
extern void travel_GUI(int t, ROUTES *p, ROUTES *rou, int state, int day);//在地图上输出路线
extern void delete_Risk(Cost **Risk);//释放数组空间
extern void delete_Risk_xy(Cost **Risk, int x, int y);//释放Risk[i][j]以外的空间
extern void draw_Route(int T, ROUTES *p, ROUTES *rou, int state);//在地图中生成T时刻的路线
#endif // !RISKANA H
