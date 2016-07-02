// HSQuanTradeCtrl.cpp : CHSQuanTradeCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "HSQuanTrade.h"
#include "HSQuanTradeCtrl.h"
#include "HSQuanTradePropPage.h"
#include "afxdialogex.h"
#include "FunctionPub.h"
#include "HundsunMdCenterStruct.h"
#include "HundsunMdCenterApi.h"
#include "StrategyBase.h"
#include "ThostFtdcMdApi.h"
#include "MdSpiCTP.h"
#include "tool.h"
#include "TradeCommHeaders.h"
#include "data.h"
#include "mdSpi.h"
#include "CDump.h"




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CHSQuanTradeCtrl, COleControl)

// 消息映射

BEGIN_MESSAGE_MAP(CHSQuanTradeCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_MESSAGE(WM_MESSAGE_TRADE,&CHSQuanTradeCtrl::OnTradeMessage)
	ON_MESSAGE(WM_MESSAGE_ENTRUST,&CHSQuanTradeCtrl::OnEntrustMessage)
	ON_MESSAGE(WM_MESSAGE_SOCK,&CHSQuanTradeCtrl::OnSockMsg)
	ON_MESSAGE(WM_MODEL_RESULT_MSG, &CHSQuanTradeCtrl::OnBackTestResultMessage)
	ON_MESSAGE(WM_MESSAGE_BACKTEST_STEP, &CHSQuanTradeCtrl::OnBackTestStep)
	ON_MESSAGE(WM_MESSAGE_SUBCODE, &CHSQuanTradeCtrl::OnSubCode)
END_MESSAGE_MAP()

// 调度映射

BEGIN_DISPATCH_MAP(CHSQuanTradeCtrl, COleControl)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "GetAllStrategy", dispidGetAllStrategy, GetAllStrategy, VT_BSTR, VTS_I4 VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "GetStrategyParamInfo", dispidGetStrategyParamInfo, GetStrategyParamInfo, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "SetStrategyParam", dispidSetStrategyParam, SetStrategyParam, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "AddRunStrategy", dispidAddRunStrategy, AddRunStrategy, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "StopStrategy", dispidStopStrategy, StopStrategy, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "PauseStrategy", dispidPauseStrategy, PauseStrategy, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "RunStrategy", dispidRunStrategy, RunStrategy, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "GetStrategyStatusInfo", dispidGetStrategyStatusInfo, GetStrategyStatusInfo, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "Init", dispidInit, Init, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "Exit", dispidExit, Exit, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "GetParamValue", dispidGetParamValue, GetParamValue, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "GetStragyLogInfo", dispidGetStragyLogInfo, GetStragyLogInfo, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "GetStragyTradeSignalList", dispidGetStragyTradeSignalList, GetStragyTradeSignalList, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "StragegyBackTest", dispidStragegyBackTest, StragegyBackTest, VT_BSTR, VTS_BSTR VTS_BSTR VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "GetAllStockFilterName", dispidGetAllStockFilterName, GetAllStockFilterName, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "RunStockFilterStragegy", dispidRunStockFilterStragegy, RunStockFilterStragegy, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "GetStockFilterStragegyParamInfo", dispidGetStockFilterStragegyParamInfo, GetStockFilterStragegyParamInfo, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "SetStockFilterStragegyParam", dispidSetStockFilterStragegyParam, SetStockFilterStragegyParam, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "RunStockFilter", dispidRunStockFilter, RunStockFilter, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR)
	
	DISP_PROPERTY_EX_ID(CHSQuanTradeCtrl, "BackTestStatus", dispidBackTestStatus, GetBackTestStatus, SetBackTestStatus, VT_I4)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "GetStrategyContext", dispidGetStrategyContext, GetStrategyContext, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CHSQuanTradeCtrl, "SetStrategyContext", dispidSetStrategyContext, SetStrategyContext, VT_BSTR, VTS_BSTR VTS_BSTR)
END_DISPATCH_MAP()

// 事件映射

