/*
��Cost�ṹ��Ķ�ά�����ڴ�ռ��ͷ�
*/
#include"RiskAna.h"

void delete_Risk(Cost **Risk)//���ڼ�ʱ�ͷ������ڴ�ռ�
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

void delete_Risk_xy(Cost **Risk,int x,int y)//�ͷ�Risk[i][j]����Ŀռ�
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