//该程序是rectangleBF的源码
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <math.h>
#include "key-value.h"
#include "std_bf.h"
#include <fstream>
#include "hash_function.h"
#include "PeacockHash.h"
#include "dleftHash.h"
#include "DoubleHash.h"
#include "LinearHash.h"
#include "LinkHash.h"
#include "RTBHash.h"
#include "cuckooHash.h"

#define HT_STAGE_NUM 6

#define HASH_ENTRY_MAX 500000

#include "md5.h"
#include "sha1.h"

using namespace std;


int counter1=0;
int counter2=0;
int counter3=0;

void finetuning_obverse(unsigned long long element,int index,
						unsigned long long M1[],unsigned long long M2[],unsigned long long M3[],
						long M_Level1,long M_Level2,long M_Level3,bool flag_1[],bool flag_3[])
{
	int tmp=element;

	int yushu1=element%M_Level1;
	if (0==M1[yushu1] || element==M1[yushu1])//第一级没有冲突
	{
		M1[yushu1]=element;
	}
	//else if (0==M1[yushu1+1]||element==M1[yushu1+1])//放在邻接位置
	//{
	//	M1[yushu1+1]=element;
	//}

	else
	{
		element=M1[yushu1];
		M1[yushu1]=tmp;


		int yushu2=element%M_Level2;
		if (0==M2[yushu2] || element==M2[yushu2])//第一级冲突，第二级没有冲突
		{
			M2[yushu2]=element;
		}
		//else if (0==M2[yushu2+1]||element==M2[yushu2+1])//放在邻接位置
		//{
		//	M2[yushu2+1]=element;
		//}




		else //两级都冲突
		{

			int yushu3=element%M_Level3;
			if (0==M3[yushu3] || element==M3[yushu3])
			{
				M3[yushu3]=element;
			}
			//else if (0==M3[yushu3+1] || element == M3[yushu3+1])//放在邻接位置
			//{
			//	M3[yushu3+1]=element;
			//}


			else			//三级都冲突
			{
				
				//cout<<++counter3<<"正面微调失败！！！:"<<element<<"<->"<<M3[yushu3]<<endl;
				printf("\r正面微调失败\t%d",counter3++);
				//printf("\n%d:still collision:%u<->%ld\n",++counter,element,M3[yushu3]);
				//system("pause");
			}
		}
	}
}

void sandGlass(unsigned long long element,int index,
			   unsigned long long M1[],unsigned long long M2[],unsigned long long M3[],
			   long M_Level1,long M_Level2,long M_Level3,bool flag_1[],bool flag_3[])
{
	if (element<0)
	{
		printf("\nelement is minus...\n");
		return;
	}


	if (0==index%8 || 1==index%8 || 2==index%8 || 3==index%8)//偶数
	{
		long yushu1=element%M_Level1;
		if (0==M1[yushu1] || element==M1[yushu1])//第一级没有冲突
		{
			M1[yushu1]=element;
		}

		else
		{
			long yushu2=element%M_Level2;
			if (0==M2[yushu2] || element==M2[yushu2])//第二级没有冲突
			{
				M2[yushu2]=element;
			}

			else //两级都冲突
			{
				long yushu3=element%M_Level3;
				if (0==M3[yushu3] || element==M3[yushu3])
				{
					M3[yushu3]=element;
				}

				else			//三级都冲突
				{
					int tmp=M3[yushu3];
					M3[yushu3]=element;
					element=tmp;

					counter1++;
					//cout<<"\neven number---"<<++counter<<"still collision:"<<element<<"<->"<<M3[yushu3]<<endl;
					finetuning_obverse(element,index,M1,M2,M3,M_Level1,M_Level2,M_Level3,flag_1,flag_3);
					
					//system("pause");
				}
			}
		}
	}



	else//奇数
	{
		long yushu3=element%M_Level3;
		if (0==M3[yushu3] || element==M3[yushu3])//第三级没有冲突
		{
			M3[yushu3]=element;
		}

		else
		{
			long yushu2=element%M_Level2;
			if (0==M2[yushu2] || element==M2[yushu2])//第二级没有冲突
			{
				M2[yushu2]=element;
			}

			else
			{
				long yushu1=element%M_Level1;
				if (0==M1[yushu1] || element==M1[yushu1])//第一级没有冲突
				{
					M1[yushu1]=element;
				}

				else			//三级都冲突
				{
					counter2++;
					//cout<<"\nodd number---"<<++counter<<"still collision:"<<element<<"<->"<<M1[yushu1]<<endl;
					finetuning_obverse(element,index,M1,M2,M3,M_Level1,M_Level2,M_Level3,flag_1,flag_3);
					//system("pause");
				}
			}
		}
	}
}

