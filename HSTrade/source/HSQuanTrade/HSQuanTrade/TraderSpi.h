#pragma once

#include "..\hsquantrade\include\ThostFtdcTraderApi.h"

#include <string.h>

#include <vector>

#include <atomic> 
#include<mutex>

using namespace std;

class CThostFtdcTraderApi;

struct CInsPosition;

class CBaseTradeApi;

class CTraderSpi : public CThostFtdcTraderSpi
{
public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField *pUserLogout, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///投资者结算结果确认响应
	virtual void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询资金账户响应
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询投资者持仓响应
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单录入请求响应
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///报单操作请求响应
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnFrontDisconnected(int nReason);
		
	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	virtual void OnHeartBeatWarning(int nTimeLapse);
	
	///报单通知
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);

	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);

	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField *pOrderAction, CThostFtdcRspInfoField *pRspInfo);

	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///合约交易状态通知
	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField *pInstrumentStatus);

	///请求查询合约保证金率响应
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

private:
	///用户登录请求
	void ReqUserLogin();
	///用户登出请求
	int ReqUserLogout();
	///投资者结算结果确认
	void ReqSettlementInfoConfirm();
	///请求查询合约
	void ReqQryInstrument();	
	

	///报单录入请求	
	// 是否收到成功的响应
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);
	// 是否我的报单回报
	bool IsMyOrder(CThostFtdcOrderField *pOrder);
	// 是否正在交易的报单
	bool IsTradingOrder(CThostFtdcOrderField *pOrder);

public:
	void ReqOrderAction(CThostFtdcOrderField *pOrder);

	int ReqOrderAction(int orderid);

	int ReqOrderAction(int orderid,char *sCode);

	int ReqOrderInsert(char * szHeyue,TThostFtdcOffsetFlagType flagtype,TThostFtdcDirectionType dir,double price,int count,TThostFtdcTimeConditionType timecondition,char fakfok);

	//std::atomic_int m_iNextOrderRef;

	//改为全局静态变量，因为多个账户同时登录的情况，会存在报单引用重复的情况
	//现在改为统一用tradeinterface的计数,确保每个接口不重复
	static std::atomic_int m_iNextOrderRef;

	static bool bHaveQryInstrument;

	vector<CInsPosition *> m_listPos;

	///请求查询投资者持仓
	void ReqQryInvestorPosition();

	char m_szBroker[16];

	char m_szUserID[16];

	char m_szPwd[16];

	int m_iRequestID;

	CThostFtdcTraderApi* m_pUserApiTrade;

	char m_szInstrumentID[32];

	///请求查询资金账户
	void ReqQryTradingAccount();

	void ReqQryOrder(int orderref);

	void ReqQryInvestFee(void);

	void ReqQryInvestMargin(void);

	int m_nOrderRefOffset;

	bool m_bLogin;

	CThostFtdcInputOrderField m_reqOrderIndset;

	CThostFtdcInputOrderActionField m_reqOrderAction;

	void Init(void);

	void ReqQryTrade(int orderref);

	int m_nOrderRef;

	int m_nTradeQry;

	CBaseTradeApi * m_pTrade;

	///用户从新发起登录
	void ReLogin();

	int LogOut();
	///登录相关

	HANDLE m_hThread;

	static unsigned int _stdcall ThreadRtnProcess(void * pt);

	char m_offsetflag;

	char m_direction;

	int m_ordersysid;

	int m_orderref;

	TThostFtdcFrontIDType	FRONT_ID;	//前置编号

	TThostFtdcSessionIDType	SESSION_ID;	//会话编号
	
};