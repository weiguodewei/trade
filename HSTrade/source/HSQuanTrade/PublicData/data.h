#pragma once

#include "CinfoLogger.h"
#include "typedefine.h"
#include "string"
#include <map>
#include "vector"
#include <string>

/*
**每个账户的数据,持仓,委托,资金等
*/
class AFX_EXT_CLASS CAccData
{
	//定义一个数据区域
public:

	CAccData();

public:

	char m_szUserID[16];

	bool m_bReady;               //数据是否准备好

	double m_dEnableMoney;		 //查询出来的可用资金

	HWND m_hMsgWnd;

	std::map<std::string,CCodeStatus> m_sCodeStatus;   //合约状态信息

	std::vector<CHoldsinfo> m_sHoldsInfo;              //查询出来的持仓信息

	std::vector<CEntrustInfo>m_sEntrustInfo;           //委托信息

	std::map<std::string,CHoldsEx> m_sHoldsReal;       //实时更新的持仓信息


	//账户风控信息阀值	
	double m_dMargin;									//保证金占用额度

	int m_iCheDanCount;									//撤单次数

	double m_dPL;										//达到该盈亏阀值就不再开仓

private:

	std::vector<int> m_vOrderRef;

	//当前风险信息
	double m_dUseMargin;								//已经占用的保证金

	std::map<std::string,int> m_mapUseCheDan;			//已经撤单次数

	double m_dUsePL;									//当前盈亏


public:
	void SetRiskInfo(double margin,int count,double pl);

	bool CheckRisk(char *code,int count,double price,char bs,char kp,int type); //type=0 下单  type=1 撤单

	double UpdateHoldsInfo(char *code,int vol,double price,char bs,char kp);

	void UpdateRiskInfo(char *code,int vol,double price,char bs,char kp);

	double GetMargin(char *code);

	void AddOrder(int orderref);

	bool ExistOrder(int orderref);

};


/*
**公共数据域
*/

class AFX_EXT_CLASS CPubData
{
public:

	CPubData();

public:

	 static  std::map<std::string,StockInfo> m_sCodeInfo;           //股票合约

	 static std::map<std::string,FutuCodeInfo> m_sFutuCodeInfo;    //期货合约

	 static std::vector<ArbitrageCodeInfo> s_vFutuCodeInfo;

	 static std::map<std::string,StockOpt> m_mapOptCode;			//股票期权代码

	 static std::map<std::string, ArbitrageCodeInfo> m_mapArbitrageCode;		//标准套利合约

	 static std::map<std::string, int> m_mapExchaneId;
};


/*
**获取合约信息
*/


class AFX_EXT_CLASS CStockInfo
{

public:
	CStockInfo();

	CPubData m_PubData;                                //公共数据域

public:
	
	char* getCode(char* name)
	{
		return "";
	}

	char* getName(const char* code)
	{
		return m_PubData.m_sCodeInfo[code].Name;
	}

	long getStart(char* code)
	{
		return m_PubData.m_sCodeInfo[code].J_start;
	}

	double getActiveCapital(char* code)
	{
		return m_PubData.m_sCodeInfo[code].ActiveCapital;
	}

	int getExchid(char* code)
	{
		std::string str(code);
		//认为是股票期权合约
		if(str[0] == '1' && str.length() >6)
			return 3;

		//50etf
		if(strcmp("510050",code) ==0)
			return 1;

		//131810 回购
		if(strcmp("131810",code) ==0)
			return 2;

		return m_PubData.m_sCodeInfo[code].exchid;
	}


	char* getfutuName(char* code)
	{
		return m_PubData.m_sFutuCodeInfo[code].name;
	
	}

	char* getfutuexchid(char* code)
	{
		return m_PubData.m_sFutuCodeInfo[code].exchid;
	}

	void LoadCodeInfo();			//加载txt合约信息
	

};