unsigned long long my_hashFunc(char tohash[])
{
	unsigned long long retu=0;

	//MD5 md;
	//md.reset();
	//md.update(tohash);
	//string res=md.toString();
	//
	//for (int i=0;i<8;i++)
	//{
	//	//retu+=(res[i]-'0')*16*i;
	//	if (res[i]<'a')retu+=(res[i]-'0')*pow(16.0,i);
	//	
	//	else
	//		retu+=(res[i]-'a')*pow(16.0,i);
	//}


	return retu;
}

//后面写代码记得写备注，不然再次捡起来的时候会非常痛苦！
//这个代码如果没有输入参数，则加载默认值。主要是支持批处理，输入参数为：元素个数、瘦腰比例、装载因子
//读取的用于hash的是形如rrc*.txt的路由表文件。
//只有屏幕输出
void test(int argc, char* argv[])
{

	//printf("%d-----",'a'-'0');
	int ElementCount;
	double ratio,alph;

	if (argc<3)
	{
		printf("输入为空！加载默认值。格式：元素个数、瘦腰比例、装载因子\n");
		ElementCount=354641;
		ratio=3;
		alph=0.5;
	}
	else
	{
		ElementCount=atoi(argv[1]);
		ratio=atof(argv[2]);
		alph=atof(argv[3]);
	}

	long M_Level2=ElementCount/alph/(2*ratio+1);	//第二级hash的bloom长度
	long M_Level1=M_Level2*ratio+1;				//第1级hash的bloom长度
	long M_Level3=M_Level1-2;					//第三级

	unsigned long long *M1=new unsigned long long[M_Level1+1];
	unsigned long long *M2=new unsigned long long[M_Level2+1];
	unsigned long long *M3=new unsigned long long[M_Level3+1];

	bool *flag_1=new bool[M_Level1+1];
	bool *flag_3=new bool[M_Level3+1];

	for (int i=0;i<M_Level1+1;i++)
	{
		M1[i]=0;
		flag_3[i]=0;
	}
	for (int i=0;i<M_Level2+1;i++)M2[i]=0;
	for (int i=0;i<M_Level3+1;i++)
	{
		M3[i]=0;
		flag_3[i]=0;
	}

	system("dir /B /ON rrc*.txt > fileList");
	//char strName[200];

	FILE * fList=fopen("fileList","r");
	while (!feof(fList)) 
	{
		char filename[200];
		memset(filename,0,sizeof(filename));
		fscanf(fList,"%s",filename);

		if (strlen(filename)<5)continue;

		//FILE * fp=fopen(filename,"r");

		ifstream fin(filename);

		int index=0;
		int jishu_C,oushu_C;
		jishu_C=oushu_C=0;
		while (!fin.eof()) 
		{
			int ip1,ip2,ip3,ip4,maskbit,hop;
			ip1=ip2=ip3=ip4=maskbit=hop=-1;
			//fscanf(fp,"%d.%d.%d.%d\/%d\t%d",&ip1,&ip2,&ip3,&ip4,&maskbit,&hop);
			char str[50];
			memset(str,0,sizeof(str));
			
			//sprintf(str,"%d.%d.%d.%d.%d",&ip1,&ip2,&ip3,&ip4,&maskbit);
			fin>>str>>hop;

			if (-1==maskbit&&-1==hop)continue;
			unsigned long long tohash=ip1*256+ip2+ip3*65536+ip4*16777216;

			//hashing_cone(tohash,++index);

			unsigned long long hash_value=my_hashFunc(str);
			sandGlass(hash_value,++index,M1,M2,M3,M_Level1,M_Level2,M_Level3,flag_1,flag_3);
			//if (0==index%1000)printf(".");
		
			//printf("%d.%d.%d.%d\/%d\t%d\t...%d\n",ip1,ip2,ip3,ip4,maskbit,hop,tohash);
		}
		fin.close();

		printf("\t未微调冲突:\t%d\t%d\n",counter1,counter2);
	}
	fclose(fList);
	

	//printf("程序执行完毕！");
	//system("pause");
}

