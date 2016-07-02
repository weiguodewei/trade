#include "stdafx.h"

#include "StrategyLimitupSale.h"
#include "TradeInterface.h"
#include "FunctionPub.h"


CStrategyLimitupSale::CStrategyLimitupSale(void)
{
	m_fyingkuiPosition = 0;

	m_fOpenPrice = 0;

	m_bRun = false;

	m_strStrategyName = "涨停板股票卖出策略";

	m_nStrategyStatus = STRATEGY_STATUS_BK;
	
    m_hignPrice = 0;

	m_lastPrice = 0;

	m_isToHighPrice =false;

	m_strParamList = "交易合约;交易数量;开仓价;止损率;换手率;回调率;是否追单;超时时间;账户;";

	m_params.insert(pair<string, CString>("交易合约",""));

	m_params.insert(pair<string, CString>("交易数量",""));

	m_params.insert(pair<string, CString>("开仓价",""));

	m_params.insert(pair<string, CString>("止损率",""));

	m_params.insert(pair<string, CString>("换手率",""));

	m_params.insert(pair<string, CString>("回调率",""));

	m_params.insert(pair<string, CString>("是否追单",""));

	m_params.insert(pair<string, CString>("超时时间",""));

	m_params.insert(pair<string, CString>("账户",""));

}


CStrategyLimitupSale::~CStrategyLimitupSale(void)
{
}



void  CStrategyLimitupSale::Tick(CTickData * pt)
{

	if(strcmp(m_strInstrument,pt->szInstrument)!=0)
		return;

	m_hignPrice = pt->highestprice;                                  //记录最高价

	if(pt->lastprice==pt->UpperLimitPrice)
	{
		m_isToHighPrice = true;                                       //达到过涨停价
	}


	if(m_nStrategyStatus!=STRATEGY_STATUS_BK)
		return;

	CString log = "";
	int iCount = GetStockCount(pt->szInstrument);
	m_fyingkuiPosition = (pt->lastprice - m_fOpenPrice) * iCount;   //计算盈亏

	m_lastPrice = pt->lastprice;                                      //记录下最新价

	//打开涨停板
	if((pt->lastprice < pt->UpperLimitPrice) && (m_isToHighPrice == true))
	{
		//CMediaAux::PlayWavSound();

		//平仓
		if(CloseSale(m_strInstrument.GetBuffer(0),iCount,pt->lastprice,m_fundAccount.GetBuffer(0),'1')>=0)
		{

			log = pt->szInstrument;

			log = log + ":打开涨停板->平仓 ";

			AddLog(log.GetBuffer(0));

		}

		m_nStrategyStatus = STRATEGY_STATUS_INIT;

		return;
		
	}
	
	//亏损%就平仓
	if( m_fOpenPrice*(1-m_loseRate) > pt->lastprice )  
	{
		//CMediaAux::PlayWavSound();

		//平仓
		if(CloseSale(m_strInstrument.GetBuffer(0),iCount,pt->lastprice,m_fundAccount.GetBuffer(0),'1')>=0)
		{

			log = pt->szInstrument;

			CString rate;

			rate.Format("%f",m_loseRate);

			log = log + ":亏损" +rate + "->平仓";

			AddLog(log.GetBuffer(0));

		}

		m_nStrategyStatus = STRATEGY_STATUS_INIT;

		return;
	}
	
	//涨停板，换手率大于%
	double rate = pt->Volume/(m_pStrategyCodeInfo->getActiveCapital(pt->szInstrument));

	if((pt->UpperLimitPrice == pt->lastprice) &&(  rate> m_changeRate) )
	{
		//CMediaAux::PlayWavSound();

		//平仓
		if(CloseSale(m_strInstrument.GetBuffer(0),iCount,pt->lastprice,m_fundAccount.GetBuffer(0),'1')>=0)
		{
			
			log = pt->szInstrument;

			CString rate;

			rate.Format("%f",m_changeRate);

			log = log + ":换手率大于" +rate + "->平仓";

			AddLog(log.GetBuffer(0));

			
		}

		m_nStrategyStatus = STRATEGY_STATUS_INIT;

		return;
	}

	//买入后最高价回调%
	if( (m_hignPrice * (1-m_downRate)) > pt->lastprice)
	{
		CMediaAux::PlayWavSound();
	

		if(CloseSale(m_strInstrument.GetBuffer(0),iCount,pt->lastprice,m_fundAccount.GetBuffer(0),'1')>=0)
		{
			

			log = pt->szInstrument;

			CString rate;

			rate.Format("%f",m_downRate);

			log = log + ":回调" +rate + "->平仓";

			AddLog(log.GetBuffer(0));

		}

		m_nStrategyStatus = STRATEGY_STATUS_INIT;

		return;
	}
	
	return;
}


