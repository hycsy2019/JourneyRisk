/*
将Cost结构体的二维数组内存空间释放
*/
#include"RiskAna.h"

void delete_Risk(Cost **Risk)//用于及时释放数组内存空间
{
	int i, j;
	ROUTES *p;
	ROUTES *pre_p;
	for (i = 1; i <= city_number; i++)
	{
		for (j = 1; j <= city_number; j++)
		{
			p = Risk[i][j].routes;
			while (p != NULL)
			{
				pre_p = p;
				p = p->next;
				free(pre_p);
			}
		}
		delete[] Risk[i];
	}
	delete Risk;
}

void delete_Risk_xy(Cost **Risk,int x,int y)//释放Risk[i][j]以外的空间
{
	int i, j;
	ROUTES *p;
	ROUTES *pre_p;
	for (i = 1; i <= city_number; i++)
	{
		for (j = 1; j <= city_number&&j!=y; j++)
		{
			p = Risk[i][j].routes;
			while (p != NULL)
			{
				pre_p = p;
				p = p->next;
				delete pre_p;
			}
		}
		if(i!=x)  delete[] Risk[i];
	}
}