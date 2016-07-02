#include "HundsunMdCenterApi.h"
#include "public.h"

class CDBSpi;

class CMdSpi : public CHundsunMdCenterSpi
{
public:
	CMdSpi(CHundsunMdCenterApi* lpObj, CDBSpi* lpDb)
	{
		lpUserApi = lpObj;
		lpUserDBSpi = lpDb;
		m_RequestID = 0;
	}

	bool IsErrorRspInfo(CHundsunFtdcRspInfoField *pRspInfo);

	void SubAll();
private:
	CHundsunMdCenterApi* lpUserApi;
	CDBSpi* lpUserDBSpi;
	int m_RequestID;

	//合约信息
	map<pair<int, string>, CHundsunFtdcInstrumentField*> m_InstrumentMap; 

	//获得合约信息
	CHundsunFtdcInstrumentField* GetInstrument(THundsunExchangeType ExchangeID, THundsunFtdcInstrument pszInstrument);
	//更新合约信息
	void UpdateInstrument(CHundsunFtdcInstrumentField *pInstrument);
public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    virtual void OnFrontConnected();

    ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    virtual void OnFrontDisconnected(int nReason);

	///登录请求响应
	virtual void OnRspUserLogin(CHundsunFtdcRspUserLoginField *pRspUserLogin, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///请求查询合约响应
	virtual void OnRspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅行情应答
	virtual void OnRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///深度行情通知
	virtual void OnRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData);

	///K线行情通知
	virtual void OnRtnKLineData(CHundsunKLineDataField *pKLineData) {};
};
