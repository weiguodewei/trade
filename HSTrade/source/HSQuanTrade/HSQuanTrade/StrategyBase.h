#pragma once
#include "typedefine.h"
#include <mutex>
#include<map>
#include<string>
#include "data.h"



using namespace std;

#include <vector>
#include "CinfoLogger.h"
#include "CKdata.h"
#include "TradeStatistic2.h"
#include "StragegyManager.h"
#include "FundManager.h"
#include "EasyThread.h"

#define STRATEGY_STATUS_INIT 0

#define STRATEGY_STATUS_BK   1

#define STRATEGY_STATUS_SK   2

#define STRATEGY_STATUS_WAIT_BK 3

#define STRATEGY_STATUS_WAIT_SK 4

#define STRATEGY_STATUS_WAIT_CANCEL 5
#define STRATEGY_STATUS_CANCEL_DONE 6

#define  STRATEGY_STATUS_WAIT_BP 7
#define  STRATEGY_STATUS_WAIT_SP 8

#define CLOSEFLAG_CLOSE        '1'

#define CLOSEFLAG_CLOSE_TODAY  '3'

#define WM_MESSAGE_SUBCODE WM_USER + 1006



class CTradeInterface;

class CStrategyBase
{
public:

	CStrategyBase(void);

	virtual ~CStrategyBase(void);

public:

	CString m_strStrategyName;            //策略名称

	CString m_strInstrument;              //合约

	CString m_strParamList;               //参数列表

	//CString m_strModelName;               //模型名称

	int     m_nCount;                      //数量	

	int     m_nCircleType;

	int		m_nStrategyStatus;             //策略状态

	double   m_fStopLossPrice;             //止损价

	double   m_fStopProfitPrice;           //止盈价

	double   m_fOpenPrice;                 //开仓价

	double   m_fyingkuiPosition;           //浮动盈亏

	double   m_lastPrice;                  //最新价      

	double   m_dbBeginQuanyi;

	double	 m_dbCostRate;				  //手续费率

                                   
	CString m_fundAccount;                 //帐号

	CString m_follow;                      //下单方式

	int m_timeOut;                         //超时时间

	double m_jiacha;					       //价差

	double m_hignPrice;                    //最高价


	bool    m_bRun;                        //运行

	bool    m_Exit;                        //退出

	int		m_iPriceOrderType;				   //0, 指令价；1，收盘价

	map<int,COrderInfo> m_orderinfo;                         //策略指令信息



	vector<string> m_listLog;                                //日志

	vector<string> m_listSignal;                             //信号

	std::mutex m_LockMutex;


	std::map<std::string,CString> m_params;                  //便于参数的管理
	
	

	CAccData *data;                                             //资金 委托 持仓等信息

	CStockInfo *m_pStrategyCodeInfo;                         //合约信息获取

	CKdataManager *m_pKManager;

	

	CTradeStatistic2 *m_pStatistic;

	bool m_bBackTest;
	//int m_nSignalStatus;

	bool m_bRealTrade; //false, 只出信号不交易；true,有信号真实交易
	
	char m_szTickTime[32];

	void SetModuleId(CString sModuleId)
	{
		m_sModuleId = sModuleId;
	}
public:
	void TickPeek(CTickData * pt);
	void JoinAllTask();
	
	virtual void BackTest(CKStrdata *pk, const char* sCode);

	virtual CStrategyBase * NewInstance() = 0;

	virtual void SetParam(CString paramname,CString vlue)=0;

	virtual void SetParam(CString value);

	virtual CString GetParamValue() ;          //实际上获取参数具体值

	virtual CString GetParamValue(CString paramname);

	virtual CString GetStatusInfo(void);

	virtual void BeginRun()=0;

	virtual void Pause(){};

	virtual void OnTradeReturn(char * ins,char * time,int orderref,int ordersystemid,char direction,char offsetflag,int vol,double price)=0;

