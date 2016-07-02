#pragma once

#include "..\hsquantrade\strategybase.h"
#include <thread>
#include <mutex>
#include <map>

#define STOCK_TOTAL_MAX 1000000000

#define STOCK_TOTAL_MIN 200000000


class CStrategyBreakHigh : public CStrategyBase
{

public:

	CStrategyBreakHigh();

	~CStrategyBreakHigh();

	virtual void  Tick(CTickData * pt);

	virtual void BackTest(CKStrdata *pk, const char* sCode);

	//virtual CStrategyBase * NewInstance() { return new CStrategyBreakHigh(); };

	virtual CStrategyBase * NewInstance() { 
		
		CStrategyBase* pStrategy = new CStrategyBreakHigh();
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

	virtual void Pause();

	virtual void OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price);

	virtual void OnErrorRtn(int code,char * errormsg){} ;

	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins);

	virtual void reload();                //重新加载

	virtual void reload(CString strParam);

	virtual void saveModel() ;             //保存模型

	virtual int CloseAll(char* szInstrument, double dbPrice);

	virtual void CreateStatisticObject();

	virtual void OnSubcode(); 



/*
//	策略参数
*/

public:

	int m_iCycle;                                       //日线周期

	int m_iPriceDay;									//多少日新高

	double m_dFluctuateRange;							//波动幅度

	double m_dAbsoluteStop;                             //绝对止损

	double m_dMoveStop;									//移动止损

	double m_dStartFund;								//初始资金

	double m_dTradePrice;                               //成本价

	int m_iTradeCount;									//成交数量

	double m_preHignPrice;								//前N天最高价

	char m_sCurCode[7];									//当前合约

	//map<string,double> m_mapHighPrice;					//记录买入过后的最高价

	map<string,double> m_mapPreHighPrice;				//前N天的最高价

	map<string,int> m_mapIN;							//-1 表示当日之前周期不在振幅，0表示初始值，1表示满足振幅,-2表示当天开仓不能平仓

	int m_iInType;										//0新高买入，1涨停买入

	double m_dPrePriceS;								//满足条件的前一天的收盘价

	double m_dGet;										//止盈点

	//int m_xiaopan;										//是否买小盘股

	int m_junxian;										//是否看均线

/*
//	程序参数
//
*/
	vector<CKStrdata> m_v30KData;						//前30天的K线数据

	char m_sCurDate[9];									//当前日期

	bool m_bDaPan;										//大盘指数是否处于20日均线以上

	vector<string> m_vHold;						//保存持仓信息


public:
	int GetPreNDaySumKData(const char* szCurDate,vector<CKStrdata>*vk,int iDay);

	bool GetPreNDayKData(const char* szCurDate, CKStrdata &oKData, int iDay);

	bool CheckZhenFu(char *code,char *curDate);

	bool OrderBuy(char *code,double dPrice,double dStartFund);
	
	bool FilterSub(char *code);					//过滤不满足条件合约

	bool DaPan();								//20日均线

	void SetPreHgPrice(const char *code,double price);

	double GetPreHgPrice(const char *code);

	//重新订阅合约
	CEasyThread *m_pThdSubcode;

	void ReSubCode();

	static void Task_Subcode(void *lpData);



};