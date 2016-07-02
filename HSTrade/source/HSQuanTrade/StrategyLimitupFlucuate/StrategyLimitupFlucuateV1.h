#pragma once
#include "..\HSQuanTrade\StrategyBase.h"
#include "..\hsquantrade\typedefine.h"
#include<vector>
#include <map>

class CStrategyLimitupFlucuate :
	public CStrategyBase
{
public:
	CStrategyLimitupFlucuate(void);
	~CStrategyLimitupFlucuate(void);

	virtual void OnSubcode();
	virtual void  Tick(CTickData * pt);
	virtual void BackTest(CKStrdata *pk, const char* sCode);
	virtual CStrategyBase * NewInstance() { 
		
		CStrategyBase* pStrategy = new CStrategyLimitupFlucuate();
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

	virtual void BeginRun();

	virtual void OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price);

	virtual void OnErrorRtn(int code,char * errormsg) ;

	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins) ;

	virtual int CloseAll(char* szInstrument, double dbPrice);
	virtual void CreateStatisticObject();
	virtual void reload();                //重新加载

	virtual void saveModel() ;             //保存模型

protected:
	CUIntArray m_listOrderRef;
	std::mutex m_CheckSelfMutex;
	void AddOrderInfo(int ref,int count,double price,char bs,char kp, const char* szCode);
	/*double getPreDayVolume(const char* szCurDate);
	double getPreDayHighPrice(const char* szCurDate);*/

	bool GetDayKData(const char* szCode, const char* szCurDate, CKStrdata &oKData);
	//bool GetPreNDayKData(const char* szCurDate, CKStrdata &oKData, int iDay);
	bool GetPreNDayKData(const char* szCode, const char* szCurDate, CKStrdata &oKData, int iDay);
	int CalcPositionDays(time_t tOpen, time_t tNow);
	bool isExistRef(int ref,char bs,char kp,char * ins);
	double m_dbPreVolume;

//	double m_dbAvgPrice;


	std::map<string, double> m_mapHighPrice;

	double GetHighPrice(const char* szCode);
	void SetHighPrice(const char* szCode, double dbPrice);

	double GetOrderPrice(CTickData *pt, double dbSigPrice);

	std::map<string, int> m_arCodeSubs;

public:
	double m_loseRate;              //止损率
	double m_profitRate;			//止盈率

	double m_downRate1;			    //跌破收盘价比率1
	double m_downRate2;			    //跌破收盘价比率2

	double m_changeRate;            //换手率

	double m_dbShiftLost;			//移动止损

	int m_iStockType;				//涨停类别，0,昨日涨停；1，连续一字涨停
	int m_bAboveIndexMa20;			//20日均线之上





};

