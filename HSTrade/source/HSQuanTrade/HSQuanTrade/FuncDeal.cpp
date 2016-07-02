#include "stdafx.h"
#include "FuncDeal.h"
#include "StrategyBase.h"


CRequest::CRequest()
{

}

CRequest::~CRequest()
{


}

void CRequest::init(CStrategyManager *p1,CTradeInterface  *p2,CStockInfo *p3)
{
	m_stgy = p1;

	m_tde = p2;

	m_code = p3;

}


void CRequest::handle(string str)
{
	//查询期权
	if(str.find("REQ#queryopt") !=-1)
	{
		CHundsunFtdcQryInstrumentField m_Instrument;
		memset(&m_Instrument, 0, sizeof(CHundsunFtdcQryInstrumentField));
		m_Instrument.ExchangeID = 3;				//代表上海的股票期权
		m_stgy->lpUserMdApi->ReqQryInstrument(&m_Instrument, 0);

	}
	//合约订阅
	else if(str.find("subcode#")!=-1)
	{
		subcode(str);
	}
	//取消订阅
	else if(str.find("unsub#")!=-1)
	{
		//unsubcode(str);
	}
	////新ip建立连接
	else if(str.find("newip#") !=-1 )
	{
		CFunctionPub pub;
		
		CStringArray list;

		pub.ParseParam(str.c_str(),list,'#');

		m_stgy->Log((char*)str.c_str());

		CString type = list[1];

		CString ip= list[2];

		//ufx订阅期货行情
		if(type=="ufx")
		{
			m_stgy->register_ufx(ip);
		}
		//订阅ctp期货行情
		else if(type=="ctp")
		{
			m_stgy->register_ctp(ip);
		}

		
	}
	//添加账户
	else if(str.find("account#") !=-1)
	{
		CFunctionPub pub;
		
		CStringArray list;

		pub.ParseParam(str.c_str(),list,'#');

		if( list.GetSize() < 2)
			return ;

		if( list[0] == "account" )
		{
			CStringArray list2;

			pub.ParseParam(list[1],list2,';');

			m_tde->OpenAccount(list2[0],list2[1],list2[2],list2[3],list2[4]);

			//WRITE_LOG(1,"account");
		}
		
		m_stgy->Log((char*)str.c_str());
	}
	//撤单
	else if(str.find("withdraw") !=-1)
	{
		CFunctionPub pub;
		
		CStringArray list;

		pub.ParseParam(str.c_str(),list,';');

		string account = list[1];
		int entrustno = atoi(list[2]);	
		string code = list[3];

		m_tde->WithDraw((char*)account.c_str(),entrustno,(char*)code.c_str());

		m_stgy->Log((char*)str.c_str());

	}
	//下单
	else if(str.find("order") !=-1)
	{
		CFunctionPub pub;
		
		CStringArray list;

		pub.ParseParam(str.c_str(),list,';');

		CString code = list[1];
		int amount = atoi(list[2]);		
		string sb(list[3].GetBuffer(0));
		string kp(list[4].GetBuffer(0));
		double d = atof(list[5]);
		int type = atoi(list[6]);
		int timeout = atoi(list[7]);
		double jiacha = atof(list[8]);
		char* account = list[9].GetBuffer();

		m_tde->InsertOrder(code.GetBuffer(0),amount,sb[0],kp[0],d,type,timeout,jiacha,account);

		m_stgy->Log((char*)str.c_str());

	}
	//按组下单
	else if(str.find("grpxiadan") !=-1)
	{
		CFunctionPub pub;
		
		CStringArray list;

		pub.ParseParam(str.c_str(),list,';');

		CString code = list[1];
		int amount = atoi(list[2]);		
		string sb(list[3].GetBuffer(0));
		string kp(list[4].GetBuffer(0));
		double d = atof(list[5]);
		int type = atoi(list[6]);
		int timeout = atoi(list[7]);
		double jiacha = atof(list[8]);
		char* account = list[9].GetBuffer();
		int grpid = atoi(list[10]);	
		

		m_tde->InsertOrderGroup(code.GetBuffer(0),amount,sb[0],kp[0],d,type,timeout,jiacha,account,grpid);

		m_stgy->Log((char*)str.c_str());
	
	}
	//风控信息
	else if(str.find("risk") !=-1)
	{
		CFunctionPub pub;
		
		CStringArray list;

		pub.ParseParam(str.c_str(),list,';');

		string account = list[1];
		string margin =  list[2];
		string withdraw =  list[3];
		string pl =  list[4];
		string open = list[5];

		m_tde->SetRiskInfo((char*)account.c_str(),atof(margin.c_str()),atoi(withdraw.c_str()),atof(pl.c_str()),atoi(open.c_str()));

		m_stgy->Log((char*)str.c_str());

	}
	//登出
	else if(str.find("logout") !=-1)
	{
		m_tde->LogOut();
		m_stgy->Log((char*)str.c_str());
	}
	//登陆
	else if(str.find("login") !=-1)
	{
		m_tde->Login();
		m_stgy->Log((char*)str.c_str());
	}
#if 0
	else if (str.find("REQ#queryarbit") != -1)
	{

		/*
		///上海期货交易所
		#define HUNDSUN_FTDC_SHFE      11
		///郑州商品交易所
		#define HUNDSUN_FTDC_CZCE      12
		///大连商品交易所
		#define HUNDSUN_FTDC_DCE       13

		*/
		CHundsunFtdcQryInstrumentField m_Instrument;
		memset(&m_Instrument, 0, sizeof(CHundsunFtdcQryInstrumentField));
		m_Instrument.ExchangeID = HUNDSUN_FTDC_CZCE;				
		m_stgy->lpUserMdApi->ReqQryInstrument(&m_Instrument, 0);
	}
#endif
}


