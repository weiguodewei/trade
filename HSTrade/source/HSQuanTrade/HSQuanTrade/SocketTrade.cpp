#include "stdafx.h"
#include "SocketTrade.h"
#include "TradeInterface.h"
#include "TradeCommHeaders.h"
#include "tool.h"


CSocketTrade:: CSocketTrade()
{

}

CSocketTrade::~CSocketTrade()
{

}


void CSocketTrade::init(CString serverAddress)
{
	//这里需要注册
	SubEntrust();

	SubTrade();

}


int CSocketTrade::InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int orderref)
{
	string info = Tool::TransEntrustInfo(instrument,count,direction,offsetflag,price,orderref,this->m_szUserID);

	char entrustinfo[256];
	
	strcpy(entrustinfo,info.c_str());


	char buf[MAX_BUF_LEN];
	memset(buf,0,MAX_BUF_LEN);

	int pos = 0;
	PackerHeader packer;
	packer.HeaderFlag = 7611;
	packer.DataLength = sizeof(ProtocolHeader) + sizeof(entrustinfo);
	memcpy(buf,(void*)&packer,sizeof(PackerHeader));
	pos += sizeof(PackerHeader);

	ProtocolHeader protocol;
	protocol.Action = 8888;
	protocol.Data1 = 0;
	protocol.Data2 = 0;
	protocol.Data3 = 0;
	protocol.Data4 = 0;
	protocol.DataLength = sizeof(entrustinfo);
	protocol.Data5 = 0;
	memcpy(buf+pos,(void*)&protocol,sizeof(ProtocolHeader));
	pos += sizeof(ProtocolHeader);

	memcpy(buf+pos,(void*)entrustinfo,sizeof(entrustinfo));
	pos += sizeof(entrustinfo);
	SOCKET client = m_pTradeInterface->m_ServerSock.GetSockAccept();

	if(!m_pTradeInterface->m_ServerSock.Send(client,buf,pos))
	{
		TRACE("委托请求失败！\n");
	}
	else
	{
		CTime tm = CTime::GetCurrentTime();
		CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		CString str;
		str.Format(_T("%s    委托请求：%s\n"), strTime, entrustinfo);
		//m_lstBoxSockMsg.InsertString(0, str);
	
	}

	return orderref;
}

int CSocketTrade::WithDraw(int entrustno,char *sCode)
{
	//test
	if(entrustno==-1)
	{
		SubEntrust();

		//SubTrade();

		return 0;
	}

	//test
	if(entrustno==-2)
	{
		QueryEntrust(0,1);

		return 0;
	
	}
	else if(entrustno==-3)
	{
		QueryFund(m_szUserID);

		return 0;
	
	}
	else if(entrustno==-4)
	{
		QueryPosition(m_szUserID);

		return 0;
	
	}


	string info = Tool::TransWithDraw(entrustno,m_szUserID);

	//test
	//return 0;
	//test

	char withdrawinfo[128];
	
	strcpy(withdrawinfo,info.c_str());

	char buf[MAX_BUF_LEN];
	memset(buf,0,MAX_BUF_LEN);

	int pos = 0;
	PackerHeader packer;
	packer.HeaderFlag = 7611;
	packer.DataLength = sizeof(ProtocolHeader) + sizeof(withdrawinfo);
	memcpy(buf,(void*)&packer,sizeof(PackerHeader));
	pos += sizeof(PackerHeader);

	ProtocolHeader protocol;
	protocol.Action = 8888;
	protocol.Data1 = 0;
	protocol.Data2 = 0;
	protocol.Data3 = 0;
	protocol.Data4 = 0;
	protocol.DataLength = sizeof(withdrawinfo);
	protocol.Data5 = 0;
	memcpy(buf+pos,(void*)&protocol,sizeof(ProtocolHeader));
	pos += sizeof(ProtocolHeader);

	memcpy(buf+pos,(void*)withdrawinfo,sizeof(withdrawinfo));
	pos += sizeof(withdrawinfo);
	SOCKET client = m_pTradeInterface->m_ServerSock.GetSockAccept();

	if(!m_pTradeInterface->m_ServerSock.Send(client,buf,pos))
	{
		TRACE("撤单请求失败！\n");
	}
	else
	{
		CTime tm = CTime::GetCurrentTime();
		CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		CString str;
		str.Format(_T("%s    撤单请求：%s\n"), strTime, withdrawinfo);
		//m_lstBoxSockMsg.InsertString(0, str);
	
	}

	return 0;
}

