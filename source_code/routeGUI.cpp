/*
选路界面及输入转化
*/

#include"GUI.h"
#include"RiskAna.h"

int Depa;//出发地
int Dest;//目的地
int T0=-1;//出发时间
int t_limit = -1;//限时
TRA *traveller=NULL;//旅客链表
int CON_RISK;//是否已构造RISK数组

void draw_arrow(float sx, float sy,float ex, float ey,float r, float len) //由起点、终点坐标画箭头
{
	float c = cos(r), s = sin(r);
	float dx = sx - ex, dy = sy - ey;
	ege_line(sx, sy, ex, ey);
	ege_point points[3] = {
		ex, ey,
		len * (dx * c + dy * s) + ex,
		len * (-dx * s + dy * c) + ey,
		len * (dx * c - dy * s) + ex,
		len * (dx * s + dy * c) + ey
	};
	ege_fillpoly(3, points);
}

void input_box_limit()//输入限时，t_limit赋值
{
	sys_edit editBox;
	const int buffSize = 10;
	char strBuff[10] = "";

	editBox.create(false);
	editBox.size(50, 24 + 8);
	editBox.setfont(24, 0, "方正舒体");
	editBox.move(980, 540);
	editBox.visable(true);
	editBox.setfocus();
	setcolor(BLACK);
	setfont(22, 0, "方正舒体");
	xyprintf(1020, 500, "请输入限时（单位：小时）");

	for (; is_run(); delay_fps(60)) {
		if (keystate(key_enter))
			break;
	}
	editBox.gettext(buffSize, strBuff);
	sscanf(strBuff, "%d", &t_limit);
	if (CON_RISK != 1)
	{
		RISK = cons_RISK_time(T0, t_limit);
		for (int i = 1; i <= 3; i++)
			RISK = Floyd_time(T0, RISK, t_limit, 0);
		if (RISK[Depa][Dest].risk == MAXRISK)
		{
			xyprintf(1000, 600, "时间太短啦!\n");
			Sleep(3000);
			delete_Risk(RISK);
			CON_RISK = 0;
		}
		else
		{
			CON_RISK = 1;
		}
	}
}

void input_box_T0()//输入出发时间，T0赋值
{
	sys_edit editBox;
	const int buffSize = 10;
	char strBuff[10] = "";

	editBox.create(false);
	editBox.size(50, 24 + 8);
	editBox.setfont(24, 0, "方正舒体");
	editBox.move(980, 540);
	editBox.visable(true);
	editBox.setfocus();
	setcolor(BLACK);
	setfont(22, 0, "方正舒体");
	xyprintf(1020, 500, "请输入出发时间（0~23）");

	for (; is_run(); delay_fps(60)) {
		if (keystate(key_enter))
			break;
	}
	editBox.gettext(buffSize, strBuff);
	sscanf(strBuff, "%d", &T0);
	//xyprintf(1020, 500, "%d", T0);
}

int judge_city(int x, int y)//通过输入在地图上的坐标判断是哪座城市
{
	for (int i = 1; i <= 12; i++)
	{
		if (x >= (C[i].x - 5) && x <= (C[i].x + 5) && y >= (C[i].y - 5) && y <= (C[i].y + 5))
			return i;
	}
	return NOV;
}

void outputRoute()//把从起点到终点的路线策略完整输出
{
	ROUTES *p = RISK[Depa][Dest].routes;
	int h = 0;
	setfillcolor(WHITE);
	setcolor(WHITE);
	bar(800, 400, 1200, 600);

	for (; ege::is_run()&&STATE==ROUTE; ege::delay_fps(60))
	{
		setcolor(BLACK);
		setfont(18, 0, "等线");
		while (p != NULL)
		{
			if (p->route.t0 == p->route.t1) continue;
			switch (p->route.traf)
			{
			case CAR:
				xyprintf(860, 450 + 50 * h, "汽车："); break;
			case FLIGHT:
				xyprintf(860, 450 + 50 * h, "%d号航班：", p->route.seq); break;
			case TRAIN:
				xyprintf(860, 450 + 50 * h, "%d号列车：", p->route.seq); break;
			}
			xyprintf(940, 450 + 50 * h, " %d:00   %s", p->route.t0, city[p->route.p0].NAME);
			xyprintf(1050, 450 + 50 * h, "  →  %d:00   %s", p->route.t1, city[p->route.p1].NAME);
			if (p->next == NULL)  draw_Route(0, p, RISK[Depa][Dest].routes, ARRIVE);
			p = p->next;
			h++;
		}
		setfont(20, 0, "方正舒体");
		setcolor(BLACK);
		xyprintf(920, 400, "最低风险路线：");
		setcolor(RED);
		xyprintf(1000, 620, "按空格键开始旅行");
		if (getch()==32)
		{
			STATE = TRAVELLING;
			break;
		}
	}
}