//hashentry * readHashEntries(int & readline, char *inputfile)
//{
//	hashentry *pHashentries=new hashentry[500000];
//	memset(pHashentries,0,sizeof(hashentry)*500000);
//
//	char *hashfile=inputfile;
//	FILE *fp = fopen(hashfile,"r");
//	if (NULL == fp)
//	{
//		printf("open %s failed", hashfile);
//		return NULL;
//	}
//	while(!feof(fp))
//	{
//		fscanf(fp, "%s %u", pHashentries[readline].key, &(pHashentries[readline].value));
//		if (strlen((const char *)(pHashentries[readline].key))>4)readline++;
//	}
//	fclose(fp);	
//
//    //test the correctness of this function, 读出了不少问题啊! 因为没有初始化
//	//for (int j=300000; j<readline; j++)
//	//{
//	//	printf("%d:\t%s\t%d\n", j, pHashentries[j].key, pHashentries[j].value);
//	//}
//
//	return pHashentries;
//}


//void naiveHash(int factor, FILE * f_out, int iFunc, char *inputfile)
//{
//	int lines=0;
//	int numCollisions=0;
//
//	hashentry * pHashentries=readHashEntries(lines, inputfile);
//
//	int tablelen = (factor*lines)%2==0 ? factor*lines+3 : factor*lines;
//	hashentry * pHashtable=new hashentry[tablelen];
//	
//	for (int k=0; k<lines; k++)
//	{
//		memset(pHashtable[k].key,0,KEY_SIZE);
//		pHashtable[k].value=0;
//	}
//	
//	for (int i=0; i<lines; i++)
//	{
//		int len=strlen((const char *)(pHashentries[i].key));
//		uint pos=hash_func[iFunc]((const unsigned char *)(pHashentries[i].key), len) % (tablelen);
//
//		if (pHashtable[pos].value == 0)
//		{
//			memcpy(pHashtable[pos].key, pHashentries[i].key, len);
//			pHashtable[pos].value=pHashentries[i].value;
//		}
//		else
//		{
//			/*static int i=1;
//			printf("pos=%d\t, here key_value=%s<->%u, there are %d hash collison...\n",pos, pHashentries[i].key, pHashtable[pos].value, i++);*/
//			numCollisions++;
//		}
//	}
//	printf("factor/collisions/ratio:\t%d:\t%d\t%.7f\n",factor, numCollisions, (0.0+numCollisions)/lines); 
//	fprintf(f_out, "%.7f\t", (0.0+numCollisions)/lines);
//
//	delete pHashentries;
//	delete pHashtable;
//}

