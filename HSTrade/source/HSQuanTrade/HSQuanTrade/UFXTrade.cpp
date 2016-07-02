#include "stdafx.h"
#include "UFXTrade.h"
#include "TradeInterface.h"
#include "Order.h"
#include <string>
#include "StrategyBase.h"
using namespace std;

/*
CUFXTrade * gTrade;

void Log(char * msg)
{
	gTrade->Log(msg);
	
}
*/

string FindField(const char *str, int index, char spliter)
{
	const char* temp = str-1;
	const char *begin=NULL;
	for (int i=1; i<=index; i++)
	{
		begin = ++temp;
		temp = strchr(temp, spliter);
	}

	char buf[256];
	memcpy(buf, begin, temp-begin);
	buf[temp-begin] = '\0';
	return string(buf);
}


//打印应答包
void ShowPacket(int iIssueType, IF2UnPacker *pUnPacker) //解包器接口
{
	int i = 0, t = 0, j = 0, k = 0;
	
    for (i = 0; i < pUnPacker->GetDatasetCount(); ++i)
    {
        // 设置当前结果集
		pUnPacker->SetCurrentDatasetByIndex(i);

        // 打印字段
		CStringArray arList;
        for (t = 0; t < pUnPacker->GetColCount(); ++t)
        {
			char msg[128];

            sprintf(msg,"%20s", pUnPacker->GetColName(t));

			arList.Add(msg);
			//Log(msg);
        }		
		
        // 打印所有记录
        for (j = 0; j < (int)pUnPacker->GetRowCount(); ++j)
        {
			char msg[512];

            // 打印每条记录
            for (k = 0; k < pUnPacker->GetColCount(); ++k)
            {

                switch (pUnPacker->GetColType(k))
                {
                case 'I':
                    sprintf(msg,"%20d", pUnPacker->GetIntByIndex(k));
                    break;
					
                case 'C':
                    sprintf(msg,"%20c", pUnPacker->GetCharByIndex(k));
                    break;
					
                case 'S':
                    sprintf(msg,"%20s", pUnPacker->GetStrByIndex(k));
                    break;
					
                case 'F':
                    sprintf(msg,"%20f", pUnPacker->GetDoubleByIndex(k));
                    break;
					
                case 'R':
                    {
                        break;
                    }				
                default:
                    // 未知数据类型
                    printf("未知数据类型。\n");
                    break;
                }

				char msg2[600];

				sprintf(msg2,"%s %s",arList[k],msg);

				//Log(msg2);
            }	           

            pUnPacker->Next();
        }
        putchar('\n');
    }
}

CUFXTrade::CUFXTrade(void)
{
}


CUFXTrade::~CUFXTrade(void)
{
}


void CUFXTrade::init(CString serverAddress)
{
	//gTrade = this;

	g_szBusiness.m_pTrade = this;

	m_lpConfig = NewConfig(); 

	m_lpConfig->AddRef();

	 // [t2sdk] servers指定需要连接的IP地址及端口
	m_lpConfig->SetString("t2sdk", "servers", serverAddress.GetBuffer(0));
	serverAddress.ReleaseBuffer();

    // [t2sdk] license_file指定许可证文件
    m_lpConfig->SetString("t2sdk", "license_file", "license.dat");

    // [t2sdk] send_queue_size指定T2_SDK的发送队列大小
    m_lpConfig->SetString("t2sdk", "send_queue_size", "100");
    //在此设置一下就可以支持自动重连
    m_lpConfig->SetString("t2sdk", "auto_reconnect", "1");

	g_szBusiness.SetConfig(m_lpConfig);

    //通过T2SDK的引出函数，来获取一个新的CConnection对象指针
	g_szBusiness.g_pConnection = NewConnection(m_lpConfig);

    g_szBusiness.g_pConnection->AddRef();
	
	m_szCallback.m_pBusiness = &g_szBusiness;
	m_szCallback.m_secuBusiness = NULL;

    int iRet = 0, iLogin = 0;

	//初始化连接对象，返回0表示初始化成功，注意此时并没开始连接服务器
	//if ( ( 0 == (iRet = g_szBusiness.g_pConnection->CreateEx(&m_szCallback)) ) )
	if ( ( 0 == (iRet = g_szBusiness.g_pConnection->Create2BizMsg(&m_szCallback)) ) )
	{
		// 开启断开重连线程，由于在非断开情况下，该线程处于Wait状态，故对应用性能影响甚微
		//正式开始连接，参数5000为超时参数，单位毫秒
		if (iRet = g_szBusiness.g_pConnection->Connect(5000))
		{
			CString msg;

			msg.Format("连接服务器失败, 错误号: %d, 原因: %s!\r\n", iRet, g_szBusiness.g_pConnection->GetErrorMsg(iRet));

			Log(msg.GetBuffer(msg.GetLength()));

			msg.ReleaseBuffer();

		}	
		else
		{
			Log("连接服务器成功!");

			g_szBusiness.Login(m_szUserID,m_szPwd,-9999);
		}
	}
	
}


int CUFXTrade::InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int orderref)
{
	return g_szBusiness.FutureEntrust(g_iSystemNo,m_szUserID,m_szPwd,instrument,count,direction,offsetflag,price,orderref);
}

int CUFXTrade::WithDraw(int entrustno,char *sCode)
{
	return g_szBusiness.WithDraw(m_szUserID,m_szPwd,entrustno,1);

}

int CUFXTrade::QueryEntrust(int entrustno,int reqid)
{
	return g_szBusiness.QueryEntrust(m_szUserID,m_szPwd,entrustno,reqid);

}


void CUFXTrade::RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount)
{
	m_pTradeInterface->RtnOrder(orderref, ordersysid,status,bs,kp,ins);

}

void CUFXTrade::RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)
{

	m_pTradeInterface->RtnTrade(userid,orderref,ordersysid,price,offsetflag,direction,tradetime,vol,ins);

}

void CUFXTrade::OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{
	m_pTradeInterface->OnRspOrderInsert(ref,entrustno,entrust_status,errorid);
}




//////////////////////////////////////////////////////////////////
//CSECUTrade

CSECUTrade::CSECUTrade(void)
{
}

CSECUTrade::~CSECUTrade(void)
{

}

void CSECUTrade::init(CString serverAddress)
{
	g_secuBusiness.m_secuTrade = this;

	m_secuConfig = NewConfig(); 

	m_secuConfig->AddRef();

	//m_secuConfig->Load("D:\\HS\\策略交易\\trunk\\Sources\\HSQuanTrade\\Debug\\t2sdk.ini");


    //配置连接对象
	m_secuConfig->SetString("t2sdk", "servers", serverAddress.GetBuffer(0));
	serverAddress.ReleaseBuffer();
	//m_secuConfig->SetString("t2sdk", "servers", "192.168.78.151:19013");
	m_secuConfig->SetString("t2sdk", "license_file", "(20130306)tyjrnb-TYJRTYJR-ALL-0001_3rd.dat");
	//m_secuConfig->SetString("t2sdk", "license_file", "client_license_ufx.dat");

	m_secuConfig->SetInt("t2sdk", "init_recv_buf_size", 512);
	m_secuConfig->SetInt("t2sdk", "init_send_buf_size", 512);
	m_secuConfig->SetInt("t2sdk", "send_queue_size", 10000);
	m_secuConfig->SetInt("t2sdk", "heartbeat_time", 5);

	//如果接入前置t2通道设置了safe_level = ssl
	m_secuConfig->SetString("safe", "safe_level", "ssl");
	m_secuConfig->SetString("safe", "cert_file", "c20150907.pfx"); //Cert文件
	m_secuConfig->SetString("safe", "cert_pwd", "12345678"); //Cert密码

	g_secuBusiness.SetConfig(m_secuConfig);


    //通过T2SDK的引出函数，来获取一个新的CConnection对象指针
	g_secuBusiness.g_SecuConnection = NewConnection(m_secuConfig);

    g_secuBusiness.g_SecuConnection->AddRef();
	
	m_secuCallback.m_secuBusiness = &g_secuBusiness;
	m_secuCallback.m_pBusiness = NULL;

    int iRet = 0, iLogin = 0;

	//初始化连接对象，返回0表示初始化成功，注意此时并没开始连接服务器
	//if ( ( 0 == (iRet = g_secuBusiness.g_SecuConnection->CreateEx(&m_secuCallback)) ) )
	if ( ( 0 == (iRet = g_secuBusiness.g_SecuConnection->Create2BizMsg(&m_secuCallback)) ) )
	{
		// 开启断开重连线程，由于在非断开情况下，该线程处于Wait状态，故对应用性能影响甚微
		//正式开始连接，参数5000为超时参数，单位毫秒
		if (iRet = g_secuBusiness.g_SecuConnection->Connect(5000))
		{
			CString msg;

			msg.Format("连接服务器失败, 错误号: %d, 原因: %s!\r\n", iRet, g_secuBusiness.g_SecuConnection->GetErrorMsg(iRet));

			Log(msg.GetBuffer(msg.GetLength()));

			msg.ReleaseBuffer();

		}	
		else
		{
			Log("连接服务器成功!");

			g_secuBusiness.Login(m_szUserID,m_szPwd,-9999);
		}
	}

}