BEGIN_EVENT_MAP(CHSQuanTradeCtrl, COleControl)
	EVENT_CUSTOM_ID("TradeNotify", eventidTradeNotify, TradeNotify, VTS_I4 VTS_I4 VTS_BSTR VTS_BSTR)
	EVENT_CUSTOM_ID("BackTestNotify", eventidBackTestNotify, BackTestNotify, VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENT_MAP()

// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CHSQuanTradeCtrl, 1)
	PROPPAGEID(CHSQuanTradePropPage::guid)
END_PROPPAGEIDS(CHSQuanTradeCtrl)

// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CHSQuanTradeCtrl, "HSQUANTRADE.HSQuanTradeCtrl.1",
	0xad4abdea, 0x8d04, 0x4ebe, 0x89, 0xde, 0xda, 0x1, 0xe1, 0xef, 0x35, 0x5d)

// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CHSQuanTradeCtrl, _tlid, _wVerMajor, _wVerMinor)

// 接口 ID

const IID IID_DHSQuanTrade = { 0x5E64DA04, 0x4094, 0x4246, { 0x9A, 0xD6, 0xD7, 0x6D, 0x53, 0x47, 0xB0, 0xA1 } };
const IID IID_DHSQuanTradeEvents = { 0xE2482DB0, 0x6A90, 0x4C95, { 0x9C, 0x65, 0x3A, 0xBB, 0xAB, 0x4D, 0x14, 0xB4 } };

// 控件类型信息

static const DWORD _dwHSQuanTradeOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CHSQuanTradeCtrl, IDS_HSQUANTRADE, _dwHSQuanTradeOleMisc)

// CHSQuanTradeCtrl::CHSQuanTradeCtrlFactory::UpdateRegistry -
// 添加或移除 CHSQuanTradeCtrl 的系统注册表项

BOOL CHSQuanTradeCtrl::CHSQuanTradeCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_HSQUANTRADE,
			IDB_HSQUANTRADE,
			afxRegApartmentThreading,
			_dwHSQuanTradeOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


// CHSQuanTradeCtrl::CHSQuanTradeCtrl - 构造函数

CHSQuanTradeCtrl::CHSQuanTradeCtrl()
{
	InitializeIIDs(&IID_DHSQuanTrade, &IID_DHSQuanTradeEvents);
	// TODO: 在此初始化控件的实例数据。
}

// CHSQuanTradeCtrl::~CHSQuanTradeCtrl - 析构函数

CHSQuanTradeCtrl::~CHSQuanTradeCtrl()
{
	// TODO: 在此清理控件的实例数据。
}

// CHSQuanTradeCtrl::OnDraw - 绘图函数

void CHSQuanTradeCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}

// CHSQuanTradeCtrl::DoPropExchange - 持久性支持

void CHSQuanTradeCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}


// CHSQuanTradeCtrl::OnResetState - 将控件重置为默认状态

void CHSQuanTradeCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}


// CHSQuanTradeCtrl::AboutBox - 向用户显示“关于”框

void CHSQuanTradeCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_HSQUANTRADE);
	dlgAbout.DoModal();
}


// CHSQuanTradeCtrl 消息处理程序


BSTR CHSQuanTradeCtrl::GetAllStrategy(LONG param1, LPCTSTR param2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	strResult = m_strategy.GetAllStrategyName();

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::GetStrategyParamInfo(LPCTSTR strategyname)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	strResult  = m_strategy.GetStrategyParamInfo(strategyname);

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::SetStrategyParam(LPCTSTR modelname,LPCTSTR paramname, LPCTSTR paramvalue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	m_strategy.SetStrategyParam(modelname,paramname,paramvalue);

	return strResult.AllocSysString();
}

/*
新建一个运行策略
*/
BSTR CHSQuanTradeCtrl::AddRunStrategy(LPCTSTR paramname)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	strResult = m_strategy.AddRunStragegy(paramname);

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::StopStrategy(LPCTSTR strategynname)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	
	m_strategy.StopModel(strategynname);

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::PauseStrategy(LPCTSTR strategyname)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	m_strategy.PauseModel(strategyname);

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::RunStrategy(LPCTSTR strategyname)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	//TradeNotify(0,0,"fuhaoming","test");

	m_strategy.RunModel(strategyname);

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::GetStrategyStatusInfo(LPCTSTR strategy)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	strResult = m_strategy.GetStrategyStatus(strategy);

	return strResult.AllocSysString();
}