void CRequest::subcode(string str)
{
	int exchid = -9999;

	CFunctionPub pub;
		
	CStringArray list;

	m_stgy->Log((char*)str.c_str());


	pub.ParseParam(str.c_str(),list,'#');

	if( list.GetSize() < 1)
		return ;

	if( list[0] == "subcode" )
	{
		CStringArray list2;

		pub.ParseParam(list[1],list2,';');

		char** Instrument;
		int iInsCount = 0;
		CString subtype;
		char szCode1[32], szCode2[32];

		CString strInstruments = list2[0];
		int nt = strInstruments.Find('$');

		if (nt > 0)
		{
			Instrument = new char*[2];

			CString sCode = strInstruments.Left(nt);
			strcpy(szCode1, sCode);
			Instrument[0] = szCode1;


			sCode = strInstruments.Right(strInstruments.GetLength() - nt -1);
			strcpy(szCode2, sCode);
			Instrument[1] = szCode2;
	
			iInsCount = 2;

		}
		else 
		{
			Instrument = new char*[1];
			strcpy(szCode1, list2[0]);

			Instrument[0] = szCode1;
			iInsCount = 1;
		}


		subtype = list2[2];


		exchid = m_code->getExchid(Instrument[0]);

		if(strcmp("600570",Instrument[0]) == 0)
			return;

		//维护一份已经订阅的合约列表
		if(m_stgy->lpUserMdSpi_CTP!=NULL && (subtype.MakeLower().Compare("ctp") == 0))
		{
			int iLen = strlen(Instrument[0]);
			char * pt = new char[iLen + 1];

			strcpy(pt,Instrument[0]);


			m_stgy->m_code.Add(pt);

			m_stgy->lpUserMdSpi_CTP->m_nInsNum = 0;
			for(int i=0;i<m_stgy->m_code.GetSize();i++)
			{	
				strcpy(m_stgy->lpUserMdSpi_CTP->m_ppInstrumentID[i],(char*)m_stgy->m_code[i]);
				m_stgy->lpUserMdSpi_CTP->m_nInsNum++;
			}

			}

		if(m_stgy->lpUserMdSpi!=NULL && (subtype.MakeLower().Compare("ufx") == 0))
		{

			m_stgy->lpUserMdSpi->m_UfxCode[Instrument[0]] = exchid;

		}


		//先去证券里面查询exchid，没有赋值14
		if(exchid!=1 && exchid!=2 && exchid != 3)
		{
			m_stgy->Log(m_code->getfutuName(Instrument[0]));

			if(str.find("FUTURES")!=-1)
			{
				exchid = Tool::TransExchId(m_code->getfutuexchid(Instrument[0]));
			}
		}
		else
		{
			m_stgy->Log(m_code->getName(Instrument[0]));
		}

		//区分是ufx接口订阅和ctp接口
		if((subtype.MakeLower().Compare("ufx") == 0)  && (m_stgy->lpUserMdApi !=NULL))
		{
			m_stgy->lpUserMdApi->SubscribeMarketData(exchid, Instrument, iInsCount);
			
			//先单独处理131810订阅
			if(strcmp(Instrument[0],"131810")==0)
			{
				m_stgy->lpUserMdApi->SubscribeMarketData(2, Instrument, iInsCount);
			}
				
		}
		else if((subtype.MakeLower().Compare("ctp") == 0) && (m_stgy->lpUserMdApi_CTP!=NULL) )
		{
			
				
			m_stgy->lpUserMdApi_CTP->SubscribeMarketData(Instrument, iInsCount);

		}

		if(Instrument)
		{
			delete []Instrument;
		}

	}


}


