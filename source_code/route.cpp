/*
不限时最低风险策略路线生成
*/

#include"RiskAna.h"

Cost **RISK;//风险量化数组
int Traf_risk=-1;//标志是否考虑交通工具风险

Cost** init_cost()//给Cost结构体二维数组分配空间
{
	Cost **cost;
	cost = new Cost*[city_number + 1];
	for (int i = 1; i <= city_number; i++)
	{
		cost[i] = new Cost[city_number + 1];
	}
	return cost;
}

Cost minWait(int t0,int p0,int p1)//找到等待时间最少的航班或列车
{
	Cost sche;
	int t;
	int flag = 0;
	struct seq *ptr;
	sche.routes = new ROUTES;
	sche.routes->next = NULL;
	sche.routes->route.traf = 0;
	sche.risk = MAXRISK;
	sche.routes->route.seq = 0;

    for (t = 1; t <= 24 && flag!=1; t++)//找到等待时间最短的航班或列车
	{
		for (ptr = flight_seq[p0][p1]; ptr != NULL && ptr->seq!=0 && flag != 1; ptr = ptr->next)
		{
			for (int i = 0; i < flight[ptr->seq].shift; i++)
			{
				if ((t0 + t) % 24 == flight[ptr->seq].route[i].t0 && 
					p0== flight[ptr->seq].route[i].p0 &&
					p1 ==flight[ptr->seq].route[i].p1)
				{
					sche.routes = new ROUTES;
					if (Traf_risk == 0) sche.risk = t * city[p0].risk;
					else sche.risk = t * city[p0].risk + flight[ptr->seq].time * FLIGHT_RISK;
					sche.routes->route= flight[ptr->seq].route[i];
					if (t0 + t >= 24) sche.routes->route.day = '1';
					sche.routes->next = NULL;
					sche.routes->route.traf = FLIGHT;
					sche.routes->route.seq = ptr->seq;
					flag=1; break;
				}
			}
		}
		for (ptr = train_seq[p0][p1]; ptr != NULL && ptr->seq!=0 && flag != 1; ptr = ptr->next)
		 {
				for (int i = 0; i < train[ptr->seq].shift; i++)
				{
					if ((t0 + t) % 24 == train[ptr->seq].route[i].t0 &&
						p0 == train[ptr->seq].route[i].p0 &&
						p1 == train[ptr->seq].route[i].p1 )
					{
						sche.routes = new ROUTES;
						if (Traf_risk == 0) sche.risk = t * city[p0].risk;
						else sche.risk = t * city[p0].risk + train[ptr->seq].time * TRAIN_RISK;
						sche.routes->route = train[ptr->seq].route[i];
						if (t0 + t >= 24) sche.routes->route.day = '1';
						sche.routes->next = NULL;
						sche.routes->route.traf = TRAIN;
						sche.routes->route.seq = ptr->seq;
						flag = 1; break;
					}
				}
			}
	 }
	return sche;
}

Cost** cons_RISK(int T0)//构造两城之间只有直达路线或不可达的RISK二维数组
{
	int i, j,k=0;
	Cost **Risk= init_cost();

	for (i = 1; i <= city_number; i++)
	{
		for (j = 1; j <= city_number; j++)
		{
			if (car_t[i][j] != MAXTIME)//如果汽车可达则乘汽车
			{
				if (Traf_risk == 0)  Risk[i][j].risk = city[i].risk;
				else Risk[i][j].risk = city[i].risk + CAR_RISK* car_t[i][j];
				Risk[i][j].routes = new ROUTES;
				Risk[i][j].routes->route.traf = CAR;
				Risk[i][j].routes->next = NULL;
				Risk[i][j].routes->route.t0 = (T0 + 1)%24;
				if (T0 + 1 + car_t[i][j] >= 24) Risk[i][j].routes->route.day = '1';
				Risk[i][j].routes->route.t1 = (T0 + 1+car_t[i][j])%24;
				Risk[i][j].routes->route.p0 = i;
				Risk[i][j].routes->route.p1 = j;
			}
			else if (minWait(T0, i, j).routes->route.traf != 0)
			{
				Risk[i][j] = minWait(T0, i, j);
			}
			else
			{
				Risk[i][j].risk = MAXRISK;
				Risk[i][j].routes = new ROUTES;
				Risk[i][j].routes->route.traf = 0;
				Risk[i][j].routes->route.seq = 0;
				Risk[i][j].routes->next = NULL;
			}
		}
	}
	return Risk;
}

