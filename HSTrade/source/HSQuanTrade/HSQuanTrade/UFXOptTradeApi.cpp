#include "stdafx.h"

#include "UFXOptTradeApi.h"

#include "TradeInterface.h"

#include "StrategyBase.h"

#include "UFXOptTradeSpi.h"


CUFXOptTradeApi::CUFXOptTradeApi(void)
{

}


CUFXOptTradeApi::~CUFXOptTradeApi(void)
{

}


void CUFXOptTradeApi::init(CString serverAddress)
{
	//WRITE_LOG(1,"init");

	m_pOptApiTrade = CHundsunTraderApi::CreateHundsunTraderApi();			

	m_pOptSpiTrade = new CUFXOptTraderSpi(m_pOptApiTrade);	

	strcpy(m_pOptSpiTrade->m_szBroker,m_szBroker);

	strcpy(m_pOptSpiTrade->m_szUserID ,m_szUserID);

	strcpy(m_pOptSpiTrade->m_szPwd,m_szPwd);

	m_pOptSpiTrade->m_pTrade = this;

	m_pOptApiTrade->RegisterSpi(m_pOptSpiTrade);

	//m_pOptApiTrade->RegisterFront("115.236.45.199:7221", 5);
	m_pOptApiTrade->RegisterFront(serverAddress.GetBuffer(0), 5);

	//m_pOptApiTrade->RegisterFront("192.168.73.165:9055", 5);

	m_pOptApiTrade->Init("license.dat");



}



//下单---需要实现订单管理----按下单方式进行处理，
int CUFXOptTradeApi::InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int orderref)
{
	//交易类别，期权合约编码，买卖方向，开平仓方向，委托数量，委托价格"1", "11000527", '1', 'O', 1, 0.3

	//在上层统一报单引用，就需要使用入参orderref
	m_pOptSpiTrade->m_orderref = orderref;

	return m_pOptSpiTrade->ReqFuncOptEntrust("1", instrument, direction, offsetflag, count, price);//"2", "20003928", '1', 'O', 1, 16

}

int CUFXOptTradeApi::WithDraw(int entrustno,char *sCode)
{
	return m_pOptSpiTrade->ReqFuncOptWithdraw(entrustno);
}


int CUFXOptTradeApi::QueryEntrust(int entrustno,int reqid)
{
	return 1;
}


int CUFXOptTradeApi::QueryPosition(char *account)
{
	//CUFXOptTraderSpi->ReqQryInvestorPosition();

	return 1;
}

void CUFXOptTradeApi::ReqFuncOptOptcodeQry()
{

}


void CUFXOptTradeApi::RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount)
{
	m_pTradeInterface->RtnOrder(orderref, ordersysid,status,bs,kp,ins);
}


void CUFXOptTradeApi::RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins)
{
	m_pTradeInterface->RtnTrade(userid,orderref,ordersysid,price,offsetflag,direction,tradetime,vol,ins);
}


void CUFXOptTradeApi::OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid)
{
	m_pTradeInterface->OnRspOrderInsert(ref,entrustno,entrust_status,errorid);

}


//报单状态信息
bool CUFXOptTradeApi::IsWeiBao(char status)
{
	bool ret = false;
	if(status == '0')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsDaiBao(char status)
{
	bool ret = false;
	if(status == '1')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsYiBao(char status)
{
	bool ret = false;
	if(status == '2')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsBuChe(char status)
{
	bool ret = false;
	if(status == '5')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsYiChe(char status)
{
	bool ret = false;
	if(status == '6')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsBuCheng(char status)
{
	bool ret = false;
	if(status == '7')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsYiCheng(char status)
{
	bool ret = false;
	if(status == '8')
		ret = true;

	return ret;
}

bool CUFXOptTradeApi::IsFeiDan(char status)
{
	bool ret = false;
	if(status == '9')
		ret = true;

	return ret;

}


string CUFXOptTradeApi::TransBS(char bs)
{
	if(bs == UFX_ORDER_BUY)
		return "买";
	else if(bs== UFX_ORDER_SELL)
		return "卖";
	else
		return "未知";
}

string CUFXOptTradeApi::TransKP(char kp)
{
	if(kp == UFX_SP_OPEN)
		return "开仓";
	else if(kp == UFX_SP_CLOSE)
		return "平仓";
	else
		return "未知";
}

string CUFXOptTradeApi::TransStatus(char st)
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


int CUFXOptTradeApi::GetInterfaceType()
{
	return TRADE_UFX_OPT;
}