//param1  -- system 系统初始化
//        -- account#account;pwd;ip;apitype;brokerid;# 帐号登录，后面是帐号信息
BSTR CHSQuanTradeCtrl::Init(LPCTSTR param1)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	CString temp = param1;

	int exchid = -9999;

	/*
	if( temp.Find("order")!=-1 )
	{
		CFunctionPub pub;
		
		CStringArray list;

		pub.ParseParam(param1,list,',');

		CString code = list[1];
		int amount = atoi(list[2]);		
		string sb(list[3].GetBuffer(0));
		string kp(list[4].GetBuffer(0));
		double d = atof(list[5]);
		int type = atoi(list[6]);
		int timeout = atoi(list[7]);
		double jiacha = atof(list[8]);
		char* account = list[9].GetBuffer();

		//m_tradeinterface.InsertOrder("600570",100,'1','0',price,type,timeout,jiacha,account);

		//m_tradeinterface.InsertOrder(code.GetBuffer(0),amount,sb[0],kp[0],price,type,timeout,jiacha,account);

		m_tradeinterface.InsertOrder(code.GetBuffer(0),amount,sb[0],kp[0],d,type,timeout,jiacha,account);

		m_tradeinterface.Log(temp.GetBuffer(0));

		temp.ReleaseBuffer();


	}
	
	if( temp.Find("withdraw")!=-1)
	{

		CFunctionPub pub;
		
		CStringArray list;

		pub.ParseParam(param1,list,',');

		string account = list[1];
		int entrustno = atoi(list[2]);		

		m_tradeinterface.WithDraw((char*)account.c_str(),entrustno,"1");

		m_tradeinterface.Log(temp.GetBuffer(0));

		temp.ReleaseBuffer();


		//{
		//	string path = Tool::GetProgramDir() + "\\msg.wav";

		//	PlaySound(path.c_str(),AfxGetInstanceHandle(), SND_FILENAME|SND_ASYNC );  

		//
		//}
	
	}
	*/

	if( temp.Find("system") != -1 )
	{
		CFunctionPub pub;
		
		CStringArray list;

		pub.ParseParam(param1,list,'#');

		CString ufxip = list[1];

		CString ctpip= list[2];

		CString port = list[3];

		m_tradeinterface.m_hMsgWnd = m_hWnd;

		m_strategy.m_hMsgWnd = m_hWnd;

		if(port == "")
			m_tradeinterface.m_port = 5150;
		else
			m_tradeinterface.m_port = atoi(port);

		m_tradeinterface.init();	

		m_strategy.m_pTradeInterface = &m_tradeinterface;

		m_strategy.init(ufxip,ctpip);

		m_strategy.Log(temp.GetBuffer(0));

		temp.ReleaseBuffer();

		m_tradeinterface.m_strManger = &m_strategy;

		//Tool::LoadCodeInfo();//加载合约
		//加载合约信息
		m_codeinfo.LoadCodeInfo();

		//向策略管理类传递该指针
		m_strategy.m_pGetCodeInfo = &m_codeinfo;

		m_Req.init(&m_strategy,&m_tradeinterface,&m_codeinfo);

		//设置dump
		InitMinDump();
	
	}
	else
	{
		m_Req.handle(temp.GetBuffer(0));
	}
	/*
	else if(temp.Find("subcode#")!=-1)    //订阅合约
	{
		CFunctionPub pub;
		
		CStringArray list;

		m_strategy.Log(temp.GetBuffer(0));

		temp.ReleaseBuffer();

		pub.ParseParam(param1,list,'#');

		if( list.GetSize() < 1)
			return strResult.AllocSysString();

		if( list[0] == "subcode" )
		{
			CStringArray list2;

			pub.ParseParam(list[1],list2,';');

			char* Instrument[1];

			//int exchid = -9999;

			CString subtype;

			Instrument[0] = list2[0].GetBuffer(0);

			list2[0].ReleaseBuffer();

			subtype = list2[2].GetBuffer(0);

			list2[1].ReleaseBuffer();

			exchid = m_codeinfo.getExchid(Instrument[0]);

			//维护一份已经订阅的合约列表
			if(m_strategy.lpUserMdSpi_CTP!=NULL && (subtype.MakeLower().Compare("ctp") == 0))
			{
				char * pt = new char[1];
				strcpy(pt,Instrument[0]);
				m_strategy.m_code.Add(pt);
				m_strategy.lpUserMdSpi_CTP->m_nInsNum = 0;
				for(unsigned int i=0;i<m_strategy.m_code.GetSize();i++)
				{	
					strcpy(m_strategy.lpUserMdSpi_CTP->m_ppInstrumentID[i],(char*)m_strategy.m_code[i]);
					m_strategy.lpUserMdSpi_CTP->m_nInsNum++;
				}

			}

			if(m_strategy.lpUserMdSpi!=NULL && (subtype.MakeLower().Compare("ufx") == 0))
			{
				
				//char * pt = new char[1];
				//strcpy(pt,Instrument[0]);
				//m_strategy.m_code.Add(pt);
				//m_strategy.lpUserMdSpi->m_nInsNum = 0;
				//for(unsigned int i=0;i<m_strategy.m_code.GetSize();i++)
				//{	
				//	strcpy(m_strategy.lpUserMdSpi->m_ppInstrumentID[i],(char*)m_strategy.m_code[i]);
				//	m_strategy.lpUserMdSpi->m_nInsNum++;
				//}

				m_strategy.lpUserMdSpi->m_UfxCode[Instrument[0]] = exchid;

			}


			//先去证券里面查询exchid，没有赋值14
			if(exchid!=1 && exchid!=2 && exchid != 3)
			{
				m_strategy.Log(m_codeinfo.getfutuName(Instrument[0]));

				if(temp.Find("FUTURES")!=-1)
				{
					exchid = Tool::TransExchId(m_codeinfo.getfutuexchid(Instrument[0]));
				}
			}
			else
			{
				m_strategy.Log(m_codeinfo.getName(Instrument[0]));
			}

			//区分是ufx接口订阅和ctp接口
			if((subtype.MakeLower().Compare("ufx") == 0)  && (m_strategy.lpUserMdApi !=NULL))
			{
				m_strategy.lpUserMdApi->SubscribeMarketData(exchid, Instrument, sizeof(Instrument)/sizeof(char*));
				//m_strategy.lpUserMdApi->SubscribeMarketData(1, Instrument, sizeof(Instrument)/sizeof(char*));

				//先单独处理131810订阅
				if(strcmp(Instrument[0],"131810")==0)
				{
					m_strategy.lpUserMdApi->SubscribeMarketData(2, Instrument, sizeof(Instrument)/sizeof(char*));
				}
				
			}
			else if((subtype.MakeLower().Compare("ctp") == 0) && (m_strategy.lpUserMdApi_CTP!=NULL) )
			{
				
				m_strategy.lpUserMdApi_CTP->SubscribeMarketData(Instrument,sizeof(Instrument)/sizeof(char*));

			}

		
		}
	}
	else if(temp.Find("account#")!=-1)
	{
		CFunctionPub pub;
		
		CStringArray list;

		pub.ParseParam(param1,list,'#');

		if( list.GetSize() < 2)
			return strResult.AllocSysString();

		if( list[0] == "account" )
		{
			CStringArray list2;

			pub.ParseParam(list[1],list2,';');

			m_tradeinterface.OpenAccount(list2[0],list2[1],list2[2],list2[3],list2[4]);

			WRITE_LOG(1,"account");
		}
		
		m_tradeinterface.Log(temp.GetBuffer(0));
	}
	//新ip建立连接
	else if(temp.Find("newip#")!=-1)
	{
		CFunctionPub pub;
		
		CStringArray list;

		pub.ParseParam(param1,list,'#');

		m_strategy.Log(temp.GetBuffer(0));

		CString type = list[1];

		CString ip= list[2];

		//ufx订阅期货行情
		if(type=="ufx")
		{
			m_strategy.register_ufx(ip);
		}
		//订阅ctp期货行情
		else if(type=="ctp")
		{
			m_strategy.register_ctp(ip);
		}

	}
	else if(temp.Find("test")!=-1)
	{
		//m_k.ReadKdata("x",(char*)m_k.GetDir().c_str());
		m_strategy.m_oKManager.ReadFile(1);
	}
	//功能请求，格式:REQ#queryopt;
	else if(temp.Find("queryopt") != -1)
	{
		CHundsunFtdcQryInstrumentField m_Instrument;
		memset(&m_Instrument, 0, sizeof(CHundsunFtdcQryInstrumentField));
		m_Instrument.ExchangeID = 3;				//代表上海的股票期权
		m_strategy.lpUserMdApi->ReqQryInstrument(&m_Instrument, 0);

	}
	*/

	

	
	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::Exit(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	//m_strategy.Exit();

	//m_tradeinterface.Exit();

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::GetParamValue(LPCTSTR modelname, LPCTSTR paramname)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult,str;

	str = paramname;

	if(str!="")
	{
		strResult = m_strategy.GetStrategyParamValue(modelname,paramname);
	}
	else
	{
		strResult = m_strategy.GetStrategyParamValue(modelname);
	}

	return strResult.AllocSysString();
}


