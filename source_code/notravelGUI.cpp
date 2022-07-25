/*
未出行状态，程序的初始界面
*/
#include"GUI.h"
#include"RiskAna.h"

int STATE;//当前的操作状态
int t_now = -1;//时钟系统当前时刻
int LIMIT_TIME;//标志是否选择限时策略

ege::ege_point getpos(ege::ege_point center, float rad, float r)//当前时钟指针的坐标
{
	ege::ege_point pos;
	pos.x = sin(rad) * r + center.x;
	pos.y = -cos(rad) * r + center.y;
	return pos;
}

void draw_clock(int t,int day)//由时刻与天数画时钟
{
	float pi2 = ege::PI * 2;
	ege::ege_point center;
	center.x = 1000, center.y = 200;
	float r = 100;
	ege::settextjustify(ege::CENTER_TEXT, ege::CENTER_TEXT);
	ege::setfont(20, 0, "方正舒体");
	ege::setbkmode(TRANSPARENT);

	ege::ege_enable_aa(true);
	ege::setfillcolor(BLACK);
	ege::setcolor(BLACK);
	ege::ege_fillellipse(center.x - r * 1.2f, center.y - r * 1.2f,
		r * 1.2f * 2.0f, r * 1.2f * 2.0f);

	ege::setcolor(ege::BLACK);
	for (int num = 1; num <= 12; ++num)
	{
		char str[8];
		ege::ege_point p = getpos(center, float(num * pi2 / 12), r);
		sprintf(str, "%d", num);
		ege::outtextxy((int)p.x, (int)p.y, str);
	}
	
	ege::setcolor(EGEARGB(0xff, 0xff, 0, 0));
	ege::setfillcolor(EGEARGB(0xff, 0, 0, 0));
	ege::setlinewidth(1.0f);
	{
		ege::ege_point p = getpos(center, float(t * pi2 / 12), r * 0.5f);
		ege::ege_line(p.x, p.y, center.x, center.y);
		ege::ege_fillellipse(center.x - r * 0.05f, center.y - r * 0.05f,
			r * 0.1f, r * 0.1f);
	}
	{
		char str[7];
		sprintf(str, "第%d天",day);
		ege::setcolor(EGEARGB(0xff, 0, 0, 0));
		ege::outtextxy((int)center.x, (int)(center.y + r * 1.4f), str);
	}
}

void  Start_img()//开始旅程前右下角的按钮
{
	PIMAGE start_img;
	start_img = newimage(280, 99);
		getimage(start_img, "start.jpg", 0, 0);
		putimage(880, 520, start_img);
		ege::settextjustify(ege::CENTER_TEXT, ege::CENTER_TEXT);
		ege::setfont(24, 0, "方正舒体");
		ege::setbkmode(TRANSPARENT);
		char str[22];
		sprintf(str, "点击策略开始新的旅程");
		ege::setcolor(BLACK);
		ege::outtextxy(1060, 630, str);
}

void background()//背景图
{
	putimage(0, 0, map_img);
}

void draw_sche()//策略选择按钮
{
	setfillcolor(RGB(181,228,255));
	bar(900, 450, 1100, 480);
	setfillcolor(RGB(144, 238, 144));
	bar(900,500 , 1100, 530);
	ege::settextjustify(ege::CENTER_TEXT, ege::CENTER_TEXT);
	ege::setfont(20, 0, "方正舒体");
	ege::setbkmode(TRANSPARENT);
	char str[20];
	sprintf(str, "不限时最低风险策略");
	ege::setcolor(RGB(30,105,210));
	ege::outtextxy(1000, 465, str);
	sprintf(str, "限时最低风险策略");
	ege::setcolor(RGB(87, 139, 46));
	ege::outtextxy(1000, 515, str);
}

void Sche()//判断鼠标所选策略，转至下一状态
{
	mouse_msg msg = { 0 };
	int x, y;
	if (traveller == NULL) traveller = new TRA;

	draw_sche();
	
	if (mousemsg())
	{
		msg = getmouse();
		if (msg.is_left())
		{
			mousepos(&x, &y); 
			if (x >= 850 && x <= 1150 && y >= 445 && y <= 485)
			{
				traveller->next = new TRA;
				traveller = traveller->next;
				tra_seq++;
				LIMIT_TIME = 0; 
				STATE = ROUTE;
			}
			if (x >= 850 && x <= 1150 && y >= 490 && y <=535)
			{
				traveller->next = new TRA;
				traveller = traveller->next;
				tra_seq++;
				LIMIT_TIME = 1; 
				STATE = ROUTE;
			}
			
		}
	}
}

void notravel_GUI()//未出行状态程序的界面，并将有关路线选择相关的变量初始化
{
	draw_clock(0, 1);
	Start_img();

	Depa = 0;
	Dest = 0;
	LIMIT_TIME = 0;
	t_limit = 0;
	T0 = -1;
	CON_RISK = 0;

	Sche();
}

/*int main()
{
	initgraph(1200,667 );
	setbkcolor(WHITE);

	for (; ege::is_run()&&STATE!=TRAVELLING; ege::delay_fps(60))
	{
		cleardevice();
		background();
		switch (STATE)
		{
		case NOTRAVEL:
			notravel_state(); break;
			
		case ROUTE:
			route_state(); break;
		}
	}

	closegraph();
	return 0;
}*/
