#include <stdio.h>     
#include <string.h>     
#include <windows.h>     
#include <sql.h>     
#include <sqlext.h>     
#include <sqltypes.h>     
#include <odbcss.h> 

//存放城市及坐标的结构体
typedef struct CityMsg{
	char	city[50];	//城市	city
	char	lon[50];	//经度	longitude
	char	lat[50];	//纬度	latitude
	struct CityMsg	*next;		//下一节点
}CityMsg;

//函数声明
CityMsg * nextNood(CityMsg * );//构建链表 函数
CityMsg * getCityMsg();		//获取城市及坐标 函数

//构建链表
CityMsg * nextNood(CityMsg * oldCM)
{
	//尾插法
	CityMsg * newCM;
	newCM = (CityMsg *)malloc(sizeof(CityMsg));//为新节点开辟空间
	newCM->next = NULL;	//初始化新节点
	oldCM->next = newCM;
	return newCM;	//返回新节点
}

//获取城市及坐标
CityMsg * getCityMsg()
{
	CityMsg * head,		//头指针 
			* citymsg;	//存放城市及坐标的结构体
	FILE *fp;			//文件指针->存放城市及坐标的文件
	
	head = (CityMsg *)malloc(sizeof(CityMsg));//为头指针开辟空间
	citymsg = (CityMsg *)malloc(sizeof(CityMsg));//为首节点开辟空间
	head->next = citymsg;			//初始化头指针
	citymsg->next = NULL;			//初始化首节点

	fp = fopen("CityCoord.txt","rt");//尝试打开文件
	if(fp == NULL){					//若打不开，反馈信息并退出
		printf("Cannot Open This File,Press Any Key to Exit.\n");
		getchar();
		exit(1);
	}
	while(!feof(fp)){		//循环读取城市及坐标信息，直到文件末尾
		fscanf(fp,"%s %s %s",citymsg->city,citymsg->lon,citymsg->lat);//读入城市及坐标
		citymsg = nextNood(citymsg);								//开辟并指向下一节点
	}
	return head;
}
