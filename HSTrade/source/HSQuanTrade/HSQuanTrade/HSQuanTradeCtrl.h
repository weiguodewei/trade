#pragma once
#include "stragegymanager.h"
#include "TradeInterface.h"
#include "CKdata.h"
#include "FuncDeal.h"
// HSQuanTradeCtrl.h : CHSQuanTradeCtrl ActiveX 控件类的声明。


// CHSQuanTradeCtrl : 有关实现的信息，请参阅 HSQuanTradeCtrl.cpp。

#define BACKTEST_TRADE_RESULT 0
#define BACKTEST_EQUITY_RESULT 1

class CHSQuanTradeCtrl : public COleControl
{
	DECLARE_DYNCREATE(CHSQuanTradeCtrl)

// 构造函数
public:
	CHSQuanTradeCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CHSQuanTradeCtrl();

	DECLARE_OLECREATE_EX(CHSQuanTradeCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CHSQuanTradeCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CHSQuanTradeCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CHSQuanTradeCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();
	afx_msg LRESULT OnTradeMessage(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnEntrustMessage(WPARAM wp,LPARAM lp);
	afx_msg LRESULT OnSockMsg(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnBackTestResultMessage(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBackTestStep(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSubCode(WPARAM wParam, LPARAM lParam);
// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
		dispidSetStrategyContext = 22L,
		dispidGetStrategyContext = 21L,
		dispidBackTestStatus = 20,
		eventidBackTestNotify = 2L,

		dispidRunStockFilter = 19L,
		dispidSetStockFilterStragegyParam = 18L,
		dispidGetStockFilterStragegyParamInfo = 17L,
		dispidRunStockFilterStragegy = 16L,
		dispidGetAllStockFilterName = 15L,
		dispidStragegyBackTest = 14L,
		dispidGetStragyTradeSignalList = 13L,
		dispidGetStragyLogInfo = 12L,
		dispidGetParamValue = 11L,
		dispidExit = 10L,
		dispidInit = 9L,
		eventidTradeNotify = 1L,
		dispidGetStrategyStatusInfo = 8L,
		dispidRunStrategy = 7L,
		dispidPauseStrategy = 6L,
		dispidStopStrategy = 5L,
		dispidAddRunStrategy = 4L,
		dispidSetStrategyParam = 3L,
		dispidGetStrategyParamInfo = 2L,
		dispidGetAllStrategy = 1L
	};
protected:
	BSTR GetAllStrategy(LONG param1, LPCTSTR param2);
	BSTR GetStrategyParamInfo(LPCTSTR strategyname);
	BSTR SetStrategyParam(LPCTSTR modelname,LPCTSTR paramname, LPCTSTR paramvalue);
	BSTR AddRunStrategy(LPCTSTR paramname);
	BSTR StopStrategy(LPCTSTR strategynname);
	BSTR PauseStrategy(LPCTSTR strategyname);
	BSTR RunStrategy(LPCTSTR strategyname);
	BSTR GetStrategyStatusInfo(LPCTSTR strategy);

	void TradeNotify(LONG pamam1, LONG param2, LPCTSTR param3, LPCTSTR param4)
	{
		FireEvent(eventidTradeNotify, EVENT_PARAM(VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR), pamam1, param2, param3, param4);
	}

	void BackTestNotify(LPCTSTR sCode, LONG lParam1, LONG lParam2, LONG lParam3, LONG lParam4)
	{
		FireEvent(eventidBackTestNotify, EVENT_PARAM(VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4), 
			sCode, lParam1, lParam2, lParam3, lParam4);
	}
	
	BSTR Init(LPCTSTR param1);
	BSTR Exit(void);
public:
	CStrategyManager m_strategy;
	CTradeInterface  m_tradeinterface;
	//CKdataManager m_k; //move to StrageyManager
	CStockInfo m_codeinfo;
	CRequest m_Req;    //专门处理请求
protected:
	BSTR GetParamValue(LPCTSTR modelname, LPCTSTR paramname);
	BSTR GetStragyLogInfo(LPCTSTR modelname);
	BSTR GetStragyTradeSignalList(LPCTSTR modelname);
	BSTR StragegyBackTest(LPCTSTR name, LPCTSTR paramlist, LONG time1, LONG time2, LPCTSTR coderanger, LPCTSTR sCycle);
	BSTR GetAllStockFilterName(void);
	BSTR RunStockFilterStragegy(LPCTSTR name, LPCTSTR paramfield, LPCTSTR paramvalue);
	BSTR GetStockFilterStragegyParamInfo(LPCTSTR name);
	BSTR SetStockFilterStragegyParam(LPCTSTR field, LPCTSTR value);
	BSTR RunStockFilter(LPCTSTR name, LPCTSTR field1, LPCTSTR value1);

	
	LONG GetBackTestStatus(void);
	void SetBackTestStatus(LONG newVal);
	BSTR GetStrategyContext(LPCTSTR sStrategyId);
	BSTR SetStrategyContext(LPCTSTR sStrategyId, LPCTSTR sContext);
};

