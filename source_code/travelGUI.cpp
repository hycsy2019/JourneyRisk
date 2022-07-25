/*
旅行界面，负责每一时刻路线的绘制和时钟的更新
*/

#include"GUI.h"
#include"RiskAna.h"

PIMAGE inquire_img;//查询图片
CITY_GUI C[13];//存储每座城市在界面上的坐标
int inquire;//标志是否有查询请求

void init_C()//将城市坐标按城市序号存储为数组
{
	C[1].x = 665; C[1].y = 165;
	C[2].x = 538; C[2].y = 473;
	C[3].x = 420; C[3].y = 441;
	C[4].x = 589; C[4].y = 403;
	C[5].x = 569; C[5].y = 274;
	C[6].x = 642; C[6].y = 391;
	C[7].x = 582; C[7].y = 287;
	C[8].x = 453; C[8].y = 454;
	C[9].x = 616; C[9].y = 528;
	C[10].x = 411; C[10].y = 350;
	C[11].x = 504; C[11].y = 520;
	C[12].x = 659; C[12].y = 196;
}

void draw_Route(int T,ROUTES *p,ROUTES *rou,int state)//根据路线当前节点与时间绘制走过的路
{
	ROUTES *ptr0=rou;
	int x0, y0, x1, y1,t;
	float speedx, speedy;
	setcolor(EGERGB(255, 255, 255));
	setfillcolor(EGERGB(0, 128, 128));
	ege_enable_aa(true);
	while (ptr0 != p)//把之前走过的路画上
	{
		x0 = C[ptr0->route.p0].x;
		y0 = C[ptr0->route.p0].y;
		x1 = C[ptr0->route.p1].x;
		y1 = C[ptr0->route.p1].y;
		t = ptr0->route.t1 - ptr0->route.t0;
		if (t<0) t += 24;
		speedx = (float)(x1 - x0) / t;
		speedy = (float)(y1 - y0) / t;
		for (int i = 0; i < t; i++)   fillellipse(x0 + i * speedx, y0 + i * speedy, 4, 4);
		ptr0 = ptr0->next;
	}
	
	switch (state)
	{
	case WAIT:
		fillellipse(C[p->route.p0].x, C[p->route.p0].y, 4, 4); break;
	
	case TRAVEL:
		x0 = C[p->route.p0].x;
		y0 = C[p->route.p0].y;
		x1 = C[p->route.p1].x;
		y1 = C[p->route.p1].y;
		t = p->route.t1 - p->route.t0;
		if (t < 0) t += 24;
		speedx = (float)(x1 - x0) / t;
		speedy = (float)(y1 - y0) / t;
		T -= p->route.t0;
		if (T < 0) T += 24;
		for (int i = 0; i <= T; i++)   fillellipse(x0 + i * speedx, y0 + i * speedy, 4, 4); break;
	case SWITCH:
	case ARRIVE:
		x0 = C[p->route.p0].x;
		y0 = C[p->route.p0].y;
		x1 = C[p->route.p1].x;
		y1 = C[p->route.p1].y;
		t = p->route.t1 - p->route.t0;
		if (t < 0) t += 24;
		speedx = (float)(x1 - x0) / t;
		speedy = (float)(y1 - y0) / t;
		for (int i = 0; i < t; i++)   fillellipse(x0 + i * speedx, y0 + i * speedy, 4, 4);
		fillellipse(C[p->route.p1].x, C[p->route.p1].y, 4, 4); break;
	}
}

void Inquire()//查询文字提示
{
	putimage(860, 500, inquire_img);
	ege::settextjustify(ege::CENTER_TEXT, ege::CENTER_TEXT);
	ege::setfont(28, 0, "方正舒体");
	ege::setbkmode(TRANSPARENT);
	char str[22];
	ege::setcolor(BLACK);
	//setfillcolor(EGERGB(105,105,105));
	if(inquire==0)  sprintf(str, "按任意键查询");
	else sprintf(str, "按任意键继续");
	//bar(970, 545, 1050, 590);
	outtextxy(1010, 565, str);
}

void travel_GUI(int t,ROUTES *p,ROUTES *rou,int state,int day)//检查键盘消息，绘制路线、时钟
{
	draw_Route(t,p,rou,state);
	draw_clock(t, day);
	if (kbhit())
		inquire = 1;//查询
	else inquire = 0;//不查询
}

/*
int main()
{
	int t;
	ROUTES *rou=new ROUTES;
	ROUTES *p=rou;
	initgraph(1200, 667);
	setbkcolor(WHITE);
	initialize();
	init_C();
	
	p->route.p0 = city[1].seq;
	p->route.p1 = city[12].seq;
	p->route.t0 = 5;
	p->route.t1 = 7;
	
	p->next = new ROUTES;
	p = p->next;
	p->route.p0 = city[12].seq;
	p->route.p1 = city[5].seq;
	p->route.t0 = 8;
	p->route.t1 = 11;

	p->next = new ROUTES;
	p = p->next;
	p->route.p0 = city[5].seq;
	p->route.p1 = city[4].seq;
	p->route.t0 = 13;
	p->route.t1 = 17;
	
	p = rou;

	for (t = 4; t <= 17; t++)
	{
		for (; ege::is_run(); ege::delay_fps(60))
		{
			cleardevice();
			background();
			if (t < 5)  travel_GUI(t, p, rou, WAIT, 2);
			if (t >= 5 && t < 7) travel_GUI(t, p, rou, TRAVEL, 2);
			if (t == 7)  travel_GUI(t, p, rou, SWITCH, 2);
			if(t==8) p = p->next;
			if (t >= 8 && t < 11)
			travel_GUI(t, p, rou, TRAVEL, 2);
			if(t>=11&&t<13)	travel_GUI(t, p, rou, SWITCH, 2);
			if(t==13) p = p->next;
			if (t >=13 && t < 17)
			travel_GUI(t, p, rou, TRAVEL, 2);
			if (t == 17)  travel_GUI(t, p, rou, ARRIVE, 2);
			Sleep(2000); break;
		}
		
	}
}
*/