void input_city()//实现鼠标点击输入出发地、目的地
{
	int x0, y0, x,y,x1, y1;
	mouse_msg msg = { 0 };

		ege::settextjustify(ege::CENTER_TEXT, ege::CENTER_TEXT);
		ege::setfont(20, 0, "黑体");
		ege::setbkmode(TRANSPARENT);
		char str[32];
		sprintf(str, "请在地图中点击旅程的起点与终点");
		ege::setcolor(RGB(0, 0, 139));
		ege::outtextxy(280, 80, str);
			if (mousemsg())
			{
				msg = getmouse();
				mousepos(&x0, &y0);
				//xyprintf(500, 50, "%d,%d", x0, y0);
				if (msg.is_left())
				{
					//Sleep(3000);
					Depa = judge_city(x0, y0);
				}
				if (Depa > 0)
				{
						for (; ege::is_run() && Dest<=0; ege::delay_fps(60))
						{
							cleardevice();
							background();
							draw_clock(0, 1);
							msg = getmouse();
							mousepos(&x, &y);
							//xyprintf(500, 50, "%d,%d", x, y);
							ege_enable_aa(true);
							setcolor(EGEARGB(0xff, 178, 34, 34));//箭头线的颜色
							setfillcolor(EGEARGB(0xff,178,34, 34));//箭头头的颜色
							setlinewidth(2.0f);//箭头线的粗细
							draw_arrow(x0, y0, x, y,
								(float)(PI / 8), 0.2f);//起点终点的坐标，箭头头的大小、长度
							setcolor(YELLOW);
							circle(x0, y0, 10);
						
							if (msg.is_left())
							{
								mousepos(&x1, &y1);
								Dest = judge_city(x1, y1);
								if (Dest == Depa) 
								{
									Dest = 0;
									continue;
								}
							}
							if (Dest != Depa && Dest > 0)
							{
								circle(x1, y1, 10);
								if (LIMIT_TIME!=1&&CON_RISK!=1)
								{
										RISK = cons_RISK(T0);
										for (int i = 1; i <= 3; i++)
											RISK = Floyd(T0, RISK, 0);
										CON_RISK = 1;
								}
							}
						}
								Sleep(2000);
						
				}
		   }
}

void draw_traf_sche()//是否考虑交通工具风险选项按钮
{
	setfillcolor(RGB(181, 228, 255));
	bar(900, 360, 1100, 390);
	setfillcolor(RGB(144, 238, 144));
	bar(900, 400, 1100, 430);
	ege::settextjustify(ege::CENTER_TEXT, ege::CENTER_TEXT);
	ege::setfont(20, 0, "方正舒体");
	ege::setbkmode(TRANSPARENT);
	char str[20];
	sprintf(str, "考虑交通工具风险");
	ege::setcolor(RGB(30, 105, 210));
	ege::outtextxy(1000, 375, str);
	sprintf(str, "忽略交通工具风险");
	ege::setcolor(RGB(87, 139, 46));
	ege::outtextxy(1000, 415, str);
}

void sche_traf_risk()//由鼠标点击选择判断是否考虑交通工具风险
{
	mouse_msg msg = { 0 };
	int x, y;
	if (traveller == NULL) traveller = new TRA;

	draw_traf_sche();

	for (; ege::is_run() && Traf_risk == -1; ege::delay_fps(60))
	{
		if (mousemsg())
		{
			msg = getmouse();
			if (msg.is_left())
			{
				mousepos(&x, &y);
				if (x >= 850 && x <= 1150 && y >= 355 && y <= 395)
				{
					Traf_risk = 1;
				}
				if (x >= 850 && x <= 1150 && y >= 395 && y <= 435)
				{
					Traf_risk = 0;
				}

			}
		}
	}
}

void route_GUI()//选路核心逻辑模块，推进输入
{
	for (; ege::is_run() && STATE == ROUTE; ege::delay_fps(60))
	{
		cleardevice();
		background();
		draw_clock(0, 1);
		if (Traf_risk == -1)
			sche_traf_risk();
		if (Traf_risk != -1 && T0 == -1)
			input_box_T0();
		if (Traf_risk != -1 && T0 != -1 && (Depa <= 0 || Dest <= 0))
			input_city();
		if (Traf_risk != -1 && LIMIT_TIME == 1 && CON_RISK != 1 && T0 != -1 && Depa > 0 && Dest > 0)
			input_box_limit();
		if (CON_RISK == 1) outputRoute();
	}
}

/*
int main()
{
	initialize();
	init_C();
	LIMIT_TIME = 1;
	CON_RISK = 0;
	initgraph(1200, 667);
	setbkcolor(WHITE);
	STATE = ROUTE;

	for (; ege::is_run() && STATE != TRAVELLING; ege::delay_fps(60))
	{
		cleardevice();
		background();
		draw_clock(0, 1);
		route_GUI(); 
	}

	closegraph();
	return 0;
}
*/
