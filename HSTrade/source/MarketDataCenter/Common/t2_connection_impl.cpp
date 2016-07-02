#include "t2_connection_impl.h"

/**********************************************************************/
/*                                                                    */
/*                        T2连接接口的实现                            */
/*                                                                    */
/**********************************************************************/

static bool bGlobalVariableInited = false;
static CConnectionCenter* gpConnCenter;
CThreadSafeValue CQueryInfo::currRequestId(1);

void InitGlobalValues()
{
	if(bGlobalVariableInited == false)
	{
		gpConnCenter = new CConnectionCenter();
		bGlobalVariableInited = true;
	}
}

void CConnectionCenter::Reconnect(void* lpvoid)
{
	CConnectionCenter *helper = (CConnectionCenter*)lpvoid;
	while(helper->bStop == false)
	{	
		//检测有效性 
		helper->logiclock.Lock();
		helper->dataLock.Lock();
		if( helper->reconnSet.size() <= 0)
		{
			helper->dataLock.UnLock();
			helper->logiclock.UnLock();
			helper->readyEvent.Wait(-1);
			continue;
		}
		//通知连接关闭
		CenterItemList::iterator it;
		for(it = helper->notifyList.begin() ; it != helper->notifyList.end(); it++)
		{
			(*it)->OnClosed();
		}
		helper->notifyList.clear();
		helper->dataLock.UnLock();

		//检测有效性 
		helper->dataLock.Lock();
		if( helper->reconnSet.size() <= 0)
		{
			helper->dataLock.UnLock();
			helper->logiclock.UnLock();
			Sleep(0);
			continue;
		}

		CConnectionCenterItem* item = NULL;
		while(!(helper->connecions.empty()))
		{
			item = helper->connecions.front();
			if(helper->reconnSet.find(item) == helper->reconnSet.end())
			{
				helper->connecions.pop();
				continue;
			}
			else
			{
				break;
			}
		}

		if( helper->connecions.empty() )
		{
			helper->dataLock.UnLock();
			helper->logiclock.UnLock();
			continue;
		}

		helper->dataLock.UnLock();
		Sleep(0);
		//连接开始，无论成功还是失败都要通知连接完成
		int ret = item->GetT2Connection()->Connect(5000);
		if(ret == 0)
		{
			helper->dataLock.Lock();
			helper->connecions.pop();
			helper->reconnSet.erase(item);
			//通知完成
			ConnectEvenMap::iterator mapit = helper->eventMap.find(item);
			if(mapit != helper->eventMap.end())
			{
				mapit->second->Notify();
				helper->eventMap.erase(mapit);
			}
			helper->dataLock.UnLock();
			item->OnConnected();
			helper->logiclock.UnLock();
			Sleep(0);
			continue;
		}
		if(!item->AutoReConnect())
		{
			//通知完成 
			helper->dataLock.Lock();
			ConnectEvenMap::iterator mapit = helper->eventMap.find(item);
			if(mapit != helper->eventMap.end())
			{
				mapit->second->Notify();
				helper->eventMap.erase(mapit);
			}
			helper->connecions.pop();
			helper->connecions.push(item);
			helper->dataLock.UnLock();
			helper->logiclock.UnLock();
		}
		Sleep(0);
	}
}

void CConnectionCenter::ReConnect(CConnectionCenterItem* item, bool bsync, bool bNotifyClosed)
{
	//将消亡的接口无须处理
	if( item->IsDying() )
		return;
	CEventEx* ent = NULL;
	dataLock.Lock();
	if(reconnSet.find(item) == reconnSet.end())
	{
		reconnSet.insert(item);
		if(bNotifyClosed)
		{
			notifyList.push_back(item);
		}
		connecions.push(item);
		if(bsync == true)
		{
			ent =  new CEventEx();
			eventMap[item] = ent;
		}
	}
	dataLock.UnLock();
	readyEvent.Notify();
	if( bsync == true && ent != NULL)
	{
		ent->Wait(-1);
	}
}

void CConnectionCenter::RemoveMine(CConnectionCenterItem* item)
{
	logiclock.Lock();
	dataLock.Lock();
	if(reconnSet.find(item) != reconnSet.end())
	{
		reconnSet.erase(item);
		CenterItemList::iterator it;
		for(it = notifyList.begin(); it != notifyList.end(); it++)
		{
			if((*it) == item)
			{
				notifyList.erase(it);
				break;
			}
		}
		ConnectEvenMap::iterator mapit = eventMap.find(item);
		if(mapit != eventMap.end())
		{
			mapit->second->Notify();
			eventMap.erase(mapit);
		}
	}
	dataLock.UnLock();
	logiclock.UnLock();
}

CConnectionCenter* CConnectionCenter::GetInstance()
{
	return gpConnCenter;
}


CConnectionCenter::CConnectionCenter()
{
	bStop = false;
	trd.Start(Reconnect,0,this);
}

