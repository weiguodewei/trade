/**********************************************************************
* 源程序名称: h5_impl.h
* 跟踪日志:
* 2015-10-19 创建(从接口定义来看，一个进程只允许创建一个对象)
* 2015-10-25 优化为可单独使用
* 2016-01-11 增加查询合约信息
**********************************************************************/

#if !defined(H5_IMPL_H)
#define H5_IMPL_H

#include "h5hq_api_interface.h"
#include "HundsunMdCenterApi.h"
#include "public.h"

///H5回调虚类
class CHundsunH5Spi
{
public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnH5Connected(int iMktType){};

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnH5Disconnected(int nReason){};

	///请求查询合约响应
	virtual void OnH5RspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///订阅行情应答
	virtual void OnH5RspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///取消订阅行情应答
	virtual void OnH5RspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///深度行情通知
	virtual void OnH5RtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData) {};
};

class CH5_Impl
{
public:
	//获取公共指针
	CH5_Impl* GetInstance();

	///删除接口对象本身
	void ReleaseH5();

	///初始化
	int InitH5(const char *pszLicFile, const char *pszCfgFile, bool bSubLevel2 = false);

	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	void RegisterH5Spi(CHundsunH5Spi *pSpi);

	///请求查询合约
	int ReqQryInstrumentH5(CHundsunFtdcQryInstrumentField *pQryInstrument, int nRequestID);

	///订阅行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	int SubscribeH5(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount);

	///取消订阅
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	int UnSubscribeH5(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount);

public:
	//获得合约信息
	void GetH5MarketInfo(int iMktType);

	//处理行情主推
	void DealRecv(IStockInfo* lpStockInfo, int iResults, int* lpResultid);
private:
	//回调类指针
	CHundsunH5Spi*            lpH5Spi;
	//是否订阅level2行情
	bool                      m_SubLevel2;

	//合约信息
	map<pair<int, string>, CHundsunFtdcInstrumentField*> m_InstrumentMap; 
	//当前行情缓存
	map<pair<int, string>, CHundsunDepthMarketDataField*> m_DepthMDMap;

	//判断是否为被H5改为合约代码
	bool IsSpecialInstrument(THundsunFtdcInstrument pszInstrument);

	//从IStockInfo获取行情数据
	void GetH5StockInfo(IStockInfo* pStockInfo, CHundsunDepthMarketDataField *pDepthMarketData, int iSetID = -1);
	//更新基本报价
	void UpdateSimpleQuote(IStockInfo* pStockInfo, CHundsunDepthMarketDataField *pDepthMarketData);
	//更新买卖档位信息
	void UpdateBidoff(IStockInfo* pStockInfo, CHundsunDepthMarketDataField *pDepthMarketData);

	//获得合约信息
	CHundsunFtdcInstrumentField* GetInstrument(THundsunExchangeType ExchangeID, THundsunFtdcInstrument pszInstrument);
	//获得当前行情
	CHundsunDepthMarketDataField* GetDepthMD(THundsunExchangeType ExchangeID, const char* pszInstrument);

	//更新合约信息
	void UpdateInstrument(CHundsunFtdcInstrumentField *pInstrument);
	//更新当前行情
	void UpdateDepthMD(CHundsunDepthMarketDataField *pDepthMarketData);
};

#endif
