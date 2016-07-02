#pragma once
#include "basetradeapi.h"

class CSocketTrade :	public CBaseTradeApi
{
public:
	CSocketTrade(void);

	~CSocketTrade(void);

	virtual void init(CString serverAddress);

	virtual int InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int orderref);

	virtual int WithDraw(int entrustno,char *sCode);

	virtual int QueryEntrust(int entrustno,int reqid);

	virtual int QueryPosition(char *account);

	virtual int QueryTrade(char *account);

	virtual int QueryFund(char *account);

	virtual void RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount);

	virtual void RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

	virtual void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid);


	//报单状态信息
	virtual bool IsWeiBao(char status);    //未报

	virtual bool IsDaiBao(char status);    //待报

	virtual bool IsYiBao(char status);     //已报

	virtual bool IsBuChe(char status);     //部撤

	virtual bool IsYiChe(char status);     //已撤

	virtual bool IsBuCheng(char status);   //部成

	virtual bool IsYiCheng(char status);   //已成

	virtual bool IsFeiDan(char status);    //废单

	virtual std::string TransBS(char bs);

	virtual std::string TransKP(char kp);

	virtual std::string TransStatus(char st);


	virtual int GetInterfaceType();


public:
	void SubEntrust();

	void SubTrade();


};

////////////////////////////////////////////////////////
//const char HEARTBEAT_PACKET[]	= "<root Version=\"1.0\" FuncID=\"999\"/>";
//const char QUERY_FUND_ACCOUNT[] = "<root Version=\"1.0\" FuncID=\"1000\"/>";
//const char ENTRUST_INFO[] = "<root Version=\"1.0\" FuncID=\"1004\" SessionID=\"1000001\">\
//										<item Code=\"600570\" Market=\"Stock\" Account=\"10000013\" BS=\"Buy\" Dir=\"Open\" Amount=\"100\" Price=\"66.55\" Prop=\"0\"/>\
//									</root>";
////////////////////////////////////////////////////////
