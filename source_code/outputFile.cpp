/*
时钟模拟系统，路线输出到文件、地图
*/

#include"GUI.h"
#include"RiskAna.h"

ROUTES* travellerState(int t0, int t, ROUTES *rou,int tra_seq)//由出发时间、已过时间，得到当前旅客所处状态和路线链表的节点
{
	ROUTES *p = rou;
	int time=0;
	if (p->route.p0 == p->route.p1) p = p->next;
	int flag=0;
	int T0 = t0;
	while (p != NULL)
	{
		if (p->route.p0!= p->route.p1)
		{
			if (p->route.t0 < t0)  time += 24;
				time+=p->route.t0-t0;
				if (t == 28)
					flag = 1;
			if (time > t)
			{
				traveller[tra_seq].state = WAIT;
				return p;
			}

			if (p->route.t1 < p->route.t0)  time += 24;
			time += p->route.t1 - p->route.t0;
			if (time == t&&p->next!=NULL)
			{
				traveller[tra_seq].state = SWITCH;
				return p;
			}

			if (time > t)
			{
				traveller[tra_seq].state = TRAVEL;
				return p;
			}
		}
		t0 = (T0 + time) % 24;
		p = p->next;
	} 
	traveller[tra_seq].state = ARRIVE;
	return p;
}

void newJourney()//判断空格是否按下，从而转换到NOTRAVEL状态
{
	for (; ege::is_run() && STATE == TRAVELLING; ege::delay_fps(60))
	{
		ege::settextjustify(ege::CENTER_TEXT, ege::CENTER_TEXT);
		ege::setfont(26, 0, "方正舒体");
		ege::setbkmode(TRANSPARENT);
		setcolor(GREEN);
		xyprintf(1000, 550, "按下空格开始新的旅程");
		if (keystate(32)) STATE = NOTRAVEL;//敲击空格开始新的旅程
	}
}

