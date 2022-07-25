/*
限时最低风险策略路线生成
*/

#include"RiskAna.h"

int routeTime(int t0,ROUTES *rou)//返回从起始时间开始到路线结束总共耗时
{
	ROUTES *cur_p = rou;
	int time=0;

	if ((cur_p->route.traf == FLIGHT || cur_p->route.traf == TRAIN) && cur_p->route.t0<t0)  time += 24 - t0 + cur_p->route.t0;
	else time += cur_p->route.t0 - t0;

	while (cur_p != NULL)
	{
		if (cur_p->route.traf == CAR && cur_p->route.day=='1')   time += 24 - cur_p->route.t0 + cur_p->route.t1;
		else time += cur_p->route.t1 - cur_p->route.t0;
		cur_p = cur_p->next; 
	}
	return time;
}

Cost minWait_time(int t0, int p0, int p1,int limit_time)//返回等待时间最少的航班或列车，且等待时间加上车程不超过限制时间
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

	for (t = 1; t <= 24 && flag != 1; t++)//找到等待时间最短且未超出限时的航班或列车
	{
		for (ptr = flight_seq[p0][p1]; ptr != NULL && ptr->seq != 0 
			&& t+flight[ptr->seq].time<limit_time &&flag!=1; ptr = ptr->next)
		{
			for (int i = 0; i < flight[ptr->seq].shift; i++)
			{
				if ((t0 + t) % 24 == flight[ptr->seq].route[i].t0 &&
					p0 == flight[ptr->seq].route[i].p0 &&
					p1 == flight[ptr->seq].route[i].p1)
				{
					sche.routes = new ROUTES;
					if (Traf_risk == 0) sche.risk = t * city[p0].risk;
					else sche.risk = t * city[p0].risk + flight[ptr->seq].time * FLIGHT_RISK;
					sche.routes->route = flight[ptr->seq].route[i];
					if (t0 + t >= 24) sche.routes->route.day = '1';
					sche.routes->next = NULL;
					sche.routes->route.traf = FLIGHT;
					sche.routes->route.seq = ptr->seq;
					flag = 1; break;
				}
			}
		}
		for (ptr = train_seq[p0][p1]; ptr != NULL && ptr->seq != 0 && 
			t + train[ptr->seq].time < limit_time&&flag != 1; ptr = ptr->next)
		{
			for (int i = 0; i < train[ptr->seq].shift; i++)
			{
				if ((t0 + t) % 24 == train[ptr->seq].route[i].t0 &&
					p0 == train[ptr->seq].route[i].p0 &&
					p1 ==train[ptr->seq].route[i].p1)
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

Cost** cons_RISK_time(int T0,int limit_time)//构造直达或不可达的RISK数组，若超时则视为不可达
{
	int i, j, k = 0;
	Cost **Risk = init_cost();
	int flag;
	for (i = 1; i <= city_number; i++)
	{
		for (j = 1; j <= city_number; j++)
		{
			if (i == 1 && j == 6 )
				flag = 1;
			if (car_t[i][j] != MAXTIME && car_t[i][j]+1<=limit_time)//如果汽车可达且小于限制时间则乘汽车
			{
				if (Traf_risk == 0)  Risk[i][j].risk = city[i].risk;
				else Risk[i][j].risk = city[i].risk + CAR_RISK * car_t[i][j];
				Risk[i][j].routes = new ROUTES;
				Risk[i][j].routes->route.traf = CAR;
				Risk[i][j].routes->next = NULL;
				Risk[i][j].routes->route.t0 = (T0 + 1) % 24;
				if (T0 + 1 + car_t[i][j] >= 24) Risk[i][j].routes->route.day = '1';
				Risk[i][j].routes->route.t1 = (T0 + 1 + car_t[i][j]) % 24;
				Risk[i][j].routes->route.p0 = i;
				Risk[i][j].routes->route.p1 = j;
				Risk[i][j].routes->route.seq = 0;
			}
			else if (minWait_time(T0, i, j,limit_time).routes->route.traf != 0)
			{
				Risk[i][j] = minWait_time(T0, i, j,limit_time);
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

Cost** Floyd_time(int T0, Cost **Risk,int limit_time,int loop)//排除超时路线的弗洛伊德求解最优路线算法
{
	Cost **Risk_future;
	int t0_future,t_future;
	ROUTES *cur_p = NULL;
	ROUTES *p;

	for (int k = 1; k <= city_number; k++)//取一个中介
	{
		for (int i = 1; i <= city_number; i++)//遍历起点
		{
			for (int j = 1; j <= city_number; j++)//遍历终点
			{
				if (Risk[i][k].risk == MAXRISK || Risk[k][j].risk == MAXRISK) continue;
				
				cur_p = Risk[i][k].routes;
				while (cur_p->next != NULL) cur_p = cur_p->next;
				t0_future = cur_p->route.t1;
				Risk_future = cons_RISK_time(t0_future,limit_time);//假如经过中介节点，计算经过时的风险数组
				//if (loop == 1) return Risk_future;
				//else Risk_future = Floyd_time(t0_future, Risk_future, limit_time-routeTime(t0_future,Risk[i][k].routes), loop + 1);
				 if( Risk[i][k].risk + Risk_future[k][j].risk < Risk[i][j].risk)
					//如果 从i到k 和 从k到j 能够走通，且风险更小那么更新最小风险
				{
					t_future = routeTime(T0, Risk[i][k].routes) + routeTime(cur_p->route.t1, Risk_future[k][j].routes);
					if (t_future <= limit_time)//判断更新后的路线是否超出限时
					{
						Risk[i][j].routes = addNodeRou(Risk[i][k].routes, Risk_future[k][j].routes);
						p = Risk[i][j].routes;
						while (p != NULL)
						{
							switch (p->route.traf)
							{
							case CAR:
								printf("汽车："); break;
							case FLIGHT:
								printf("%d号航班：", p->route.seq); break;
							case TRAIN:
								printf("%d号列车：", p->route.seq); break;
							}
							printf("%d   %s", p->route.t0, city[p->route.p0].NAME);
							printf("   →   %d   %s\n", p->route.t1,city[p->route.p1].NAME);
							p = p->next;
						}
						printf("\n");
						Risk[i][j].risk = Risk[i][k].risk + Risk_future[k][j].risk;
						delete_Risk_xy(Risk_future, k, j);
					}
					else delete_Risk(Risk_future);
				}
			}
		}
	}
	return Risk;
}

/*int main()
{
	int i, j;
	ROUTES *cur_p;

	initialize();

	RISK = cons_RISK_time(5,20);
	RISK = Floyd_time(5, RISK, 20,0);

	for (i = 1; i <= city_number; i++)
	{
		for (j = 1; j <= city_number; j++)
		{
			printf("%.2lf\t", RISK[i][j].risk);
		}
		printf("\n");
	}

	scanf_s("%d%d", &i, &j);
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
}*/