/*
TradeNotify 参数说明
#define TRADE_MSG_TYPE_LOG 0
#define TRADE_MSG_TYPE_LOG_PRV 1
#define TRADE_MSG_TYPE_OTHER 2
#define TRADE_MSG_TYPE_HQ 3
#define TRADE_MSG_TYPE_QUERY 4
#define TRADE_MSG_TYPE_SIG 5

param1, 0, global log; 1,private log; 2,other msg, 3, 行情, 4,功能请求返回 , 5, 成交信号
*/

LRESULT CHSQuanTradeCtrl::OnTradeMessage(WPARAM wp,LPARAM lp)
{
	switch(wp)
	{
	case 0:
	case 3:
	case TRADE_MSG_TYPE_CODE_QUERY_DONE:
		{
			CString str1,str2;

			str1 = (char *)lp;

			TradeNotify(wp,0,str1,str2);
		}

		break;
	case 1:
	case 4:
	case 5:
	case TRADE_MSG_TYPE_ARBITRAGE_HQ:
	case TRADE_MSG_TYPE_POSTION_INFO:
		{
			CString str1,str2;

			str1 = (char *)lp;
			int nt = str1.ReverseFind('$');
			if (nt > 0)
			{
				str2 = str1.Right(str1.GetLength() - nt - 1);//module id
				str1 = str1.Left(nt);
			}
			TradeNotify(wp,0,str1,str2);
			
		}
		break;

	}
	return 0L;
}