//void multiHash(float factor, FILE * f_out, int iFunc, char *inputfile, int numStage)
//{
//	int lines=0;
//	int numCollisions=0;
//
//	hashentry * pHashentries=readHashEntries(lines, inputfile);
//
//	//int tablelen = (factor*lines)%2==0 ? factor*lines+3 : factor*lines;
//	int HT_len[HT_STAGE_NUM];
//	float tmp=6/factor+0.0;
//
//	//等长，0.0577
//	//HT_len[0]=lines/tmp-3;
//	//HT_len[1]=lines/tmp-2;
//	//HT_len[2]=lines/tmp-1;
//	//HT_len[3]=lines/tmp;
//	//HT_len[4]=lines/tmp+1;
//	//HT_len[5]=lines/tmp+2;
//
//	//等差长度的更好，哈哈，0.052
//	//HT_len[0]=lines/tmp*0.19*6-3;
//	//HT_len[1]=lines/tmp*0.18*6-2;
//	//HT_len[2]=lines/tmp*0.17*6-1;
//	//HT_len[3]=lines/tmp*0.16*6;
//	//HT_len[4]=lines/tmp*0.155*6+1;
//	//HT_len[5]=lines/tmp*0.145*6+2;
//
//	//等差长度的更好，哈哈，0.0407，至今最好结果
//	//HT_len[0]=lines/tmp*0.30*6-3;
//	//HT_len[1]=lines/tmp*0.24*6-2;
//	//HT_len[2]=lines/tmp*0.18*6-1;
//	//HT_len[3]=lines/tmp*0.12*6;
//	//HT_len[4]=lines/tmp*0.09*6+1;
//	//HT_len[5]=lines/tmp*0.07*6+2;
//
//	//。。。近似伪等差数列,0.03857
//	//HT_len[0]=lines/tmp*0.30*6-3;
//	//HT_len[1]=lines/tmp*0.24*6-2;
//	//HT_len[2]=lines/tmp*0.18*6-1;
//	//HT_len[3]=lines/tmp*0.13*6;
//	//HT_len[4]=lines/tmp*0.09*6+1;
//	//HT_len[5]=lines/tmp*0.06*6+2;
//
//	//。。。严格伪等差数列,0.03839, 至今最好结果
//	HT_len[0]=lines/tmp*0.29583*6-3;
//	HT_len[1]=lines/tmp*0.23083*6-2;
//	HT_len[2]=lines/tmp*0.17583*6-1;
//	HT_len[3]=lines/tmp*0.13083*6;
//	HT_len[4]=lines/tmp*0.09583*6+1;
//	HT_len[5]=lines/tmp*0.07083*6+2;
//	
//	hashentry * pHT[HT_STAGE_NUM];
//	for (int j=0; j<HT_STAGE_NUM; j++)
//	{
//		pHT[j]=new hashentry[HT_len[j]];
//		memset(pHT[j], 0, HT_len[j]*sizeof(hashentry));
//	}
//
//	//to do...
//	//hashentryP * pHT_Tail;
//	//define a load factor for each stage...
//	
//	for (int i=0; i<lines; i++)
//	{
//		int len=strlen((const char *)(pHashentries[i].key));
//		uint hash_value=hash_func[iFunc]((const unsigned char *)(pHashentries[i].key), len);
//		
//		int kk=0;
//		while (kk<6)
//		{
//			uint pos = hash_value%HT_len[kk];
//			if (0==pHT[kk][pos].value)
//			{
//				memcpy(pHT[kk][pos].key, pHashentries[i].key, len);
//				pHT[kk][pos].value=pHashentries[i].value;
//				break;
//			}
//			kk++;
//		}
//
//		if (6==kk)
//		{
//			/*static int i=1;
//			printf("pos=%d\t, here key_value=%s<->%u, there are %d hash collison...\n",pos, pHashentries[i].key, pHashtable[pos].value, i++);*/
//			numCollisions++;
//		}
//	}
//
//	printf("factor/collisions/ratio:\t%f:\t%d\t%.7f\n",factor, numCollisions, (0.0+numCollisions)/lines); 
//	fprintf(f_out, "%.7f\t", (0.0+numCollisions)/lines);
//
//
//	//release the memory...
//	delete pHashentries;
//	for (int jjj=0; jjj<HT_STAGE_NUM; jjj++)
//	{
//		if(pHT[jjj])delete pHT[jjj];
//	}
//}

//void main_hashCollisionTest(int argc, char* argv[])
//{
//	char inputfile[100];
//	memset(inputfile, 0, sizeof(inputfile));
//	if (argc>1)strcpy(inputfile,argv[1]);
//	else       strcpy(inputfile,"rib.txt");
//
//	printf("\n\nStart to process %s...\n", inputfile);
//
//	char outputfile[100];
//	memset(outputfile, 0, sizeof(outputfile));
//	sprintf(outputfile, "%s.stat", inputfile);
//
//	FILE *f_out = fopen(outputfile, "w");
//	fprintf(f_out,"BOB1\tJSHash\tOCaml\tOAAT\tPJWHash\tRSHash\tSDBM\tSimple\tSML\tSTL\tAPHash\tBKDR\tDEKHash\tDJBHash\tFNV32\tHsieh\n");
//	//for (int i=0; i<16; i++)
//	//{
//	//	for (int factor = 2; factor < 16; factor++)
//	//	{
//	//		naiveHash(factor, f_out, i, inputfile);
//	//	}
//	//	fprintf(f_out, "\n");
//	//}
//	//for (int i=0; i<16; i++)
//	{
//		//printf("hash function %d:\n", i);
//		for (float factor = 1; factor < 1.06; factor+=0.01)
//		{
//			multiHash(factor, f_out, 11, inputfile, HT_STAGE_NUM);
//		}
//	}
//	fclose(f_out);
//	
//}

