#if !defined(MARKDATATRADERAPI_H)
#define MARKDATATRADERAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include "MarkDataUserApiStruct.h"

#if defined(ISLIB)

#ifdef WIN32
#ifdef LIB_TRADER_API_EXPORT
#define TRADER_API_EXPORT __declspec(dllexport)
#else
#define TRADER_API_EXPORT __declspec(dllimport)
#endif
#endif

#ifdef LINUX
#ifdef LIB_TRADER_API_EXPORT
#define TRADER_API_EXPORT __attribute__((visibility("default")))
#else
#define TRADER_API_EXPORT
#endif
#endif

#else
#define TRADER_API_EXPORT
#endif

//回调虚类
class CMarkDataTraderSpi
{
public:
    ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    virtual void OnFrontConnected(){};

    ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    virtual void OnFrontDisconnected(int nReason){};

    ///338499 行情回库应答
    virtual void OnRspFuncMarkdatainsert(CMarkDataRspFuncMarkdatainsertField *pRspFuncMarkdatainsert, CMarkDataRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

};

///交易
class TRADER_API_EXPORT CMarkDataTraderApi
{
public:
    ///创建TraderApi
    static CMarkDataTraderApi *CreateMarkDataTraderApi(const char *pszFlowPath = "", bool bSaveLog = false);

    ///删除接口对象本身
    virtual void ReleaseApi() = 0;

    ///初始连接
    virtual void Init(const char *pszLicFile, const char *pszPwd = "", const char *pszSslFile = "", const char *pszSslPwd = "") = 0;

    ///等待接口线程结束运行
    virtual int Join() = 0;

    ///注册前置机网络地址
    ///@param pszFrontAddress：前置机网络地址。
    ///@param nSubSystemNo：子系统号(根据具体期货公司部署来定，一般期货是115)
    ///@param nCompanyID：公司编号(对接ITN时需要，可以具体可以向ITN询问)
    virtual void RegisterFront(const char *pszFrontAddress, int nSubSystemNo = 0, int nCompanyID = 0, const char *pszConnName = "") = 0;

    ///注册回调接口
    ///@param pSpi 派生自回调接口类的实例
    virtual void RegisterSpi(CMarkDataTraderSpi *pSpi) = 0;

    ///返回用户信息
    virtual CMarkDataUserInfoField* GetUserInfo() = 0;

    ///338499 行情回库请求
    virtual int ReqFuncMarkdatainsert(CMarkDataReqFuncMarkdatainsertField *pReqFuncMarkdatainsert, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CMarkDataRspInfoField *pRspInfo = NULL) = 0;

protected:
    ~CMarkDataTraderApi(){};
};

#endif
