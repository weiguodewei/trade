#if !defined(MARKDATATRADERAPI_IMPL_H)
#define MARKDATATRADERAPI_IMPL_H

#define LIB_TRADER_API_EXPORT
#define ISLIB

#include "t2_connection_impl.h"
#include "public.h"
#include "MarkDataTraderApi.h"
#include "MarkDataTraderApi_log.h"

///交易
class CMarkDataTraderApi_impl : public CMarkDataTraderApi, public CT2ConnectionImpl
{
public:
    ///默认构造函数
    CMarkDataTraderApi_impl(const char* pszFlowPath, bool bSaveLog);

    ///析构函数
    ~CMarkDataTraderApi_impl();

    ///删除接口对象本身
    virtual void ReleaseApi();

    ///初始连接
    virtual void Init(const char *pszLicFile, const char *pszPwd = "", const char *pszSslFile = "", const char *pszSslPwd = "");

    ///等待接口线程结束运行
    virtual int Join();

    ///注册前置机网络地址
    ///@param pszFrontAddress：前置机网络地址。
    ///@param nSubSystemNo：子系统号(根据具体期货公司部署来定，一般期货是115)
    ///@param nCompanyID：公司编号(对接ITN时需要，可以具体可以向ITN询问)
    virtual void RegisterFront(const char *pszFrontAddress, int nSubSystemNo = 0, int nCompanyID = 0, const char *pszConnName = "");

    ///注册回调接口
    ///@param pSpi 派生自回调接口类的实例
    virtual void RegisterSpi(CMarkDataTraderSpi *pSpi);

    ///返回用户信息
    virtual CMarkDataUserInfoField* GetUserInfo();

    ///338499 行情回库请求
    virtual int ReqFuncMarkdatainsert(CMarkDataReqFuncMarkdatainsertField *pReqFuncMarkdatainsert, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CMarkDataRspInfoField *pRspInfo = NULL);

public:
    //连接回调
    void OnConnected();

    //断开回调
    void OnClosed();

    //处理异步BizMessage
    void DealAsyBizMessage(IBizMessage* lpMsg);

    //处理同步BizMessage
    int DealSynBizMessage(const void* lpBuffer, int nLen, void** ppRetData, int* pnItems, void *pRspInfo);

    //获得心跳成功
    void RecHeartbeet();

    //获得T2通讯错误信息
    void GetMsgErrorInfo(int nErrorCode, void *pRspInfo);

protected:
    //338499 行情回库-异步
    void OnAsyResponse_338499(IF2UnPacker *lpUnPacker, int nRequestID);

    ///338499 行情回库-同步
    void OnSynResponse_338499(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CMarkDataRspInfoField *pRspInfo);

private:
    //回调类指针
    CMarkDataTraderSpi *lpTraderSpi;
    //日志对象指针
    CMarkDataTraderApi_log *lpTraderApi_log;
    //用户信息
    CMarkDataUserInfoField     m_userInfo;
    string                    m_localip;        //从t2sdk中再获得一次真实的外网ip
    string                    m_localMac;       //本地mac地址
    bool                      m_Logined;        //是否登录登录成功
    bool                      m_SaveLog;        //是否记录日志
    CEventEx                  hJoinEvent;       //事件用来事件Join
    ManualLock                QueryLock;        //分页查询锁
    map<int, CQueryInfo*>     QueryMap;         //分页查询保存请求包Map
    int                       m_iHeartKeep;     //心跳超时计数器
    CThread                   m_HeartKeepTrd;   //维护心跳线程
    CEventEx                  m_HeartKeepEvent; //心跳事件
    bool volatile             m_bHeartKeepStop; //是否停止心跳线程，跳出循环

    static void HeartKeep(void* lpvoid);
};

#endif