LRESULT CHSQuanTradeCtrl:: OnEntrustMessage(WPARAM wp,LPARAM lp)
{


	switch(wp)
	{
	case 0:
		{
			COrderInfo *c = (COrderInfo*)lp;

			char instrument[32];
			strcpy(instrument,c->szInstrument);
			int count = c->ordercount;
			char bs = c->bs;
			char kp = c->kp;
			double price = c->orderprice;
			int follow = c->ordertype;
			int m_timeOut = c->timeOut;
			double jiacha = c->jiacha; 
			char account[20];
			strcpy(account,c->account);

			if(strcmp("600570",instrument)==0)
				return 0L;

			c->ref = m_tradeinterface.InsertOrder(instrument,count,bs,kp,price,follow,m_timeOut,jiacha,account);
			break;

		}
	case 1:
		{
			CEntrustInfo *c =(CEntrustInfo*)lp;
			m_tradeinterface.WithDraw(c->account,c->entrustno,c->szInstrument);
			break;

		}
	case 2:
		{
			m_tradeinterface.QueryPosition((char*)lp);
			break;
		}
	case 3:
		{
			m_tradeinterface.Login();
			break;
		}
	case 4:
		{
			m_tradeinterface.QueryEntrust((char*)lp,0);
			break;
		}
	case 5:
		{
			m_tradeinterface.QueryTrade((char*)lp);
			break;
		}
	case 6:
		{
			m_tradeinterface.QueryFund((char*)lp);
			break;
		}
	case 7:
		{
			//暂时先这样把，感觉不太好
			//"subcode#" + "131810" + ";" + "STOCK;UFX;" + "#"
			Init((char*)lp);
			break;
		}

	}

	return 0L;

}



