#pragma once
#include "HundsunMdCenterApi.h"
#include "public.h"
#include <atomic> 
#include <string>
#include "typedefine.h"

class CStrategyManager;

class CMdSpi : public CHundsunMdCenterSpi
{
public:
	CMdSpi(CHundsunMdCenterApi* lpObj)
	{
		lpUserApi = lpObj;
		m_RequestID = 0;
	}

	bool IsErrorRspInfo(CHundsunFtdcRspInfoField *pRspInfo);

	CStrategyManager*m_MdStrategy;

private:
	CHundsunMdCenterApi* lpUserApi;

	int m_RequestID;

	void SubscribeMarketData();

	//合约信息
	map<pair<int, string>, CHundsunFtdcInstrumentField*> m_InstrumentMap; 

public:

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    virtual void OnFrontConnected();

    ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    virtual void OnFrontDisconnected(int nReason);

	///订阅行情应答
	virtual void OnRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
	virtual void OnRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData);

	///K线行情通知
	virtual void OnRtnKLineData(CHundsunKLineDataField *pKLineData) {};

	///查询合约通知
	virtual void OnRspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);


public:

	char ** m_ppInstrumentID;

	std::atomic_int  m_nInsNum;

	std::map<string,int> m_UfxCode;				   //ufx接口订阅的合约

	char m_szDate[16];
};
