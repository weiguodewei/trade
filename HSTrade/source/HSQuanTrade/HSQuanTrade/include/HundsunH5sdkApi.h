#if !defined(HUNDSUNH5SDKAPI_H)
#define HUNDSUNH5SDKAPI_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string.h>
#include "HundsunH5sdkUserApiStruct.h"

#if defined(ISLIB)

#ifdef WIN32
#ifdef LIB_H5SDK_API_EXPORT
#define H5SDK_API_EXPORT __declspec(dllexport)
#else
#define H5SDK_API_EXPORT __declspec(dllimport)
#endif
#endif

#ifdef LINUX
#ifdef LIB_H5SDK_API_EXPORT
#define H5SDK_API_EXPORT __attribute__((visibility("default")))
#else
#define H5SDK_API_EXPORT
#endif
#endif

#else
#define H5SDK_API_EXPORT
#endif

//回调虚类
class CHundsunH5sdkSpi
{
public:
    ///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
    virtual void OnH5sdkFrontConnected(){};

    ///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
    virtual void OnH5sdkFrontDisconnected(int nReason){};

    ///1001 登入请求应答
    virtual void OnRspH5sdkLogin(CHundsunH5sdkRspLoginField *pRspLogin, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///1002 登出信息应答
    virtual void OnRspH5sdkLogout(CHundsunH5sdkRspLogoutField *pRspLogout, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///1003 心跳应答
    virtual void OnRspH5sdkHeartbeat(CHundsunH5sdkRspHeartbeatField *pRspHeartbeat, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///1004 请求服务器信息应答
    virtual void OnRspH5sdkServerInfo(CHundsunH5sdkRspServerInfoField *pRspServerInfo, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///1006 批处理应答
    virtual void OnRspH5sdkBatch(CHundsunH5sdkRspBatchField *pRspBatch, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///2001 请求静态文件应答
    virtual void OnRspH5sdkFile(CHundsunH5sdkRspFileField *pRspFile, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///2002 市场分类信息应答
    virtual void OnRspH5sdkMarketTypes(CHundsunH5sdkRspMarketTypesField *pRspMarketTypes, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///2003 市场代码表应答
    virtual void OnRspH5sdkMarketReference(CHundsunH5sdkRspMarketReferenceField *pRspMarketReference, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///3001 行情快照应答
    virtual void OnRspH5sdkSnapshot(CHundsunH5sdkRspSnapshotField *pRspSnapshot, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///3002 键盘精灵消息应答
    virtual void OnRspH5sdkKeyboardWizard(CHundsunH5sdkRspKeyboardWizardField *pRspKeyboardWizard, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///4001 行情快照订阅应答
    virtual void OnRspH5sdkSubscribe(CHundsunH5sdkRspSubscribeField *pRspSubscribe, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///5001 排序请求应答
    virtual void OnRspH5sdkSort(CHundsunH5sdkRspSortField *pRspSort, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6001 按指定的日期或偏移取分时数据应答
    virtual void OnRspH5sdkTrend(CHundsunH5sdkRspTrendField *pRspTrend, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6002 按偏移取K线应答
    virtual void OnRspH5sdkCandleByOffset(CHundsunH5sdkRspCandleByOffsetField *pRspCandleByOffset, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6003 按日期时间范围取K线应答
    virtual void OnRspH5sdkCandleByRange(CHundsunH5sdkRspCandleByRangeField *pRspCandleByRange, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6004 指定分钟的分笔应答
    virtual void OnRspH5sdkTickMin(CHundsunH5sdkRspTickMinField *pRspTickMin, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6005 按偏移取分笔应答
    virtual void OnRspH5sdkTickDirection(CHundsunH5sdkRspTickDirectionField *pRspTickDirection, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6006 补线使用_分钟k线应答
    virtual void OnRspH5sdkCandleByRangeMaintain(CHundsunH5sdkRspCandleByRangeMaintainField *pRspCandleByRangeMaintain, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///6007 5日分时应答
    virtual void OnRspH5sdkDay5Trend(CHundsunH5sdkRspDay5TrendField *pRspDay5Trend, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

    ///7001 补线返回应答
    virtual void OnRspH5sdkMaintainReturn(CHundsunH5sdkRspMaintainReturnField *pRspMaintainReturn, CHundsunH5sdkRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

};

///交易
class H5SDK_API_EXPORT CHundsunH5sdkApi
{
public:
    ///创建H5sdkApi
    static CHundsunH5sdkApi *CreateHundsunH5sdkApi(const char *pszFlowPath = "", bool bSaveLog = false);

    ///删除接口对象本身
    virtual void ReleaseApi() = 0;

    ///初始连接
    virtual void Init() = 0;

    ///等待接口线程结束运行
    virtual int Join() = 0;

    ///注册前置机网络地址
    ///@param pszFrontAddress：前置机网络地址。
    virtual void RegisterFront(const char *pszFrontAddress) = 0;

    ///注册回调接口
    ///@param pSpi 派生自回调接口类的实例
    virtual void RegisterSpi(CHundsunH5sdkSpi *pSpi) = 0;

    ///1001 登入请求请求
    virtual int ReqH5sdkLogin(CHundsunH5sdkReqLoginField *pReqLogin, int nRequestID = 0) = 0;

    ///1002 登出信息请求
    virtual int ReqH5sdkLogout(CHundsunH5sdkReqLogoutField *pReqLogout, int nRequestID = 0) = 0;

    ///1003 心跳请求
    virtual int ReqH5sdkHeartbeat(CHundsunH5sdkReqHeartbeatField *pReqHeartbeat, int nRequestID = 0) = 0;

    ///1004 请求服务器信息请求
    virtual int ReqH5sdkServerInfo(CHundsunH5sdkReqServerInfoField *pReqServerInfo, int nRequestID = 0) = 0;

    ///1006 批处理请求
    virtual int ReqH5sdkBatch(CHundsunH5sdkReqBatchField *pReqBatch, int nRequestID = 0) = 0;

    ///2001 请求静态文件请求
    virtual int ReqH5sdkFile(CHundsunH5sdkReqFileField *pReqFile, int nRequestID = 0) = 0;

    ///2002 市场分类信息请求
    virtual int ReqH5sdkMarketTypes(CHundsunH5sdkReqMarketTypesField *pReqMarketTypes, int nRequestID = 0) = 0;

    ///2003 市场代码表请求
    virtual int ReqH5sdkMarketReference(CHundsunH5sdkReqMarketReferenceField *pReqMarketReference, int nRequestID = 0) = 0;

    ///3001 行情快照请求
    virtual int ReqH5sdkSnapshot(CHundsunH5sdkReqSnapshotField *pReqSnapshot, int nRequestID = 0) = 0;

    ///3002 键盘精灵消息请求
    virtual int ReqH5sdkKeyboardWizard(CHundsunH5sdkReqKeyboardWizardField *pReqKeyboardWizard, int nRequestID = 0) = 0;

    ///4001 行情快照订阅请求
    virtual int ReqH5sdkSubscribe(CHundsunH5sdkReqSubscribeField *pReqSubscribe, int nRequestID = 0) = 0;

    ///5001 排序请求请求
    virtual int ReqH5sdkSort(CHundsunH5sdkReqSortField *pReqSort, int nRequestID = 0) = 0;

    ///6001 按指定的日期或偏移取分时数据请求
    virtual int ReqH5sdkTrend(CHundsunH5sdkReqTrendField *pReqTrend, int nRequestID = 0) = 0;

    ///6002 按偏移取K线请求
    virtual int ReqH5sdkCandleByOffset(CHundsunH5sdkReqCandleByOffsetField *pReqCandleByOffset, int nRequestID = 0) = 0;

    ///6003 按日期时间范围取K线请求
    virtual int ReqH5sdkCandleByRange(CHundsunH5sdkReqCandleByRangeField *pReqCandleByRange, int nRequestID = 0) = 0;

    ///6004 指定分钟的分笔请求
    virtual int ReqH5sdkTickMin(CHundsunH5sdkReqTickMinField *pReqTickMin, int nRequestID = 0) = 0;

    ///6005 按偏移取分笔请求
    virtual int ReqH5sdkTickDirection(CHundsunH5sdkReqTickDirectionField *pReqTickDirection, int nRequestID = 0) = 0;

    ///6006 补线使用_分钟k线请求
    virtual int ReqH5sdkCandleByRangeMaintain(CHundsunH5sdkReqCandleByRangeMaintainField *pReqCandleByRangeMaintain, int nRequestID = 0) = 0;

    ///6007 5日分时请求
    virtual int ReqH5sdkDay5Trend(CHundsunH5sdkReqDay5TrendField *pReqDay5Trend, int nRequestID = 0) = 0;

    ///7001 补线返回请求
    virtual int ReqH5sdkMaintainReturn(CHundsunH5sdkReqMaintainReturnField *pReqMaintainReturn, int nRequestID = 0) = 0;

protected:
    ~CHundsunH5sdkApi(){};
};

#endif
