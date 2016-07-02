#include "HundsunMdCenterApi.h"
#include "MarkDataTraderApi.h"
#include "public.h"

class CDBSpi : public CMarkDataTraderSpi
{
public:
	CDBSpi(CMarkDataTraderApi* lpObj)
	{
		lpUserApi = lpObj;
		m_RequestID = 0;
		bConnected = false;
	}

	bool IsErrorRspInfo(CMarkDataRspInfoField *pRspInfo);

	int MarkdataInsert(CHundsunDepthMarketDataField *pDepthMarketData);
private:
	CMarkDataTraderApi* lpUserApi;
	int m_RequestID;
	bool bConnected;
public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    virtual void OnFrontConnected();

    ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    virtual void OnFrontDisconnected(int nReason);

	///338499 行情回库应答
    virtual void OnRspFuncMarkdatainsert(CMarkDataRspFuncMarkdatainsertField *pRspFuncMarkdatainsert, CMarkDataRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
};
