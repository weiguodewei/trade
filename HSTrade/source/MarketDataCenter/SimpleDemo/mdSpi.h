#include "HundsunMdCenterApi.h"
#include <iostream>
#include <string>

using namespace std;

class CMdSpi : public CHundsunMdCenterSpi
{
public:
	CMdSpi(CHundsunMdCenterApi* lpObj)
	{
		lpUserApi = lpObj;
		m_RequestID = 0;
	}

	bool IsErrorRspInfo(CHundsunFtdcRspInfoField *pRspInfo);
private:
	CHundsunMdCenterApi* lpUserApi;
	int m_RequestID;
public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    virtual void OnFrontConnected();

    ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    virtual void OnFrontDisconnected(int nReason);

	///订阅行情应答
	virtual void OnRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///深度行情通知
	virtual void OnRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData);

	///K线行情通知
	virtual void OnRtnKLineData(CHundsunKLineDataField *pKLineData) {};
};