int CSocketTrade::QueryEntrust(int entrustno,int reqid)
{
	string info = Tool::TransQueryInfo(0,m_szUserID);

	char socketinfo[128];
	
	strcpy(socketinfo,info.c_str());

	char buf[MAX_BUF_LEN];
	memset(buf,0,MAX_BUF_LEN);

	int pos = 0;
	PackerHeader packer;
	packer.HeaderFlag = 7611;
	packer.DataLength = sizeof(ProtocolHeader) + sizeof(socketinfo);
	memcpy(buf,(void*)&packer,sizeof(PackerHeader));
	pos += sizeof(PackerHeader);

	ProtocolHeader protocol;
	protocol.Action = 8888;
	protocol.Data1 = 0;
	protocol.Data2 = 0;
	protocol.Data3 = 0;
	protocol.Data4 = 0;
	protocol.DataLength = sizeof(socketinfo);
	protocol.Data5 = 0;
	memcpy(buf+pos,(void*)&protocol,sizeof(ProtocolHeader));
	pos += sizeof(ProtocolHeader);

	memcpy(buf+pos,(void*)socketinfo,sizeof(socketinfo));
	pos += sizeof(socketinfo);
	SOCKET client = m_pTradeInterface->m_ServerSock.GetSockAccept();

	if(!m_pTradeInterface->m_ServerSock.Send(client,buf,pos))
	{
		TRACE("委托查询失败！\n");
	}
	else
	{
		CTime tm = CTime::GetCurrentTime();
		CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		CString str;
		str.Format(_T("%s    委托查询：%s\n"), strTime, socketinfo);
		//m_lstBoxSockMsg.InsertString(0, str);
	
	}

	return 0;
}

int CSocketTrade::QueryPosition(char *account)
{
	string info = Tool::TransQueryInfo(2,m_szUserID);

	char socketinfo[128];
	
	strcpy(socketinfo,info.c_str());

	char buf[MAX_BUF_LEN];
	memset(buf,0,MAX_BUF_LEN);

	int pos = 0;
	PackerHeader packer;
	packer.HeaderFlag = 7611;
	packer.DataLength = sizeof(ProtocolHeader) + sizeof(socketinfo);
	memcpy(buf,(void*)&packer,sizeof(PackerHeader));
	pos += sizeof(PackerHeader);

	ProtocolHeader protocol;
	protocol.Action = 8888;
	protocol.Data1 = 0;
	protocol.Data2 = 0;
	protocol.Data3 = 0;
	protocol.Data4 = 0;
	protocol.DataLength = sizeof(socketinfo);
	protocol.Data5 = 0;
	memcpy(buf+pos,(void*)&protocol,sizeof(ProtocolHeader));
	pos += sizeof(ProtocolHeader);

	memcpy(buf+pos,(void*)socketinfo,sizeof(socketinfo));
	pos += sizeof(socketinfo);
	SOCKET client = m_pTradeInterface->m_ServerSock.GetSockAccept();

	if(!m_pTradeInterface->m_ServerSock.Send(client,buf,pos))
	{
		TRACE("委托查询失败！\n");
	}
	else
	{
		CTime tm = CTime::GetCurrentTime();
		CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		CString str;
		str.Format(_T("%s    委托查询：%s\n"), strTime, socketinfo);
		//m_lstBoxSockMsg.InsertString(0, str);
	
	}

	return 0;

}

int CSocketTrade::QueryTrade(char *account)
{

	return 0;
}

int CSocketTrade::QueryFund(char *account)
{

	string info = Tool::TransQueryInfo(3,m_szUserID);

	char socketinfo[128];
	
	strcpy(socketinfo,info.c_str());

	char buf[MAX_BUF_LEN];
	memset(buf,0,MAX_BUF_LEN);

	int pos = 0;
	PackerHeader packer;
	packer.HeaderFlag = 7611;
	packer.DataLength = sizeof(ProtocolHeader) + sizeof(socketinfo);
	memcpy(buf,(void*)&packer,sizeof(PackerHeader));
	pos += sizeof(PackerHeader);

	ProtocolHeader protocol;
	protocol.Action = 8888;
	protocol.Data1 = 0;
	protocol.Data2 = 0;
	protocol.Data3 = 0;
	protocol.Data4 = 0;
	protocol.DataLength = sizeof(socketinfo);
	protocol.Data5 = 0;
	memcpy(buf+pos,(void*)&protocol,sizeof(ProtocolHeader));
	pos += sizeof(ProtocolHeader);

	memcpy(buf+pos,(void*)socketinfo,sizeof(socketinfo));
	pos += sizeof(socketinfo);
	SOCKET client = m_pTradeInterface->m_ServerSock.GetSockAccept();

	if(!m_pTradeInterface->m_ServerSock.Send(client,buf,pos))
	{
		TRACE("委托资金失败！\n");
	}
	else
	{
		CTime tm = CTime::GetCurrentTime();
		CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		CString str;
		str.Format(_T("%s    委托资金：%s\n"), strTime, socketinfo);
		//m_lstBoxSockMsg.InsertString(0, str);
	
	}

	return 0;
}


void CSocketTrade::RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount)
{

}

void CSocketTrade::RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)
{
}

void CSocketTrade::OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{
	m_pTradeInterface->OnRspOrderInsert(ref,entrustno,entrust_status,errorid);
}