void CRequest::unsubcode(string str)
{

	int exchid = -9999;

	CFunctionPub pub;
		
	CStringArray list;

	m_stgy->Log((char*)str.c_str());


	pub.ParseParam(str.c_str(),list,'#');

	if( list.GetSize() < 1)
		return ;

	if( list[0] == "unsub" )
	{
		CStringArray list2;

		pub.ParseParam(list[1],list2,';');

		char* Instrument[1];

		CString subtype;

		Instrument[0] = list2[0].GetBuffer(0);
		
		subtype = list2[2];
		
		exchid = m_code->getExchid(Instrument[0]);

		//维护一份已经订阅的合约列表
		if(m_stgy->lpUserMdSpi_CTP!=NULL && (subtype.MakeLower().Compare("ctp") == 0))
		{
			int len = strlen(Instrument[0])+1;

			char * pt = new char[len];

			strcpy(pt,Instrument[0]);
			
			int i=0;
			for(;i<m_stgy->m_code.GetSize();i++)
			{
				if(pt == m_stgy->m_code[i])
					break;
			}
			m_stgy->m_code.RemoveAt(i);

			m_stgy->lpUserMdSpi_CTP->m_nInsNum = 0;
			for(int i=0;i<m_stgy->m_code.GetSize();i++)
			{	
				strcpy(m_stgy->lpUserMdSpi_CTP->m_ppInstrumentID[i],(char*)m_stgy->m_code[i]);
				m_stgy->lpUserMdSpi_CTP->m_nInsNum++;
			}

		}

		if(m_stgy->lpUserMdSpi!=NULL && (subtype.MakeLower().Compare("ufx") == 0))
		{

			//m_stgy->lpUserMdSpi->m_UfxCode[Instrument[0]] = exchid;
			m_stgy->lpUserMdSpi->m_UfxCode.erase(Instrument[0]);
		}


		//先去证券里面查询exchid，没有赋值14
		if(exchid!=1 && exchid!=2 && exchid != 3)
		{
			m_stgy->Log(m_code->getfutuName(Instrument[0]));

			if(str.find("FUTURES")!=-1)
			{
				exchid = Tool::TransExchId(m_code->getfutuexchid(Instrument[0]));
			}
		}
		else
		{
			m_stgy->Log(m_code->getName(Instrument[0]));
		}

		//区分是ufx接口订阅和ctp接口
		if((subtype.MakeLower().Compare("ufx") == 0)  && (m_stgy->lpUserMdApi !=NULL))
		{
			m_stgy->lpUserMdApi->UnSubscribeMarketData(exchid, Instrument, sizeof(Instrument)/sizeof(char*));
			
			//先单独处理131810订阅
			if(strcmp(Instrument[0],"131810")==0)
			{
				m_stgy->lpUserMdApi->UnSubscribeMarketData(2, Instrument, sizeof(Instrument)/sizeof(char*));
			}
				
		}
		else if((subtype.MakeLower().Compare("ctp") == 0) && (m_stgy->lpUserMdApi_CTP!=NULL) )
		{
				
			m_stgy->lpUserMdApi_CTP->UnSubscribeMarketData(Instrument,sizeof(Instrument)/sizeof(char*));

		}

		
	}


}