#pragma once

#include "..\hsquantrade\strategybase.h"
#include <thread>
#include <mutex>


class CStrategyLimitupBuy : public CStrategyBase
{

public:

	CStrategyLimitupBuy();

	~CStrategyLimitupBuy();

	virtual void  Tick(CTickData * pt);

	virtual CStrategyBase * NewInstance() { 
		
		CStrategyBase* pStrategy = new CStrategyLimitupBuy();
		if (pStrategy)
		{
			pStrategy->CreateStatisticObject();
		}
		
		return new CStrategyLimitupBuy(); 
	
	};

	virtual void SetParam(CString paramname,CString value);    

	virtual void SetParam(CString value);

	virtual CString GetParamValue();

	virtual CString GetParamValue(CString paramname);

	virtual CString GetStatusInfo(void);

	virtual void BeginRun();

	virtual void OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price);

	virtual void OnErrorRtn(int code,char * errormsg){} ;

	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins);

	virtual void reload(){};                //重新加载

	virtual void saveModel(){} ;             //保存模型


public:

	//委托信息串,格式:code,count,price|code,count,price|
	CString m_sEntrustInfo;

	double m_rate;                        //买一比买二

	double m_dChangeRate;                 //换手率

	char m_bs;

	char m_kp; 

	bool m_done;

	int m_strType;                         //1 早上下单 下午回购；2 下午回购；3 单个涨停板策略合约卖出

	bool m_huigou;                         //是否可以回购

	thread m_tWorkThread;

	std::mutex m_entrustMutex;


public:

	void order();

	void process();

	bool isOrderTime();                     //买入的交易时间

	bool isWithDrawTime();					//买入的撤单时间

	bool isCollectiveBidTime();             //9:20~9:30

	bool isDoNothingTime();					//卖出不操作的时间

	bool isCloeTime();						//卖出必须平仓的时间

	void withDraw();                        //全部撤单

	map<string,long> m_sSignalTime;         //合约对应触发操作的时间点

	void BussInessDeal(int type);



};