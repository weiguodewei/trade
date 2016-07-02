/**********************************************************************
* 源程序名称: h5sdk_impl.h
* 跟踪日志:
* 2016-01-27 创建，简单实现异步发送功能
**********************************************************************/

#if !defined(H5SDK_IMPL_H)
#define H5SDK_IMPL_H

#include "h5sdk.h"
#include "public.h"

class CH5sdkImpl : public H5SdkCallback
{
public:
	///当会话连接成功时回调
    virtual void OnConnect(Session *session, const char *peerIp, int peerPort, const char *localIp, int localPort);

    ///当会话登录成功时回调
    virtual void OnSdkLogin(Session *session);

    ///当会话收到异步消息时回调
    ///response需要业务层来释放
    virtual void OnReceived(Session *session, IHsCommMessage *response);

    ///当会话连接断开时回调
    virtual void OnClose(Session *session, int reason);

    ///当会话出错时回调
    ///errorNo目前可能为H5SDK_SDK_LOGIN_TIMEOUT，表示sdk登录超时
    virtual void OnError(Session *session, int errorNo);

    ///当重连指定次数后仍然无法连上则回调
    virtual void OnCore(Session *session);
public:
	///构造函数
	CH5sdkImpl();

	///析构函数
	~CH5sdkImpl();

	//线程开始
	void StartTrd();

	//线程停止
	void StopTrd();

	///初始化
	int InitH5sdk();

	//发送请求包(支持同步)
	int SendCommMessage(IHsCommMessage* lpRequest, int nRequestID);

public:
	///连接回调
	virtual void OnConnected() = 0;

	///断开回调
	virtual void OnClosed(int nReason) = 0;

	///处理异步HsCommMessage
	virtual void DealAsyCommMessage(IHsCommMessage* lpResponse) = 0;

protected:
	//会话选项
	SessionOptions*           lpSessionOpt;
	//会话
	Session*                  lpSession;
	//前置地址  
	char                      m_szFrontAddress[64];

private:
	//异步应答包接收队列
	CMessageQueue<IHsCommMessage *> msgList;
	//处理异步应答包事件
	CEventEx                     hStopEvent;
	//处理异步应答包状态
	volatile bool                bStopAsy;
	//处理异步应答包线程
	CThread                      hTrdDealAsyComm;

	///消息包入队列
	void AddCommMsg(Session *session, IHsCommMessage *response);

	///删除消息包
	void DelCommMsg();

	///处理异步应答包线程函数
	static void RunTrdDealAsyComm(void* lpdata);
};

#endif

