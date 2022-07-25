/*
图形界面模块所使用的数据
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

//非法值
#ifndef NOV
#define NOV -1
#endif

//程序所处状态
#ifndef NOTRAVEL
#define NOTRAVEL 0
#endif

#ifndef ROUTE 
#define ROUTE 1
#endif

#ifndef TRAVELLING
#define TRAVELLING 2
#endif

typedef struct city_gui//界面坐标
{
	int x, y;
}CITY_GUI;

extern int STATE;//图形界面模式状态
extern int LIMIT_TIME;//是否限时
extern int t_limit;//限时时间
extern CITY_GUI C[13];//储存各个城市的坐标
extern int Depa;//出发地
extern int Dest;//目的地
extern int T0;//出发时间
extern int CON_RISK;//标志RISK数组是否构造完毕
extern int tra_seq;//标志旅客的数量
extern int inquire;//是否查询
extern PIMAGE map_img;//地图背景
extern PIMAGE inquire_img;//放大镜图片

extern void background();//地图与时钟
extern void notravel_GUI();//未开始旅行模式
extern void init_C();//给城市地图上的坐标赋值
extern void draw_clock(int t, int day);//时钟
extern void route_GUI();//选择路线模式
extern void Inquire();//查询字样

#endif // !_GUI_H_
