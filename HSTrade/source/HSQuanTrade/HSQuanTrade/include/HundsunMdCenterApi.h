/**********************************************************************
* 源程序名称: HundsunMdCenterApi.h
* 使用建议：
*   -->使用DBF行情源：
*      1.调用RegisterDBFPath，注册本地DBF路径
*      2.调用Init，且参数传入HUNDSUN_FTDC_DBF
*   -->使用H5行情源：
*      1.调用RegisterH5，注册H5需要的证书+配置
*      2.调用Init，且参数传入HUNDSUN_FTDC_H5
*   -->使用Futu行情源：
*      1.调用RegisterFutu，注册T2通讯属性
*      2.调用RegisterFront，注册前置地址
*      3.调用Init，且参数传入HUNDSUN_FTDC_Futu
*      4.调用ReqUserLogin之后，才能调用ReqQryInstrument
* 跟踪日志:
* 2015-10-09 创建
* 2015-10-25 优化实现方式，DBF、H5实现代码可单独使用
* 2015-11-07 增加期货UFX行情获取
* 2015-11-20 兼容证券期货行情同时订阅的功能
* 2016-01-11 增加证券查询合约信息功能，IsLast=True时，参数=NULL。
* 2016-01-13 增加期货查询合约信息功能，前提需要资金账号登录。
* 2016-01-22 增加支持H5订阅Level2行情。
**********************************************************************/

#if !defined(HUNDSUNMDCENTERAPI_H)
#define HUNDSUNMDCENTERAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HundsunMdCenterStruct.h"

#if defined(ISLIB)

#ifdef WIN32
#ifdef LIB_MD_CENTER_API_EXPORT
#define MD_CENTER_API_EXPORT __declspec(dllexport)
#else
#define MD_CENTER_API_EXPORT __declspec(dllimport)
#endif
#endif

#ifdef LINUX
#ifdef LIB_MD_CENTER_API_EXPORT
#define MD_CENTER_API_EXPORT __attribute__((visibility("default")))
#else
#define MD_CENTER_API_EXPORT
#endif
#endif

#else
#define MD_CENTER_API_EXPORT
#endif

///回调虚类
class CHundsunMdCenterSpi
{
public:
    ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    virtual void OnFrontConnected(){};

    ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    virtual void OnFrontDisconnected(int nReason){};

	///登录请求响应
	virtual void OnRspUserLogin(CHundsunFtdcRspUserLoginField *pRspUserLogin, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询合约响应
	virtual void OnRspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///订阅行情应答
	virtual void OnRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///取消订阅行情应答
	virtual void OnRspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	///深度行情通知
	virtual void OnRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData) {};

	///K线行情通知
	virtual void OnRtnKLineData(CHundsunKLineDataField *pKLineData) {};

};

///行情
class MD_CENTER_API_EXPORT CHundsunMdCenterApi
{
public:
    ///创建MdApi
    static CHundsunMdCenterApi *CreateHundsunMdCenterApi(const char *pszFlowPath = "");

    ///删除接口对象本身
    virtual void ReleaseApi() = 0;

    ///初始连接
    virtual void Init(THundsunMarkerDataType MDType) = 0;

    ///等待接口线程结束运行
    virtual int Join() = 0;

	///注册DBF文件地址(适用证券行情)
	///@param pszFilePath：DBF路径
	virtual void RegisterDBFPath(const char *pszFilePath) = 0;

	///注册H5(适用所有行情)
	///@param pszLicFile：H5证书文件
	///@param pszCfgFile：配置文件
	///@param bSubLevel2：是否level2订阅
	virtual void RegisterH5(const char *pszLicFile, const char *pszCfgFile, bool bSubLevel2 = false) = 0;

	///注册T2通讯配置
	///@param pszLicFile：T2通讯证书。
	///@param pszPwd：safe_level = pwd，则需要设置通讯密码。
	///@param pszSslFile：safe_level = ssl，ssl证书。
	///@param pszSslPwd：ssl证书密码。
	virtual void RegisterFutu(const char *pszLicFile, const char *pszPwd = "", const char *pszSslFile = "", const char *pszSslPwd = "") = 0;

    ///注册前置机网络地址
    ///@param pszFrontAddress：前置机网络地址。
    virtual void RegisterFront(const char *pszFrontAddress) = 0;

    ///注册回调接口
    ///@param pSpi 派生自回调接口类的实例
    virtual void RegisterSpi(CHundsunMdCenterSpi *pSpi) = 0;

	///用户登录请求
	virtual int ReqUserLogin(CHundsunFtdcReqUserLoginField *pReqUserLoginField, int nRequestID) = 0;

	///请求查询合约
	virtual int ReqQryInstrument(CHundsunFtdcQryInstrumentField *pQryInstrument, int nRequestID) = 0;

	///订阅行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	virtual int SubscribeMarketData(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount) = 0;

	///退订行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	virtual int UnSubscribeMarketData(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount) = 0;

protected:
    ~CHundsunMdCenterApi(){};
 };

#endif
