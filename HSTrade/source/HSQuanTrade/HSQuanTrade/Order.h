#include <mutex>
#include <string>
#include <thread>

using namespace std;

#pragma once


class CBaseTradeApi;

class CTradeInterface;

class COrder
{
public:
	COrder(char *account,char * instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,CBaseTradeApi * api,CTradeInterface *ptrade,int orderref);

	~COrder(void);

	CBaseTradeApi *m_pApi;

	CTradeInterface *m_ptrade;

	char m_account[15];

	int m_nOrderRef;

	int m_nEntrustNo;

	int m_nOrderSysID;

	char m_szIntrument[16];

	char m_szLeg2[16];

	bool m_bDone;			//终止状态:成交，已撤，已经发出撤单请求

	long m_timestamp;		 //生成时间
	
	void RtnOrder(int orderref,int ordersysid,char status,char bs1,char kp1,char * ins);

	void RtnTrade(int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

	/*解决不支持报单引用的接口
	  entrustno != -9999 表示不支持报单引用需要更新entrustno
	*/
	void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid);

private:

	int  m_count;			 //委托数量

	int m_iTradeCount;	     //成交数量

	char m_direction;

	char m_offsetflag;

	double m_price;

	int  m_nOrderType;       //-1不进行订单管理 0超时撤单 1下一笔行情来了还没成交立即对价追价 2 盘口价差符合参数 追价 

	int  m_ntimeout;

	double m_jiacha;

	char m_status;			 //订单状态  

	bool m_bWithDrawING;     //是否正在撤单  

	std::thread m_tTrd;

	std::thread m_tTrdQuery; //查询线程


public:

	void DealOrder();							  //模式0	超时则撤单

	void DealPrice(double buy1,double sale1);     //模式1	需要追价 

	void QueryOrder();							  //查询委托	

	void DealOrderEx();							  //其他模式 也需要超时撤单继续报单


};

