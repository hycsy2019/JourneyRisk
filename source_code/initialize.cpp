/*
��ʼ���������������������Ϣ��ʱ�̱�
*/
#include"RiskAna.h"

int city_number;//���е�����
int flight_number;//���������
int train_number;//�𳵵�����
struct seq ***flight_seq;//�洢����֮��ĺ����,�����������б�ţ���ȡ�����ĺ����
struct seq ***train_seq;//�洢����֮��Ļ𳵺�,�洢����֮��ĺ����,�����������б�ţ���ȡ�������г���
int **car_t;//����֮������������̵�ʱ��

struct schedule *flight;//����ʱ�̱�
struct schedule *train;//�г�ʱ�̱�
struct City *city;//��������

void New(int ncity,int nflight,int ntrain )//�����������洢�ռ�
{
	int i;

	city = new struct City[ncity+1];

	//�ȳ�ʼ������֮�亽��š��𳵺Ŷ�Ϊ0���޺���𳵣�

	flight = new struct schedule[nflight+1];
	train = new struct schedule[ntrain+1];

	car_t = new int*[ncity+1];
	for (i = 0; i <= ncity; i++) car_t[i] = new int[ncity+1];

	for (i = 0; i <= city_number; i++)
	{
		memset(car_t[i], MAXTIME, sizeof(car_t[i]));
	}//����֮��������ʱ����󣨲��ɴ

}

struct seq* addNodeSeq(struct seq *tracSeq,int seq)//��������ʽ�洢ʱ�̱�
{
	struct seq *cur_p = NULL;

	if (tracSeq!=NULL && tracSeq->seq == 0)
	{
		tracSeq->seq = seq;
		cur_p = tracSeq;
	}
	else
	{
		//ͷ�巨
		cur_p= new struct seq;
		cur_p->seq = seq;
		cur_p->next = tracSeq;
	}
	return cur_p;
}

void  init_flight(FILE *fp)//���뺽��ʱ�̱�
{
    int seq;
	int p0, p1;//�������
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
		//fgets(str,MAXLINE,fp);//��ȡ����
		//��ȡ����š�����ʱ�䡢���
		fscanf_s(fp, "%d",& seq);
		fscanf_s(fp, "%d%d", &flight[seq].time, &flight[seq].shift);

		flight[seq].route = new struct Route[flight[seq].shift];
		for (int j = 0; j < flight[seq].shift; j++)
		{
			//��ȡ�����͵���ʱ��
			fscanf_s(fp, "%d", &flight[seq].route[j].t0);
			fgetc(fp);//������-������
			fscanf_s(fp, "%d", &flight[seq].route[j].t1);

			//��ȡ�����͵������
			fscanf_s(fp, "%d", &p0);
			flight[seq].route[j].p0 = p0;
			fscanf_s(fp, "%d", &p1);
			flight[seq].route[j].p1 = p1;
			flight_seq[p1][p0]=addNodeSeq(flight_seq[p1][p0],seq);
			flight_seq[p0][p1] = flight_seq[p1][p0];
		}
	}
}

void  init_train(FILE *fp)//�����ʱ�̱�
{
	int seq;
	char str[MAXLINE];
	int p0, p1;//�������
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
		fgets(str, MAXLINE, fp);//��ȡ����
		//��ȡ�г��š�����ʱ�䡢���
		fscanf_s(fp, "%d", &seq);
		fscanf_s(fp, "%d%d", &train[seq].time, &train[seq].shift);

		train[seq].route = new struct Route[train[seq].shift];
		for (int j = 0; j < train[seq].shift; j++)
		{
			//��ȡ�����͵���ʱ��
			fscanf_s(fp, "%d", &train[seq].route[j].t0);
			fgetc(fp);//������-������
			fscanf_s(fp, "%d", &train[seq].route[j].t1);

			//��ȡ�����͵������
			fscanf_s(fp, "%d", &p0);
			train[seq].route[j].p0 = p0;
			fscanf_s(fp, "%d", &p1);
			train[seq].route[j].p1 =p1;
			train_seq[p1][p0] = addNodeSeq(train_seq[p1][p0], seq);
			train_seq[p0][p1] = train_seq[p1][p0];
		}
	}
}

void initialize()//����city.txt���ݸ�ʽ������Ϣ
{
	FILE *fp = NULL;
	char str[MAXLINE];

	errno_t err;
	if ((err = fopen_s(&fp, "city.txt", "r")) != 0)
		printf("The file 'city.txt' was not opened\n");

	fscanf_s(fp, "%d%d%d", &city_number,&flight_number,&train_number);
	New(city_number,flight_number, train_number); // �����������洢�ռ�
	fgets(str, MAXLINE, fp);//��ȡ����

	//��ȡ�����ļ����и�ֵ
	for (int i = 0; i < city_number; i++)
	{
		int seq;
		fscanf_s(fp, "%d",&seq);
		city[seq].seq = seq;//��ȡ���б�ţ�����������Ӧ��λ����
		fscanf_s(fp, "%s", city[seq].NAME, sizeof(city[seq].NAME));//��ȡ��������
		fscanf_s(fp,"%f\n", &city[seq].risk);//��ȡ����ֵ
		//printf("%f ", city[seq].risk);
	}

	init_flight(fp);//��ȡ����

	init_train(fp);//��ȡ�г�
	
	fgets(str, MAXLINE, fp);
	//��ȡ����ʱ��
	for (int i = city_number;i>1; i--)
	{
		for (int j = 1; j < i; j++)
		{
			fscanf_s(fp, "%d", &car_t[i][j]);
			//�ɶԳ��Ը�ֵ�����������
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
	printf("������%d\t������%d\t�г���%d\n",city_number,flight_number,train_number);
	for (i = 1; i <= city_number; i++)  printf("%d\t%s\t%2f\n", city[i].seq,city[i].NAME,city[i].risk);
	for (i = 1; i <= flight_number; i++)
	{
		printf("%d�ź��ࣺ\n", i);
		for (j = 0; j < flight[i].shift; j++) printf("%d��  %s �� %d��  %s\n", 
			flight[i].route[j].t0, flight[i].route[j].p0.NAME, flight[i].route[j].t1, flight[i].route[j].p1.NAME);
		printf("\n");
	}
	printf("\n");
	for (i = 1; i <= train_number; i++)
	{
		printf("%d���г���\n", i);
		for (j = 0; j < train[i].shift; j++) printf("%d��  %s �� %d��  %s\n",
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