LRESULT CHSQuanTradeCtrl:: OnSockMsg(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(lParam))
	{
	case FD_ACCEPT:
		{
			SOCKET client = m_tradeinterface.m_ServerSock.Accept();
			WSAAsyncSelect(client, this->m_hWnd, WM_MESSAGE_SOCK, FD_READ | FD_CLOSE);

		}
		break;
	case FD_READ:
		{

			char buf[MAX_BUF_LEN];
			int length = 0;
			memset(buf, 0, MAX_BUF_LEN);

			//数据没接收完整
			if(!m_tradeinterface.m_bEnd)
			{
				if ((length = m_tradeinterface.m_ServerSock.Read(wParam, buf, m_tradeinterface.m_iRestLength )) < 1)
				{
					TRACE("接收数据包失败！\n");
					return 0;
				}
				buf[length] = 0x00;

				
				//收到的数据
				string str(buf);
				int len = str.length();

				//累加收到的数据
				m_tradeinterface.buf.append(str);
				/*
				m_tradeinterface.Log(buf);
				CString yy;
				yy.Format("%d-------%d",len,m_tradeinterface.m_iRestLength);
				m_tradeinterface.Log(yy.GetBuffer(0));
				WRITE_LOG(1,str);
				*/

				if(len>=m_tradeinterface.m_iRestLength)
				{
					m_tradeinterface.m_bEnd = true;
					m_tradeinterface.m_iRestLength = 0;

				}
				else
				{
					m_tradeinterface.m_iRestLength = m_tradeinterface.m_iRestLength - len;

					return 0;
				}
				
			}
			else 
			{

				if ((length = m_tradeinterface.m_ServerSock.Read(wParam, buf, sizeof(PackerHeader))) < 1)
				{
					TRACE("接收报文头失败！\n");
					return 0;
				}
				buf[length] = 0x00;

				// 接收协议头
				memset(buf, 0, MAX_BUF_LEN);
				if ((length = m_tradeinterface.m_ServerSock.Read(wParam, buf, sizeof(ProtocolHeader))) < 1)
				{
					TRACE("接收协议头失败！\n");
					return 0;
				}
				buf[length] = 0x00;

				ProtocolHeader* ptl = (ProtocolHeader*)buf;
				int nDataLength = ptl->DataLength;
				// 协议数据包
				memset(buf, 0, MAX_BUF_LEN);
				if ((length = m_tradeinterface.m_ServerSock.Read(wParam, buf, nDataLength)) < 1)
				{
					TRACE(_T("接收协议数据包失败！"));
					return 0;
				}
				buf[length] = 0x00;

				if (strcmp(buf, HEARTBEAT_PACKET) != 0)
				{
					string str(buf);
					int len = str.length();
					/*
					m_tradeinterface.Log(buf);
					CString yy;
					yy.Format("begin-----%d-------%d",len,nDataLength);
					m_tradeinterface.Log(yy.GetBuffer(0));
					WRITE_LOG(1,str);
					*/
					m_tradeinterface.buf.append(str);
					if(len < nDataLength)
					{
						m_tradeinterface.m_bEnd = false;
						m_tradeinterface.m_iRestLength = nDataLength - len;

						return 0;	
					}
						
				}

			}


			if (strcmp(buf, HEARTBEAT_PACKET) == 0)
			{
				// 心跳包
				CTime tm = CTime::GetCurrentTime();
				CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");
				CString str;
				str.Format(_T("%s    心跳包：%s\n"), strTime, buf);
				//m_lstBoxSockMsg.InsertString(0, str);

				// 向客户端发送心跳包
				memset(buf, 0, MAX_BUF_LEN);

				int pos = 0;
				PackerHeader packer;
				packer.HeaderFlag = 7611;
				packer.DataLength = sizeof(ProtocolHeader) + sizeof(HEARTBEAT_PACKET);
				memcpy(buf, (void*)&packer, sizeof(PackerHeader));
				pos += sizeof(PackerHeader);

				ProtocolHeader protocol;
				protocol.Action = 8888;
				protocol.Data1 = 0;
				protocol.Data2 = 0;
				protocol.Data3 = 0;
				protocol.Data4 = 0;
				protocol.DataLength = sizeof(HEARTBEAT_PACKET);
				protocol.Data5 = 0;
				memcpy(buf + pos, (void*)&protocol, sizeof(ProtocolHeader));
				pos += sizeof(ProtocolHeader);

				memcpy(buf + pos, (void*)HEARTBEAT_PACKET, sizeof(HEARTBEAT_PACKET));
				pos += sizeof(HEARTBEAT_PACKET);

				if (!m_tradeinterface.m_ServerSock.Send(wParam, buf, pos))
				{
					TRACE("向客户端发送心跳包失败！\n");
				}
			}
			else
			{
				CTime tm = CTime::GetCurrentTime();
				CString strTime = tm.Format("%Y-%m-%d %H:%M:%S");
				CString str;
				str.Format(_T("%s    功能包：%s\n"), strTime, m_tradeinterface.buf);
				//m_lstBoxSockMsg.InsertString(0, str);
				TRACE(str);

				//这里处理下单应答，委托回报，撤单回报
				int funcid = atoi(Tool::getRootAttribute(0,m_tradeinterface.buf));				//int funcid = 0;
				//return 0;
				char info[512];
				int entrustno = -1;
				int errorid = 0;

				char error[256];				
				char code[10];
				char account[32];
				char subtype[10];
				char datatype[10];
				int bs = 0;
				double price = 0;
				char time[16];
				int count = 0;
				switch (funcid)
				{
				//委托应答
				case 1004:					
					entrustno = Tool::TransRspEntrust(m_tradeinterface.buf);					
					strcpy(error,Tool::getErrorInfo(m_tradeinterface.buf));
					sprintf(info,"委托号:%d,反馈信息:%s",entrustno,error);
					m_strategy.Log(info);
					if(entrustno == -9999)
					{
						errorid = -1;     //报单失败
						WRITE_LOG(1,info);
					}
					m_tradeinterface.OnRspOrderInsert(Tool::getSessionid(m_tradeinterface.buf),Tool::TransRspEntrust(m_tradeinterface.buf),'c',errorid);				
					WRITE_LOG(1,m_tradeinterface.buf);
					break;
				//撤单应答
				case 1005:				
					strcpy(error,Tool::getErrorInfo(m_tradeinterface.buf));
					char msg[128];
					if(strcmp(error,"")==0)
					{
						sprintf(msg,"委托号 %d  撤单成功",Tool::getEntrustNo(m_tradeinterface.buf));
						m_strategy.Log(msg);
					}
					else
					{
						sprintf(msg,"撤单失败");
						m_strategy.Log(msg);
					}
					WRITE_LOG(1,msg);
					break;
				//回报
				case 1500:
					Tool::TransRspSub(m_tradeinterface.buf,code,account,subtype,datatype,&entrustno,&bs,&price,time,&count);
					if(strcmp("Add",datatype)==0)
					{
						//撤单回报走的是成交回报
						if(strcmp("Deal",subtype)==0)
							m_tradeinterface.RtnTrade(account,entrustno,entrustno,price,'0',bs,time,count,code);
						
						//WRITE_LOG(1,m_tradeinterface.buf);
					}
					WRITE_LOG(1,m_tradeinterface.buf);
					break;
				//查询
				case 1501:
					m_strategy.Log("查询成功");
					//account = Tool::getRootAttribute(1,m_tradeinterface.buf);
					strcpy(account,Tool::getRootAttribute(1,m_tradeinterface.buf));
					m_tradeinterface.getQueryInfo((char*)m_tradeinterface.buf.c_str(),account);
					WRITE_LOG(1,m_tradeinterface.buf);
					break;
				default:
					break;
				}

				//处理完回复
				m_tradeinterface.buf = "";
				m_tradeinterface.m_bEnd = true;
				m_tradeinterface.m_iRestLength = 0;
			}

		}
		break;
	case FD_CLOSE:
		{
			if (WSAGETSELECTERROR(lParam) == 0)
			{
				WRITE_LOG(1,"socket close...");
				m_strategy.Log("socket close...");
			}
			else
			{
				WRITE_LOG(1,"socket close...");
				m_strategy.Log("socket close...");
				m_tradeinterface.m_ServerSock.CloseSockAccept();
			}
		}
		break;
	}
	return 0;

}

