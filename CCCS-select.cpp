/*
China City Coord System

	SELECT
*/

#include "functions.h"

//定义查询方式宏
#define		SELECT_ALL		0
#define		SELECT_CITY		Still_Unsigned_Yet>_<|||
#define		SELECT_LAT_EXACT		1
#define		SELECT_LAT_SCOPE		2


SQLHENV henv = SQL_NULL_HENV;   
SQLHDBC hdbc1 = SQL_NULL_HDBC;   
SQLHSTMT hstmt1 = SQL_NULL_HSTMT;   

/******************************************************

				全 部 查 询
					 
*******************************************************/
void selectAll(
	RETCODE retcode,
	char * sql,
	char * sqlh1,
	char * initialize)

{
	/*
	1.确认一个结果集是否可用。
	2.将结果集的列绑定在适当的变量上。
	3.取得行
	*/
	CityMsg citymsg;

	getchar();//储存在choose界面的回车字符，防止第一次翻页之前输出两倍的行数
	strcpy(sql,sqlh1);//拼合sql语句字符串
	SQLExecDirect (hstmt1,(UCHAR *)sql,strlen(sql));//对数据库发送select all语句
	//SQLBindCol(hstmt1, 1, SQL_C_CHAR, list, 5, 0);	//该函数对数据库发送语句
	SQLBindCol(hstmt1, 1, SQL_C_CHAR, citymsg.city, 10, 0);//该函数是数据库的反馈信息函数
	SQLBindCol(hstmt1, 2, SQL_C_CHAR, citymsg.lon, 11, 0);//第二个参数是目标表中的列号
	SQLBindCol(hstmt1, 3, SQL_C_CHAR, citymsg.lat, 11, 0);//第五个参数是传回的字符串长度
	do{
		retcode = SQLFetch(hstmt1);
		if(retcode == SQL_NO_DATA){
			break;
			}
		printf("%s		%s		%s\n",citymsg.city, citymsg.lon, citymsg.lat);
		static int n=1;//翻页计数器
		n++;
		if(n%20 == 0)	//每页20行
		{
			printf("\n【第%d页】",n/20);//页码
			getchar();	//按回车换页
		}
	}while(1);
	strcpy(sql,initialize);	//重新初始化字符串
	
	getchar();//结束标志，按一下回车
}


/******************************************************

				纬 度 精 确 查 询
					 
*******************************************************/
void selectByLat(
		char * sqlh1,
		char * sql,
		char * initialize)
{
	
	char sqlh5[10]="\0";
	CityMsg citymsg;

	getchar();
	printf("输入要查询的纬度：\n");
	printf("	例如：34.17\n");
	printf("北纬");
	gets(sqlh5);

	strcpy(sql,sqlh1);
	strcat(sql,"where latitude='北纬");
	strcat(sql,sqlh5);
	strcat(sql,"'");

	puts(sql);		//显示向数据库发送的sql语句
	SQLExecDirect (hstmt1,(UCHAR *)sql,strlen(sql));
	SQLBindCol(hstmt1, 1, SQL_C_CHAR, citymsg.city, 10, 0);//该函数是数据库的反馈信息函数
	SQLBindCol(hstmt1, 2, SQL_C_CHAR, citymsg.lon, 11, 0);//第二个参数是目标表中的列号
	SQLBindCol(hstmt1, 3, SQL_C_CHAR, citymsg.lat, 11, 0);//第五个参数是传回的字符串长度
	SQLFetch(hstmt1);
	printf("%s		%s		%s\n",citymsg.city, citymsg.lon, citymsg.lat);
	strcpy(sql,initialize);//重新初始化字符串
	
	getchar();//结束标志，按一下回车
}

/*
	查询SQLSERVER数据库,1.条件查询,2.直接查询全部
*/
int main(){       
	RETCODE retcode;     
	UCHAR   szDSN[SQL_MAX_DSN_LENGTH+1]   =   "CCCS";     //数据库名
	UCHAR	szUID[MAXNAME]   =   "sa";					//用户名
	UCHAR	szAuthStr[MAXNAME]   =   "";				//密码
	char	sql[57] = "\0";								//插入时是用的sql语句的存放变量 
	char	initialize[2] = "\0";						//初始化变量
	char	sqlh1[20] = "select * from city ";				//拼合字符串

	retcode   =   SQLAllocHandle   (SQL_HANDLE_ENV,   NULL,   &henv);   
	retcode   =   SQLSetEnvAttr(henv,   SQL_ATTR_ODBC_VERSION,   
				  (SQLPOINTER)SQL_OV_ODBC3,   
				  SQL_IS_INTEGER);    
	retcode   =   SQLAllocHandle(SQL_HANDLE_DBC,   henv,   &hdbc1);   
	//1.连接数据源
	retcode   =   SQLConnect(hdbc1,   szDSN,   4,   szUID,   2,   szAuthStr,   0);    
	if   (   (retcode   !=   SQL_SUCCESS)   &&   (retcode   !=   SQL_SUCCESS_WITH_INFO)   )   {   
		printf("连接失败!");
	}   else   {   
		//2.创建并执行一条或多条SQL语句
		/*
		1.分配一个语句句柄(statement handle)
		2.创建SQL语句
		3.执行语句
		4.销毁语句
		*/
		retcode   =   SQLAllocHandle(SQL_HANDLE_STMT,   hdbc1,   &hstmt1);   

		//第一种方式
		/*
		//直接执行
		SQLExecDirect (hstmt1,(UCHAR *)sql,strlen(sql));
		char list[5];
		SQLBindCol(hstmt1, 1, SQL_C_CHAR, list, 5, 0);
		SQLFetch(hstmt1);
		printf("%s\n",list);*/
			
		//第二种方式
		/*
		//绑定参数方式
		char a[200]="aaa";
		SQLINTEGER   p   =   SQL_NTS;
		//1.预编译
		SQLPrepare(hstmt1,sql2,35); //第三个参数与数组大小相同，而不是数据库列相同
		//2.绑定参数值
		SQLBindParameter(hstmt1,1,SQL_PARAM_INPUT,SQL_C_CHAR,SQL_CHAR,200,0,&a,0,&p);
		//3.执行
		SQLExecute(hstmt1);
		char list[5];
		SQLBindCol(hstmt1, 1, SQL_C_CHAR, list, 5, 0);
		SQLFetch(hstmt1);
		printf("%s\n",list);
		*/
		
		int choose;	//用户选择序号的储存变量

		while(1){
			printf("输入查询方式：\n");
			printf("	0.全部查询\n");
			printf("	1.纬度精确查询\n");
			printf("	2.纬度区间查询\n");
			printf("\n");
			scanf("%d",&choose);
			
			switch(choose){
				case SELECT_ALL:{
				//全部输出
					selectAll(retcode, sql, sqlh1, initialize);
					break;
				}
				case SELECT_LAT_EXACT:{
					//通过具体纬度查找
					selectByLat(sqlh1,sql,initialize);
					break;
				}
				case SELECT_LAT_SCOPE:{
				//通过纬度区间查找
				
				
				}
			}
		}
		
		//释放语句句柄
		SQLCloseCursor (hstmt1);
		SQLFreeHandle (SQL_HANDLE_STMT, hstmt1);
	
	}   
 
	//4.断开数据源
	/*
     1.断开与数据源的连接.
	 2.释放连接句柄.
	 3.释放环境句柄 (如果不再需要在这个环境中作更多连接)
    */
	SQLDisconnect(hdbc1);    
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc1);   
	SQLFreeHandle(SQL_HANDLE_ENV, henv);   
	getchar();
	getchar();
	return(0);   
}