int CSECUTrade::InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int reqid)
{

	return g_secuBusiness.SecuEntrust(g_iSystemNo,m_szUserID,m_szPwd,instrument,count,direction,price,reqid);

}

int CSECUTrade::WithDraw(int entrustno,char *sCode)
{
	return g_secuBusiness.SecuEntrustWithdraw(m_szUserID,m_szPwd,entrustno,1);
}

int CSECUTrade::QueryEntrust(int entrustno,int reqid)
{
	return 1;
}


void CSECUTrade::RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount)
{
	m_pTradeInterface->RtnOrder(orderref, ordersysid,status,bs,kp,ins);
}

void CSECUTrade::RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)
{
	m_pTradeInterface->RtnTrade(userid,orderref,ordersysid,price,offsetflag,direction,tradetime,vol,ins);
}

void CSECUTrade::OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{
	//m_pTradeInterface->OnRspOrderInsert(ref,entrust_status,errorid);
	
}




/////////////////////////////////////////////////////////////
//CBusiness 实现
CBusiness::CBusiness()
{
	//memset(g_cUserToken, 0, sizeof(g_cUserToken));

	//g_iSystemNo = -1;

	//g_iBranchNo = 0;

	//g_bLogin = false;

}

CBusiness::~CBusiness()
{
	
}

int CBusiness::SendBizMessage(IF2Packer *lpF2Packer, int nFunction, int nIssueType, int nRequestID, void** ppRetData, int* pnItems, void *pRspInfo)
{
	//创建消息包
	IBizMessage *lpBizMessage = NewBizMessage();
	lpBizMessage->AddRef();
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	lpBizMessage->SetSubSystemNo(m_pTrade->g_iSystemNo);
	//lpBizMessage->SetCompanyID(m_CompanyID);
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
		g_pConnection->SendBizMsg(lpBizMessage, 1);
		//释放消息包
		lpBizMessage->Release();
		return 0;
	}
	else
	{
		//同步发送
		int hSend = g_pConnection->SendBizMsg(lpBizMessage);
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
		int iRet = g_pConnection->RecvBizMsg(hSend, &lpRecvBizMessage);
		if(iRet == 0)
		{
			//同步接收数据
			int iMsgLen = 0;
			void * lpMsgBuffer = lpRecvBizMessage->GetBuff(iMsgLen);
			//处理应答包
			//return DealSynBizMessage(lpMsgBuffer, iMsgLen, ppRetData, pnItems, pRspInfo);
		}
		else
		{
			//GetMsgErrorInfo(iRet, pRspInfo);
		}
		return iRet;
	}
}


int CBusiness::DealReceive(int funcid,IF2UnPacker *unpacker,int reqid)
{
	switch (funcid)
	{
	case	331100:onRsp331100(unpacker,reqid);break;
	case    338202:onRsp338202(unpacker,reqid);break;
	case    338217:onRsp338217(unpacker,reqid);break;
	case    338301:onRsp338301(unpacker,reqid);break;
	default:
		break;
	}


	return 1;
}

//登录
int CBusiness::onRsp331100(IF2UnPacker *unpacker,int reqid)
{
		CString login;

		login.Format("UFX->%s 登录成功", unpacker->GetStr("fund_account"));

		m_pTrade->Log(login.GetBuffer(0));

		m_pTrade->g_iSystemNo = unpacker->GetInt("sysnode_id");

		char * pt = (char *)unpacker->GetStr("user_token");

		if( pt )
		{
			strcpy_s(m_pTrade->g_cUserToken,pt);
		}

		m_pTrade->g_iBranchNo = unpacker->GetInt("branch_no");

		m_pTrade->g_bLogin = true;		

		Subscribe(33101,-9999);//订阅委托,成交回报
		
		//FutureAccountInfo();//查询账户

		return 1;
}


//下单应答
int CBusiness::onRsp338202(IF2UnPacker *unpacker,int reqid)
{
	CString code = (char*)unpacker->GetStr("futu_code"); 

	char entrust_status = unpacker->GetChar("entrust_status");

	int entrust_no = unpacker->GetInt("entrust_no");

	string refstr = unpacker->GetStr("entrust_reference");

	int ref = atoi(refstr.c_str());

	CString log;

	log.Format("下单成功，委托号:%d,报单引用:%d",entrust_no,ref);

	log = "onRsp338202->合约:" + code + log;

	this->m_pTrade->Log(log.GetBuffer(0));


	m_pTrade->OnRspOrderInsert(ref,-9999,entrust_status,0);

	return 1;
}

//撤单
int CBusiness::onRsp338217(IF2UnPacker *unpacker,int reqid)
{
	
	int entrust_no = unpacker->GetInt("entrust_no");

	CString code = (char*)unpacker->GetStr("futu_code");

	CString log;

	log.Format("撤单成功，委托号:%d",entrust_no);

	log = "onRsp338217->合约:" + code +  + log;

	this->m_pTrade->Log(log.GetBuffer(0));



	//需要判断一些是否是订单管理发出的请求
	//有两种情况 1.reqid=-1  2. reqid=-2 

	if(reqid!=-1 && reqid!=-2)
		return 1;

	for(int i=0;i<m_pTrade->m_pTradeInterface->m_arListOrder.GetSize();i++)
	{
			COrder * order = (COrder *)(m_pTrade->m_pTradeInterface->m_arListOrder[i]);

			if((order->m_nOrderRef == entrust_no) && (reqid==-1))     //reqid=-1   说明是线程发出，超时就撤单
			{
				//list里面删除对应订单

				//order->

				//m_secuTrade->m_pTradeInterface->m_arListOrder.RemoveAt(i);   //感觉需要去委托回报里面去删除

				//delete order;    //这里还需要考虑下

				break;
			}
			else if((order->m_nOrderRef == entrust_no) && (reqid==-2))   //需要重新追单
			{
				//order->m_szIntrument
				//order->m_pApi->InsertOrder(code1,);

				break;
			}
	}

	return 1;
	
}

int CBusiness::onRsp338301(IF2UnPacker *unpacker,int reqid)
{

	int entrust_no = unpacker->GetInt("entrust_no");

	char status = unpacker->GetChar("entrust_status");

	CString code = (char*)unpacker->GetStr("futu_code");

	CString log,st;

	log.Format("委托查询成功，委托号:%d,状态:%c",entrust_no,status);

	log = "onRsp338301->合约:" + code +  + log;

	m_pTrade->Log(log.GetBuffer(0));

	return 1;
}

