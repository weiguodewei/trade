#if !defined(T2_CONNECTION_IMPL_H)
#define T2_CONNECTION_IMPL_H

#include <t2sdk_interface.h>
#include <public.h>

void InitGlobalValues();

class CQueryInfo
{
public:
	CQueryInfo(int iFuncid, int iRepid, void* pData, int iLen)
	{
		this->iApiRequestID = NewRequestId();
		this->iFuncionID = iFuncid;
		this->iRequestID = iRepid;
		if(pData)
		{
			this->ReqData = malloc(iLen);
			memcpy( this->ReqData, pData, iLen);
		}
	};
	~CQueryInfo()
	{
		free(this->ReqData);
	};
	int NewRequestId()
	{
		return currRequestId.Increase();
	}
	void* GetReqData()
	{
		return ReqData; 
	}
	const int GetApiRequestID()
	{
		return iApiRequestID;
	};
	const int GetRequestID()  
	{
		return iRequestID; 
	};
private:
	//请求唯一编号
	static CThreadSafeValue currRequestId;
	int iFuncionID;
	int iRequestID;
	int iApiRequestID;
	void* ReqData;
};

class CConnectionCenterItem
{
public:
	CConnectionCenterItem() 
	{ 
		bIsDying = false; 
	}
	virtual void OnConnected() = 0;
	virtual void OnClosed() = 0;
	virtual CConnectionInterface* GetT2Connection() = 0;
	virtual bool AutoReConnect() = 0;
	bool  IsDying(){  return bIsDying; }
protected:
	void ImDying() { bIsDying = true; };
	bool bIsDying;
};

class CConnectionCenter
{
private:
	typedef std::set<CConnectionCenterItem*> CenterItemSet;
	typedef std::list<CConnectionCenterItem*> CenterItemList;
	typedef std::map<CConnectionCenterItem*,CEventEx*> ConnectEvenMap;
	typedef std::queue<CConnectionCenterItem*> CenterItemQueue;
	volatile bool bStop;
	ManualLock logiclock;
	ManualLock dataLock;

	CenterItemSet reconnSet;
	CenterItemList notifyList;
	CenterItemQueue connecions;
	ConnectEvenMap eventMap;

	CEventEx readyEvent;
	CThread trd;
	static void Reconnect(void* lpvoid);
public:
	static CConnectionCenter* GetInstance();
	CConnectionCenter();
	~CConnectionCenter();
	void RemoveMine(CConnectionCenterItem* item);
	void ReConnect(CConnectionCenterItem* item, bool bsync, bool bNotifyClosed);
};

//T2通信接口的实现
class CT2ConnectionImpl : public CCallbackInterface, public CConnectionCenterItem
{
public:
	// 因为CCallbackInterface的最终纯虚基类是IKnown，所以需要实现一下这3个方法
	unsigned long  FUNCTION_CALL_MODE QueryInterface(const char *iid, IKnown **ppv) {return 0;};
	unsigned long  FUNCTION_CALL_MODE AddRef() {return 0;};
	unsigned long  FUNCTION_CALL_MODE Release() {return 0;};

	// 各种事件发生时的回调方法，实际使用时可以根据需要来选择实现，对于不需要的事件回调方法，可直接return
	// Reserved?为保留方法，为以后扩展做准备，实现时可直接return或return 0。
	void FUNCTION_CALL_MODE OnConnect(CConnectionInterface *lpConnection);
	void FUNCTION_CALL_MODE OnSafeConnect(CConnectionInterface *lpConnection);
	void FUNCTION_CALL_MODE OnRegister(CConnectionInterface *lpConnection);
	void FUNCTION_CALL_MODE OnClose(CConnectionInterface *lpConnection);
	void FUNCTION_CALL_MODE OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData){};
	void FUNCTION_CALL_MODE Reserved1(void *a, void *b, void *c, void *d){};
	void FUNCTION_CALL_MODE Reserved2(void *a, void *b, void *c, void *d){};
	int  FUNCTION_CALL_MODE Reserved3(){return 0;};
	void FUNCTION_CALL_MODE Reserved4(){};
	void FUNCTION_CALL_MODE Reserved5(){};
	void FUNCTION_CALL_MODE Reserved6(){};
	void FUNCTION_CALL_MODE Reserved7(){};
	void FUNCTION_CALL_MODE OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult){};
	void FUNCTION_CALL_MODE OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult){};
	void FUNCTION_CALL_MODE OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage *lpMsg);

public:
	//构造函数
	CT2ConnectionImpl();

	//析构函数
	~CT2ConnectionImpl();

	//获取T2配置对象
	CConfigInterface* GetT2Config();

	//获取T2通信对象
	CConnectionInterface* GetT2Connection();

	//是否自动重连
	bool AutoReConnect();

	//发送请求包(支持同步)
	int SendBizMessage(IF2Packer *lpF2Packer, int nFunction, int nIssueType, int nRequestID, void** ppRetData, int* pnItems, void *pRspInfo);

	//添加QueryInfo
	void AddCQueryInfo(CQueryInfo *qryinfo);

	//销毁QueryInfo
	void DelQuery(CQueryInfo *qryinfo);

	//获得CQueryInfo
	CQueryInfo *GetCQueryInfo(int request);

public:
	//处理异步BizMessage
	virtual void DealAsyBizMessage(IBizMessage* lpMsg) = 0;

	//处理同步BizMessage
	virtual int DealSynBizMessage(const void* lpBuffer, int nLen, void** ppRetData, int* pnItems, void *pRspInfo) = 0;

	//获得心跳成功
	virtual void RecHeartbeet() = 0;

	//获得T2通讯错误信息
	virtual void GetMsgErrorInfo(int nErrorCode, void *pRspInfo) = 0;

protected:
	//T2配置对象
	CConfigInterface*      lpConfig;
	//T2连接对象
	CConnectionInterface*  lpConnection;
	//子系统号
	int                    m_SubSystemNo; 
	//公司编号
	int                    m_CompanyID;
	//前置地址  
	char                   m_szFrontAddress[64];
	//T2有名连接的连接名  
	char                   m_szConnName[32];

	//线程开始
	void StartTrd();

	//线程停止
	void StopTrd();

	//初始化连接
	void InitT2Connect(const char *pszLicFile, const char *pszPwd, const char *pszSslFile, const char *pszSslPwd);

private:
	//异步应答包接收队列
	CMessageQueue<IBizMessage *> msgList;
	//处理异步应答包事件
	CEventEx                     hStopEvent;
	//处理异步应答包状态
	volatile bool                bStopAsy;
	//处理异步应答包线程
	CThread                      hTrdDealAsyBiz;
	//请求队列锁
	ManualLock                   QueryLock;
	//请求对接
	map<int, CQueryInfo*>        QueryMap;

	//消息包入队列
	void AddBizMsg(CConnectionInterface *lpConnection, IBizMessage* lpData);

	//删除消息包
	void DelBizMsg();

	//处理异步应答包线程函数
	static void RunTrdDealAsyBiz(void* lpdata);
};

#endif
