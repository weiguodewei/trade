#pragma once
#include "strategybase.h"
class CStrategyLimitupSale :
	public CStrategyBase
{
public:
	CStrategyLimitupSale(void);

	~CStrategyLimitupSale(void);

	virtual void  Tick(CTickData * pt);
	virtual void BackTest(CKStrdata *pk, const char* sCode){}
	virtual CStrategyBase * NewInstance() {

		CStrategyBase* pStrategy = new CStrategyLimitupSale();
		if (pStrategy)
		{
			pStrategy->CreateStatisticObject();
		}
		return pStrategy;
		
	};

	virtual void SetParam(CString paramname,CString value);    

	virtual void SetParam(CString value);

	virtual CString GetParamValue();

	virtual CString GetParamValue(CString paramname);

	virtual CString GetStatusInfo(void);

	virtual void BeginRun(){};

	virtual void OnTradeReturn(char * ins,char * time,int orderref,int ordersystemid,char direction,char offsetflag,int vol,double price);

	virtual void OnErrorRtn(int code,char * errormsg) {};

	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins) {};

	virtual void reload() {};                //重新加载

	virtual void saveModel() {};             //保存模型

	virtual void setData(const vector<CHoldsinfo> * phold,const std::map<std::string,CCodeStatus> * pcode,HWND m_hMsgWnd){};             //保存数据

public:
	double m_loseRate;              //止损率

	double m_downRate;			    //最高价回调率

	double m_changeRate;            //换手率

	bool m_isToHighPrice;           //是否达到过最高价

};

