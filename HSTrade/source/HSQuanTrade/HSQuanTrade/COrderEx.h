#include <vector>
#include <mutex>
#include <string>
#include <thread>
#include "typedefine.h"
#include <map>


class CBaseTradeApi;

class CTradeInterface;


class COrderEx
{
public:
	COrderEx(char *account,char * instrument, int count, char direction, char offsetflag, double price,int ordertype,int timeout,double jiacha,CBaseTradeApi * api,CTradeInterface *ptrade,int orderref);

	~COrderEx(void);

	CBaseTradeApi *m_pApi;

	CTradeInterface *m_ptrade;

	char m_account[15];

	int m_nOrderRef;

	int m_nEntrustNo;

	char m_szIntrument[16];

	bool m_bDone;			//终止状态:成交，已撤，已经发出撤单请求

	bool m_bReOrder;		//是否需要平仓

	void RtnOrder(int orderref,int ordersysid,char status,char bs1,char kp1,char * ins);

	void RtnTrade(int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

	void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid);

private:

	int  m_count;			 //委托数量

	int m_iTradeCount;	     //成交数量

	char m_direction;

	char m_offsetflag;

	double m_price;


	int  m_nordertype;       //-1不进行订单管理 0超时撤单 1下一笔行情来了还没成交立即对价追价 2 盘口价差符合参数 追价 

	int  m_ntimeout;

	double m_jiacha;

	long m_timestamp;		 //生成时间


	char m_status;			 //订单状态  

	bool m_bWithDrawING;     //是否正在撤单  

	std::mutex m_lockWithDraw;


public:

	bool DealPrice(CTickData *pd);     //模式1	需要追价 

	bool IsDone();

	void SetDone(bool done);

	bool IsWithDraw();

	void SetWithDraw(bool withdraw);

	bool IsReOrder();					//是否反向开仓

	void SetReOrder(bool order);

	void RervseOrder(CTickData *pd);					//反向做

	int WithDraw(int ref);		

	int GetEntrustAmount();

	int GetTradeAmount();

	double GetEntrustPrice();
};



class COrderGroup
{

public:
	COrderGroup(int id);

	~COrderGroup();

	void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid);

	void RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins);

	void RtnTrade(int orderref,int ordersysid,double price,char bs,char kp,char * tradetime,int vol,char *ins);

	void Tick(CTickData *pd);

	void AddOrder(int ref, COrderEx *order);

	int GetGrpId();

	void DealReverse(int orderref);


private:

	std::map<int,COrderEx*> m_mapOrder;				//报单map

	std::vector<int> m_vecChedan;					//追价时，价差过大，撤单不再追单的报单

	std::map<std::string,CTickData> m_mapTick;		//缓存行情

	std::vector<int> m_vecNotTrade;					//其他未成交的报单 撤单队列

	long m_timestamp;								//生成时间

	int m_iGrpId;


	std::mutex m_lockNotTrade;
};
