/*
China City Coord System

	INSERT
*/

#include "functions.h"

SQLHENV henv = SQL_NULL_HENV;     
SQLHDBC hdbc1 = SQL_NULL_HDBC;     
SQLHSTMT hstmt1 = SQL_NULL_HSTMT; 

/* 
cpp文件功能说明： 
1.数据库操作中的添加，修改，删除，主要体现在SQL语句上 
2.采用直接执行方式和参数预编译执行方式两种 
*/  
int main(){     
	RETCODE retcode;     
	UCHAR   szDSN[SQL_MAX_DSN_LENGTH+1]   =   "CCCS";     //数据库名
	UCHAR	szUID[MAXNAME]   =   "sa";					//用户名
	UCHAR	szAuthStr[MAXNAME]   =   "";				//密码
	char	sql[60] = "\0";										//插入时是用的sql语句的存放变量 
	char	sqlh1[26] = "insert into city values('";				//拼合字符串
	char	sqlh2[4] = "','";
	char	sqlh3[3] = "')";
	//UCHAR   pre_sql[31] = "insert into city values(?,?,?)";			 //预编译SQL语句  
	CityMsg * citymsg;		//城市信息

	//SQL语句  
	//1.连接数据源  
	//1.环境句柄  
	retcode   =   SQLAllocHandle   (SQL_HANDLE_ENV,   NULL,   &henv);     
	retcode   =   SQLSetEnvAttr(henv,   SQL_ATTR_ODBC_VERSION,     
		(SQLPOINTER)SQL_OV_ODBC3,     
		SQL_IS_INTEGER);     
	//2.连接句柄    
	retcode   =   SQLAllocHandle(SQL_HANDLE_DBC,   henv,   &hdbc1);     
	retcode   =   SQLConnect(hdbc1,   szDSN,   4,   szUID,   2,   szAuthStr,   0);  

	//判断连接是否成功  
	if ( (retcode   !=   SQL_SUCCESS)   &&   (retcode   !=   SQL_SUCCESS_WITH_INFO)   )   {       
		printf("连接失败!\n");  
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
		//直接执行  
		//添加操作

		//打开文件
		citymsg = getCityMsg();
		citymsg = citymsg->next;
		while(citymsg->next != NULL){
			//拼合字符串
			strcpy(sql,sqlh1);
			strcat(sql,citymsg->city);
			strcat(sql,sqlh2);
			strcat(sql,citymsg->lat);
			strcat(sql,sqlh2);
			strcat(sql,citymsg->lon);
			strcat(sql,sqlh3);
			//执行sql语句
			//SQLExecDirect (hstmt1,(UCHAR *)sql,50); 
			//测试
			switch( SQLExecDirect (hstmt1,(UCHAR *)sql,strlen(sql))) { 
			case SQL_SUCCESS_WITH_INFO: { 
				printf("SQL_SUCCESS_WITH_INFO\n");
				break;
										} 
			case SQL_SUCCESS: { 
				printf("SQL_SUCCESS\n"); 
				break; 
							  }  
			case SQL_ERROR: {
				printf("SQL_ERROR\n"); 
				break; 
				} 
			default: 
				printf("else Return\n"); 

			} 
			//测试结束
			printf("%s\n",sql);//test
			//重新初始化sql语句存放变量
			strcpy(sql,sqlh1);
			//链表指向下一节点
			citymsg = citymsg->next;
		}

		//第二种方式  
		//绑定参数方式  
		/*char a[200]="bbb";  
		char b[200]="200";  
		char c[200]="200";
		SQLINTEGER   p   =   SQL_NTS;  
		//1预编译  
		SQLPrepare(hstmt1,pre_sql,31); //第三个参数与数组大小相同，而不是数据库列相同  
		//2绑定参数值  
		SQLBindParameter(hstmt1,1,SQL_PARAM_INPUT,SQL_C_CHAR,SQL_CHAR,200,0,&a,0,&p);  
		SQLBindParameter(hstmt1,2,SQL_PARAM_INPUT,SQL_C_CHAR,SQL_CHAR,200,0,&b,0,&p);  
		SQLBindParameter(hstmt1,2,SQL_PARAM_INPUT,SQL_C_CHAR,SQL_CHAR,200,0,&c,0,&p); 
		//3 执行  
		SQLExecute(hstmt1);*/  

		printf("操作成功!");  
		//释放语句句柄  
		SQLCloseCursor (hstmt1);  
		SQLFreeHandle (SQL_HANDLE_STMT, hstmt1);  

	}     
	//3.断开数据源  
	/* 
	1.断开与数据源的连接. 
	2.释放连接句柄. 
	3.释放环境句柄 (如果不再需要在这个环境中作更多连接) 
	*/  
	SQLDisconnect(hdbc1);      
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc1);     
	SQLFreeHandle(SQL_HANDLE_ENV, henv);     
	getchar();
	return 0;     
}     