BSTR CHSQuanTradeCtrl::GetStragyLogInfo(LPCTSTR modelname)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码
	strResult = m_strategy.GetStragyLog(modelname);

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::GetStragyTradeSignalList(LPCTSTR modelname)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码
	strResult = m_strategy.GetStrategySignal(modelname);

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::StragegyBackTest(LPCTSTR name, LPCTSTR paramlist, LONG time1, LONG time2, LPCTSTR coderanger, LPCTSTR sCycle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码

	m_strategy.StragegyBackTest(name, paramlist, time1, time2, coderanger, sCycle);

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::GetAllStockFilterName(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::RunStockFilterStragegy(LPCTSTR name, LPCTSTR paramfield, LPCTSTR paramvalue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::GetStockFilterStragegyParamInfo(LPCTSTR name)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::SetStockFilterStragegyParam(LPCTSTR field, LPCTSTR value)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码

	return strResult.AllocSysString();
}


BSTR CHSQuanTradeCtrl::RunStockFilter(LPCTSTR name, LPCTSTR field1, LPCTSTR value1)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码

	return strResult.AllocSysString();
}
LRESULT CHSQuanTradeCtrl::OnBackTestResultMessage(WPARAM wParam, LPARAM lParam)
{
	int iResultType = (int)wParam;
	
	CStringArray *pResult = (CStringArray*)lParam;
	if (pResult)
	{
		CStdioFile fResult;

		CString sFileName;
		switch(iResultType)
		{
		case BACKTEST_TRADE_RESULT:
			sFileName = "~backtest.txt";
			break;
		case BACKTEST_EQUITY_RESULT:
			sFileName = "~backtest_equity.txt";
			break;
		default:
			sFileName = "~backtest.txt";
			break;
		}

		sFileName = CFunctionPub::GetModuleDir() + "\\" + sFileName;
		if(!fResult.Open(sFileName,CFile::modeWrite | CFile::modeCreate ))
			return -1;


		for (int i = 0; i < pResult->GetCount(); i++)
		{
			CString strData = pResult->GetAt(i);
			
			fResult.WriteString(strData);
			fResult.WriteString("\n");
		}
		fResult.Close();

		TradeNotify(2, iResultType, (LPCSTR)sFileName, 0);
	}
	
	return 0L;
}

 LRESULT CHSQuanTradeCtrl::OnBackTestStep(WPARAM wParam, LPARAM lParam)
 {
	 St_BackTest_Step *pStep = (St_BackTest_Step*) lParam;
	 if (pStep)
	 {
		 
		BackTestNotify(pStep->m_sCode, pStep->m_iTotalCount, pStep->m_iTotalStep,
			pStep->m_iSubCount, pStep->m_iSubStep);

		delete pStep;
		pStep = NULL;
	 }
	
	 return 0L;
 }






 LONG CHSQuanTradeCtrl::GetBackTestStatus(void)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加调度处理程序代码

	 if(m_strategy.HasBackTestOver())
	 {
		 return 0;
	 }

	 return 1;
 }


 void CHSQuanTradeCtrl::SetBackTestStatus(LONG newVal)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 // TODO: 在此添加属性处理程序代码

	 SetModifiedFlag();
 }

 LRESULT CHSQuanTradeCtrl::OnSubCode(WPARAM wParam, LPARAM lParam)
 {
	 char* sSubCode = (char*) lParam;
	 if (sSubCode)
	 {
		 this->Init(sSubCode);
	 }

	 return 0L;
 }

 BSTR CHSQuanTradeCtrl::GetStrategyContext(LPCTSTR sStrategyId)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 CString strResult;

	 strResult = m_strategy.GetStrategyContext(sStrategyId);

	 return strResult.AllocSysString();
 }


 BSTR CHSQuanTradeCtrl::SetStrategyContext(LPCTSTR sStrategyId, LPCTSTR sContext)
 {
	 AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 CString strResult;

	 // TODO: 在此添加调度处理程序代码

	 m_strategy.SetStrategyContext(sStrategyId, sContext);

	 return strResult.AllocSysString();
 }
