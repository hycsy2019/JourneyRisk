/*
ѡ·���漰����ת��
*/

#include"GUI.h"
#include"RiskAna.h"

int Depa;//������
int Dest;//Ŀ�ĵ�
int T0=-1;//����ʱ��
int t_limit = -1;//��ʱ
TRA *traveller=NULL;//�ÿ�����
int CON_RISK;//�Ƿ��ѹ���RISK����

void draw_arrow(float sx, float sy,float ex, float ey,float r, float len) //����㡢�յ����껭��ͷ
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

void input_box_limit()//������ʱ��t_limit��ֵ
{
	sys_edit editBox;
	const int buffSize = 10;
	char strBuff[10] = "";

	editBox.create(false);
	editBox.size(50, 24 + 8);
	editBox.setfont(24, 0, "��������");
	editBox.move(980, 540);
	editBox.visable(true);
	editBox.setfocus();
	setcolor(BLACK);
	setfont(22, 0, "��������");
	xyprintf(1020, 500, "��������ʱ����λ��Сʱ��");

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
			xyprintf(1000, 600, "ʱ��̫����!\n");
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

void input_box_T0()//�������ʱ�䣬T0��ֵ
{
	sys_edit editBox;
	const int buffSize = 10;
	char strBuff[10] = "";

	editBox.create(false);
	editBox.size(50, 24 + 8);
	editBox.setfont(24, 0, "��������");
	editBox.move(980, 540);
	editBox.visable(true);
	editBox.setfocus();
	setcolor(BLACK);
	setfont(22, 0, "��������");
	xyprintf(1020, 500, "���������ʱ�䣨0~23��");

	for (; is_run(); delay_fps(60)) {
		if (keystate(key_enter))
			break;
	}
	editBox.gettext(buffSize, strBuff);
	sscanf(strBuff, "%d", &T0);
	//xyprintf(1020, 500, "%d", T0);
}

int judge_city(int x, int y)//ͨ�������ڵ�ͼ�ϵ������ж�����������
{
	for (int i = 1; i <= 12; i++)
	{
		if (x >= (C[i].x - 5) && x <= (C[i].x + 5) && y >= (C[i].y - 5) && y <= (C[i].y + 5))
			return i;
	}
	return NOV;
}

void outputRoute()//�Ѵ���㵽�յ��·�߲����������
{
	ROUTES *p = RISK[Depa][Dest].routes;
	int h = 0;
	setfillcolor(WHITE);
	setcolor(WHITE);
	bar(800, 400, 1200, 600);

	for (; ege::is_run()&&STATE==ROUTE; ege::delay_fps(60))
	{
		setcolor(BLACK);
		setfont(18, 0, "����");
		while (p != NULL)
		{
			if (p->route.t0 == p->route.t1) continue;
			switch (p->route.traf)
			{
			case CAR:
				xyprintf(860, 450 + 50 * h, "������"); break;
			case FLIGHT:
				xyprintf(860, 450 + 50 * h, "%d�ź��ࣺ", p->route.seq); break;
			case TRAIN:
				xyprintf(860, 450 + 50 * h, "%d���г���", p->route.seq); break;
			}
			xyprintf(940, 450 + 50 * h, " %d:00   %s", p->route.t0, city[p->route.p0].NAME);
			xyprintf(1050, 450 + 50 * h, "  ��  %d:00   %s", p->route.t1, city[p->route.p1].NAME);
			if (p->next == NULL)  draw_Route(0, p, RISK[Depa][Dest].routes, ARRIVE);
			p = p->next;
			h++;
		}
		setfont(20, 0, "��������");
		setcolor(BLACK);
		xyprintf(920, 400, "��ͷ���·�ߣ�");
		setcolor(RED);
		xyprintf(1000, 620, "���ո����ʼ����");
		if (getch()==32)
		{
			STATE = TRAVELLING;
			break;
		}
	}
}

void input_city()//ʵ���������������ء�Ŀ�ĵ�
{
	int x0, y0, x,y,x1, y1;
	mouse_msg msg = { 0 };

		ege::settextjustify(ege::CENTER_TEXT, ege::CENTER_TEXT);
		ege::setfont(20, 0, "����");
		ege::setbkmode(TRANSPARENT);
		char str[32];
		sprintf(str, "���ڵ�ͼ�е���ó̵�������յ�");
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
							setcolor(EGEARGB(0xff, 178, 34, 34));//��ͷ�ߵ���ɫ
							setfillcolor(EGEARGB(0xff,178,34, 34));//��ͷͷ����ɫ
							setlinewidth(2.0f);//��ͷ�ߵĴ�ϸ
							draw_arrow(x0, y0, x, y,
								(float)(PI / 8), 0.2f);//����յ�����꣬��ͷͷ�Ĵ�С������
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

void draw_traf_sche()//�Ƿ��ǽ�ͨ���߷���ѡ�ť
{
	setfillcolor(RGB(181, 228, 255));
	bar(900, 360, 1100, 390);
	setfillcolor(RGB(144, 238, 144));
	bar(900, 400, 1100, 430);
	ege::settextjustify(ege::CENTER_TEXT, ege::CENTER_TEXT);
	ege::setfont(20, 0, "��������");
	ege::setbkmode(TRANSPARENT);
	char str[20];
	sprintf(str, "���ǽ�ͨ���߷���");
	ege::setcolor(RGB(30, 105, 210));
	ege::outtextxy(1000, 375, str);
	sprintf(str, "���Խ�ͨ���߷���");
	ege::setcolor(RGB(87, 139, 46));
	ege::outtextxy(1000, 415, str);
}

void sche_traf_risk()//�������ѡ���ж��Ƿ��ǽ�ͨ���߷���
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

void route_GUI()//ѡ·�����߼�ģ�飬�ƽ�����
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