CConnectionCenter::~CConnectionCenter()
{
	bStop = true;
	while(!connecions.empty())
	{
		connecions.pop();
	}
	reconnSet.clear();
	notifyList.clear();
	readyEvent.Notify();
}

/**********************************************************************/
/*                                                                    */
/*                        T2回调接口的实现                            */
/*                                                                    */
/**********************************************************************/
//套接字连接成功
void FUNCTION_CALL_MODE CT2ConnectionImpl::OnConnect(CConnectionInterface *lpConnection)
{

}

//完成安全连接
void FUNCTION_CALL_MODE CT2ConnectionImpl::OnSafeConnect(CConnectionInterface *lpConnection)
{
	
}

//完成注册
void FUNCTION_CALL_MODE CT2ConnectionImpl::OnRegister(CConnectionInterface *lpConnection)
{
	
}

//连接被断开
void FUNCTION_CALL_MODE CT2ConnectionImpl::OnClose(CConnectionInterface *lpConnection)
{		
	//连接断开
	if(AutoReConnect())
		CConnectionCenter::GetInstance()->ReConnect(this, false, false);
	else
		CConnectionCenter::GetInstance()->ReConnect(this, false, true);
}

void FUNCTION_CALL_MODE CT2ConnectionImpl::OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg)
{    
	AddBizMsg(lpConnection, lpMsg);
}

/**********************************************************************/
/*                                                                    */
/*                           公有函数的实现                           */
/*                                                                    */
/**********************************************************************/

//构造
CT2ConnectionImpl::CT2ConnectionImpl()
{
	InitGlobalValues();
	lpConfig = NULL;
	lpConnection = NULL;
	m_SubSystemNo = 0;
	m_CompanyID = 0;

	lpConfig = NewConfig();
	lpConfig->AddRef();

	bStopAsy = true;
}

//析构
CT2ConnectionImpl::~CT2ConnectionImpl()
{
	ImDying();
	if(lpConfig)
	{
		lpConfig->Release();
		lpConfig = NULL;
	}
	StopTrd();
	CConnectionCenter::GetInstance()->RemoveMine(this);
	if(lpConnection)
	{
		lpConnection->Close();
		lpConnection->Release();
		lpConnection = NULL;
	}
}

//获取T2配置对象
CConfigInterface* CT2ConnectionImpl::GetT2Config()
{
	assert(lpConfig);
	return lpConfig;
}

//获取T2通信对象
CConnectionInterface* CT2ConnectionImpl::GetT2Connection()
{
	assert(lpConnection);
	return lpConnection;
}

bool CT2ConnectionImpl::AutoReConnect()
{
	return true;
}

int CT2ConnectionImpl::SendBizMessage(IF2Packer *lpF2Packer, int nFunction, int nIssueType, int nRequestID, void** ppRetData, int* pnItems, void *pRspInfo)
{
	//创建消息包
	IBizMessage *lpBizMessage = NewBizMessage();
	lpBizMessage->AddRef();
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	lpBizMessage->SetSubSystemNo(m_SubSystemNo);
	lpBizMessage->SetCompanyID(m_CompanyID);
	lpBizMessage->SetFunction(nFunction);
	lpBizMessage->SetSenderId(nRequestID);
	if(nIssueType == 0)
	{
		lpBizMessage->SetContent(lpF2Packer->GetPackBuf(),lpF2Packer->GetPackLen());
	}
	else
	{
		lpBizMessage->SetIssueType(nIssueType);
		lpBizMessage->SetKeyInfo(lpF2Packer->GetPackBuf(),lpF2Packer->GetPackLen());
	}

	if(ppRetData == NULL)
	{
		//异步发送
		lpConnection->SendBizMsg(lpBizMessage, 1);
		//释放消息包
		lpBizMessage->Release();
		return 0;
	}
	else
	{
		//同步发送
		int hSend = lpConnection->SendBizMsg(lpBizMessage);
		if(hSend < 0)
		{
			//释放消息包
			lpBizMessage->Release();
			return hSend;
		}
		//释放消息包
		lpBizMessage->Release();
		//接收应答包
		IBizMessage* lpRecvBizMessage = NULL;
		int iRet = lpConnection->RecvBizMsg(hSend, &lpRecvBizMessage);
		if(iRet == 0)
		{
			//同步接收数据
			int iMsgLen = 0;
			void * lpMsgBuffer = lpRecvBizMessage->GetBuff(iMsgLen);
			//处理应答包
			return DealSynBizMessage(lpMsgBuffer, iMsgLen, ppRetData, pnItems, pRspInfo);
		}
		else
		{
			GetMsgErrorInfo(iRet, pRspInfo);
		}
		return iRet;
	}
}