bool linkExperiments(char * fibfile, char * trafficfile, char * updatefile, float ht_times)
{
	try
	{
		int fiblen=getFIBSize(fibfile);
		LHash aHashtable;
		cout << "successful initial" <<endl;

		printf("\n\nStart to process %s...\n", fibfile);

		char outputfileInit[100];
		memset(outputfileInit, 0, sizeof(outputfileInit));
		sprintf(outputfileInit, "%s.link.init", fibfile);

		char outputfileSearch[100];
		memset(outputfileSearch, 0, sizeof(outputfileSearch));
		sprintf(outputfileSearch, "%s.link.search", fibfile);

		aHashtable.initialFromFile(fibfile, ht_times);

		//test the correctness of initialFromFile
#ifdef _DEBUG
		if(aHashtable.test(fibfile) == true)
		{
			printf("Congratulation! pass test!\n");
		}
		else
		{
			printf("not pass test");
		}
#endif 
		aHashtable.report(outputfileInit);

		//read the traffic, then search the hash table.
		FILE * fp_traffic =fopen(trafficfile,"r");
		if (NULL == fp_traffic)  //If FilePtr is NULL
		{
			fprintf(stderr , "fibfile %s open failed!\n", trafficfile);
			return false;
		}

		printf("\nstart traffic query...\n");
		char key[KEYLENGTH];
		memset(key , 0 , sizeof(key));
		int readlines=0;
		int val=0;
		while(!feof(fp_traffic))
		{
			fscanf(fp_traffic, "%s", key);
			readlines++;
			aHashtable.search((const unsigned char *)key, val);
			if (readlines%1000==0)
			{
				printf("\r%.2f%%", 100.0*readlines/fiblen/10);
			}
		}
		printf("val=%d\n",val);
		printf("\nafter the traffic query...\n");
		aHashtable.report(outputfileSearch);
	}
	catch(char *s)
	{
		fprintf(stderr, "error: %s\n",s);
	}

	printf("Congrats! Traffic query is completed!\n");
	return true;
}

bool linearExperiments(char * fibfile, char * trafficfile, char * updatefile, float ht_times)
{
	try
	{
		int fiblen=getFIBSize(fibfile);
		LinearHash aHashtable;
		cout << "successful initial" <<endl;

		printf("\n\nStart to process %s...\n", fibfile);

		char outputfileInit[100];
		memset(outputfileInit, 0, sizeof(outputfileInit));
		sprintf(outputfileInit, "%s.linear.init", fibfile);

		char outputfileSearch[100];
		memset(outputfileSearch, 0, sizeof(outputfileSearch));
		sprintf(outputfileSearch, "%s.linear.search", fibfile);

		aHashtable.initialFromFile(fibfile, ht_times);

		//test the correctness of initialFromFile
#ifdef _DEBUG
		if(aHashtable.test(fibfile) == true)
		{
			printf("Congratulation! pass test!\n");
		}
		else
		{
			printf("not pass test");
		}
#endif 
		aHashtable.report(outputfileInit);

		//read the traffic, then search the hash table.
		FILE * fp_traffic =fopen(trafficfile,"r");
		if (NULL == fp_traffic)  //If FilePtr is NULL
		{
			fprintf(stderr , "fibfile %s open failed!\n", trafficfile);
			return false;
		}

		printf("\nstart traffic query...\n");
		char key[KEYLENGTH];
		memset(key , 0 , sizeof(key));
		int readlines=0;
		int val=0;
		while(!feof(fp_traffic))
		{
			fscanf(fp_traffic, "%s", key);
			readlines++;
			aHashtable.search((const unsigned char *)key, val);
			if (readlines%1000==0)
			{
				printf("\r%.2f%%", 100.0*readlines/fiblen/10);
			}
		}
		printf("val=%d\n",val);
		printf("\nafter the traffic query...\n");
		aHashtable.report(outputfileSearch);
	}
	catch(char *s)
	{
		fprintf(stderr, "error: %s\n",s);
	}

	printf("Congrats! Traffic query is completed!\n");
	return true;
}