void CStrategyLimitupSale::SetParam(CString paramname,CString value)
{
	m_params[(LPCTSTR)paramname] = value;

	int iCount;
	//还需要将对应的值更新
	if(paramname=="止损率")
	{
		m_loseRate = atof(value);
	}
	else if(paramname=="换手率")
	{
		m_changeRate = atof(value);
	}
	else if(paramname=="回调率")
	{
		m_downRate = atof(value);
	}
	else if(paramname=="是否追单")
	{
		m_follow = value; 
	}
	else if(paramname=="超时时间")
	{
		m_timeOut = atoi(value);
	}
	else if(paramname=="账户")
	{
		m_fundAccount = value;
	}
	else if(paramname=="交易数量")
	{
		iCount = atoi(value);
		SetStockCount(m_strInstrument, iCount);
	}
	else if(paramname=="价差")
	{
		m_jiacha = atof(value);
	}else if(paramname=="开仓价")
	{
		m_fOpenPrice = atof(value);
	}

}

void CStrategyLimitupSale::SetParam(CString value)
{
	
	//格式是600570;10;40;0.01;0.02;

	CStringArray list;

	CFunctionPub pub;

	pub.ParseParam(value,list,';');

	if( list.GetSize() >= 3 )
	{

		m_strInstrument = list[0];

		m_params["交易合约"] = list[0];

		int iCount;
		iCount = atoi(list[1]);
		SetStockCount(m_strInstrument, iCount);
		m_params["交易数量"] = list[1];


		m_fOpenPrice = atof(list[2]);

		m_params["开仓价"] = list[2];


		m_loseRate = atof(list[3]);
			
		m_params["止损率"] = list[3];                      //亏损率


		m_changeRate = atof(list[4]);
			
	    m_params["换手率"] = list[4];		               //换手率


		m_downRate = atof(list[5]);
		
		m_params["回调率"] = list[5];						//回调率


		m_follow = list[6];                                 //是否跟单

		m_params["是否追单"] = list[6];


		m_timeOut = atoi(list[7]);							//超时时间

		m_params["超时时间"] = list[7];


		m_fundAccount = list[8];							//帐号   用逗号隔开的  a,b,c,d,

		m_params["账户"] = list[8];				


		//m_strModelName = m_strStrategyName + m_strInstrument;     //modelname

		
	}
}


CString CStrategyLimitupSale::GetParamValue()
{

	CString str;
	int iCount = GetStockCount(m_strInstrument);
	str.Format("%s;%d;%.4f;%.2f;%.2f;%.2f;%s;%d;%s;",m_strInstrument,iCount,m_fOpenPrice,m_loseRate,m_changeRate,m_downRate,m_follow,m_timeOut,m_fundAccount);

	return str;

}

CString CStrategyLimitupSale:: GetParamValue(CString paramname)
{

	return m_params[(LPCTSTR)paramname];
}

CString CStrategyLimitupSale::GetStatusInfo(void)
{
	CString result;

	CString status;

	CString openrice;

	CString yingkuiPos;

	CString modelstatus;

	CString codename;

	CString yinkuirate;

	CString huitiaorate;

	if( m_nStrategyStatus ==  STRATEGY_STATUS_INIT )
	{
		status = "空仓";

		openrice.Format("%.4f",m_fOpenPrice);
	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_BK )
	{
		status = "买开";

		openrice.Format("%.4f",m_fOpenPrice);

		//yingkuiPos.Format("%.2f",m_fyingkuiPosition);
	}
	else if(m_nStrategyStatus == STRATEGY_STATUS_SK )
	{
		status = "卖开";

		openrice.Format("%.4f",m_fOpenPrice);

		//yingkuiPos.Format("%.2f",m_fyingkuiPosition);
	}

	if( m_bRun == false )
		modelstatus = "暂停";
	else
		modelstatus = "运行";

	yingkuiPos.Format("%.4f",m_fyingkuiPosition);

	codename = m_pStrategyCodeInfo->getName(m_strInstrument.GetBuffer(0));

	m_strInstrument.ReleaseBuffer();

	yinkuirate.Format("%.2f",(m_lastPrice - atof(openrice)) / atof(openrice));

	if(m_hignPrice < 0.001)
	{
		huitiaorate = "0.00";
	}
	else
	{
		huitiaorate.Format("%.2f",((m_hignPrice - m_lastPrice) / m_hignPrice));
	}
		int iCount = GetStockCount(m_strInstrument);
	result.Format("%s;%s;%s;%d;%s;%s;%s(%s);%s;%s;%.4f;%.4f(%s);%s;",m_strStrategyName,m_strInstrument,codename,iCount,status,openrice,yingkuiPos,yinkuirate,"0",modelstatus,m_lastPrice,m_hignPrice,huitiaorate,m_fundAccount);

	return result;
}


 void CStrategyLimitupSale::OnTradeReturn(char * ins,char * time,int orderref,int localorderid,char direction,char offsetflag,int vol,double price)
 {
 }