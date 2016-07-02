#pragma once
#include <map>
#include "TradeStatistic2.h"
#include <string>

using namespace std;

typedef enum tagE_Fund_Policy_Type
{
	FUND_NO_POLICY = 0,
	FUND_POLICY_STOCK_AVG = 1,
}EFundPolicyType;

typedef struct tagPostionDesc
{
	double m_dbOpenPrice;
	int m_iCount;//手数
}StPostionDesc;

class CStrategyBase;
class CFundPolicyBase
{
	friend class CFundManager;
protected:
	CFundPolicyBase();
public:
	
	~CFundPolicyBase();

	void SetInitFund(double dbInitFund);
	double GetCurrentFund()
	{
		return m_dbFund;
	}
	virtual int GetPostionCount(){return 0;}
	void SetCostRate(double dbCostRate){
		m_dbCostRate = dbCostRate;
	}
	virtual bool GetAvailableFund(double& dbFund);
	virtual void SetParams(CString paramname,CString value);
	virtual CString GetParams(CString paramname);
	virtual CStockEquity GetEveryDayEquity(CStrategyBase* pStrategy,const char* szDate){ 
		CStockEquity oEquity = {m_dbFund, m_dbFund};
		return oEquity; }

	virtual void OpenBuy(const char* szCode, double dbPrice, int iCount);
	virtual void OpenSell(const char* szCode, double dbPrice, int iCount);
	virtual void CloseBuy(const char* szCode, double dbPrice, int iCount);
	virtual void CloseSell(const char* szCode, double dbPrice, int iCount);


protected:

	double m_dbInitFund;	   //初始资金
	double m_dbFund;		   //实时资金
	double m_dbCostRate;
	int m_iFuli;			   //是否复利

	std::map<string, CTradeRecord2> m_mpTrade;
	std::map<string, double> m_mapLastClosePrice;
	void RecordTrade(int iOpenClose, int iDir, const char* szCode, double dbPrice, int iCount);
};


class CFundAvgPolicy : public CFundPolicyBase
{
	friend class CFundManager;
protected:
	CFundAvgPolicy();
public:
	
	~CFundAvgPolicy();
	virtual int GetPostionCount();
	virtual bool GetAvailableFund(double& dbFund);
	virtual void SetParams(CString paramname,CString value);
	virtual CString GetParams(CString paramname);
	virtual CStockEquity GetEveryDayEquity(CStrategyBase* pStrategy, const char* szDate);
	virtual void OpenBuy(const char* szCode, double dbPrice, int iCount);
	virtual void OpenSell(const char* szCode, double dbPrice, int iCount);
	virtual void CloseBuy(const char* szCode, double dbPrice, int iCount);
	virtual void CloseSell(const char* szCode, double dbPrice, int iCount);
protected:


	int m_nPositionCount;     //最大开仓
	int m_nPositionAcc;		  //已开仓

	
};
class CFundManager
{
public:
	CFundManager(void);
	~CFundManager(void);
	static CFundPolicyBase* GetPolicyInstance(int iFundPolicy = 0);

};