void outputFile(int t0,ROUTES *rou,int tra_seq)//按小时模拟时钟推进，将每一时刻旅客的状态和路线输出到文件、界面
{
	FILE *fp = NULL;
	int t = 0;
	int day;

	errno_t err;
	if ((err = fopen_s(&fp,"journal.txt", "a")) != 0)
		printf("The file 'city.txt' was not opened\n");

	ROUTES *p ;
	ROUTES *pre_p=NULL;
	fprintf(fp, "旅客%d的状态:\n", tra_seq);
	for (; ege::is_run() && STATE==TRAVELLING; ege::delay_fps(60))
	{
		cleardevice();
		background();
		Inquire();
		p = travellerState(t0, t, rou, tra_seq);
			for (; ege::is_run() && traveller[tra_seq].state != ARRIVE; ege::delay_fps(60))
			{
				day = 2 + (t + t0) / 24;
				cleardevice();
				background();
				Inquire();
				travel_GUI((t + t0) % 24, p, rou, traveller[tra_seq].state, day);
				fprintf(fp, "第%d天%d点\t ", day, (t + t0) % 24);
				if (traveller[tra_seq].state == WAIT)
				{
					fprintf(fp, "在%s市等待，准备搭乘", city[p->route.p0].NAME);
					switch (p->route.traf)
					{
					case CAR:
						fprintf(fp, "汽车"); break;
					case FLIGHT:
						fprintf(fp, "%d号航班", p->route.seq); break;
					case TRAIN:
						fprintf(fp, "%d号列车", p->route.seq); break;
					}
					fprintf(fp, "\n");
				}
				else if (traveller[tra_seq].state == TRAVEL)
				{
					switch (p->route.traf)
					{
					case CAR:
						fprintf(fp, "在汽车上（"); break;
					case FLIGHT:
						fprintf(fp, "在%d号航班上（", p->route.seq); break;
					case TRAIN:
						fprintf(fp, "在%d号列车上（", p->route.seq); break;
					}
					fprintf(fp, " %d:00   %s", p->route.t0, city[p->route.p0].NAME);
					fprintf(fp, "   →   %d:00   %s）\n", p->route.t1, city[p->route.p1].NAME);
				}
				else if (traveller[tra_seq].state == SWITCH)
				{
					fprintf(fp, "在%s市换乘\n", city[p->route.p1].NAME);
				}

				for (; ege::is_run() && inquire==1; ege::delay_fps(60))
				{
					cleardevice();
					background();
					Inquire();
					travel_GUI((t + t0) % 24, p, rou, traveller[tra_seq].state, day);
					ege::settextjustify(ege::CENTER_TEXT, ege::CENTER_TEXT);
					ege::setfont(20, 0, "方正舒体");
					ege::setbkmode(TRANSPARENT);
					setcolor(RED);
					xyprintf(1000, 460, "旅客%d的状态:\n", tra_seq);
					if (traveller[tra_seq].state == WAIT)
					{
						xyprintf(950, 480, "！在%s市等待，准备搭乘", city[p->route.p0].NAME);
						switch (p->route.traf)
						{
							ege::settextjustify(ege::LEFT_TEXT, ege::LEFT_TEXT);
						case CAR:
							xyprintf(1120, 480, " 汽车！"); break;
						case FLIGHT:
							xyprintf(1120, 480, " %d号航班！", p->route.seq); break;
						case TRAIN:
							xyprintf(1120, 480, " %d号列车！", p->route.seq); break;
						}
						ege::settextjustify(ege::CENTER_TEXT, ege::CENTER_TEXT);
					}
					else if (traveller[tra_seq].state == TRAVEL)
					{
						switch (p->route.traf)
						{
						case CAR:
							xyprintf(850, 480, "！在汽车上（"); break;
						case FLIGHT:
							xyprintf(850, 480, "！在%d号航班上（", p->route.seq); break;
						case TRAIN:
							xyprintf(850, 480, "！在%d号列车上（", p->route.seq); break;
						}
						xyprintf(955, 480, "   %d:00   %s", p->route.t0, city[p->route.p0].NAME);
						xyprintf(1090, 480, "   →   %d:00   %s）！", p->route.t1, city[p->route.p1].NAME);
					}
					else if (traveller[tra_seq].state == SWITCH)
					{
						xyprintf(1000, 480, "！在%s市换乘！", city[p->route.p1].NAME);
					}
					if (getch() == 13)
					{
						inquire = 0;
						setfillcolor(WHITE);
						bar(780, 450, 1200, 510);
					}
				}
				Sleep(10000);
				pre_p = p;
				p = p->next;
				t++;
				p = travellerState(t0, t, rou, tra_seq);
			}
		cleardevice();
		background();
		day = 2 + (t + t0) / 24;
		
		fprintf(fp, "第%d天%d点\t", day, (t + t0) % 24);
		if (p == NULL)
		{
			fprintf(fp, "已到达%s市\n", city[pre_p->route.p1].NAME);
			travel_GUI((t + t0) % 24, pre_p, rou, traveller[tra_seq].state, day);
		}
		else
		{
			travel_GUI((t + t0) % 24, p, rou, traveller[tra_seq].state, day);
			fprintf(fp, "已到达%s市\n", city[p->route.p1].NAME);
		}
		if (inquire == 1)
		{
			if (p == NULL)
				xyprintf(1000, 480, "已到达%s市\n", city[pre_p->route.p1].NAME);
			else xyprintf(1000, 480, "已到达%s市\n", city[p->route.p1].NAME);
		}
		newJourney();
		fclose(fp);
	}
}

/*
int main()
{
	//char ch;
	initialize();
	init_C();
	traveller = new TRA;
	STATE = TRAVELLING;
	/*printf("开始旅行？");
	scanf_s("%c", &ch,1);
	traveller = new TRA;
	while (ch == 'Y')
	{
		tra_seq++;
		printf("起始时间：");
		scanf_s("%d", &t0);
		printf("限时：");
		scanf_s("%d", &limit_time);
		printf("出发地和目的地：");
		scanf_s("%d%d", &i,&j);
		RISK = cons_RISK_time(t0, limit_time);
		for (int i = 1; i <= 5; i++)
			RISK = Floyd_time(t0, RISK, limit_time, 0);
		if (RISK[i][j].routes == NULL) printf("时间太短啦\n");
		else outputFile(t0, RISK[i][j].routes, tra_seq);
		printf("开始旅行？");
		getchar();
		scanf_s("%c", &ch, 1);
		traveller->next = new TRA;
		traveller = traveller->next;
	}
	
	initgraph(1200, 667);
	map_img = newimage(809, 667);
	getimage(map_img, "map.jpg", 0, 0);
	inquire_img = newimage(280, 99);
	getimage(inquire_img, "inquire.jpg", 0, 0);
	setbkcolor(WHITE);
	RISK = cons_RISK_time(5,50);
    for (int i = 1; i <= 5; i++)  RISK = Floyd_time(5, RISK,50,0);
	outputFile(5, RISK[1][10].routes, 1);
}*/