//期货登录
int CBusiness::Login(char * account,char * pwd,int reqid)
{
	
	int iRet = 0, hSend = 0;
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);

	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}

	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);//操作分支机构
	pPacker->AddField("op_entrust_way", 'C', 1);//委托方式 
	pPacker->AddField("op_station", 'S', 255);//站点地址    
	pPacker->AddField("input_content", 'C'); 
	pPacker->AddField("account_content", 'S', 30);//资金账号
	pPacker->AddField("content_type", 'S', 6);
	pPacker->AddField("password", 'S', 10);  //账号密码 
	pPacker->AddField("password_type", 'C');   

	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('7');				
	pPacker->AddStr("1");				
	pPacker->AddChar('1');					
	pPacker->AddStr(account);			
	pPacker->AddStr("0");	
	pPacker->AddStr(pwd);			
	pPacker->AddChar('2'/*'1'*/);	  //交易密码			
	///结束打包
	pPacker->EndPack();

	///异步发送登录请求，应答在回调函数OnReceivedBizEx中处理。
	//if((hSend = g_pConnection->SendBizEx(331100,pPacker,NULL,ASYNCSEND,0,0,1,NULL)) < 0)
	if((hSend = SendBizMessage(pPacker,331100,0,reqid,NULL,NULL,NULL)) < 0)
	{
		char msg[128];

		sprintf(msg,"发送功能331100失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));

		m_pTrade->Log(msg);

		iRet = -2;
		
	}
	/*else
		m_pTrade->Log("发送功能331100成功!\r\n");*/


	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}

//期货客户资金查询
int CBusiness::FutureAccountInfo(char * account,char * pwd,int reqid)
{
	int iRet = 0, hSend = 0;
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);

	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}

	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);//操作分支机构
	pPacker->AddField("op_entrust_way", 'C', 1);//委托方式 
	pPacker->AddField("op_station", 'S', 255);//站点地址
	pPacker->AddField("branch_no", 'I',5); 
	pPacker->AddField("client_id", 'S', 18);
	pPacker->AddField("fund_account", 'S', 18);
	pPacker->AddField("password", 'S', 10);  //账号密码 
	pPacker->AddField("user_token", 'S',512);   

	///加入对应的字段值
	pPacker->AddInt(m_pTrade->g_iBranchNo);						
	pPacker->AddChar('7');				
	pPacker->AddStr("");				
	pPacker->AddInt(m_pTrade->g_iBranchNo);					
	pPacker->AddStr("");			
	pPacker->AddStr(m_pTrade->m_szUserID);	
	pPacker->AddStr(m_pTrade->m_szPwd);			
	pPacker->AddStr(m_pTrade->g_cUserToken);				
	///结束打包
	pPacker->EndPack();

	///异步发送登录请求，应答在回调函数OnReceivedBizEx中处理。
	//if((hSend = g_pConnection->SendBizEx(338300,pPacker,NULL,ASYNCSEND,0,0,1,NULL)) < 0)
	if((hSend = SendBizMessage(pPacker,338300,0,reqid,NULL,NULL,NULL)) < 0)
	{
		char msg[128];

		sprintf(msg,"发送功能338300失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));

		m_pTrade->Log(msg);

		iRet = -2;
		
	}
	/*else
		m_pTrade->Log("发送功能338300成功!\r\n");*/


	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}

//期货持仓查询
int CBusiness::QryFuturePosition(char * account,char * pwd,int reqid)
{
	int iRet = 0, hSend = 0;
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);

	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}

	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);//操作分支机构
	pPacker->AddField("op_entrust_way", 'C', 1);//委托方式 
	pPacker->AddField("op_station", 'S', 255);//站点地址
	pPacker->AddField("branch_no", 'I',5); 
	pPacker->AddField("client_id", 'S', 18);
	pPacker->AddField("fund_account", 'S', 18);
	pPacker->AddField("password", 'S', 10);  //账号密码 
	pPacker->AddField("user_token", 'S',512);   

	///加入对应的字段值
	pPacker->AddInt(m_pTrade->g_iBranchNo);						
	pPacker->AddChar('7');				
	pPacker->AddStr("");				
	pPacker->AddInt(m_pTrade->g_iBranchNo);					
	pPacker->AddStr(m_pTrade->m_szUserID);			
	pPacker->AddStr(m_pTrade->m_szUserID);	
	pPacker->AddStr(m_pTrade->m_szPwd);			
	pPacker->AddStr(m_pTrade->g_cUserToken);				
	///结束打包
	pPacker->EndPack();

	///异步发送登录请求，应答在回调函数OnReceivedBizEx中处理。
	//if((hSend = g_pConnection->SendBizEx(338303,pPacker,NULL,ASYNCSEND,0,0,1,NULL)) < 0)
	if((hSend = SendBizMessage(pPacker,338303,0,reqid,NULL,NULL,NULL)) < 0)
	{
		char msg[128];

		sprintf(msg,"发送功能338303失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));

		m_pTrade->Log(msg);

		iRet = -2;
		
	}
	/*else
		m_pTrade->Log("发送功能338303成功!\r\n");*/


	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}

//期货委托
int CBusiness::FutureEntrust(int iSystemNo,char * fundaccount,char * pwd,char * instrument, int count, char direction, char offsetflag, double price,int reqid)
{
	int iRet = 0, hSend = 0;	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);

	if(!pPacker)
	{
		m_pTrade->Log("取打包器失败!\r\n");
		return -1;
	}

	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///其他的应答信息
	LPRET_DATA pRetData = NULL;

	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("user_token", 'S', 512);//名字 类型 长度
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C');
	pPacker->AddField("op_station", 'S', 255); 
	pPacker->AddField("branch_no", 'I', 5);
	pPacker->AddField("client_id", 'S', 18);//客户ID
	pPacker->AddField("fund_account", 'S', 18);//资金账号
	pPacker->AddField("password", 'S', 50);

	pPacker->AddField("futu_exch_type", 'S', 4);
	pPacker->AddField("futures_account", 'S', 12);
	pPacker->AddField("futu_code", 'S', 30);
	pPacker->AddField("entrust_bs", 'C', 1);
	pPacker->AddField("futures_direction", 'C', 1);
	pPacker->AddField("hedge_type", 'C', 1);
	pPacker->AddField("entrust_amount", 'N', 10);
	pPacker->AddField("futu_entrust_price", 'F', 12, 6);	
	pPacker->AddField("entrust_reference", 'S', 32);
	

	
	///加入对应的字段值
	pPacker->AddStr(m_pTrade->g_cUserToken);							
	pPacker->AddInt(m_pTrade->g_iBranchNo);	
	pPacker->AddChar('7');
	pPacker->AddStr("");			
	pPacker->AddInt(m_pTrade->g_iBranchNo);	
	pPacker->AddStr("");
	pPacker->AddStr(fundaccount);
	pPacker->AddStr(pwd);
	//交易所类别
	pPacker->AddStr("F4");
	//交易编码
	pPacker->AddStr("");
	//futu_code
	pPacker->AddStr(instrument);
	//entrust_bs 1 买入，2卖出,


	//对外的下单接口是ctp标准的，ufx的还需要转换一下，
	char entrust_bs = ' ';
	if( direction == '0' )
		entrust_bs = '1';
	else if( direction == '1' )
		entrust_bs = '2';

	pPacker->AddChar(entrust_bs);

	//1	开仓  2	平仓  3	交割   4平今仓
	char offset = ' ';
	if( offsetflag == '0' )
		offset = '1';
	else if( offsetflag == '1' )
		offset = '2';
	else if( offsetflag == '3' )
		offset = '2';

	pPacker->AddChar(offset);

	//0	投机 1	套保 2	套利 3	做市商
	pPacker->AddChar('0');

	pPacker->AddInt(count);

	pPacker->AddDouble(price);	

	char OrderRef[32];
	sprintf(OrderRef, "%d", reqid);
	pPacker->AddStr(OrderRef);	

	
	///结束打包
	pPacker->EndPack();

	///异步发送委托功能，应答在回调函数OnReceivedBizEx中处理。
	//if((hSend = g_pConnection->SendBizEx(338202,pPacker,NULL,ASYNCSEND,iSystemNo,0,1,NULL)) < 0)
	if((hSend = SendBizMessage(pPacker,338202,0,reqid,NULL,NULL,NULL)) < 0)
	{
		char msg[256];

		sprintf(msg,"发送功能338202失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));

		m_pTrade->Log(msg);
		
		iRet = -2;
		
	}
	else
	{
		//char msg[256];

		//sprintf(msg,"发送功能338202成功, 返回接收句柄: %d!\r\n", hSend);

		//m_pTrade->Log(msg);

	}


	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return reqid;
}