	virtual void OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins)=0;

	virtual void OnErrorRtn(int code,char * errormsg)=0;

	virtual void OnRspOrderInsert(int ref,char entrust_status,int errorid){};

	bool CheckPosition();

	int OpenBuy(char * instrument, int count, double price,const char * account, int follow = 0);

	int OpenSale(char * instrument, int count, double price,const char * account, int follow = 0);

	int CloseBuy(char * instrument, int count, double price,const char * account,char closeflag, int follow = 0);

	int CloseSale(char * instrument, int count, double price,const char * account,char closeflag, int follow = 0);

	virtual int CloseAll(char* szInstrument, double dbPrice){ return 0;};

	virtual void CreateStatisticObject()
	{
		m_pStatistic = new CTradeStatistic2();
	}

	void OrderAction(int orderref);

	void QueryPosition();

	void QueryOrder();

	void QueryTrade();

	void QueryFund();

	void AddLog(const char * log, int iMsgType = 0);



	CString GetLogInfo();

	void AddSignal(const char * signal);

	CString GetSignalInfo();



	virtual void reload(){};                //重新加载

	virtual void reload(CString strParam){};                //重新加载

	virtual void saveModel(){};             //保存模型
	
	//int ReadData(CString strCode, long lTimeBegin, long lTimeEnd);

	void SetWndHwnd(HWND hWnd);

	void SetStrategyManager(CStrategyManager* pStragyManager);
	CStrategyManager* GetStrategyManager();

	void SetBeginQuanyi(double dbQuanyi);

	bool ApplyForFund(double& dbFund);
	
	int GetPositionCount();
	
	
	int GetSignalStatus(const char* szCode);
	CFundPolicyBase* m_pPolicy;

	void SetStrategyId(CString sStrategyId)
	{
		m_sStrategyId = sStrategyId;
	}
	CString GetStrategyId()
	{
		return m_sStrategyId;
	}

	virtual void ResumePostionInfo(CString strPosInfo);
	virtual CString GetPostionInfo();
	void StartTick();
	void StartSubcode();

	HWND m_hMsgWnd;                                          //控件的句柄

	//std::map<std::string,FutuCodeInfo> * m_pMapFutuCodeInfo;
	//std::map<std::string,StockInfo>* m_pCodeInfo;
protected:

	virtual void  Tick(CTickData * pt)=0;

	std::vector<CTickData> m_vTicks;
	CEasyThread m_thdTickProcess;
	CMutex m_mtxTick;
	//bool m_bRunTick;
	static void Task_TickProcess(void *lpData);

	
	//bool m_bRunSubcode;
	CEasyThread *m_pThdSubcode;
	static void Task_Subcode(void *lpData);
	virtual void OnSubcode(){} //由StrategyManager 调用，策略如果有合约订阅需求则实现，如无则忽略
	

	CStrategyManager* m_pStrategyManager;


	

	
	std::map<string, int> m_mapSignalStatus;
	std::map<string, int> m_mapStockCount;
	//time_t m_dtLastTrade;
	std::map<string, time_t> m_mapOpenTime;
	std::map<string, double> m_mapAvgPrice;
	std::map<string,double> m_mapHighPrice;					//记录买入过后的最高价

	int GetStockCount(const char* szCode);
	void SetStockCount(const char* szCode, int iCount);

	double GetAvgPrice(const char* szCode);
	void SetAvgPrice(const char* szCode, double dbAvgPrice);
	
	double GetHgPrice(const char* szCode);
	void SetHgPrice(const char* szCode, double dbAvgPrice);
	
	void SetSignalStatus(const char* szCode, int iSignalStatus);

	time_t GetOpenPostionTime(const char* szCode);
	void SetOpenPostionTime(const char* szCode, time_t tOpen);

	CString m_sModuleId;

	CString m_sStrategyId;

	virtual bool IsRightTickTime(const char* szTime);

	char GetFutuCloseFlag(const char* szCode, const char* szOpenTime);
	char* GetFutuName(const char* szCode);
};







