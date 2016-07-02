#include "h5sdk_impl.h"

void CH5sdkImpl::OnConnect(Session *session, const char *peerIp, int peerPort, const char *localIp, int localPort)
{
	printf("连接已建立: 使用本机的IP地址[%s]端口[%d]连接到远程IP地址[%s]端口[%d]\n", localIp, localPort, peerIp, peerPort);

	// H5SDK登录
	lpSession->AsyncSdkLogin(1000);

	OnConnected();
}

void CH5sdkImpl::OnSdkLogin(Session *session)
{
	printf("H5SDK底层登录成功\n");

	// 发起业务登录（只能在这里调用）
	IHsCommMessage *loginRequest = session->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_LOGIN, REQUEST);
	loginRequest->GetItem(H5SDK_TAG_USER_NAME)->SetString("any name is ok");
	loginRequest->GetItem(H5SDK_TAG_HQ_TOKEN)->SetString("any token is ok");
	int ret = session->AsyncSend(loginRequest);
	if (ret != 0)
	{
		cout << "session->AsyncSend() = " << ret << endl;
	    return;
	}
	loginRequest->Release();
}

void CH5sdkImpl::OnReceived(Session *session, IHsCommMessage *response)
{
	AddCommMsg(session, response);
}

void CH5sdkImpl::OnClose(Session *session, int reason)
{
	printf("连接断开: 原因[%d]\n", reason);
	OnClosed(reason);
}

void CH5sdkImpl::OnError(Session *session, int errorNo)
{
	printf("发生错误: 错误号[%d]\n", errorNo);
}

void CH5sdkImpl::OnCore(Session *session)
{
	printf("尝试多次连接后仍然无法连接成功\n");
}

CH5sdkImpl::CH5sdkImpl()
{
	lpSessionOpt = NULL;
	lpSession = NULL;
	memset(m_szFrontAddress, 0, sizeof(m_szFrontAddress));

	bStopAsy = true;
}

CH5sdkImpl::~CH5sdkImpl()
{

}

void CH5sdkImpl::StartTrd()
{
	if(bStopAsy)
	{
		bStopAsy = false;
		hStopEvent.Reset();
		hTrdDealAsyComm.Start(RunTrdDealAsyComm, 0, this);
	}
}

void CH5sdkImpl::StopTrd()
{
	if(!bStopAsy)
	{
		bStopAsy = true;
		hStopEvent.Wait(-1);
		DelCommMsg();
	}
}

int CH5sdkImpl::InitH5sdk()
{
	if(strcmp(m_szFrontAddress, "") == 0)
		return -1;

	char IP[64];
	char Port[16];

	char* toksaveptr;
	char* token;
	token = strtok_t(m_szFrontAddress, ":", &toksaveptr);
	hs_strncpy(IP, token, sizeof(IP));
	hs_strncpy(Port, toksaveptr, sizeof(Port));

	// 创建会话选项
	lpSessionOpt = CreateSessionOptions();
	lpSessionOpt->AddRef();
	// 设置会话选项
	lpSessionOpt->SetHeartbeatTimeout(3);
	lpSessionOpt->SetServerIp(IP);
	lpSessionOpt->SetServerPort(atoi(Port));

	// 通过会话选项创建会话
	lpSession = CreateSession(lpSessionOpt);
	lpSession->AddRef();
	// 开始设置回调
	StartSetCallback();
	// 设置会话的回调
	lpSession->SetCallback(this);
	// 开始所有会话
	StartAllSessions();

	return 0;
}

int CH5sdkImpl::SendCommMessage(IHsCommMessage* lpRequest, int nRequestID)
{
	lpRequest->GetItem(H5PROTO_TAG_USER_KEY)->SetInt32(nRequestID);
	return lpSession->AsyncSend(lpRequest);
}

void CH5sdkImpl::AddCommMsg(Session *session, IHsCommMessage *response)
{
	session->CreateMessage(BIZ_H5PROTO, H5SDK_MSG_MARKET_TYPES, REQUEST);

	if(!bStopAsy)
	{
		IHsCommMessage* lpMsg = response->Clone();

		msgList.Push(lpMsg);
	}
}

void CH5sdkImpl::DelCommMsg()
{
	IHsCommMessage* lpMsg;
	while((lpMsg = msgList.Pop(0)) != NULL)
	{
		lpMsg->Release();
	}
}

void CH5sdkImpl::RunTrdDealAsyComm(void* lpdata)
{
	CH5sdkImpl *pros = (CH5sdkImpl *)lpdata;
	while(pros->bStopAsy == false)
	{
	    IHsCommMessage* msg = pros->msgList.Pop(1000);
	    if(msg != NULL)
	    {
	        pros->DealAsyCommMessage(msg); 
	        msg->Release();
	    }
	}
	pros->hStopEvent.Notify();
}