void CT2ConnectionImpl::AddCQueryInfo(CQueryInfo *qryinfo)
{
	QueryLock.Lock();
	map<int, CQueryInfo*>::iterator it  = QueryMap.find(qryinfo->GetApiRequestID());
	if( it != QueryMap.end())
	{
		delete it->second;
		assert(false);
	}
	QueryMap[qryinfo->GetApiRequestID()] = qryinfo;
	QueryLock.UnLock();
}

void CT2ConnectionImpl::DelQuery(CQueryInfo *qryinfo)
{
	QueryLock.Lock();
	map<int, CQueryInfo*>::iterator it  = QueryMap.find(qryinfo->GetApiRequestID());
	if( it != QueryMap.end())
	{
		delete it->second;
		QueryMap.erase(it);
	}
	else
	{
		assert(false);
	}
	QueryLock.UnLock();
}

CQueryInfo *CT2ConnectionImpl::GetCQueryInfo(int nRequestID)
{
	QueryLock.Lock();
	map<int, CQueryInfo*>::iterator it  = QueryMap.find(nRequestID);
	QueryLock.UnLock();
	if( it == QueryMap.end())
		return NULL;
	return it->second;
}

void CT2ConnectionImpl::StartTrd()
{
	if( bStopAsy == true)
	{
		bStopAsy = false;
		hStopEvent.Reset();
		hTrdDealAsyBiz.Start(RunTrdDealAsyBiz, 0, this);
	}
}

void CT2ConnectionImpl::StopTrd()
{
	if(bStopAsy == false)
	{
		bStopAsy = true;
		hStopEvent.Wait(-1);
		DelBizMsg();
	}
}

void CT2ConnectionImpl::InitT2Connect(const char *pszLicFile, const char *pszPwd, const char *pszSslFile, const char *pszSslPwd)
{
	//配置连接对象
	lpConfig->SetString("t2sdk", "servers", m_szFrontAddress);
	lpConfig->SetString("t2sdk", "license_file", pszLicFile);
	lpConfig->SetString("t2sdk", "login_name", m_szConnName);

	lpConfig->SetInt("t2sdk", "init_recv_buf_size", 512);
	lpConfig->SetInt("t2sdk", "init_send_buf_size", 512);
	lpConfig->SetInt("t2sdk", "send_queue_size", 10000);
	lpConfig->SetInt("t2sdk", "heartbeat_time", 5);

	//通讯加密设置，必须在NewConnection之前
	//如果接入前置t2通道设置了safe_level = pwd
	if(strcmp(pszPwd, "") != 0)
	{
		lpConfig->SetString("safe", "safe_level", "pwd");
		lpConfig->SetString("safe", "comm_pwd", pszPwd); //通信密码
	}

	//如果接入前置t2通道设置了safe_level = ssl
	if(strcmp(pszSslFile, "") != 0)
	{
		lpConfig->SetString("safe", "safe_level", "ssl");
		lpConfig->SetString("safe", "cert_file", pszSslFile); //Cert文件
		lpConfig->SetString("safe", "cert_pwd", pszSslPwd); //Cert密码
	}

	int iRet = 0;
	if(lpConnection != NULL)
	{
	    lpConnection->Release();
	    lpConnection = NULL;
	}
	lpConnection = NewConnection(lpConfig);
	lpConnection->AddRef();

	if (0 == (iRet = lpConnection->Create2BizMsg(this)))
	{
	    CConnectionCenter::GetInstance()->ReConnect(this,true,false);
	}

	return;
}

void CT2ConnectionImpl::AddBizMsg(CConnectionInterface *lpConnection, IBizMessage* lpData)
{
	if(bStopAsy == false)
	{
		int iLen;
		const void * lpBuffer = lpData->GetBuff(iLen);
		IBizMessage* lpMsg = NewBizMessage();
		lpMsg->AddRef();
		lpMsg->SetBuff(lpBuffer, iLen);

		if(lpMsg->GetFunction() == 620000)
		{
			if (lpMsg->GetPacketType() == REQUEST_PACKET)
			{
				lpMsg->ChangeReq2AnsMessage();
				if(NULL != lpConnection)
					lpConnection->SendBizMsg(lpMsg, 1);
			}
			lpMsg->Release();

			//回调心跳接收成功
			RecHeartbeet();

			return;
		}
		msgList.Push(lpMsg);
	}
}

void CT2ConnectionImpl::DelBizMsg()
{
	IBizMessage *esb;
	while((esb = msgList.Pop(0)) != NULL)
	{
		esb->Release();
	}
}

void CT2ConnectionImpl::RunTrdDealAsyBiz(void* lpdata)
{
	CT2ConnectionImpl *pros = (CT2ConnectionImpl *)lpdata;
	while(pros->bStopAsy == false)
	{
	    IBizMessage* msg = pros->msgList.Pop(1000);
	    if(msg != NULL)
	    {
	        pros->DealAsyBizMessage(msg); 
	        msg->Release();
	    }
	}
	pros->hStopEvent.Notify();
}

