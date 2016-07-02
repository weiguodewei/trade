#include <atomic> 
#include <string>
#include "typedefine.h"
#include "MgServerSocket.h"
#include <thread>
#include <mutex>
#include "public.h"


#pragma once


struct OrderRtn
{
	int returntype;
	char account[32];
	int orderref;
	int ordersysid;
	char status;
	char direction;
	char offsetflag;
	char instrument[32];
	double price;
	int   count;
	char  tradetime[32];
	OrderRtn()
	{
		memset(this,0,sizeof(OrderRtn));
	}
};




class CStrategyManager;

class CTradeInterface
{
public:
	CTradeInterface(void);

	~CTradeInterface(void);

	std::atomic_int m_iOrderRef;

	int InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,char * account);

	//按组下单
	int InsertOrderGroup(char * instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,char * account,int group);

	int WithDraw(char*account,int entrustno,char *sCode);

	void QueryPosition(char *account);

	void QueryEntrust(char*account,int entrustno);

	void QueryTrade(char*account);

	void QueryFund(char*account);


	//添加账户
	int OpenAccount(CString account,  CString pwd,CString serverip, CString apitype,CString brokeid);

	//设置账户风控信息
	void SetRiskInfo(char* account,double margin,int count,double pl,int open);

	void init();

	HWND m_hMsgWnd;

	bool m_bStop;

	std::thread m_tTrd;

	std::mutex m_LockOrder;


	/*
	第二个字段是为了兼容不支持报单引用的接口
	*/
	void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid);

	void OnRspWithdraw(int ref);

	void RtnOrder(int orderref,int ordersysid,char status,char bs ,char kp,char * ins);

	void RtnOrderEx(int orderref,int ordersysid,char status,char bs ,char kp,char * ins);
		
	void RtnTrade(char *account,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

	void RtnTradeEx(char *account,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

    CString  getAccount();            //获取已加入的账户

	void DealOrder();               //通过一个线程定时刷下订单的状态

	void OrderTick(CTickData *pd);

	void Login();

	void LogOut();

	void Exit();

	void SetAccountInfo(char *account,bool isQueryHold);

private:

	CPtrArray m_arApiList;

public:

	CPtrArray m_arListOrder;

	CStrategyManager* m_strManger;

	//针对需要成组下单
	CPtrArray m_arListOrderGroup;

//针对socket接口
public:
	
	bool InitSock();

	void InitListen();

	CMgServerSocket m_ServerSock;

	void getQueryInfo(char* buf,char *account);       //socket查询的应答无法在socket交易接口中获取,

	int m_iRestLength;								  //还需要接收的长度

	std::string buf;								  //保存接收到的socket数据

	bool m_bEnd;									  //本次数据是否接收完毕

	int m_port ;									  //监听端口

	CPtrArray m_arListRtn;  //存储报单和成交回报

	ManualLock m_lockRtn;

	static unsigned int _stdcall ThreadRtnProcess(void * pt);

	HANDLE m_hThread;

};

