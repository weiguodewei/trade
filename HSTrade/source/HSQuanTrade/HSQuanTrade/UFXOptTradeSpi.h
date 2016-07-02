#pragma once
#include "HundsunTraderApi.h"
#include "public.h"

class CBaseTradeApi;

class CUFXOptTraderSpi : public CHundsunTraderSpi
{
public:
	CUFXOptTraderSpi(CHundsunTraderApi* lpObj)
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

	int ReqFuncOptOptcodeQry();
	int ReqFuncOptEnTrade(char* szExchType, char* szOptionCode, char cEntrustBS, char cEntrustOC, double dEntrustPrice);
	int ReqFuncOptEntrust(char* szExchType, char* szOptionCode, char cEntrustBS, char cEntrustOC, int nEntrustAmount, double dEntrustPrice);
	int ReqFuncOptWithdraw(int EntrustNo);
	int ReqFuncOptEntrustQry();
	int ReqFuncOptRealtimeQry();
	int ReqFuncOptEntrustSubscribe();
	int ReqFuncOptDealSubscribe();
private:
	CHundsunTraderApi* lpUserApi;
	int m_RequestID;
public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	void OnFrontConnected();
	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	void OnFrontDisconnected(int nReason);
	//331100 客户登录应答
	void OnRspFuncClientLogin(CHundsunRspFuncClientLoginField *pfunc_client_login, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//330851 期货合约保证金查询应答
	void OnRspFuncFutuContractbailQry(CHundsunRspFuncFutuContractbailQryField *pRspfunc_futu_contractbail_qry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//338202 期货委托确认应答
	void OnRspFuncOrderinsert(CHundsunRspFuncOrderinsertField *pRspFuncOrderinsert, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338301 期货当日委托查询应答
    void OnRspFuncOrderQry(CHundsunRspFuncOrderQryField *pRspFuncOrderQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//620003_33101主推-期货委托回报应答
	void OnRtnFuncQhEntrustPush(CHundsunRtnFuncQhEntrustPushField *pFuncQhEntrustPush);
	//620003_33101主推-期货成交回报应答
	void OnRtnFuncQhRealPush(CHundsunRtnFuncQhRealPushField *pFuncQhRealPush);
	///338000 期权代码信息查询应答
	void OnRspFuncOptOptcodeQry(CHundsunRspFuncOptOptcodeQryField *pRspFuncOptOptcodeQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338010 期权可交易数量获取应答
	void OnRspFuncOptEnTrade(CHundsunRspFuncOptEnTradeField *pRspFuncOptEnTrade, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338011 期权委托应答
	void OnRspFuncOptEntrust(CHundsunRspFuncOptEntrustField *pRspFuncOptEntrust, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338012 期权撤单应答
	void OnRspFuncOptWithdraw(CHundsunRspFuncOptWithdrawField *pRspFuncOptWithdraw, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338020 期权委托查询应答
	void OnRspFuncOptEntrustQry(CHundsunRspFuncOptEntrustQryField *pRspFuncOptEntrustQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///338021 期权成交查询应答
	void OnRspFuncOptRealtimeQry(CHundsunRspFuncOptRealtimeQryField *pRspFuncOptRealtimeQry, CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
    //620003_33012 主推-个股期权委托回写应答
	void OnRtnFuncOptEntrustPush(CHundsunRtnFuncOptEntrustPushField *pFuncOptEntrustPush);
	//620003_33011 主推-个股期权成交回报应答
	void OnRtnFuncOptDealPush(CHundsunRtnFuncOptDealPushField *pFuncOptDealPush);
	//620001_33011 订阅-个股期权成交回写应答
    void OnRspFuncOptDealSubscribe(CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//620001_33012 订阅-个股期权委托回写应答
    void OnRspFuncOptEntrustSubscribe(CHundsunRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	char m_szBroker[16];

	char m_szUserID[16];

	char m_szPwd[16];

	CBaseTradeApi * m_pTrade;

	int m_orderref;

};