bool doubleExperiments(char * fibfile, char * trafficfile, char * updatefile, float ht_times)
{
	try
	{
		int fiblen=getFIBSize(fibfile);
		DHash aHashtable;
		cout << "successful initial" <<endl;

		printf("\n\nStart to process %s...\n", fibfile);

		char outputfileInit[100];
		memset(outputfileInit, 0, sizeof(outputfileInit));
		sprintf(outputfileInit, "%s.double.init", fibfile);

		char outputfileSearch[100];
		memset(outputfileSearch, 0, sizeof(outputfileSearch));
		sprintf(outputfileSearch, "%s.double.search", fibfile);

		aHashtable.initialFromFile(fibfile, ht_times);

		//test the correctness of initialFromFile
#ifdef _DEBUG
		if(aHashtable.test(fibfile) == true)
		{
			printf("Congratulation! pass test!\n");
		}
		else
		{
			printf("not pass test");
		}
#endif 
		aHashtable.report(outputfileInit);

		//read the traffic, then search the hash table.
		FILE * fp_traffic =fopen(trafficfile,"r");
		if (NULL == fp_traffic)  //If FilePtr is NULL
		{
			fprintf(stderr , "fibfile %s open failed!\n", trafficfile);
			return false;
		}

		printf("\nstart traffic query...\n");
		char key[KEYLENGTH];
		memset(key , 0 , sizeof(key));
		int readlines=0;
		int val=0;
		while(!feof(fp_traffic))
		{
			fscanf(fp_traffic, "%s", key);
			readlines++;
			aHashtable.search((const unsigned char *)key, val);
			if (readlines%1000==0)
			{
				printf("\r%.2f%%", 100.0*readlines/fiblen/10);
			}
		}
		printf("val=%d\n",val);
		printf("\nafter the traffic query...\n");
		aHashtable.report(outputfileSearch);
	}
	catch(char *s)
	{
		fprintf(stderr, "error: %s\n",s);
	}

	printf("Congrats! Traffic query is completed!\n");
	return true;

}

bool cuckooExperiments(char * fibfile, char * trafficfile, char * updatefile, float ht_times)
{
	try
	{
		int fiblen=getFIBSize(fibfile);
		cuckooHash aHashtable;
		cout << "successful initial" <<endl;

		printf("\n\nStart to process %s...\n", fibfile);

		char * fibfileShort=&(fibfile[4]);
		char outputfileInit[100];
		memset(outputfileInit, 0, sizeof(outputfileInit));
		sprintf(outputfileInit, "%s.cuckoo%.2f.init", fibfileShort, ht_times);

		char outputfileSearch[100];
		memset(outputfileSearch, 0, sizeof(outputfileSearch));
		sprintf(outputfileSearch, "%s.cuckoo%.2f.search", fibfileShort,ht_times);

		aHashtable.initialFromFile(fibfile, ht_times);

		//test the correctness of initialFromFile
#ifdef _DEBUG
		if(aHashtable.test(fibfile) == true)
		{
			printf("Congratulation! pass test!\n");
		}
		else
		{
			printf("not pass test");
		}
#endif 
		//aHashtable.report(outputfileInit);

		//read the traffic, then search the hash table.
		FILE * fp_traffic =fopen(trafficfile,"r");
		if (NULL == fp_traffic)  //If FilePtr is NULL
		{
			fprintf(stderr , "fibfile %s open failed!\n", trafficfile);
			return false;
		}

		printf("\nstart traffic query...\n");
		char key[KEYLENGTH];
		memset(key , 0 , sizeof(key));
		int readlines=0;
		int val=0;
		while(!feof(fp_traffic))
		{
			fscanf(fp_traffic, "%s", key);
			readlines++;
			aHashtable.search((const unsigned char *)key, val);
			if (readlines%1000==0)
			{
				printf("\r%.2f%%", 100.0*readlines/fiblen/10);
			}
		}
		printf("val=%d\n",val);
		printf("\nafter the traffic query...\n");
		aHashtable.report(outputfileSearch);
	}
	catch(char *s)
	{
		fprintf(stderr, "error: %s\n",s);
	}

	printf("Congrats! Traffic query is completed!\n");
	return true;
}

