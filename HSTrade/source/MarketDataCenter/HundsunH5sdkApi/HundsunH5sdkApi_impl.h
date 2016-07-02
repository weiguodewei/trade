#if !defined(HUNDSUNH5SDKAPI_IMPL_H)
#define HUNDSUNH5SDKAPI_IMPL_H

#define LIB_H5SDK_API_EXPORT
#define ISLIB

#include "h5sdk_impl.h"
#include "public.h"
#include "HundsunH5sdkApi.h"

///交易
class CHundsunH5sdkApi_impl : public CHundsunH5sdkApi, public CH5sdkImpl
{
public:
    ///默认构造函数
    CHundsunH5sdkApi_impl(const char* pszFlowPath, bool bSaveLog);

    ///析构函数
    ~CHundsunH5sdkApi_impl();

    ///删除接口对象本身
    virtual void ReleaseApi();

    ///初始连接
    virtual void Init();

    ///等待接口线程结束运行
    virtual int Join();

    ///注册前置机网络地址
    ///@param pszFrontAddress：前置机网络地址。
    virtual void RegisterFront(const char *pszFrontAddress);

    ///注册回调接口
    ///@param pSpi 派生自回调接口类的实例
    virtual void RegisterSpi(CHundsunH5sdkSpi *pSpi);

    ///1001 登入请求请求
    virtual int ReqH5sdkLogin(CHundsunH5sdkReqLoginField *pReqLogin, int nRequestID = 0);

    ///1002 登出信息请求
    virtual int ReqH5sdkLogout(CHundsunH5sdkReqLogoutField *pReqLogout, int nRequestID = 0);

    ///1003 心跳请求
    virtual int ReqH5sdkHeartbeat(CHundsunH5sdkReqHeartbeatField *pReqHeartbeat, int nRequestID = 0);

    ///1004 请求服务器信息请求
    virtual int ReqH5sdkServerInfo(CHundsunH5sdkReqServerInfoField *pReqServerInfo, int nRequestID = 0);

    ///1006 批处理请求
    virtual int ReqH5sdkBatch(CHundsunH5sdkReqBatchField *pReqBatch, int nRequestID = 0);

    ///2001 请求静态文件请求
    virtual int ReqH5sdkFile(CHundsunH5sdkReqFileField *pReqFile, int nRequestID = 0);

    ///2002 市场分类信息请求
    virtual int ReqH5sdkMarketTypes(CHundsunH5sdkReqMarketTypesField *pReqMarketTypes, int nRequestID = 0);

    ///2003 市场代码表请求
    virtual int ReqH5sdkMarketReference(CHundsunH5sdkReqMarketReferenceField *pReqMarketReference, int nRequestID = 0);

    ///3001 行情快照请求
    virtual int ReqH5sdkSnapshot(CHundsunH5sdkReqSnapshotField *pReqSnapshot, int nRequestID = 0);

    ///3002 键盘精灵消息请求
    virtual int ReqH5sdkKeyboardWizard(CHundsunH5sdkReqKeyboardWizardField *pReqKeyboardWizard, int nRequestID = 0);

    ///4001 行情快照订阅请求
    virtual int ReqH5sdkSubscribe(CHundsunH5sdkReqSubscribeField *pReqSubscribe, int nRequestID = 0);

    ///5001 排序请求请求
    virtual int ReqH5sdkSort(CHundsunH5sdkReqSortField *pReqSort, int nRequestID = 0);

    ///6001 按指定的日期或偏移取分时数据请求
    virtual int ReqH5sdkTrend(CHundsunH5sdkReqTrendField *pReqTrend, int nRequestID = 0);

    ///6002 按偏移取K线请求
    virtual int ReqH5sdkCandleByOffset(CHundsunH5sdkReqCandleByOffsetField *pReqCandleByOffset, int nRequestID = 0);

