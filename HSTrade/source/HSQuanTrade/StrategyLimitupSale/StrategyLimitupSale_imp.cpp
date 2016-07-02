#include "stdafx.h"
#include "StrategyLimitupSale_imp.h"
#include <map>
#include <string>
#include "..\HSQuanTrade\FunctionPub.h"
#include <time.h>
#include "..\HSQuanTrade\tool.h"


CStrategyLimitupSale::CStrategyLimitupSale()
{

	m_strStrategyName = "抢涨停板卖出";
	
	m_nStrategyStatus = STRATEGY_STATUS_BK;

	m_fyingkuiPosition = 0;

	m_lastPrice = 0;

	m_hignPrice = 0;

	m_follow = "-1";                   //是否跟单,默认写个-1

	m_timeOut = 0;                     //超时时间

	jiacha = 0;					       //价差

	m_nCount = 0;

	m_fLoseRate = 0.05;               //止损点

	m_fGetRate = 0.25;

	m_fBackRate = 0.05;               //最高价回调点

	m_Exit = false;

	m_bRun = false;

	m_done = false;

	m_dTradePrice = 0.0;                 //成本价

	m_iDurationTime = 60;

	m_strParamList = "交易合约;数量;开仓价格;回调率;止损率;止盈率;持续时间;账户;";

	m_params.insert(pair<string, CString>("交易合约",""));

	m_params.insert(pair<string, CString>("数量","0"));

	m_params.insert(pair<string, CString>("开仓价格","0"));

	m_params.insert(pair<string, CString>("回调率","0.05"));

	m_params.insert(pair<string, CString>("止损率","0.05"));

	m_params.insert(pair<string, CString>("止盈率","0.25"));

	m_params.insert(pair<string, CString>("持续时间","60"));

	m_params.insert(pair<string, CString>("账户",""));

	m_params.insert(pair<string, CString>("是否追单","-1"));

	m_params.insert(pair<string, CString>("超时时间","0"));

	m_params.insert(pair<string, CString>("价差","0"));

	
}

CStrategyLimitupSale::~CStrategyLimitupSale()
{
	m_bRun = false;

	saveModel();

}


