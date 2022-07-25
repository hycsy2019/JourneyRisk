/*
初始化各个变量，读入城市信息、时刻表
*/
#include"RiskAna.h"

int city_number;//城市的数量
int flight_number;//航班的数量
int train_number;//火车的数量
struct seq ***flight_seq;//存储各地之间的航班号,给定两个城市编号，能取出其间的航班号
struct seq ***train_seq;//存储各地之间的火车号,存储各地之间的航班号,给定两个城市编号，能取出其间的列车号
int **car_t;//各地之间乘坐汽车单程的时间

struct schedule *flight;//航班时刻表
struct schedule *train;//列车时刻表
struct City *city;//城市数组

void New(int ncity,int nflight,int ntrain )//给各数组分配存储空间
{
	int i;

	city = new struct City[ncity+1];

	//先初始化各地之间航班号、火车号都为0（无航班火车）

	flight = new struct schedule[nflight+1];
	train = new struct schedule[ntrain+1];

	car_t = new int*[ncity+1];
	for (i = 0; i <= ncity; i++) car_t[i] = new int[ncity+1];

	for (i = 0; i <= city_number; i++)
	{
		memset(car_t[i], MAXTIME, sizeof(car_t[i]));
	}//各地之间汽车用时无穷大（不可达）

}

struct seq* addNodeSeq(struct seq *tracSeq,int seq)//以链表形式存储时刻表
{
	struct seq *cur_p = NULL;

	if (tracSeq!=NULL && tracSeq->seq == 0)
	{
		tracSeq->seq = seq;
		cur_p = tracSeq;
	}
	else
	{
		//头插法
		cur_p= new struct seq;
		cur_p->seq = seq;
		cur_p->next = tracSeq;
	}
	return cur_p;
}

void  init_flight(FILE *fp)//读入航班时刻表
{
    int seq;
	int p0, p1;//城市序号
	int i, j;

	flight_seq = new struct seq**[city_number + 1];
	for (i = 1; i <= city_number; i++)
	{
		flight_seq[i] = new struct seq*[city_number + 1];
		for (j = 1; j <= city_number; j++)
		{
			flight_seq[i][j] = new struct seq;
			flight_seq[i][j]->seq = 0;
			flight_seq[i][j]->next = NULL;
		}
	}

	for (int i = 1; i <= flight_number; i++)
	{
		//fgets(str,MAXLINE,fp);//读取空行
		//读取航班号、单程时间、班次
		fscanf_s(fp, "%d",& seq);
		fscanf_s(fp, "%d%d", &flight[seq].time, &flight[seq].shift);

		flight[seq].route = new struct Route[flight[seq].shift];
		for (int j = 0; j < flight[seq].shift; j++)
		{
			//读取出发和到达时刻
			fscanf_s(fp, "%d", &flight[seq].route[j].t0);
			fgetc(fp);//跳过“-”符号
			fscanf_s(fp, "%d", &flight[seq].route[j].t1);

			//读取出发和到达城市
			fscanf_s(fp, "%d", &p0);
			flight[seq].route[j].p0 = p0;
			fscanf_s(fp, "%d", &p1);
			flight[seq].route[j].p1 = p1;
			flight_seq[p1][p0]=addNodeSeq(flight_seq[p1][p0],seq);
			flight_seq[p0][p1] = flight_seq[p1][p0];
		}
	}
}

