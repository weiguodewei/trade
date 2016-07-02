/**********************************************************************
* 源程序名称: dbf_impl.h
* 跟踪日志:
* 2015-10-25 创建
**********************************************************************/

#if !defined(DBF_IMPL_H)
#define DBF_IMPL_H

#include "dbf.h"
#include "HundsunMdCenterApi.h"
#include "public.h"

///H5回调虚类
class CHundsunDBFSpi
{
public:
	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnDBFConnected(){};

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	virtual void OnDBFDisconnected(int nReason){};

	///订阅行情应答
	virtual void OnDBFRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///取消订阅行情应答
	virtual void OnDBFRspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///深度行情通知
	virtual void OnDBFRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData) {};
};

class CDBF_Impl
{
public:
	///构造
	CDBF_Impl()
	{
		bStopReadDBF = false;
	}

	///初始化
	int InitDBF(const char *pszDBFFile);

	///注册回调接口
	///@param pSpi 派生自回调接口类的实例
	void RegisterDFBSpi(CHundsunDBFSpi *pSpi);

	///订阅行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	virtual int SubscribeDBF(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount);

	///退订行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	virtual int UnSubscribeDBF(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount);
private:
	//回调类指针
	CHundsunDBFSpi*           lpDBFSpi;

	//上海DBF文件路径
	char                      szPath_show2003[1024];
	//深圳DBF文件路径
	char                      szPath_sjshq[1024];
	//上海DBF对象
	CDBF                      m_dbf_show2003;
	//深圳DBF对象
	CDBF                      m_dbf_sjshq;
	//行情缓存
	map<pair<int, string>, CHundsunDepthMarketDataField*> HQMap;
	//订阅信息
	set<pair<int, string>>    SubInfoSet;

	//是否停止处理DBF
	bool                      bStopReadDBF;
	//处理行情线程
	CThread                   m_ReadDBFTrd;
	//处理行情
	static void               ReadDBF(void* lpvoid);

	//更新上海DBF文件
	void UpdateSH();
	//更新深圳DBF文件
	void UpdateSZ();

	//获得当前行情
	CHundsunDepthMarketDataField* GetHQ(THundsunExchangeType ExchangeID, const char* pszInstrument);
	//更新行情并且判断是否有新行情
	void UpdateHQ(THundsunExchangeType ExchangeID, const char* pszInstrument, CHundsunDepthMarketDataField *pDepthMarketData);
};

#endif