void CStrategyLimitupSale::Tick(CTickData * pt)
{
	if(strcmp(m_strInstrument,pt->szInstrument)!=0)
		return;

	time_t rawtime;
	time ( &rawtime );
	long timestamp = (long)rawtime;


	double buy1 = pt->bidprice[0];
	double buy2 = pt->bidprice[1];
	double buy5 = pt->bidprice[4];
	double sale1 = pt->askprice[0];
	double sale2 = pt->askprice[1];
	double limitup = pt->UpperLimitPrice;
	double limitlow = pt->LowerLimitPrice;
	double lastprice = pt->lastprice;
	char*code = pt->szInstrument;
	int buy1count = pt->bidvol[0];
	int buy2count = pt->bidvol[1];
	int sale1count = pt->askvol[0];

	//记录最高价
	if(pt->highestprice > m_hignPrice)
	{
		m_hignPrice = pt->highestprice;  //最高价
	}

	//持仓盈亏
	m_fyingkuiPosition = (pt->lastprice - m_dTradePrice) * m_nCount;

	m_lastPrice = pt->lastprice;

	
	CString price;

	price.Format("合约:%s,最新价:%.3f,最高价:%.3f",code,lastprice,m_hignPrice);

	WRITE_LOG(1,price.GetBuffer(0));

	if( (data==NULL) || (!(*data).isReady) )
		return;


	char msg[256];

	//9:35前不操作
	if(isDoNothingTime() || (m_done))
	{
		return;
	}


	//14:55，亏损5%市价卖出
	if( (isCloeTime()) && (m_lastPrice < m_dTradePrice* (1- m_fLoseRate)) )
	{
		playsound();

		OpenSale(code,m_nCount,limitlow,m_fundAccount.GetBuffer(0));

		sprintf(msg,"14:55,合约:%s,亏损大于%.3f,市价卖出...成本价%.3f,当前价格%.3f",code,m_fLoseRate,m_dTradePrice,pt->lastprice);

		AddLog(msg);

		m_done = true;

		m_nStrategyStatus =  STRATEGY_STATUS_INIT;

	}

	//止损止盈
	//由于之前有个14:55止盈卖出的条件，所以这里坐下特殊处理。只有设置了止盈率后，止损止盈才起作用
	if( m_fGetRate > 0.001)
	{
		if( (m_lastPrice < m_dTradePrice* (1 - m_fLoseRate) ) || (m_lastPrice > m_dTradePrice* (1 +  m_fGetRate)) ) 
		{
			playsound();

			OpenSale(code,m_nCount,limitlow,m_fundAccount.GetBuffer(0));

			sprintf(msg,"14:55,合约:%s,达到止盈(%.3f)止损点(%.3f),市价卖出...成本价%.3f,当前价格%.3f",code,m_fGetRate,m_fLoseRate,m_dTradePrice,pt->lastprice);

			AddLog(msg);

			m_done = true;

			m_nStrategyStatus =  STRATEGY_STATUS_INIT;
		
		}

	}

	//最高价回调5%
	if( m_lastPrice  < (m_hignPrice * (1 - m_fBackRate)) )
	{
		sprintf(msg,"合约:%s,最高价:%.4f,最新价:%.4f,回调超过%.3f,记录...",code,m_hignPrice,m_lastPrice,m_fBackRate);

		AddLog(msg);

		if(m_sSignalTime[code] == -9999 || m_sSignalTime[code] == 0 )
		{
			m_sSignalTime[code] = timestamp;

		}

		//持续一分钟的卖出信号
		if( (timestamp - m_sSignalTime[code] > m_iDurationTime) && (m_sSignalTime[code]!=0) )
		{
			playsound();

			sprintf(msg,"合约:%s,当前时刻:%ld,最初时刻%ld,持续一分钟,卖出...",code,timestamp,m_sSignalTime[code]);

			AddLog(msg);

			OpenSale(code,m_nCount,limitlow,m_fundAccount.GetBuffer(0));

			m_done = true;

			m_nStrategyStatus =  STRATEGY_STATUS_INIT;
		}

	}
	//没有卖出信号
	else
	{
		m_sSignalTime[code] = -9999;

	}

	if(isSaveTime())
	{
		saveModel();
	
	}

}


void CStrategyLimitupSale::SetParam(CString paramname,CString value)
{
	//"交易合约;数量;价格;买卖方向;开平方向;委托信息;账户;";

	m_params[(LPCTSTR)paramname] = value;

	//还需要将对应的值更新
	if(paramname=="交易合约")
	{
		m_strInstrument = value;
	}
	else if(paramname=="数量")
	{
		m_nCount = atoi(value);
	}
	else if(paramname=="开仓价格")
	{
		m_dTradePrice = atof(value);
	}
	else if(paramname=="回调率")
	{
		m_fBackRate = atof(value);
	}
	else if(paramname=="止损率")
	{
		m_fLoseRate = atof(value);
	}
	else if(paramname=="止盈率")
	{
		m_fGetRate = atof(value);
	}
	else if(paramname=="持续时间")
	{
		m_iDurationTime = atoi(value);
	}
	else if(paramname=="账户")
	{
		m_fundAccount = value;
	}

}


void CStrategyLimitupSale::SetParam(CString value)
{

	CStringArray list;

	CFunctionPub pub;

	pub.ParseParam(value,list,';');

	CStringArray listField;

	pub.ParseParam(	m_strParamList,listField,';');

	for(int i=0;i<listField.GetSize()&&i<list.GetSize();i++)
	{
		SetParam(listField[i],list[i]);
	}

	/*m_strModelName = m_strStrategyName;*/

}


//"交易合约;数量;价格;回调比例;止损比例;账户;";
CString CStrategyLimitupSale::GetParamValue()
{
	CString str;

	str.Format("%s;%d;%.3f;%.3f;%.3f;%.3f;%d;%s;",m_strInstrument,m_nCount,m_dTradePrice,m_fBackRate,m_fLoseRate,m_fGetRate,m_iDurationTime,m_fundAccount);

	return str;
}


CString CStrategyLimitupSale::GetParamValue(CString paramname)
{
	return m_params[(LPCTSTR)paramname];
}

