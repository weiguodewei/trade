#pragma once
#include "HundsunTraderApi.h"


class CBaseTradeApi;

class CUFXTraderSpiEx : public CHundsunTraderSpi
{
public:
	CUFXTraderSpiEx(CHundsunTraderApi* lpObj)
	{
		lpUserApi = lpObj;
		m_RequestID = 0;
		m_orderref = 0;
	}

	bool IsErrorRspInfo(CHundsunRspInfoField *pRspInfo);
	int ReqClientLogin(char* szFundCount, char* szPassword, char cEntrustWay);
	int ReqFutuContractbailQry();
	int ReqOrderinsert(char* szExchType, char* szFutuCode, char cEntrustBS, char cEntrustDirection, char cHedgeType, int nEntrustAmount, double dEntrustPrice);
	int ReqFuncOrderQry();
	int ReqFuncWithdraw(int EntrustNo);
	int ReqFuncQueryCode();
	int ReqFuncQueryArbCode();
	int ReqArbOrderinsert(char* szExchType, char* szFutuCode, char cEntrustBS, char cEntrustDirection, char cHedgeType, int nEntrustAmount, double dEntrustPrice);

	bool IsMyOrder(int sessionid);


private:
	CHundsunTraderApi* lpUserApi;
	int m_RequestID;
	int m_SessionId;
public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnFrontDisconnected(int nReason);
	//331100 客户登录应答
	virtual void OnRspFuncClientLogin(CHundsunRspFuncClientLoginField *pfunc_client_login, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//330851 期货合约保证金查询应答
	virtual void OnRspFuncFutuContractbailQry(CHundsunRspFuncFutuContractbailQryField *pRspfunc_futu_contractbail_qry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//338202 期货委托确认应答
	virtual void OnRspFuncOrderinsert(CHundsunRspFuncOrderinsertField *pRspFuncOrderinsert, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338301 期货当日委托查询应答
    virtual void OnRspFuncOrderQry(CHundsunRspFuncOrderQryField *pRspFuncOrderQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//620003_33101主推-期货委托回报应答
	virtual void OnRtnFuncQhEntrustPush(CHundsunRtnFuncQhEntrustPushField *pFuncQhEntrustPush);
	//620003_33101主推-期货成交回报应答
	virtual void OnRtnFuncQhRealPush(CHundsunRtnFuncQhRealPushField *pFuncQhRealPush);
	///330853 组合代码查询应答
	virtual void OnRspFuncArginstrumentQry(CHundsunRspFuncArginstrumentQryField *pRspFuncArginstrumentQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338204 组合委托确认应答
    virtual void OnRspFuncArgorderinsert(CHundsunRspFuncArgorderinsertField *pRspFuncArgorderinsert, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	char m_szBroker[16];

	char m_szUserID[16];

	char m_szPwd[16];

	CBaseTradeApi * m_pTrade;

	int m_orderref;

};
