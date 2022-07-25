/*
核心模块，按顺序组织其他模块
*/
#include"GUI.h"
#include"RiskAna.h"

PIMAGE map_img;//背景地图
int tra_seq;//当前旅客的序号

int WinMain()//主函数，负责推进状态转换
{
	initialize();
	init_C();
	initgraph(1200, 667);
	setbkcolor(WHITE);
	map_img = newimage(809, 667);
	getimage(map_img, "map.jpg", 0, 0);
	inquire_img = newimage(280, 99);
	getimage(inquire_img, "inquire.jpg", 0, 0);

	for (; ege::is_run(); ege::delay_fps(60))
	{
		cleardevice();
		background();
		switch (STATE)
		{
		case NOTRAVEL:
			notravel_GUI(); break;
		case ROUTE:
			route_GUI(); break;
		case TRAVELLING:
			outputFile(T0, RISK[Depa][Dest].routes, tra_seq);
		}
	}
	delete_Risk(RISK);

	closegraph();
	return 0;
}