    ///6003 按日期时间范围取K线请求
    virtual int ReqH5sdkCandleByRange(CHundsunH5sdkReqCandleByRangeField *pReqCandleByRange, int nRequestID = 0);

    ///6004 指定分钟的分笔请求
    virtual int ReqH5sdkTickMin(CHundsunH5sdkReqTickMinField *pReqTickMin, int nRequestID = 0);

    ///6005 按偏移取分笔请求
    virtual int ReqH5sdkTickDirection(CHundsunH5sdkReqTickDirectionField *pReqTickDirection, int nRequestID = 0);

    ///6006 补线使用_分钟k线请求
    virtual int ReqH5sdkCandleByRangeMaintain(CHundsunH5sdkReqCandleByRangeMaintainField *pReqCandleByRangeMaintain, int nRequestID = 0);

    ///6007 5日分时请求
    virtual int ReqH5sdkDay5Trend(CHundsunH5sdkReqDay5TrendField *pReqDay5Trend, int nRequestID = 0);

    ///7001 补线返回请求
    virtual int ReqH5sdkMaintainReturn(CHundsunH5sdkReqMaintainReturnField *pReqMaintainReturn, int nRequestID = 0);

public:
    ///连接回调
    void OnConnected();

    ///断开回调
    void OnClosed(int nReason);

    ///HsCommMessage
    void DealAsyCommMessage(IHsCommMessage* lpResponse);

protected:
    //1001 登入请求-异步
    void OnAsyResponse_1001(IRecord *lpBody, int nRequestID);

    //1002 登出信息-异步
    void OnAsyResponse_1002(IRecord *lpBody, int nRequestID);

    //1003 心跳-异步
    void OnAsyResponse_1003(IRecord *lpBody, int nRequestID);

    //1004 请求服务器信息-异步
    void OnAsyResponse_1004(IRecord *lpBody, int nRequestID);

    //1006 批处理-异步
    void OnAsyResponse_1006(IRecord *lpBody, int nRequestID);

    //2001 请求静态文件-异步
    void OnAsyResponse_2001(IRecord *lpBody, int nRequestID);

    //2002 市场分类信息-异步
    void OnAsyResponse_2002(IRecord *lpBody, int nRequestID);

    //2003 市场代码表-异步
    void OnAsyResponse_2003(IRecord *lpBody, int nRequestID);

    //3001 行情快照-异步
    void OnAsyResponse_3001(IRecord *lpBody, int nRequestID);

    //3002 键盘精灵消息-异步
    void OnAsyResponse_3002(IRecord *lpBody, int nRequestID);

    //4001 行情快照订阅-异步
    void OnAsyResponse_4001(IRecord *lpBody, int nRequestID);

    //5001 排序请求-异步
    void OnAsyResponse_5001(IRecord *lpBody, int nRequestID);

    //6001 按指定的日期或偏移取分时数据-异步
    void OnAsyResponse_6001(IRecord *lpBody, int nRequestID);

    //6002 按偏移取K线-异步
    void OnAsyResponse_6002(IRecord *lpBody, int nRequestID);

    //6003 按日期时间范围取K线-异步
    void OnAsyResponse_6003(IRecord *lpBody, int nRequestID);

    //6004 指定分钟的分笔-异步
    void OnAsyResponse_6004(IRecord *lpBody, int nRequestID);

    //6005 按偏移取分笔-异步
    void OnAsyResponse_6005(IRecord *lpBody, int nRequestID);

    //6006 补线使用_分钟k线-异步
    void OnAsyResponse_6006(IRecord *lpBody, int nRequestID);

    //6007 5日分时-异步
    void OnAsyResponse_6007(IRecord *lpBody, int nRequestID);

    //7001 补线返回-异步
    void OnAsyResponse_7001(IRecord *lpBody, int nRequestID);

private:
    //回调类指针
    CHundsunH5sdkSpi *lpH5sdkSpi;

    CEventEx                  hJoinEvent;       //事件用来事件Join
};

#endif