void CSocketTrade::SubEntrust()
{

	string info = Tool::TransSubInfo(0,this->m_szUserID);

	char socketinfo[128];
	
	strcpy(socketinfo,info.c_str());

	char buf[MAX_BUF_LEN];
	memset(buf,0,MAX_BUF_LEN);

	int pos = 0;
	PackerHeader packer;
	packer.HeaderFlag = 7611;
	packer.DataLength = sizeof(ProtocolHeader) + sizeof(socketinfo);
	memcpy(buf,(void*)&packer,sizeof(PackerHeader));
	pos += sizeof(PackerHeader);

	ProtocolHeader protocol;
	protocol.Action = 8888;
	protocol.Data1 = 0;
	protocol.Data2 = 0;
	protocol.Data3 = 0;
	protocol.Data4 = 0;
	protocol.DataLength = sizeof(socketinfo);
	protocol.Data5 = 0;
	memcpy(buf+pos,(void*)&protocol,sizeof(ProtocolHeader));
	pos += sizeof(ProtocolHeader);

	memcpy(buf+pos,(void*)socketinfo,sizeof(socketinfo));
	pos += sizeof(socketinfo);
	SOCKET client = m_pTradeInterface->m_ServerSock.GetSockAccept();

	if(!m_pTradeInterface->m_ServerSock.Send(client,buf,pos))
	{
		TRACE("订阅委托回报失败！\n");
	}
	else
	{
		CTime tm = CTime::GetCurrentTime();
		CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		CString str;
		str.Format(_T("%s    订阅委托回报：%s\n"), strTime, socketinfo);
		//m_lstBoxSockMsg.InsertString(0, str);
	
	}

	return;

}

void CSocketTrade::SubTrade()
{

	string info = Tool::TransSubInfo(1,this->m_szUserID);

	char socketinfo[128];
	
	strcpy(socketinfo,info.c_str());

	char buf[MAX_BUF_LEN];
	memset(buf,0,MAX_BUF_LEN);

	int pos = 0;
	PackerHeader packer;
	packer.HeaderFlag = 7611;
	packer.DataLength = sizeof(ProtocolHeader) + sizeof(socketinfo);
	memcpy(buf,(void*)&packer,sizeof(PackerHeader));
	pos += sizeof(PackerHeader);

	ProtocolHeader protocol;
	protocol.Action = 8888;
	protocol.Data1 = 0;
	protocol.Data2 = 0;
	protocol.Data3 = 0;
	protocol.Data4 = 0;
	protocol.DataLength = sizeof(socketinfo);
	protocol.Data5 = 0;
	memcpy(buf+pos,(void*)&protocol,sizeof(ProtocolHeader));
	pos += sizeof(ProtocolHeader);

	memcpy(buf+pos,(void*)socketinfo,sizeof(socketinfo));
	pos += sizeof(socketinfo);
	SOCKET client = m_pTradeInterface->m_ServerSock.GetSockAccept();

	if(!m_pTradeInterface->m_ServerSock.Send(client,buf,pos))
	{
		TRACE("订阅成交回报！\n");
	}
	else
	{
		CTime tm = CTime::GetCurrentTime();
		CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");
		CString str;
		str.Format(_T("%s    订阅成交回报：%s\n"), strTime, socketinfo);
		//m_lstBoxSockMsg.InsertString(0, str);
	
	}

	return;


}


//报单状态信息
bool CSocketTrade::IsWeiBao(char status)
{
	bool ret = false;
	if(status == '0')
		ret = true;

	return ret;
}

bool CSocketTrade::IsDaiBao(char status)
{
	bool ret = false;
	if(status == '1')
		ret = true;

	return ret;
}

bool CSocketTrade::IsYiBao(char status)
{
	bool ret = false;
	if(status == '2')
		ret = true;

	return ret;
}

bool CSocketTrade::IsBuChe(char status)
{
	bool ret = false;
	if(status == '5')
		ret = true;

	return ret;
}

bool CSocketTrade::IsYiChe(char status)
{
	bool ret = false;
	if(status == '6')
		ret = true;

	return ret;
}

bool CSocketTrade::IsBuCheng(char status)
{
	bool ret = false;
	if(status == '7')
		ret = true;

	return ret;
}

bool CSocketTrade::IsYiCheng(char status)
{
	bool ret = false;
	if(status == '8')
		ret = true;

	return ret;
}

bool CSocketTrade::IsFeiDan(char status)
{
	bool ret = false;
	if(status == '9')
		ret = true;

	return ret;

}


string CSocketTrade::TransBS(char bs)
{
	return "";
}


string CSocketTrade::TransKP(char kp)
{
	return "";
}


string CSocketTrade::TransStatus(char st)
{
	if(st == '0')
		return "未报";
	else if(st =='1')
		return "待报";
	else if(st =='2')
		return "已报";
	else if(st == '5')
		return "部撤";
	else  if(st =='6')
		return "已撤";
	else  if(st =='7')
		return "部成";
	else  if(st =='8')
		return "已成";
	else  if(st =='9')
		return "废单";
	else 
		return "未知";
}


int CSocketTrade::GetInterfaceType()
{
	return TRADE_UFX_AUTO;
}