bool peacockExperiments(char * fibfile, char * trafficfile, char * updatefile, float ht_times)
{
	try
	{
		int fiblen=getFIBSize(fibfile);
		PeacockHash aHashtable; 
		cout << "successful initial" <<endl;

		printf("\n\nStart to process %s...\n", fibfile);

		char outputfileInit[100];
		memset(outputfileInit, 0, sizeof(outputfileInit));
		sprintf(outputfileInit, "%s.peacock.%.2f.init", fibfile, ht_times);

		char outputfileSearch[100];
		memset(outputfileSearch, 0, sizeof(outputfileSearch));
		sprintf(outputfileSearch, "%s.peacock.%.2f.search", fibfile, ht_times);

		aHashtable.initialFromFile(fibfile, ht_times);

		//test the correctness of initialFromFile
#ifdef _DEBUG
		if(aHashtable.test(fibfile) == true)
		{
			printf("Congratulation! pass test!\n");
		}
		else
		{
			printf("not pass test");
		}
#endif 
		//aHashtable.report(outputfileInit);

		//read the traffic, then search the hash table.
		FILE * fp_traffic =fopen(trafficfile,"r");
		if (NULL == fp_traffic)  //If FilePtr is NULL
		{
			fprintf(stderr , "fibfile %s open failed!\n", trafficfile);
			return false;
		}

		printf("\nstart traffic query...\n");
		char key[KEYLENGTH];
		memset(key , 0 , sizeof(key));
		int readlines=0;
		int val=0;
		while(!feof(fp_traffic))
		{
			fscanf(fp_traffic, "%s", key);
			readlines++;
			aHashtable.search((const unsigned char *)key, val);
			if (readlines%1000==0)
			{
				printf("\r%.2f%%", 100.0*readlines/fiblen/10);
			}
		}
		printf("val=%d\n",val);
		printf("\nafter the traffic query...\n");
		aHashtable.report(outputfileSearch);
	}
	catch(char *s)
	{
		fprintf(stderr, "error: %s\n",s);
	}

	printf("Congrats! Traffic query is completed!\n");
	return true;
}

bool dleftExperiments(char * fibfile, char * trafficfile, char * updatefile, float ht_times)
{
	try
	{
		int fiblen=getFIBSize(fibfile);
		dleftHash aHashtable; 
		cout << "successful initial" <<endl;

		printf("\n\nStart to process %s...\n", fibfile);

		char outputfileInit[100];
		memset(outputfileInit, 0, sizeof(outputfileInit));
		sprintf(outputfileInit, "%s.dleft.init", fibfile);

		char outputfileSearch[100];
		memset(outputfileSearch, 0, sizeof(outputfileSearch));
		sprintf(outputfileSearch, "%s.dleft.search", fibfile);

		aHashtable.initialFromFile(fibfile, ht_times);

		//test the correctness of initialFromFile
#ifdef _DEBUG
		if(aHashtable.test(fibfile) == true)
		{
			printf("Congratulation! pass test!\n");
		}
		else
		{
			printf("not pass test");
		}
#endif 
		aHashtable.report(outputfileInit);

		//read the traffic, then search the hash table.
		FILE * fp_traffic =fopen(trafficfile,"r");
		if (NULL == fp_traffic)  //If FilePtr is NULL
		{
			fprintf(stderr , "fibfile %s open failed!\n", trafficfile);
			return false;
		}

		printf("\nstart traffic query...\n");
		char key[KEYLENGTH];
		memset(key , 0 , sizeof(key));
		int readlines=0;
		int val=0;
		while(!feof(fp_traffic))
		{
			fscanf(fp_traffic, "%s", key);
			readlines++;
			aHashtable.search((const unsigned char *)key, val);
			if (readlines%1000==0)
			{
				printf("\r%.2f%%", 100.0*readlines/fiblen/10);
			}
		}
		printf("val=%d\n",val);
		printf("\nafter the traffic query...\n");
		aHashtable.report(outputfileSearch);
	}
	catch(char *s)
	{
		fprintf(stderr, "error: %s\n",s);
	}

	printf("Congrats! Traffic query is completed!\n");
	return true;

}