CString CStrategyLimitupSale::GetStatusInfo(void)
{
	CString result;

	CString status;

	CString yingkuiPos;

	CString modelstatus;

	CString codename;

	double yinkuirate;

	double huitiaorate;

	if( m_nStrategyStatus ==  STRATEGY_STATUS_INIT )
	{
		status = "空仓";

	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_BK )
	{
		status = "买开";

	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_SK )
	{
		status = "卖开";

	}


	if( m_bRun == false )
		modelstatus = "暂停";
	else
		modelstatus = "运行";


	yingkuiPos.Format("%.4f",m_fyingkuiPosition);

	codename = m_pStrategyCodeInfo->getName(m_strInstrument.GetBuffer(0));

	if(m_lastPrice < 0.001)
	{
		yinkuirate = 0.0;
	}
	else
	{
		yinkuirate = (m_lastPrice - m_dTradePrice) / m_dTradePrice;
	}
	


	if(m_hignPrice < 0.001)
	{
		huitiaorate = 0.00;
	}
	else
	{
		huitiaorate =( m_hignPrice - m_lastPrice) / m_hignPrice;
	}


	result.Format("%s;%s;%s;%d;%s;%.3f;%.4f(%.3f);%.4f;%s;%.4f;%.4f(%.3f);%s;%s;",m_strStrategyName,m_strInstrument,codename,
		m_nCount,status,m_dTradePrice,m_fyingkuiPosition,yinkuirate,0.0,modelstatus,m_lastPrice,m_hignPrice,huitiaorate,m_fundAccount,/*m_strModelName*/GetStrategyId());

	return result;

}

void CStrategyLimitupSale::BeginRun()
{
	reload();

	//订阅合约
	//"subcode#" + "131810" + ";" + "STOCK;UFX;" + "#"
	string subInfo;
	subInfo.append("subcode#").append(m_strInstrument).append(";STOCK;UFX;#");

	::SendMessage(m_hMsgWnd,WM_MESSAGE_ENTRUST,7,(LPARAM)subInfo.c_str());
}

void CStrategyLimitupSale::Pause()
{
	char msg[128];

	if(!m_bRun)
	{
		sprintf(msg,"暂停...");

		AddLog(msg);

		m_sSignalTime[m_strInstrument.GetBuffer(0)] = -9999;

		return;
		
	}
}

bool CStrategyLimitupSale::isDoNothingTime()
{
	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;


	if(h==9 && m<=35)
	{
		ret =  true;
	}

	return ret;

}

bool CStrategyLimitupSale::isCloeTime()
{
	bool ret =false;

	const time_t t = time(NULL);

	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

    if(h==14 && m>=55)
	{
		ret =  true;
	}

	return ret;

}


bool CStrategyLimitupSale::isSaveTime()
{

	bool ret = false;

	const time_t t = time(NULL);
	struct tm* current_time = localtime(&t);

	int h = current_time->tm_hour;
	int m = current_time->tm_min;
	int s = current_time->tm_sec;

	if(h == 14 && m >= 58)
	{
		ret = true;
	}

	return ret;

}

//处理回报
void CStrategyLimitupSale::OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price)
{


}

void CStrategyLimitupSale::OnOrderReturn(int orderref,int ordersystemid,char orderstatus,char bs,char kp,char * ins)
{

}


void CStrategyLimitupSale::reload()
{
	//原始字符串 c++11
	string path = Tool::GetModelDir() + "StrategyLimitupSale.ini";
	
	char hignprice[32];

	GetPrivateProfileString( "HighestPrice", m_strInstrument, "0.0", hignprice,16 ,(char*)path.c_str() );

	m_hignPrice = atof(hignprice);

	
}

void CStrategyLimitupSale::saveModel()
{
	//原始字符串 c++11
	CString path ;

	string s= Tool::GetModelDir();

	path = s.c_str();	

	path = path + "StrategyLimitupSale.ini";

	char sHighPrice[12];

	sprintf(sHighPrice,"%.3f",m_hignPrice);

	WritePrivateProfileString("HighestPrice", m_strInstrument, sHighPrice, path);

}

