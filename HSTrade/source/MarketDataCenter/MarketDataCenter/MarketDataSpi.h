/**********************************************************************
* 源程序名称: MarketDataSpi.h
* 跟踪日志:
* 2015-10-25 创建
**********************************************************************/

#if !defined(MARKETDATASPI_H)
#define MARKETDATASPI_H

#include "MarketDataCenter_impl.h"

class CMarketDataSpi : public CHundsunDBFSpi, public CHundsunH5Spi, public CHundsunH5sdkSpi, public CHundsunFutuMdSpi
{
public:
	CMarketDataSpi(CHundsunMdCenterApi_impl* pApi)
	{
		lpApi = pApi;
	}
private:
	CHundsunMdCenterApi_impl* lpApi;
public:
	//////////////////////////////////////////////DBF///////////////////////////////////////////////
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnDBFConnected();

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnDBFDisconnected(int nReason){};

	///订阅行情应答
	virtual void OnDBFRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnDBFRspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///深度行情通知
	virtual void OnDBFRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData);

	//////////////////////////////////////////////H5///////////////////////////////////////////////
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnH5Connected(int iMktType);

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnH5Disconnected(int nReason){};

	///请求查询合约响应
	virtual void OnH5RspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///订阅行情应答
	virtual void OnH5RspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///取消订阅行情应答
	virtual void OnH5RspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///深度行情通知
	virtual void OnH5RtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData);

	//////////////////////////////////////////////H5SDK///////////////////////////////////////////////
	    ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    virtual void OnH5sdkFrontConnected();

    ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    virtual void OnH5sdkFrontDisconnected(int nReason){};

	//////////////////////////////////////////////Futu///////////////////////////////////////////////
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnFrontDisconnected(int nReason){};

	///331100 客户登录应答
	virtual void OnRspFuncClientLogin(CHundsunFutuRspFuncClientLoginField *pRspFuncClientLogin, CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///330851 期货合约保证金查询应答
    virtual void OnRspFuncFutuContractbailQry(CHundsunFutuRspFuncFutuContractbailQryField *pRspFuncFutuContractbailQry, CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///620001_33100 订阅-期货单腿行情应答
	virtual void OnRspFuncQhSingleHqSubscribe(CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	///620002_33100 订阅取消-期货单腿行情应答
	virtual void OnRspFuncQhSingleHqCancel(CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///620003_33100 主推-期货单腿行情应答
	virtual void OnRtnFuncQhSingleHqPush(CHundsunFutuRtnFuncQhSingleHqPushField *pFuncQhSingleHqPush);

	///620001_33102 订阅-期货组合行情应答
	virtual void OnRspFuncQhCombineSubscribe(CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///620002_33102 订阅取消-期货组合行情应答
	virtual void OnRspFuncQhCombineCancel(CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///620003_33102 主推-期货组合行情应答
	virtual void OnRtnFuncQhCombinePush(CHundsunFutuRtnFuncQhCombinePushField *pFuncQhCombinePush) {};
};

#endif

