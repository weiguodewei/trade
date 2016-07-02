#include "HundsunH5sdkApi.h"
#include "public.h"
#include "boost\date_time\gregorian\gregorian.hpp"

using namespace boost::gregorian;

const char* GetHSExchagneType(const char* pszHqTypeCode);
const char* GetCandlePeriodName(int iCandlePeriod);

class CH5sdkSpi : public CHundsunH5sdkSpi
{
public:
	CH5sdkSpi(CHundsunH5sdkApi* lpObj)
	{
		lpUserApi = lpObj;
		lpLog = NULL;

		m_RequestID = 0;
		b_Query = false;
		memset(szHqTypeCode, 0, sizeof(szHqTypeCode));
		memset(szProdCode, 0, sizeof(szProdCode));
		memset(szQueryDate, 0, sizeof(szQueryDate));
		iCandlePeriod = 0;
	}

	bool IsErrorRspInfo(CHundsunH5sdkRspInfoField *pRspInfo);

	int ReqQryMarket();
	int ReqQryInstrument();

	int ReqH5sdkSnapshot();

	int ReqH5sdkSubscribe();

	int ReqH5sdkTrend();
	int ReqH5sdkCandleByRange();
	int ReqH5sdkTickMin();
	
	bool GetQueryStatus();

	void SetQueryStatus(bool val);
	void SetHqTypeCode(const char* pszHqTypeCode);
	void SetProdCode(const char* pszProdCode);
	void SetCandlePeriod(int val);
	void SetQueryDate(const char* pszDate);
private:
	CHundsunH5sdkApi* lpUserApi;
	CLogInfo* lpLog;

	int m_RequestID;
	bool b_Query;
	char szHqTypeCode[16];
	char szProdCode[32];
	char szQueryDate[9];
	int  iCandlePeriod;
public:
    ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    virtual void OnH5sdkFrontConnected();

    ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    virtual void OnH5sdkFrontDisconnected(int nReason);

	///1001 登入请求应答
    virtual void OnRspH5sdkLogin(CHundsunH5sdkRspLoginField *pRspLogin, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///2002 市场分类信息应答
    virtual void OnRspH5sdkMarketTypes(CHundsunH5sdkRspMarketTypesField *pRspMarketTypes, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
    ///2003 市场代码表应答
    virtual void OnRspH5sdkMarketReference(CHundsunH5sdkRspMarketReferenceField *pRspMarketReference, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///3001 行情快照应答
    virtual void OnRspH5sdkSnapshot(CHundsunH5sdkRspSnapshotField *pRspSnapshot, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///4001 行情快照订阅应答
    virtual void OnRspH5sdkSubscribe(CHundsunH5sdkRspSubscribeField *pRspSubscribe, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///6001 按指定的日期或偏移取分时数据应答
    virtual void OnRspH5sdkTrend(CHundsunH5sdkRspTrendField *pRspTrend, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///6003 按日期时间范围取K线应答
    virtual void OnRspH5sdkCandleByRange(CHundsunH5sdkRspCandleByRangeField *pRspCandleByRange, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///6004 指定分钟的分笔应答
    virtual void OnRspH5sdkTickMin(CHundsunH5sdkRspTickMinField *pRspTickMin, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

};


