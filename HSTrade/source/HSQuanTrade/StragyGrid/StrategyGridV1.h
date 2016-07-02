#pragma once

#include "..\hsquantrade\strategybase.h"

#include<thread>
#include<mutex>

#include "..\hsquantrade\typedefine.h"

#include<vector>

#include <map>


struct TPosition
{
	char  ins[32];
	float price;
	int   count;
	char  direction;

	TPosition()
	{
		memset(this,0,sizeof(TPosition));
	}
};


class CStrategyGrid :	public CStrategyBase
{
public:
	CStrategyGrid(void);

	CStrategyGrid(int i);

	~CStrategyGrid(void);

	virtual void  Tick(CTickData * pt);

	virtual CStrategyBase * NewInstance() { return new CStrategyGrid(1) ; };

	virtual void SetParam(CString paramname,CString value);    

	virtual void SetParam(CString value);

	virtual CString GetParamValue();

	virtual CString GetParamValue(CString paramname);

	virtual CString GetStatusInfo(void);

	virtual void BeginRun();

	virtual void OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price);

	virtual void OnErrorRtn(int code,char * errormsg) ;

	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins) ;

	virtual void OnRspOrderInsert(int ref,char entrust_status,int errorid);

	virtual void reload();                //重新加载

	virtual void saveModel() ;             //保存模型



public:

	void Order();

	void CalCloseProfit(char direction,double price,int count);

	void ClearPosition(void);


	CUIntArray m_listOrderRef;

	CPtrArray m_arPosition;


	double m_beginPrice;

	double m_J1;

	double m_N1;

	int m_TradeDirection;

	int m_kp;

	int m_nMaxTradeCount;

	int m_nMaxChedanCount;

	int m_nMaxLost;

	int m_nTradeCountSum;

	float m_fProfitBackStop;

	float m_fyingkuiClose;

	float m_maxProfit;

	double m_pricebid1;

	double m_priceask1;

	int	m_nInstrumentValue;//合约乘数


public:

	void RemoveOrder(int orderref);

	bool isExistRef(int ref,char bs,char kp,char * ins);

	void addGrid(int count);            //增加网格

	void subGrid(int count){};

	void newOrder(int count,double price);

	void setValue();


	void AddOrderInfo(int ref,int count,double price,char bs,char kp);       //记录指令信息

	void AddOpenPosition(int ref,int count,double price,char bs,char *code);  //记录开仓信息


	//-------------------------------------------------------------------------
	// 检查各个开市信号
	//-------------------------------------------------------------------------
	
	bool checkOrdertime();             //是否开市

	bool checkothertime();             //是否非交易时间

	bool checksavetime();              //收盘保存模型

	bool reLogintime();                //检查重新登录时间

	bool checkMatchTime();             //检查是否是撮合成交的时间


	//-------------------------------------------------------------------------
	// 状态位
	//-------------------------------------------------------------------------
	
	bool isAnohterDay;                   //是否第二天

	bool isNewModel;                     //是否新建模型

	bool isSave;                         //是否已经保存

	bool isLoad;                         //是否已经加载

	bool m_btick;                        //来第一笔行情标志

	int m_tickcount;                     

	
	thread m_tOrder;                     //线程绑定函数

	void checkOrder();


	//-------------------------------------------------------------------------
	// 持仓处理函数
	//-------------------------------------------------------------------------

	void dealHoldsinfo(int count,double price,int bs);           //模型初始阶段下单

	void setHoldinfo();                                          //收盘持仓今仓变老仓

	void updateHoldinfo(char bs,char kp,char *ins,int count);    //收到回报更新持仓

	std::mutex m_QueryHoldMutex;


	//-------------------------------------------------------------------------
	// 挂单未成功的单子
	//-------------------------------------------------------------------------
	
	map<int,COrderInfo> m_failOrder;  

	std::mutex m_reOrderMutex;

	void reOrderAgain();

	int dealTrade(int orderref,int vol,int iNotTradeCount);			//处理成交回报


	//-------------------------------------------------------------------------
	// 处理频繁的分笔成交
	//-------------------------------------------------------------------------

	map<int,COrderInfo> m_mapOrder;                         //回报回来后新增的报单信息

	thread m_tStore;

	void process();                                         //线程函数

	void OrderTogether();                                   //分笔成交聚集一起报单

	void JoinOrder(int ref,int vol,double orderprice,char bs,char kp);     //存在ref相同分笔成交聚集 

	bool JoinDiffRef(int ref,int vol,double orderprice,char bs,char kp);   //ref不相同同样需要合并

	std::mutex m_JoinMutex;

	int m_iIntervalOrder;                                   //下单的间隔


	//-------------------------------------------------------------------------
	// 自成交处理
	//-------------------------------------------------------------------------
	map<int,COrderInfo> m_mapSelfOrder;                 //存放可能自成交的报单

	bool CheckSelfNotOrder(int ref,double price,char bs,char kp,int count);    //检查待报单自成交

	bool CheckSelfOrder(int ref,double price,char bs,char kp);      //检查挂单是否存在自成交

	void OrderSelfColse();                              //下单

	std::mutex m_CheckSelfMutex;

	bool ReverseOrder(int ref,int type,int count);    //将可能自成交的单子内部自成交然后反转

	void OrderByMap(map<int,COrderInfo> orderMap);							//遍历map报单

	void OrderByInfo(COrderInfo orderinfo);									
	
};