ROUTES *addNodeRou(ROUTES *rou1,ROUTES *rou2)//将两条路线相接，更新相关的RISK元素值
{
	ROUTES *cur_p = rou1;
	ROUTES *newrou =  new ROUTES;
	newrou->next = NULL;
	ROUTES *head = newrou;

	while (cur_p != NULL)
	{
		newrou->route.day = cur_p->route.day;
		newrou->route.p0 = cur_p->route.p0;
		newrou->route.p1 = cur_p->route.p1;
		newrou->route.t0 = cur_p->route.t0;
		newrou->route.t1 = cur_p->route.t1;
		newrou->route.seq = cur_p->route.seq;
		newrou->route.traf = cur_p->route.traf;
		cur_p = cur_p->next;
		if (cur_p != NULL)
		{
			newrou->next = new ROUTES;
			newrou = newrou->next;
			newrou->next = NULL;
		}
	}
	newrou->next = rou2;
	return head;
}

Cost** Floyd(int T0,Cost **Risk,int loop)//不考虑时间的弗洛伊德求解最优路径的算法
{
	Cost **Risk_future;
	int t0_future;
	ROUTES *cur_p=NULL;
	for (int k = 1; k <= city_number; k++)//取一个中介
	{
		for (int i = 1; i <= city_number; i++)//遍历起点
		{
			for (int j = 1; j <= city_number; j++)//遍历终点
			{
				cur_p = Risk[i][k].routes;
				while (cur_p->next != NULL) cur_p = cur_p->next;
				t0_future = cur_p->route.t1;
				Risk_future = cons_RISK(t0_future);//假如经过中介节点，计算经过时的风险数组
				//if (loop == 1) return Risk_future;
				//else Risk_future = Floyd(t0_future, Risk_future, loop + 1);
				if (Risk[i][k].risk < MAXRISK&&Risk[k][j].risk < MAXRISK && Risk[i][k].risk + Risk_future[k][j].risk < RISK[i][j].risk)
					//如果 从i到k 和 从k到j 能够走通，且风险更小那么更新最小风险
				{
					Risk[i][j].routes = addNodeRou(Risk[i][k].routes, Risk_future[k][j].routes);
					Risk[i][j].risk = Risk[i][k].risk + Risk_future[k][j].risk;
					delete_Risk_xy(Risk_future, k, j);
				}
				else delete_Risk(Risk_future);
			}
		}
	}
	return Risk;
}

/*
int main()
{
	int i, j;
	ROUTES *cur_p;

	initialize();
	RISK = cons_RISK(0);
	RISK=Floyd(0,RISK,0);

	for (i = 1; i <= city_number; i++)
	{
		for (j = 1; j <= city_number; j++)
		{
			printf("%.2lf\t", RISK[i][j].risk);
		}
		printf("\n");
	}
	
	scanf_s("%d%d", &i,&j);
	cur_p = RISK[i][j].routes;
	//printf("%s  %s\n", city[i].NAME, city[j].NAME);


	while (cur_p != NULL)
	{
		switch (cur_p->route.traf)
		{
		case CAR:
			printf("汽车："); break;
		case FLIGHT:
			printf("%d号航班：", cur_p->route.seq); break;
		case TRAIN:
			printf("%d号列车：", cur_p->route.seq); break;
		}
		printf("%d   %s", cur_p->route.t0, cur_p->route.p0.NAME);
		printf("   →   %d   %s\n", cur_p->route.t1, cur_p->route.p1.NAME);
		cur_p = cur_p->next;
	}
}
*/