int CBusiness::WithDraw(char * account,char * pwd,int ref,int reqid)
{
	int iRet = 0, hSend = 0;	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("op_branch_no", 'I', 5);	
	pPacker->AddField("op_entrust_way",'C',1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("client_id",'S',18);
	pPacker->AddField("password",'S',10);
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("entrust_reference", 'S', 32);
	
	///加入对应的字段值
	pPacker->AddInt(m_pTrade->g_iBranchNo);						
	pPacker->AddStr(account);					
	pPacker->AddInt(m_pTrade->g_iBranchNo);								
	pPacker->AddChar('7');							
	pPacker->AddStr("");		
	pPacker->AddStr("");					
	pPacker->AddStr(pwd);						
	pPacker->AddStr(m_pTrade->g_cUserToken);	
	char OrderRef[32];
	sprintf(OrderRef, "%d", ref);
	pPacker->AddStr(OrderRef);
	///结束打包
	pPacker->EndPack();

	//REQ_DATA szReqData;
	//memset(&szReqData,0,sizeof(REQ_DATA));
	//szReqData.issueType = issue_type;
	//szReqData.lpKeyInfo = pPacker->GetPackBuf();
	//szReqData.keyInfoLen = pPacker->GetPackLen();
	//
	///订阅
	//if((hSend = g_pConnection->SendBizEx(620001,pPacker, NULL, ASYNCSEND, 0, 0, /*branch_no*/ 1, &szReqData)) < 0)
	if((hSend = SendBizMessage(pPacker,338217,0,reqid,NULL,NULL,NULL)) < 0)
	{
		char msg[256];

		sprintf(msg,"发送功能338217失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));

		m_pTrade->Log(msg);

		iRet = -2;
	}
	else
	{
		//char msg[256];

		//sprintf(msg,"发送功能338217成功, 返回接收句柄: %d!\r\n", hSend);

		//m_pTrade->Log(msg);
	}

	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}


int CBusiness::QueryEntrust(char * account,char * pwd,int entrustno,int reqid)
{

	int iRet = 0, hSend = 0;	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("op_branch_no", 'I', 5);	
	pPacker->AddField("op_entrust_way",'C',1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("client_id",'S',18);
	pPacker->AddField("password",'S',10);
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("entrust_no", 'I', 18);
	
	///加入对应的字段值
	pPacker->AddInt(m_pTrade->g_iBranchNo);						
	pPacker->AddStr(account);					
	pPacker->AddInt(m_pTrade->g_iBranchNo);								
	pPacker->AddChar('7');							
	pPacker->AddStr("");		
	pPacker->AddStr("");					
	pPacker->AddStr(pwd);						
	pPacker->AddStr(m_pTrade->g_cUserToken);	
	pPacker->AddInt(entrustno);	
	///结束打包
	pPacker->EndPack();

	//if((hSend = g_pConnection->SendBizEx(620001,pPacker, NULL, ASYNCSEND, 0, 0, /*branch_no*/ 1, &szReqData)) < 0)
	if((hSend = SendBizMessage(pPacker,338301,0,reqid,NULL,NULL,NULL)) < 0)
	{
		char msg[256];

		sprintf(msg,"发送功能338217失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));

		m_pTrade->Log(msg);

		iRet = -2;
	}
	else
	{
		//char msg[256];

		//sprintf(msg,"发送功能338217成功, 返回接收句柄: %d!\r\n", hSend);

		//m_pTrade->Log(msg);
	}

	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;

}

int CBusiness::Subscribe(int issue_type,int reqid)
{
	int iRet = 0, hSend = 0;	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("op_branch_no", 'I', 5);	
	pPacker->AddField("op_entrust_way",'C',1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("client_id",'S',18);
	pPacker->AddField("password",'S',10);
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("issue_type",'I',8);	
	pPacker->AddField("noreport_flag",'C',1);    //订阅未报回报
	
	///加入对应的字段值
	pPacker->AddInt(m_pTrade->g_iBranchNo);						
	pPacker->AddStr(m_pTrade->m_szUserID);					
	pPacker->AddInt(m_pTrade->g_iBranchNo);								
	pPacker->AddChar('7');							
	pPacker->AddStr("");		
	pPacker->AddStr("");					
	pPacker->AddStr(m_pTrade->m_szPwd);						
	pPacker->AddStr(m_pTrade->g_cUserToken);				
	pPacker->AddInt(issue_type);
	pPacker->AddChar('1');
	///结束打包
	pPacker->EndPack();

	//REQ_DATA szReqData;
	//memset(&szReqData,0,sizeof(REQ_DATA));
	//szReqData.issueType = issue_type;
	//szReqData.lpKeyInfo = pPacker->GetPackBuf();
	//szReqData.keyInfoLen = pPacker->GetPackLen();
	//
	///订阅
	//if((hSend = g_pConnection->SendBizEx(620001,pPacker, NULL, ASYNCSEND, 0, 0, /*branch_no*/ 1, &szReqData)) < 0)
	if((hSend = SendBizMessage(pPacker,620001,issue_type,reqid,NULL,NULL,NULL)) < 0)
	{
		char msg[256];

		sprintf(msg,"发送功能620001失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));

		m_pTrade->Log(msg);

		iRet = -2;
	}
	else
	{
		//char msg[256];

		//sprintf(msg,"发送功能620001成功, 返回接收句柄: %d!\r\n", hSend);

		//m_pTrade->Log(msg);
	}

	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}

int CBusiness::CancelSub(int issueType,int reqid)
{
	int iRet = 0, hSend = 0;
	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("branch_no", 'I', 5);		
	pPacker->AddField("fund_account", 'S', 18);	
	pPacker->AddField("user_token",'S',40);    
	///加入对应的字段值
	pPacker->AddInt(1);					
	pPacker->AddStr("302000007");			
	pPacker->AddStr(m_pTrade->g_cUserToken);		    	
	///结束打包
	pPacker->EndPack();

	//REQ_DATA szReqData;
	//memset(&szReqData,0,sizeof(REQ_DATA));
	//szReqData.issueType = issueType;
	//szReqData.lpKeyInfo = pPacker->GetPackBuf();
	//szReqData.keyInfoLen = pPacker->GetPackLen();
	
	///取消订阅
	//if((hSend = g_pConnection->SendBizEx(620002,pPacker, NULL, SYNCSEND, 0, 0, /*branch_no*/ 1, &szReqData)) < 0)
	if((hSend = SendBizMessage(pPacker,620002,issueType,reqid,NULL,NULL,NULL)) < 0)
	{
		printf("发送功能620002失败, 错误号: %d, 原因: %s!\r\n", hSend, g_pConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}

	printf("发送功能620002成功, 返回接收句柄: %d!\r\n", hSend);

EXIT:
	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}




//////////////////证券实现类
/////////////////////////////////////////////////////////////
//CSECUBusiness 实现
CSECUBusiness::CSECUBusiness()
{
	//memset(g_cUserToken, 0, sizeof(g_cUserToken));

	//g_iSystemNo = -1;

	//g_iBranchNo = 0;

	//g_bLogin = false;

}

CSECUBusiness::~CSECUBusiness()
{
	
}

int CSECUBusiness::SendBizMessage(IF2Packer *lpF2Packer, int nFunction, int nIssueType, int nRequestID, void** ppRetData, int* pnItems, void *pRspInfo)
{
	//创建消息包
	IBizMessage *lpBizMessage = NewBizMessage();
	lpBizMessage->AddRef();
	lpBizMessage->SetPacketType(REQUEST_PACKET);
	lpBizMessage->SetSubSystemNo(m_secuTrade->g_iSystemNo);
	//lpBizMessage->SetCompanyID(m_CompanyID);
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
		g_SecuConnection->SendBizMsg(lpBizMessage, 1);
		//释放消息包
		lpBizMessage->Release();
		return 0;
	}
	else
	{
		//同步发送
		int hSend = g_SecuConnection->SendBizMsg(lpBizMessage);
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
		int iRet = g_SecuConnection->RecvBizMsg(hSend, &lpRecvBizMessage);
		if(iRet == 0)
		{
			//同步接收数据
			int iMsgLen = 0;
			void * lpMsgBuffer = lpRecvBizMessage->GetBuff(iMsgLen);
			//处理应答包
			//return DealSynBizMessage(lpMsgBuffer, iMsgLen, ppRetData, pnItems, pRspInfo);
		}
		else
		{
			//GetMsgErrorInfo(iRet, pRspInfo);
		}
		return iRet;
	}
}



int CSECUBusiness::DealReceive(int funcid,IF2UnPacker *unpacker,int reqid)
{
	switch (funcid)
	{
	case	331100:onRsp331100(unpacker ,reqid);break;
	case	333002:onRsp333002(unpacker, reqid);break;;
	default:
		break;
	}

	return 1;
}

int CSECUBusiness::onRsp331100(IF2UnPacker *unpacker,int orderref)
{

		m_secuTrade->g_iSystemNo = unpacker->GetInt("sysnode_id");

		char * pt = (char *)unpacker->GetStr("user_token");

		if( pt )
		{
			strcpy_s(m_secuTrade->g_cUserToken,pt);
		}

		m_secuTrade->g_iBranchNo = unpacker->GetInt("branch_no");

		m_secuTrade->g_bLogin = true;		

		Subscribe(23,-9999);//订阅委托回报

		Subscribe(12,-9999);//订阅成交回报
		
		//FutureAccountInfo();//查询账户

		return 1;
}


int CSECUBusiness::onRsp333002(IF2UnPacker *unpacker,int reqid)
{

	CString code = (char*)unpacker->GetStr("stock_code");

	int entrust_no = unpacker->GetInt("entrust_no");

	CString log;

	log.Format("下单成功，委托号：%d",entrust_no);

	log = "合约：" + code + log;

	this->m_secuTrade->Log(log.GetBuffer(0));


	//需要订单管理
	for(int i=0;i<m_secuTrade->m_pTradeInterface->m_arListOrder.GetSize();i++)
	{
			COrder * order = (COrder *)(m_secuTrade->m_pTradeInterface->m_arListOrder[i]);

			if(order->m_nOrderRef == reqid)
			{	
				order->m_nOrderRef = entrust_no;         //将报单引用更新成主场单号

				break;
			}	
	}

	//m_secuTrade->OnRspOrderInsert(ref,entrust_status);

	return 1;

}


int CSECUBusiness::onRsp333017(IF2UnPacker *unpacker,int reqid)
{

	//撤单成功
	
	int entrust_no = unpacker->GetInt("entrust_no");

	int entrust_no_old = unpacker->GetInt("entrust_no_old");

	CString code = (char*)unpacker->GetStr("stock_code");

	CString log;

	log.Format("撤单成功，原委托号：%d,新撤单委托号：",entrust_no_old,entrust_no);

	log = "合约：" + code +  + log;

	this->m_secuTrade->Log(log.GetBuffer(0));



	//需要判断一些是否是订单管理发出的请求
	//有两种情况 1.reqid=-1  2. reqid=-2 

	if(reqid!=-1 && reqid!=-2)
		return 1;

	for(int i=0;i<m_secuTrade->m_pTradeInterface->m_arListOrder.GetSize();i++)
	{
			COrder * order = (COrder *)(m_secuTrade->m_pTradeInterface->m_arListOrder[i]);

			if((order->m_nOrderRef == entrust_no_old) && (reqid==-1))     //reqid=-1   说明是线程发出，超时就撤单
			{
				//list里面删除对应订单

				//order->

				//m_secuTrade->m_pTradeInterface->m_arListOrder.RemoveAt(i);   //感觉需要去委托回报里面去删除

				//delete order;    //这里还需要考虑下

				break;
			}
			else if((order->m_nOrderRef == entrust_no_old) && (reqid==-2))   //需要重新追单
			{
				//order->m_szIntrument
				//order->m_pApi->InsertOrder(code1,);

				break;
			}
	}

	return 1;

}

//登录
int CSECUBusiness::Login(char * account,char * pwd,int reqid)
{
	int iRet = 0, hSend = 0;
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);

	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}

	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);//操作分支机构
	pPacker->AddField("op_entrust_way", 'C', 1);//委托方式 
	pPacker->AddField("op_station", 'S', 255);//站点地址    
	pPacker->AddField("input_content", 'C'); 
	pPacker->AddField("account_content", 'S', 30);//资金账号
	pPacker->AddField("content_type", 'S', 6);
	pPacker->AddField("password", 'S', 10);  //账号密码 
	pPacker->AddField("password_type", 'C');   

	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('7');				
	pPacker->AddStr("1");				
	pPacker->AddChar('1');					
	pPacker->AddStr(account);			
	pPacker->AddStr("0");	
	pPacker->AddStr(pwd);			
	pPacker->AddChar('2'/*'1'*/);	  //交易密码			
	///结束打包
	pPacker->EndPack();

	///异步发送登录请求，应答在回调函数OnReceivedBizEx中处理。
	//if((hSend = g_SecuConnection->SendBizEx(331100,pPacker,NULL,ASYNCSEND,0,0,1,NULL)) < 0)
	if((hSend = SendBizMessage(pPacker,331100,0,reqid,NULL,NULL,NULL)) < 0)
	{
		char msg[128];

		sprintf(msg,"发送功能331100失败, 错误号: %d, 原因: %s!\r\n", hSend, g_SecuConnection->GetErrorMsg(hSend));

		m_secuTrade->Log(msg);

		iRet = -2;
		
	}
	//else
	//	m_secuTrade->Log("发送功能331100成功!\r\n");


	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}

int CSECUBusiness::Subscribe(int issue_type,int reqid)
{
	int iRet = 0, hSend = 0;	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("op_branch_no", 'I', 5);	
	pPacker->AddField("op_entrust_way",'C',1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("client_id",'S',18);
	pPacker->AddField("password",'S',10);
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("issue_type",'I',8);	
	
	///加入对应的字段值
	pPacker->AddInt(m_secuTrade->g_iBranchNo);						
	pPacker->AddStr(m_secuTrade->m_szUserID);					
	pPacker->AddInt(m_secuTrade->g_iBranchNo);								
	pPacker->AddChar('7');							
	pPacker->AddStr("");		
	pPacker->AddStr("");					
	pPacker->AddStr(m_secuTrade->m_szPwd);						
	pPacker->AddStr(m_secuTrade->g_cUserToken);				
	pPacker->AddInt(issue_type);
	///结束打包
	pPacker->EndPack();

	/*REQ_DATA szReqData;
	memset(&szReqData,0,sizeof(REQ_DATA));
	szReqData.issueType = issue_type;
	szReqData.lpKeyInfo = pPacker->GetPackBuf();
	szReqData.keyInfoLen = pPacker->GetPackLen();*/
	
	///订阅
	//if((hSend = g_SecuConnection->SendBizEx(620001,pPacker, NULL, ASYNCSEND, 0, 0, /*branch_no*/ 1, &szReqData)) < 0)
	if((hSend = SendBizMessage(pPacker,620001,issue_type,reqid,NULL,NULL,NULL)) < 0)
	{
		char msg[256];

		sprintf(msg,"发送功能620001失败, 错误号: %d, 原因: %s!\r\n", hSend, g_SecuConnection->GetErrorMsg(hSend));

		m_secuTrade->Log(msg);

		iRet = -2;
	}
	else
	{
		//char msg[256];

		//sprintf(msg,"发送功能620001成功, 返回接收句柄: %d!\r\n", hSend);

		//m_secuTrade->Log(msg);
	}

	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;

}

//证券委托
int CSECUBusiness::SecuEntrust(int systemid,char * fundaccount,char * pwd,char * instrument, int count, char direction, double price,int reqid)
{
	int iRet = 0, hSend = 0;	
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);

	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();

	///定义解包器
	IF2UnPacker *pUnPacker = NULL;

	///其他的应答信息
	LPRET_DATA pRetData = NULL;

	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);//名字 类型 长度
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5); 
	pPacker->AddField("client_id", 'S', 18);//客户ID
	pPacker->AddField("fund_account", 'S', 18);//资金账号
	pPacker->AddField("password", 'S', 10);
	pPacker->AddField("password_type", 'C', 1);	
	pPacker->AddField("user_token", 'S', 40);
	pPacker->AddField("exchange_type", 'S', 4);
	pPacker->AddField("stock_account", 'S', 15);
	pPacker->AddField("stock_code", 'S', 6);
	pPacker->AddField("entrust_amount", 'F', 19, 2);
	pPacker->AddField("entrust_price", 'F', 18, 3);
	pPacker->AddField("entrust_bs", 'C', 1);
	pPacker->AddField("entrust_prop", 'S', 3);	
	pPacker->AddField("batch_no", 'I', 8);
	
	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('7');					
	pPacker->AddStr("1C659DDDD513");					
	pPacker->AddInt(m_secuTrade->g_iBranchNo);				
	pPacker->AddStr("0");			 
	pPacker->AddStr(fundaccount);			
	pPacker->AddStr(pwd);				
	pPacker->AddChar('2');					
	pPacker->AddStr(m_secuTrade->g_cUserToken);			
	printf("%s\n",m_secuTrade->g_cUserToken);
	pPacker->AddStr("");					
	pPacker->AddStr("");			
	pPacker->AddStr(instrument);				
	pPacker->AddDouble(count);				
	pPacker->AddDouble(price);				
	pPacker->AddChar(direction);					
	pPacker->AddStr("0");					
	pPacker->AddInt(reqid);						
	
	///结束打包
	pPacker->EndPack();

	///异步发送期货委托功能，应答在回调函数OnReceivedBizEx中处理。
	//if((hSend = g_SecuConnection->SendBizEx(333002,pPacker,NULL,ASYNCSEND,systemid,0,1,NULL)) < 0)
	if((hSend = SendBizMessage(pPacker,333002,0,reqid,NULL,NULL,NULL)) < 0)
	{
		char msg[256];

		sprintf(msg,"发送功能338202失败, 错误号: %d, 原因: %s!\r\n", hSend, g_SecuConnection->GetErrorMsg(hSend));

		m_secuTrade->Log(msg);

		iRet = -2;
		
	}
	//else
	//	printf("发送功能338202成功, 返回接收句柄: %d!\r\n", hSend);


	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return reqid;
}



//证券委托查询
int CSECUBusiness::SearchSecuEntrust(char * fundaccount,char * pwd,char * exchtype,int reqid)
{
	int iRet = 0, hSend = 0;
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();
	
	///定义解包器
	IF2UnPacker *pUnPacker = NULL;
	
	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("client_id", 'S', 18);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("password", 'S', 10);						
	pPacker->AddField("password_type", 'C', 1);		
	pPacker->AddField("user_token", 'S', 40);		
	
	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('7');					
	pPacker->AddStr("");					
	pPacker->AddInt(m_secuTrade->g_iBranchNo);				
	pPacker->AddStr("");			
	pPacker->AddStr(fundaccount);			
	pPacker->AddStr(pwd);				
	pPacker->AddChar('2');					
	pPacker->AddStr(m_secuTrade->g_cUserToken);			
	
	///结束打包
	pPacker->EndPack();
	
	///异步发送委托查询功能，应答在回调函数OnReceivedBizEx中处理。
	//if((hSend = g_SecuConnection->SendBizEx(333101,pPacker,NULL,ASYNCSEND,m_secuTrade->g_iSystemNo,0,1,NULL)) < 0)
	if((hSend = SendBizMessage(pPacker,333101,0,reqid,NULL,NULL,NULL)) < 0)
	{
		printf("发送功能333101失败, 错误号: %d, 原因: %s!\r\n", hSend, g_SecuConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	/*printf("发送功能333101成功, 返回接收句柄: %d!\r\n", hSend);*/
	
EXIT:
	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}

//委托撤单
int CSECUBusiness::SecuEntrustWithdraw(char * fundaccount,char * pwd,int entrustno,int reqid)
{
	int iRet = 0, hSend = 0;
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();
	
	///定义解包器
	IF2UnPacker *pUnPacker = NULL;
	
	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("client_id", 'S', 18);		
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("password", 'S', 10);						
	pPacker->AddField("password_type", 'C', 1);		
	pPacker->AddField("user_token", 'S', 40);		
	pPacker->AddField("entrust_no", 'I', 18);

	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('7');					
	pPacker->AddStr("");					
	pPacker->AddInt(m_secuTrade->g_iBranchNo);				
	pPacker->AddStr("");
	pPacker->AddStr(fundaccount);			
	pPacker->AddStr(pwd);				
	pPacker->AddChar('2');					
	pPacker->AddStr(m_secuTrade->g_cUserToken);
	pPacker->AddInt(entrustno);
	
	///结束打包
	pPacker->EndPack();
	
	///异步发送委托撤单功能，应答在回调函数OnReceivedBizEx中处理。
	//if((hSend = g_SecuConnection->SendBizEx(333017,pPacker,NULL,ASYNCSEND,m_secuTrade->g_iSystemNo,0,1,NULL)) < 0)
	if((hSend = SendBizMessage(pPacker,333017,0,reqid,NULL,NULL,NULL)) < 0)
	{
		printf("发送功能333017失败, 错误号: %d, 原因: %s!\r\n", hSend, g_SecuConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	/*printf("发送功能333017成功, 返回接收句柄: %d!\r\n", hSend);*/
	
EXIT:
	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}

//成交查询

int CSECUBusiness::SecuRealtimeQry(char * fundaccount,char * pwd,int reqid)
{
	int iRet = 0, hSend = 0;
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();
	
	///定义解包器
	IF2UnPacker *pUnPacker = NULL;
	
	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("client_id", 'S', 18);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("password", 'S', 10);						
	pPacker->AddField("password_type", 'C', 1);		
	pPacker->AddField("user_token", 'S', 40);		
	pPacker->AddField("exchange_type", 'S', 4);

	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('7');					
	pPacker->AddStr("");					
	pPacker->AddInt(m_secuTrade->g_iBranchNo);				
	pPacker->AddStr("");			
	pPacker->AddStr(fundaccount);			
	pPacker->AddStr(pwd);				
	pPacker->AddChar('2');					
	pPacker->AddStr(m_secuTrade->g_cUserToken);	
	pPacker->AddStr("");	
	
	///结束打包
	pPacker->EndPack();
	
	///异步发送成交查询功能，应答在回调函数OnReceivedBizEx中处理。
	//if((hSend = g_SecuConnection->SendBizEx(333102,pPacker,NULL,ASYNCSEND,m_secuTrade->g_iSystemNo,0,1,NULL)) < 0)
	if((hSend = SendBizMessage(pPacker,333102,0,reqid,NULL,NULL,NULL)) < 0)
	{
		printf("发送功能333102失败, 错误号: %d, 原因: %s!\r\n", hSend, g_SecuConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	/*printf("发送功能333102成功, 返回接收句柄: %d!\r\n", hSend);*/
	
EXIT:
	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}

//持仓查询
int CSECUBusiness::SecuStockQry(char * fundaccount,char * pwd,int reqid)
{
	int iRet = 0, hSend = 0;
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();
	
	///定义解包器
	IF2UnPacker *pUnPacker = NULL;
	
	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("client_id", 'S', 18);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("password", 'S', 10);						
	pPacker->AddField("password_type", 'C', 1);		
	pPacker->AddField("user_token", 'S', 40);		

	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('7');					
	pPacker->AddStr("");					
	pPacker->AddInt(m_secuTrade->g_iBranchNo);				
	pPacker->AddStr("");			
	pPacker->AddStr(fundaccount);			
	pPacker->AddStr(pwd);				
	pPacker->AddChar('1');					
	pPacker->AddStr(m_secuTrade->g_cUserToken);	

	
	///结束打包
	pPacker->EndPack();
	
	///异步发送持仓查询功能，应答在回调函数OnReceivedBizEx中处理。
	//if((hSend = g_SecuConnection->SendBizEx(333104,pPacker,NULL,ASYNCSEND,m_secuTrade->g_iSystemNo,0,1,NULL)) < 0)
	if((hSend = SendBizMessage(pPacker,333104,0,reqid,NULL,NULL,NULL)) < 0)
	{
		printf("发送功能333104失败, 错误号: %d, 原因: %s!\r\n", hSend, g_SecuConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	/*printf("发送功能333104成功, 返回接收句柄: %d!\r\n", hSend);*/
	
EXIT:
	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}

//资金
int CSECUBusiness::ClientFundAllQry(char * fundaccount,char * pwd,int reqid)
{
	int iRet = 0, hSend = 0;
	///获取版本为2类型的pack打包器
	IF2Packer *pPacker = NewPacker(2);
	if(!pPacker)
	{
		printf("取打包器失败!\r\n");
		return -1;
	}
	pPacker->AddRef();
	
	///定义解包器
	IF2UnPacker *pUnPacker = NULL;
	
	///其他的应答信息
	LPRET_DATA pRetData = NULL;
	///开始打包
	pPacker->BeginPack();
	
	///加入字段名
	pPacker->AddField("op_branch_no", 'I', 5);
	pPacker->AddField("op_entrust_way", 'C', 1);
	pPacker->AddField("op_station", 'S', 255);
	pPacker->AddField("branch_no", 'I', 5);			
	pPacker->AddField("client_id", 'S', 18);			
	pPacker->AddField("fund_account", 'S', 18);		
	pPacker->AddField("password", 'S', 10);						
	pPacker->AddField("password_type", 'C', 1);		
	pPacker->AddField("user_token", 'S', 40);		

	///加入对应的字段值
	pPacker->AddInt(0);						
	pPacker->AddChar('7');					
	pPacker->AddStr("");					
	pPacker->AddInt(m_secuTrade->g_iBranchNo);				
	pPacker->AddStr("");			
	pPacker->AddStr(fundaccount);			
	pPacker->AddStr(pwd);				
	pPacker->AddChar('2');					
	pPacker->AddStr(m_secuTrade->g_cUserToken);	
	
	
	///结束打包
	pPacker->EndPack();
	
	///异步发送资金查询功能，应答在回调函数OnReceivedBizEx中处理。
	//if((hSend = g_SecuConnection->SendBizEx(332255,pPacker,NULL,ASYNCSEND,m_secuTrade->g_iSystemNo,0,1,NULL)) < 0)
	if((hSend = SendBizMessage(pPacker,332255,0,reqid,NULL,NULL,NULL)) < 0)
	{
		printf("发送功能332255失败, 错误号: %d, 原因: %s!\r\n", hSend, g_SecuConnection->GetErrorMsg(hSend));
		iRet = -2;
		goto EXIT;
	}
	
	/*printf("发送功能332255成功, 返回接收句柄: %d!\r\n", hSend);*/
	
EXIT:
	///释放pack中申请的内存，如果不释放就直接release可能会内存泄漏
	if(pPacker)
	{
		pPacker->FreeMem(pPacker->GetPackBuf());
		///释放申请的pack
		pPacker->Release();
	}
	
	return iRet;
}





////////////////////////////////////////////////////////////
//Callback 实现
unsigned long CCallback::QueryInterface(const char *iid, IKnown **ppv)
{
    return 0;
}

unsigned long CCallback::AddRef()
{
    return 0;
}

unsigned long CCallback::Release()
{
    return 0;
}

// 以下各回调方法的实现仅仅为演示使用
void CCallback::OnConnect(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnConnect");
}

void CCallback::OnSafeConnect(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnSafeConnect");
}

void CCallback::OnRegister(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnRegister");
}

void CCallback::OnClose(CConnectionInterface *lpConnection)
{
    puts("CCallback::OnClose");
}

void CCallback::OnSent(CConnectionInterface *lpConnection, int hSend, void *reserved1, void *reserved2, int nQueuingData)
{
	puts("CCallback::OnSent");
}

void CCallback::Reserved1(void *a, void *b, void *c, void *d)
{
	puts("CCallback::Reserved1");
}

void CCallback::Reserved2(void *a, void *b, void *c, void *d)
{
	puts("CCallback::Reserved2");
}

void CCallback::OnReceivedBizEx(CConnectionInterface *lpConnection, int hSend, LPRET_DATA lpRetData, const void *lpUnpackerOrStr, int nResult)
{
//	char msg[128];
//
//	sprintf(msg,"CCallback::OnReceivedBizEx %d",lpRetData->functionID);
//
//	CallbackLog(msg);
//
//	int ret = 0;
//	
//	char* pErrInfo = lpRetData->errorInfo;
//
//	int iErrNo=lpRetData->errorNo;
//
//	int iFuncID = lpRetData->functionID;
//
//	int iIssueType = lpRetData->issueType;
//
//	int iRetCode = lpRetData->returnCode;
//
//	int iKeyInfoLen = lpRetData->keyInfoLen;
//
//	char* pKeyInfo = (char*)lpRetData->lpKeyInfo;
//
//	int iLen = lpRetData->keyInfoLen;
//	
//	if(iErrNo != 0 || iRetCode != 0)
//	{
//		ShowPacket(0, (IF2UnPacker *)lpUnpackerOrStr);
//
//		//m_secuBusiness->m_secuTrade->g_bLogin = true;    //证券
//
//		char msg[256];
//
//		sprintf(msg,"功能号: %d, 返回错误号: %d, 返回码: %d, 错误信息: %s!\r\n",iFuncID, iErrNo, iRetCode, pErrInfo ? pErrInfo : "NULL");
//
//		CallbackLog(msg);
//
//		sprintf(msg,"返回接收句柄: %d!\r\n", hSend);
//
//		CallbackLog(msg);
//
//		return;
//	}
//	else if(620000 == iFuncID)	//心跳
//	{
//		CallbackLog("收到心跳620000:");	
//
//        void *Pointer = NULL;
//
//		REQ_DATA reqData;
//
//		memset(&reqData,0,sizeof(REQ_DATA));
//
//		reqData.issueType = 12;		//此处需要确认，心跳应答是否区分订阅类型
//
//		reqData.packetType = 1;		//转换为应答包
//
//		memcpy(&reqData.routeInfo, &lpRetData->sendInfo, sizeof(reqData.routeInfo));
//
//		if(m_pBusiness!=NULL)
//		{
//			//消息中心发包   期货
//			if((ret = m_pBusiness->g_pConnection->SendBizEx(620000, NULL, NULL, ASYNCSEND, 0, 0, 0, &reqData)) < 0)
//			{
//				puts(m_pBusiness->g_pConnection->GetErrorMsg(ret));
//			}
//		}
//		else if(m_secuBusiness!=NULL)
//		{
//			//消息中心发包   证券
//			if((ret = m_secuBusiness->g_SecuConnection->SendBizEx(620000, NULL, NULL, ASYNCSEND, 0, 0, 0, &reqData)) < 0)
//			{
//				puts(m_secuBusiness->g_SecuConnection->GetErrorMsg(ret));
//			}
//		}
//
//	}
//	else if(620001 == iFuncID)	//订阅返回消息
//	{
//		puts("收到订阅返回消息620001:");
//
//		IF2UnPacker * lpUnapck = NULL;
//
//		if (lpRetData != NULL && (lpRetData->issueType == ISSUE_TYPE_REALTIME_SECU))	//成交主推
//		{
//			lpUnapck = NewUnPacker(lpRetData->lpKeyInfo, lpRetData->keyInfoLen);
//
//			if(lpUnapck)
//			{
//				lpUnapck->AddRef();
//
//				ShowPacket(0, (IF2UnPacker *)lpUnapck);
//
//				lpUnapck->Release();
//			}
//		}
//	}
//	else if(620003 == iFuncID)	//成交主推
//	{
//		//收到成交主推消息
//		puts("收到主推消息620003:");
//
//		IF2UnPacker * lpUnapck = NULL;
//
//		if (lpRetData != NULL)	//成交主推
//		{
//			int issue_type = lpRetData->issueType;
//
//			if(issue_type == ISSUE_TYPE_REALTIME_SECU) ///证券成交类型
//			{
//				lpUnapck = NewUnPacker(lpRetData->lpKeyInfo, lpRetData->keyInfoLen);
//
//				if(lpUnapck)
//				{
//					lpUnapck->AddRef();
//
//					ShowPacket(0, (IF2UnPacker *)lpUnapck);
//
//					lpUnapck->Release();
//				}
//			}
//		}
//	}
//	else if( iFuncID == 331100 || iFuncID == 338300 || iFuncID == 338303 || iFuncID == 333002 || iFuncID == 333101 || iFuncID ==333017 || iFuncID ==333102 || iFuncID == 333104 || iFuncID == 332255)	///委托查询异步应答处理
//	{
//		ShowPacket(0, (IF2UnPacker *)lpUnpackerOrStr);
//
//		if(m_pBusiness!=NULL)
//		{
//			m_pBusiness->DealReceive(iFuncID,(IF2UnPacker *)lpUnpackerOrStr);
//		}
//		else if(m_secuBusiness!=NULL)
//		{
//			m_secuBusiness->DealReceive(iFuncID,(IF2UnPacker *)lpUnpackerOrStr);
//		}
//
//	}

	
}

void CCallback::OnReceivedBiz(CConnectionInterface *lpConnection, int hSend, const void *lpUnPackerOrStr, int nResult)
{
    puts("CCallback::OnReceivedBiz");
}

int  CCallback::Reserved3()
{
	puts("CCallback::Reserved3");
    return 0;
}

void CCallback::Reserved4()
{
	puts("CCallback::Reserved4");
}

void CCallback::Reserved5()
{
	puts("CCallback::Reserved5");
}

void CCallback::Reserved6()
{
	puts("CCallback::Reserved6");
}

void CCallback::Reserved7()
{
	puts("CCallback::Reserved7");
}

void CCallback::OnReceivedBizMsg(CConnectionInterface *lpConnection, int hSend, IBizMessage* lpMsg)
{
	if (lpMsg==NULL)
		return;

	int iLen = 0;

    const void *lpBuffer = lpMsg->GetContent(iLen);

    IF2UnPacker *lpUnPacker = NewUnPacker((void *)lpBuffer,iLen);

    if(lpUnPacker != NULL)
		lpUnPacker->AddRef();

    int iLen_key = 0;

    const void *pKeyInfo = lpMsg->GetKeyInfo(iLen_key);

    IF2UnPacker *lpUnPacker_key = NewUnPacker((void *)pKeyInfo,iLen_key);

    if(lpUnPacker_key != NULL)
		lpUnPacker_key->AddRef();


	char msg[128];

	sprintf(msg,"CCallback::OnReceivedBizMsg %d",lpMsg->GetFunction());

	//if(lpMsg->GetFunction()!=620000)
	//	CallbackLog(msg);

	int ret = 0;
	
	const char* pErrInfo = lpMsg->GetErrorInfo();

	int iErrNo= lpMsg->GetErrorNo();

	int iFuncID = lpMsg->GetFunction();

	int iIssueType = lpMsg->GetIssueType();

	int iRetCode = lpMsg->GetReturnCode();

	if(iLen==0 && iLen_key==0)
		return;

	
	if(iErrNo != 0 || iRetCode != 0)
	{
		ShowPacket(0, lpUnPacker);

		char msg[256];

		sprintf(msg,"功能号: %d, 返回错误号: %d, 返回码: %d, 错误信息: %s!\r\n",iFuncID, iErrNo, iRetCode, pErrInfo ? pErrInfo : "NULL");

		CallbackLog(msg);

		sprintf(msg,"返回接收句柄: %d!\r\n", hSend);

		CallbackLog(msg);

		return;
	}
	else if(620000 == iFuncID)	//心跳
	{
		//CallbackLog("收到心跳620000:");	

		int iLen;
		const void * lpBuffer = lpMsg->GetBuff(iLen);

		IBizMessage* p = NewBizMessage();

		p->AddRef();

		p->SetBuff(lpBuffer, iLen);

        if (p->GetPacketType() == REQUEST_PACKET)
		{
			p->ChangeReq2AnsMessage();

			if(NULL != lpConnection)
				lpConnection->SendBizMsg(p, 1);
		}
		p->Release();

	}
	else if(620001 == iFuncID)	//订阅返回消息
	{
		puts("收到订阅返回消息620001:");

	}
	else if(620003 == iFuncID)	//成交主推
	{
		//收到成交主推消息
		puts("收到主推消息620003:");

		CallbackLog("收到主推消息620003");

	    int reqid = lpMsg->GetSenderId();

		int type = lpMsg->GetIssueType();

		if(m_secuBusiness!=NULL)
		{

			int entrustno = lpUnPacker_key->GetInt("entrust_no");

			char status = lpUnPacker_key->GetChar("entrust_status");

			char *code = (char *)lpUnPacker_key->GetStr("stock_code");


			if(type == 12)    //成交	
			{
				int amount = lpUnPacker_key->GetInt("business_amount");

				m_secuBusiness->m_secuTrade->RtnTrade("",entrustno,1,0,'0','0',"0",amount,code);

			}
			else if(type == 23)   //委托
			{
				m_secuBusiness->m_secuTrade->RtnOrder(entrustno,1,status,'0','0',code,0,0,0);

			}
						
		}
		else if(m_pBusiness!=NULL)
		{

			char ly = lpUnPacker_key->GetChar("LY");

			CString QH = lpUnPacker_key->GetStr("QH");

			if(type == 33101 && ly=='B')    //成交	
			{
				string entrust_no = FindField(QH, 1, '\1');

				int entrustno = atoi(entrust_no.c_str());

				string entrust_status = FindField(QH, 20, '\1');

				char status = entrust_status[0];

				char * code = (char*)(FindField(QH, 5, '\1').c_str());

				string count = FindField(QH, 9, '\1');   //此次成交

				int t_b_amount = atoi(count.c_str());
				

				m_pBusiness->m_pTrade->RtnTrade("",entrustno,1,0,'0','0',"0",t_b_amount,code);


			}
			else if(type == 33101)   //委托
			{
				string entrust_no = FindField(QH, 1, '\1');

				int entrustno = atoi(entrust_no.c_str());

				string entrust_status = FindField(QH, 17, '\1');

				char status = entrust_status[0];

				char * code = (char*)(FindField(QH, 4, '\1').c_str());


				m_pBusiness->m_pTrade->RtnOrder(entrustno,1,status,'0','0',code,0,0,0);

			}
			
		}


	}
	else /*if( iFuncID == 331100 || iFuncID == 338300 || iFuncID == 338303 || iFuncID == 333002 || iFuncID == 333101 || iFuncID ==333017 || iFuncID ==333102 || iFuncID == 333104 || iFuncID == 332255)*/	///委托查询异步应答处理
	{
		ShowPacket(0, lpUnPacker);

		int reqid = lpMsg->GetSenderId();

		if(m_pBusiness!=NULL)
		{
			m_pBusiness->DealReceive(iFuncID,lpUnPacker,reqid);
		}
		else if(m_secuBusiness!=NULL)
		{
			m_secuBusiness->DealReceive(iFuncID,lpUnPacker,reqid);
		}

	}

	if(lpUnPacker != NULL)
		lpUnPacker->Release();

    if(lpUnPacker_key != NULL)
		lpUnPacker_key->Release();


}

void CCallback::CallbackLog(char *msg)
{

	if(m_pBusiness!=NULL)
	{
		m_pBusiness->m_pTrade->Log(msg);
	}
	else if(m_secuBusiness!=NULL)
	{
		m_secuBusiness->m_secuTrade->Log(msg);
	}
	else
	{
		//nothing
	}

}