void  init_train(FILE *fp)//读入火车时刻表
{
	int seq;
	char str[MAXLINE];
	int p0, p1;//城市序号
	int i, j;

	train_seq = new struct seq**[city_number + 1];
	for (i = 1; i <= city_number; i++)
	{
		train_seq[i] = new struct seq*[city_number + 1];
		for (j = 1; j <= city_number; j++)
		{
			train_seq[i][j] = new struct seq;
			train_seq[i][j]->seq = 0;
			train_seq[i][j]->next = NULL;
		}
	}

	for (int i = 1; i <= train_number; i++)
	{
		fgets(str, MAXLINE, fp);//读取空行
		//读取列车号、单程时间、班次
		fscanf_s(fp, "%d", &seq);
		fscanf_s(fp, "%d%d", &train[seq].time, &train[seq].shift);

		train[seq].route = new struct Route[train[seq].shift];
		for (int j = 0; j < train[seq].shift; j++)
		{
			//读取出发和到达时刻
			fscanf_s(fp, "%d", &train[seq].route[j].t0);
			fgetc(fp);//跳过“-”符号
			fscanf_s(fp, "%d", &train[seq].route[j].t1);

			//读取出发和到达城市
			fscanf_s(fp, "%d", &p0);
			train[seq].route[j].p0 = p0;
			fscanf_s(fp, "%d", &p1);
			train[seq].route[j].p1 =p1;
			train_seq[p1][p0] = addNodeSeq(train_seq[p1][p0], seq);
			train_seq[p0][p1] = train_seq[p1][p0];
		}
	}
}

void initialize()//按照city.txt数据格式读入信息
{
	FILE *fp = NULL;
	char str[MAXLINE];

	errno_t err;
	if ((err = fopen_s(&fp, "city.txt", "r")) != 0)
		printf("The file 'city.txt' was not opened\n");

	fscanf_s(fp, "%d%d%d", &city_number,&flight_number,&train_number);
	New(city_number,flight_number, train_number); // 给各数组分配存储空间
	fgets(str, MAXLINE, fp);//读取空行

	//读取数据文件进行赋值
	for (int i = 0; i < city_number; i++)
	{
		int seq;
		fscanf_s(fp, "%d",&seq);
		city[seq].seq = seq;//读取城市编号，存入数组相应的位置中
		fscanf_s(fp, "%s", city[seq].NAME, sizeof(city[seq].NAME));//读取城市名称
		fscanf_s(fp,"%f\n", &city[seq].risk);//读取风险值
		//printf("%f ", city[seq].risk);
	}

	init_flight(fp);//读取航班

	init_train(fp);//读取列车
	
	fgets(str, MAXLINE, fp);
	//读取汽车时间
	for (int i = city_number;i>1; i--)
	{
		for (int j = 1; j < i; j++)
		{
			fscanf_s(fp, "%d", &car_t[i][j]);
			//由对称性赋值数组的下三角
			if (car_t[i][j] == -1)  car_t[i][j] = MAXTIME;
			car_t[j][i]=car_t[i][j] ;
		}
		car_t[i][i] = 0;
	}
	car_t[1][1] = 0;
	fclose(fp);
}
/*
int main()
{
	initialize();

	int i,j;
	printf("城市数%d\t航班数%d\t列车数%d\n",city_number,flight_number,train_number);
	for (i = 1; i <= city_number; i++)  printf("%d\t%s\t%2f\n", city[i].seq,city[i].NAME,city[i].risk);
	for (i = 1; i <= flight_number; i++)
	{
		printf("%d号航班：\n", i);
		for (j = 0; j < flight[i].shift; j++) printf("%d点  %s → %d点  %s\n", 
			flight[i].route[j].t0, flight[i].route[j].p0.NAME, flight[i].route[j].t1, flight[i].route[j].p1.NAME);
		printf("\n");
	}
	printf("\n");
	for (i = 1; i <= train_number; i++)
	{
		printf("%d号列车：\n", i);
		for (j = 0; j < train[i].shift; j++) printf("%d点  %s → %d点  %s\n",
			train[i].route[j].t0, train[i].route[j].p0.NAME, train[i].route[j].t1, train[i].route[j].p1.NAME);
		printf("\n");
	}
	for (i = 1; i <= city_number-1; i++)
	{
		printf("%15s", city[i].NAME);
	}
	printf("\n");
	for (i = city_number; i >= 1; i--)
	{
		printf("%s", city[i].NAME);
		for (j = 1; j <= city_number - 1; j++)
		{
			if (car_t[i][j] != MAXTIME)
				printf("     %2d  %2d  %2d", flight_seq[i][j]->seq, train_seq[i][j]->seq, car_t[i][j]);
			else printf("     %2d  %2d MAX", flight_seq[i][j]->seq, train_seq[i][j]->seq);
		}
		printf("\n");
	}
}
*/