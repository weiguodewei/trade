#if !defined(HUNDSUNMDCENTERAPI_IMPL_H)
#define HUNDSUNMDCENTERAPI_IMPL_H

#define LIB_MD_CENTER_API_EXPORT
#define ISLIB

#include "HundsunMdCenterApi.h"
#include "public.h"
#include "dbf_impl.h"
#include "h5_impl.h"
#include "HundsunH5sdkApi.h"
#include "HundsunFutuMdApi.h"

class CMarketDataSpi;

///行情
class CHundsunMdCenterApi_impl : public CHundsunMdCenterApi
{
public:
    ///默认构造函数
    CHundsunMdCenterApi_impl(const char* pszFlowPath);

    ///析构函数
    ~CHundsunMdCenterApi_impl();

    ///删除接口对象本身
    virtual void ReleaseApi();

    ///初始连接
    virtual void Init(THundsunMarkerDataType MDType);

    ///等待接口线程结束运行
    virtual int Join();

	///注册DBF文件地址(适用证券行情)
	///@param pszFilePath：DBF路径
	virtual void RegisterDBFPath(const char *pszFilePath);

	///注册H5
	///@param pszLicFile：H5证书文件
	///@param pszCfgFile：配置文件
	///@param bSubLevel2：是否level2订阅
	virtual void RegisterH5(const char *pszLicFile, const char *pszCfgFile, bool bSubLevel2 = false);

	///注册T2通讯配置
	///@param pszLicFile：T2通讯证书。
	///@param pszPwd：safe_level = pwd，则需要设置通讯密码。
	///@param pszSslFile：safe_level = ssl，ssl证书。
	///@param pszSslPwd：ssl证书密码。
	virtual void RegisterFutu(const char *pszLicFile, const char *pszPwd = "", const char *pszSslFile = "", const char *pszSslPwd = "");

    ///注册前置机网络地址
    ///@param pszFrontAddress：前置机网络地址。
    virtual void RegisterFront(const char *pszFrontAddress);

    ///注册回调接口
    ///@param pSpi 派生自回调接口类的实例
    virtual void RegisterSpi(CHundsunMdCenterSpi *pSpi);

	///用户登录请求
	virtual int ReqUserLogin(CHundsunFtdcReqUserLoginField *pReqUserLoginField, int nRequestID);

	///请求查询合约
	virtual int ReqQryInstrument(CHundsunFtdcQryInstrumentField *pQryInstrument, int nRequestID);

	///订阅行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	virtual int SubscribeMarketData(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount);

	///退订行情。
	///@param ppInstrumentID 合约ID  
	///@param nCount 要订阅/退订行情的合约个数
	///@remark 
	virtual int UnSubscribeMarketData(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount);

public:
	//获得上层回调对象指针
	CHundsunMdCenterSpi* GetMdCenterSpi();

	//设置期货连接是否成功
	void SetFutuConnected(bool bvl);
	//设置期货登录是否成功
	void SetFutuLogined(bool bvl);
private:
    //回调类指针
    CHundsunMdCenterSpi*      lpMdSpi;
	//证券行情类型
	THundsunMarkerDataType    m_StockMDType;
	//期货行情类型
	THundsunMarkerDataType    m_FutuMDType;

	//DBF文件路径
	char                      szDBFPath[1024];
	//DBF对象（只能唯一）
	CDBF_Impl*                lp_DBF_Impl;

	//H5证书文件路径
	char                      szH5Path_LicFile[1024];
	//H5配置文件路径
	char                      szH5Path_CfgFile[1024];
	//H5是否使用level2订阅
	bool                      bH5_SubLevel2;
	//H5对象（只能唯一）
	CH5_Impl                  m_H5_Impl;

	//H5sdk对象
	CHundsunH5sdkApi*         lp_H5sdk_Impl;

	//Futu通讯证书
	char                      szFutu_LicFile[1024];
	//Futu通讯密码
	char                      szFutu_Pwd[1024];
	//Futu_ssl证书
	char                      szFutu_SslFile[1024];
	//Futu_ssl证书密码
	char                      szFutu_SslPwd[1024];
	//Futu对象
	CHundsunFutuMdApi*        lpFutuMD;
	//是否连接成功
	bool                      bFutu_Connected;
	//是否登录登录成功
	bool                      bFutu_Logined;

	//前置地址
	char                      szFrontAddress[1024];

	//行情回调对象指针
	CMarketDataSpi*           lpMarketDataSpi;

	//合约信息
	map<THundsunExchangeType, CHundsunFtdcInstrumentField*> m_InstrumentMap; 

    string                    m_localip;        //从t2sdk中再获得一次真实的外网ip
    string                    m_localMac;       //本地mac地址
    
    bool                      m_SaveLog;        //是否记录日志
    CEventEx                  hJoinEvent;       //事件用来事件Join
    ManualLock                m_SubSetCs;       //订阅信息锁
    int                       m_iHeartKeep;     //心跳超时计数器
    CThread                   m_HeartKeepTrd;   //维护心跳线程
    CEventEx                  m_HeartKeepEvent; //心跳事件
    bool volatile             m_bHeartKeepStop; //是否停止心跳线程，跳出循环

};

#endif
