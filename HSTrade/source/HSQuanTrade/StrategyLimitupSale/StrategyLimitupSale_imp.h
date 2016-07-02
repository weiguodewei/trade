#pragma once

#include "..\hsquantrade\strategybase.h"
#include <thread>
#include <mutex>


class CStrategyLimitupSale : public CStrategyBase
{

public:

	CStrategyLimitupSale();

	~CStrategyLimitupSale();

	virtual void  Tick(CTickData * pt);

	virtual CStrategyBase * NewInstance() { 
		
		
		CStrategyBase* pStrategy = new CStrategyLimitupSale();
		if (pStrategy)
		{
			pStrategy->CreateStatisticObject();
		}
		
		return new CStrategyLimitupSale();  
	
	
	
	};

	virtual void SetParam(CString paramname,CString value);    

	virtual void SetParam(CString value);

	virtual CString GetParamValue();

	virtual CString GetParamValue(CString paramname);

	virtual CString GetStatusInfo(void);

	virtual void BeginRun();

	virtual void Pause();

	virtual void OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price);

	virtual void OnErrorRtn(int code,char * errormsg){} ;

	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins);

	virtual void reload();                //重新加载

	virtual void saveModel() ;             //保存模型


public:

	double m_fBackRate;                   //最高价回调比例

	double m_fLoseRate;                   //亏损比例

	double m_fGetRate;					  //止盈比例

	double m_dTradePrice;                 //成本价

	bool m_done;                           //是否完成

	int m_iDurationTime;                   //符合条件的持续时间,单位秒
	
public:

	bool isDoNothingTime();					//卖出不操作的时间

	bool isCloeTime();						//卖出必须平仓的时间

	bool isSaveTime();						//盘后保存信息

	map<string,long> m_sSignalTime;         //合约对应触发操作的时间点


};