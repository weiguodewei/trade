#if !defined(HUNDSUNFUTUMDAPI_IMPL_H)
#define HUNDSUNFUTUMDAPI_IMPL_H

#define LIB_MD_API_EXPORT
#define ISLIB

#include "t2_connection_impl.h"
#include "public.h"
#include "HundsunFutuMdApi.h"
#include "HundsunFutuMdApi_log.h"

///行情
class CHundsunFutuMdApi_impl : public CHundsunFutuMdApi, public CT2ConnectionImpl
{
public:
    ///默认构造函数
    CHundsunFutuMdApi_impl(const char* pszFlowPath, bool bSaveLog);

    ///析构函数
    ~CHundsunFutuMdApi_impl();

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
    virtual void RegisterSpi(CHundsunFutuMdSpi *pSpi);

    ///返回用户信息
    virtual CHundsunFutuUserInfoField* GetUserInfo();

    ///331100 客户登录请求
    virtual int ReqFuncClientLogin(CHundsunFutuReqFuncClientLoginField *pReqFuncClientLogin, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL);

    ///330851 期货合约保证金查询请求
    virtual int ReqFuncFutuContractbailQry(CHundsunFutuReqFuncFutuContractbailQryField *pReqFuncFutuContractbailQry, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL);

    ///620001_33100 订阅-期货单腿行情请求
    virtual int ReqFuncQhSingleHqSubscribe(char *ppInstrumentID[], int nCount, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL);

    ///620002_33100 订阅取消-期货单腿行情请求
    virtual int ReqFuncQhSingleHqCancel(char *ppInstrumentID[], int nCount, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL);

    ///620001_33102 订阅-期货组合行情请求
    virtual int ReqFuncQhCombineSubscribe(char *ppInstrumentID[], int nCount, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL);

    ///620002_33102 订阅取消-期货组合行情请求
    virtual int ReqFuncQhCombineCancel(char *ppInstrumentID[], int nCount, int nRequestID = 0, void** ppRetData = NULL, int* pnItems = NULL, CHundsunFutuRspInfoField *pRspInfo = NULL);

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
    //331100 客户登录-异步
    void OnAsyResponse_331100(IF2UnPacker *lpUnPacker, int nRequestID);

    //330851 期货合约保证金查询-异步
    void OnAsyResponse_330851(IF2UnPacker *lpUnPacker, int nRequestID);

    //620001_33100 订阅-期货单腿行情-异步
    void OnAsyRsp_620001_33100(IF2UnPacker *lpUnPacker, int nRequestID);

    //620002_33100 订阅取消-期货单腿行情-异步
    void OnAsyRsp_620002_33100(IF2UnPacker *lpUnPacker, int nRequestID);

    //620003_33100 主推-期货单腿行情-异步
    void OnAsyRtn_620003_33100(IF2UnPacker *lpUnPacker, int nRequestID);

    //620001_33102 订阅-期货组合行情-异步
    void OnAsyRsp_620001_33102(IF2UnPacker *lpUnPacker, int nRequestID);

    //620002_33102 订阅取消-期货组合行情-异步
    void OnAsyRsp_620002_33102(IF2UnPacker *lpUnPacker, int nRequestID);

    //620003_33102 主推-期货组合行情-异步
    void OnAsyRtn_620003_33102(IF2UnPacker *lpUnPacker, int nRequestID);

    ///331100 客户登录-同步
    void OnSynResponse_331100(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo);

    ///330851 期货合约保证金查询-同步
    void OnSynResponse_330851(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo);

    ///620001_33100 订阅-期货单腿行情-同步
    void OnSynRsp_620001_33100(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo);

    ///620002_33100 订阅取消-期货单腿行情-同步
    void OnSynRsp_620002_33100(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo);

    ///620001_33102 订阅-期货组合行情-同步
    void OnSynRsp_620001_33102(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo);

    ///620002_33102 订阅取消-期货组合行情-同步
    void OnSynRsp_620002_33102(IF2UnPacker *lpUnPacker, void** ppRetData, int* pnItems, CHundsunFutuRspInfoField *pRspInfo);

private:
    //回调类指针
    CHundsunFutuMdSpi *lpMdSpi;
    //日志对象指针
    CHundsunFutuMdApi_log *lpMdApi_log;
    //用户信息
    CHundsunFutuUserInfoField     m_userInfo;
    string                    m_localip;        //从t2sdk中再获得一次真实的外网ip
    string                    m_localMac;       //本地mac地址
    bool                      m_Logined;        //是否登录登录成功
    bool                      m_SaveLog;        //是否记录日志
    CEventEx                  hJoinEvent;       //事件用来事件Join
    ManualLock                m_SubSetCs;       //订阅信息锁
    set<string>               m_SubFutuCodeSet;
    set<string>               m_SubArbitCodeSet;
    int                       m_iHeartKeep;     //心跳超时计数器
    CThread                   m_HeartKeepTrd;   //维护心跳线程
    CEventEx                  m_HeartKeepEvent; //心跳事件
    bool volatile             m_bHeartKeepStop; //是否停止心跳线程，跳出循环

    static void HeartKeep(void* lpvoid);
};

#endif
