#pragma once
#include "basetradeapi.h"
#include "UFXOptTradeSpi.h"

class CUFXOptTradeApi :public CBaseTradeApi
{

public:

	CUFXOptTradeApi();

	~CUFXOptTradeApi();


	CUFXOptTraderSpi *m_pOptSpiTrade;

	CHundsunTraderApi *m_pOptApiTrade;


	virtual void init(CString serverAddress);

	virtual int InsertOrder(char * instrument, int count, char direction, char offsetflag, double price,int orderref);

	virtual int WithDraw(int entrustno,char *sCode);

	virtual int QueryEntrust(int entrustno,int reqid);

	virtual int QueryPosition(char *account);

	virtual void RtnOrder(int orderref,int ordersysid,char status,char bs,char kp,char * ins,int entrustAmount,int dealAmount,int restAmount);

	virtual void RtnTrade(char *userid,int orderref,int ordersysid,double price,char offsetflag,char direction,char * tradetime,int vol,char *ins);

	virtual void OnRspOrderInsert(int ref,int entrustno,char entrust_status,int errorid);

	//查询期权代码
	void ReqFuncOptOptcodeQry();


	//报单状态信息
	virtual bool IsWeiBao(char status);    //未报

	virtual bool IsDaiBao(char status);    //待报

	virtual bool IsYiBao(char status);     //已报

	virtual bool IsBuChe(char status);     //部撤

	virtual bool IsYiChe(char status);     //已撤

	virtual bool IsBuCheng(char status);   //部成

	virtual bool IsYiCheng(char status);   //已成

	virtual bool IsFeiDan(char status);    //废单

	virtual string TransBS(char bs);

	virtual string TransKP(char kp);

	virtual string TransStatus(char st);


	virtual int GetInterfaceType();


};