int RectExperiments(char * fibfile, char * trafficfile, char * updatefile, float ht_times, int blind_kick_num)
{
	try
	{
		//473611,  (a+a+7*5000)*4=
		int fiblen=getFIBSize(fibfile);
		RTBHash aHashtable;
		aHashtable.blind_kick_num=blind_kick_num;
		cout << "successful initial" <<endl;

		printf("\n\nStart to process %s...\n", fibfile);

		char * fibfileShort=&(fibfile[4]);

		char outputfileInit[100];
		memset(outputfileInit, 0, sizeof(outputfileInit));
		sprintf(outputfileInit, "%s.rect%d_%.2f.init", fibfileShort, aHashtable.blind_kick_num, ht_times);

		char outputfileSearch[100];
		memset(outputfileSearch, 0, sizeof(outputfileSearch));
		sprintf(outputfileSearch, "%s.rect%d_%.2f.search", fibfileShort, aHashtable.blind_kick_num, ht_times);

		aHashtable.initialFromFile(fibfile, ht_times);

		printf("\nafter %d Blind kick\n", aHashtable.blind_kick_num);
		aHashtable.report(outputfileInit);
		return 1;
	
		//read the traffic, then search the hash table.
		FILE * fp_traffic =fopen(trafficfile,"r");
		if (NULL == fp_traffic)  //If FilePtr is NULL
		{
			fprintf(stderr , "fibfile %s open failed!\n", trafficfile);
			return -1;
		}

		//test the correctness of initialFromFile
#ifdef _DEBUG
		if(aHashtable.test(fibfile) == true)
		{
			printf("Congratulation! pass test!\n");
		}
		else
		{
			printf("not pass test");
		}
#endif 

		printf("\nstart traffic query...\n");
		char key[KEYLENGTH];
		memset(key , 0 , sizeof(key));
		int readlines=0;
		int val=0;
		while(!feof(fp_traffic))
		{
			fscanf(fp_traffic, "%s", key);
			readlines++;
			aHashtable.search(key, val);
			if (readlines%1000==0)
			{
				printf("\r%.2f%%", 100.0*readlines/fiblen/10);
			}
		}
		printf("val=%d\n",val);
		printf("\nafter the traffic query...\n");
		aHashtable.report(outputfileSearch);
	}
	catch(char *s)
	{
		fprintf(stderr, "error: %s\n",s);
	}

	printf("Congrats! Traffic query is completed!\n");
	return 1;
}


void main(int argc, char* argv[])
{
	//main_hashCollisionTest(argc, argv); 

	char inputfile[100];
	memset(inputfile, 0, sizeof(inputfile));
	if (argc>3)
	{
		strcpy(inputfile,argv[1]);
	}
	else       
	{
		printf("Error parameters, the format should be: algorithmName, fibfile, trafficfile, updatefile\n"); // times, blind kick
		return;
	}
	
	     if (!strcmp(argv[1], "RHT"))       RectExperiments(argv[2], argv[3], "",atof(argv[4]), atoi(argv[5]));//argv[4], ht_times);
	else if (!strcmp(argv[1], "peacock"))   peacockExperiments(argv[2], argv[3],  "",atof(argv[4]));//argv[4], ht_times);
	else if (!strcmp(argv[1], "dleft"))     dleftExperiments(argv[2], argv[3],  "",atof(argv[4]));//argv[4], ht_times);
	else if (!strcmp(argv[1], "linear"))    linearExperiments(argv[2], argv[3],  "",atof(argv[4]));//argv[4], ht_times);
	else if (!strcmp(argv[1], "link"))      linkExperiments(argv[2], argv[3],  "",atof(argv[4]));//argv[4], ht_times);
	else if (!strcmp(argv[1], "double"))    doubleExperiments(argv[2], argv[3], "",atof(argv[4]));//argv[4], ht_times);
	else if (!strcmp(argv[1], "cuckoo"))    cuckooExperiments(argv[2], argv[3], "",atof(argv[4]));//argv[4], ht_times);
	else printf("error input! wrong algorithm name, program exits...\n");
}