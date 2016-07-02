#include "stdafx.h"
#include "StragegyManager.h"
#include "StrategyLimitupSale.h"
#include "HundsunMdCenterApi.h"
#include "mdSpi.h"
#include "TradeInterface.h"
#include "ThostFtdcMdApi.h"
#include "MdSpiCTP.h"
//#include "StrategyGrid.h"
#include "tool.h"
#include "data.h"
#include "FunctionPub.h"


typedef long (* Interface)(CStrategyBase ** pTradeBase);

CStrategyManager::CStrategyManager(void)
{
	lpUserMdApi = NULL;

	lpUserMdSpi = NULL;;

	lpUserMdApi_CTP = NULL;; 

	lpUserMdSpi_CTP = NULL;
	m_eBackTestMode = E_K_1_MIN_TEST;

	
}


CStrategyManager::~CStrategyManager(void)
{
	Exit();
}

CString CStrategyManager::GetAppPath()
{
	CString strFileName;

	GetModuleFileName(NULL,strFileName.GetBuffer(256),256);

	strFileName.ReleaseBuffer();

	int nt = strFileName.ReverseFind('\\');

	return strFileName.Mid(0,nt);
}

void CStrategyManager::init(CString ufxhqip,CString ctphqip)
{
	//CStrategyLimitupSale * pt = new CStrategyLimitupSale();
	
	//m_arModel.Add(pt);
	
	//CStrategyGrid * pg = new CStrategyGrid();
	//m_arModel.Add(pg);
	


	CFileFind find;

	int nt = find.FindFile(GetAppPath()+"\\StrageDLL\\*.dll");

	while(nt)
	{
		nt = find.FindNextFileA();

		if( find.IsDirectory() || find.IsDots() )
		{
			
		}
		else
		{
			CString filename = find.GetFileName();

			HINSTANCE dll = ::LoadLibrary(GetAppPath()+"\\StrageDLL\\"+filename);

			if( dll )
			{

				Interface inter = (Interface)GetProcAddress((HINSTANCE)dll,"Interface");

				if( inter )
				{
					CStrategyBase * pBase = NULL;

					inter(&pBase);

					m_arModel.Add(pBase);
				}
			}
		}
	}

	m_oKManager.Init();
	m_oKManager.m_hMsgWnd = m_hMsgWnd;

	register_ufx(ufxhqip);

	register_ctp(ctphqip);

	m_pthdSubcode = NULL;
	m_thdAux.Start(Task_Aux, 0, this);

}


void CStrategyManager::Exit(void)
{

	WRITE_LOG(1,"exit...");

	m_code.RemoveAll();

	if(lpUserMdApi_CTP)
	{
		lpUserMdApi_CTP->Release();
		lpUserMdApi_CTP = NULL;
	}
	
	if(lpUserMdSpi_CTP)
	{
		delete lpUserMdSpi_CTP;
		lpUserMdSpi_CTP = NULL;
	}

	if(lpUserMdSpi)
	{
		delete lpUserMdSpi;
		lpUserMdSpi = NULL;
	}

	if(lpUserMdApi)
	{
		
		lpUserMdApi->ReleaseApi();
		lpUserMdApi = NULL;
	}




	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_arModelRun[i];

		if (pt)
		{
			pt->m_bRun = false;

			pt->m_Exit = true;
			pt->JoinAllTask();

			delete pt;

			pt = NULL;
		}
				

	}
	m_arModelRun.RemoveAll();


	for(int i=0;i<m_arModel.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_arModel[i];

		pt->m_bRun = false;

		pt->m_Exit = true;

		delete pt;

		pt = NULL;

	}
	m_arModel.RemoveAll();

	m_thdAux.Terminate();
	if (m_pthdSubcode)
	{
		m_pthdSubcode->Terminate();
	}
	
	

}


CString CStrategyManager::GetAllStrategyName(void)
{
	CString name;

	for(int i=0;i<m_arModel.GetSize();i++)
	{
		CStrategyLimitupSale * pt = (CStrategyLimitupSale *)m_arModel[i];

		name += pt->m_strStrategyName;

		name += ";";
	}

	return name;
}


CString CStrategyManager::GetStrategyParamInfo(CString name)
{
	for(int i=0;i<m_arModel.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_arModel[i];
		if( pt->m_strStrategyName == name )
		{
			return pt->m_strParamList;
		}
	}
	return "";
}

CString CStrategyManager::GetStrategyParamValue(CString modelname)
{
	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_arModelRun[i];
		if( pt->GetStrategyId() == modelname )
		{
			return pt->GetParamValue();
		}
	}
	return "";
}

CString CStrategyManager::GetStrategyParamValue(CString modelname,CString paramname)
{
	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_arModelRun[i];
		if( pt->GetStrategyId() == modelname )
		{
			return pt->GetParamValue(paramname);
		}
	}

	return "";
}


void CStrategyManager::SetStrategyParam(CString modelname,CString paramname, CString value)
{
	//CFunctionPub pub;
	//CStringArray list;
	//pub.ParseParam(modelname,list,'#');

	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_arModelRun[i];

		if( pt->GetStrategyId() == modelname )
		{
			if(paramname=="")
			{
				pt->SetParam(value);
			}
			else
			{
				pt->SetParam(paramname,value);
			}
		}
		
	}	
}


//使用#分割，第一个参数是模型名称，后面是参数值
CString CStrategyManager::AddRunStragegy(CString param)
{
	//"涨停板股票卖出策略#600570;10;40;0.01;0.01;0.01;0;1;101090218;#"

	//param format: "StrategyName#Params#ModuleId#StrategyId#PostionInfo"
	//PostionInfo format: "code1;name;postion;price;dir;time$code2;name;postion;price;dir;time"
	this->Log(param.GetBuffer(param.GetLength()));

	CStringArray list;

	ParseParam(param,list,'#');

	if( list.GetSize() < 2 )
		return "-1";

	CString name = list[0];

	CStrategyBase * pRun = GetStrategyInstance(name);
	if (pRun)
	{

		pRun->SetStrategyManager(this);
		pRun->m_pStrategyCodeInfo = m_pGetCodeInfo;
		//pRun->m_pCodeInfo = &CPubData::m_sCodeInfo;
		pRun->m_pKManager = &m_oKManager;
		pRun->SetWndHwnd(m_hMsgWnd);

		if (list.GetSize() >= 2)
		{
			pRun->SetParam(list[1]);
		}

		if (list.GetSize() >= 3)
		{
			pRun->SetModuleId(list[2]);
		}
		if (list.GetSize() >= 4)
		{
			pRun->SetStrategyId(list[3]);
		}

		if (list.GetSize() >= 5)
		{
			pRun->ResumePostionInfo(list[4]);
		}


		//pRun->m_pMapFutuCodeInfo = &CPubData::m_sFutuCodeInfo;

		//应该在这里来指定交易接口
		//pRun->m_SPTrade = this->m_pTradeInterface;



		


		CTime tm = CTime::GetCurrentTime();

		//CString s;

		//s.Format("%d%d%d%d",tm.GetDay(),tm.GetHour(),tm.GetMinute(),tm.GetSecond());

		


		//针对一些需要查询功能的策略
		bool isQuery = false;
		if(strcmp(name,"网格交易策略")==0)
		{
			CString str = list[1];
			CStringArray list2;
			ParseParam(str,list2,';');
			CString kp = list2[7];
			if(kp=='1')
				isQuery = true;
		}

		CString strModelName = pRun->m_strStrategyName + pRun->GetStrategyId();
		CString strlog;
		strlog.Format("增加模型,%s", strModelName);
		WRITE_LOG(1,strlog.GetBuffer(0));

		if( !strModelName.IsEmpty() )
		{
			//设置公共信息指针,例如合约信息等
			pRun->m_pStrategyCodeInfo = m_pGetCodeInfo;

			m_mtxModelRun.Lock();
			m_arModelRun.Add(pRun);
			m_mtxModelRun.Unlock();
			m_pTradeInterface->SetAccountInfo(pRun->m_fundAccount.GetBuffer(0),isQuery);

			//改为这种方式，否则前台【激活】会报错
			//RunModel(pRun->m_strModelName);
			pRun->m_bRun = true;

			pRun->StartSubcode();

			pRun->StartTick();

			pRun->BeginRun();	


			return pRun->GetStrategyId();
		}
	} 
	if( pRun == NULL )
		return "-2";


	return "-3";
}


void CStrategyManager::ParseParam(CString param, CStringArray & list,char ch)
{
	int nBegin = 0;

	int nEnd = param.Find(ch);

	if( nEnd == -1 )
		list.Add(param);

	while( nEnd >= 0) 
	{
		list.Add( param.Mid(nBegin,nEnd-nBegin) );

		nBegin = nEnd + 1;

		nEnd = param.Find(ch,nBegin);
	}
}


CStrategyBase * CStrategyManager::GetStrategyInstance(CString name)
{
	for(int i=0;i<m_arModel.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_arModel[i];
		if( pt->m_strStrategyName == name )
		{
			return pt->NewInstance();
		}
	}
	return NULL;
}




CString CStrategyManager::GetStrategyStatus(CString modelname)
{
	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = ( CStrategyBase *)m_arModelRun[i];

		if( pt->GetStrategyId() == modelname )
		{
			return pt->GetStatusInfo();
		}
	}
	return CString();
}


int CStrategyManager::PauseModel(CString modelname)
{
	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = ( CStrategyBase *)m_arModelRun[i];

		if( pt->GetStrategyId() == modelname )
		{
			pt->m_bRun = false;

			pt->Pause();

			break;
		}
	}
	return 0;
}


int CStrategyManager::StopModel(CString modelname)
{
	CStrategyBase * pt = NULL;
	m_mtxModelRun.Lock();
	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		pt = (CStrategyBase *)m_arModelRun[i];

		if( pt->GetStrategyId() == modelname )
		{
			pt->m_Exit = true;
			pt->m_bRun = false;
			m_arModelRun.RemoveAt(i);

			/*pt->m_Exit = true;

			pt->m_bRun = false;

			pt->JoinAllTask();

			delete pt;

			pt = NULL;*/

			break;
		}
	}
	m_mtxModelRun.Unlock();

	if (pt && pt->m_Exit && !pt->m_bRun)
	{
		m_mtxModelRemove.Lock();
		m_arModelRemove.Add(pt);
		m_mtxModelRemove.Unlock();
		
	}
	return 0;
}


int CStrategyManager::RunModel(CString modelname)
{
	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = ( CStrategyBase *)m_arModelRun[i];

		if( pt->GetStrategyId() == modelname )
		{
			pt->m_bRun = true;

			//pt->BeginRun();

			break;
		}
	}
	return 0;
}

void CStrategyManager::Log(char * msg)
{
	::SendMessage(m_hMsgWnd,WM_MESSAGE_TRADE,0,(long)msg);
}


CString CStrategyManager::GetStragyLog(CString modelname)
{
	CString msg;

	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = ( CStrategyBase *)m_arModelRun[i];

		if( pt->GetStrategyId() == modelname )
		{
			return pt->GetLogInfo();
		}	
	}

	return "";

}

CString CStrategyManager::GetStrategySignal(CString modelname)
{

	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = ( CStrategyBase *)m_arModelRun[i];

		if( pt->GetStrategyId() == modelname )
		{
			return pt->GetSignalInfo();
		}	
	}

	return "";
}

void CStrategyManager::ModelTick(CTickData *pd)
{
	m_mtxModelRun.Lock();
	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_arModelRun[i];
		if (pt)
		{
			if(!pt->m_bRun)
				continue;
			//pt->Tick(pd);
			pt->TickPeek(pd);
		}
		
	}
	m_mtxModelRun.Unlock();


}


void CStrategyManager::register_ctp(CString ip)
{
	if(ip=="")
		return;

	if(lpUserMdApi_CTP==NULL || lpUserMdSpi_CTP==NULL)
	{

		lpUserMdApi_CTP = CThostFtdcMdApi::CreateFtdcMdApi();

		lpUserMdSpi_CTP = new CMdSpiCTP();

		//设置帐号密码
		strcpy(lpUserMdSpi_CTP->m_szBroker,"");
		strcpy(lpUserMdSpi_CTP->m_szUserID,"");
		strcpy(lpUserMdSpi_CTP->m_szPwd,"");

		lpUserMdSpi_CTP->m_ppInstrumentID = NULL;
		lpUserMdSpi_CTP->m_ppInstrumentID = (char**)calloc(32, sizeof(char*));

		for (int i = 0; i < 32; ++i)
		{
			lpUserMdSpi_CTP->m_ppInstrumentID[i] = (char*)calloc(32, sizeof(char));
		}
		lpUserMdSpi_CTP->m_nInsNum = 0;

		lpUserMdSpi_CTP->m_pUserApiHQ_CTP = lpUserMdApi_CTP;

		lpUserMdSpi_CTP->m_MdStrategy_CTP = this;

		lpUserMdApi_CTP->RegisterSpi(lpUserMdSpi_CTP);

		lpUserMdApi_CTP->RegisterFront((char*)ip.GetBuffer(0));

		lpUserMdApi_CTP->Init();
	
	}
	//说明先前注册过
	else
	{
		lpUserMdApi_CTP->Release();
		
		delete lpUserMdSpi_CTP;


		lpUserMdApi_CTP = CThostFtdcMdApi::CreateFtdcMdApi();

		lpUserMdSpi_CTP = new CMdSpiCTP();

		//设置帐号密码
		strcpy(lpUserMdSpi_CTP->m_szBroker,"");
		strcpy(lpUserMdSpi_CTP->m_szUserID,"");
		strcpy(lpUserMdSpi_CTP->m_szPwd,"");

		lpUserMdSpi_CTP->m_ppInstrumentID = NULL;
		lpUserMdSpi_CTP->m_ppInstrumentID = (char**)calloc(32, sizeof(char*));

		for (int i = 0; i < 32; ++i)
		{
			lpUserMdSpi_CTP->m_ppInstrumentID[i] = (char*)calloc(32, sizeof(char));
		}
		lpUserMdSpi_CTP->m_nInsNum = 0;

		lpUserMdSpi_CTP->m_pUserApiHQ_CTP = lpUserMdApi_CTP;

		lpUserMdSpi_CTP->m_MdStrategy_CTP = this;

		lpUserMdApi_CTP->RegisterSpi(lpUserMdSpi_CTP);

		lpUserMdApi_CTP->RegisterFront((char*)ip.GetBuffer(0));

		lpUserMdApi_CTP->Init();

	}


}

void CStrategyManager::register_ufx(CString ip)
{

	string path1 = Tool::GetProgramDir() + "\\h5apilicense.xml";

	const char* pathstr1 = path1.c_str();

	string path2 = Tool::GetProgramDir() + "\\h5config.xml";

	const char* pathstr2 = path2.c_str();

	if(lpUserMdApi==NULL || lpUserMdSpi==NULL)
	{

		//hundsun接口
		lpUserMdApi = CHundsunMdCenterApi::CreateHundsunMdCenterApi();

		lpUserMdSpi = new CMdSpi(lpUserMdApi);

		lpUserMdSpi->m_MdStrategy = this;

		lpUserMdApi->RegisterSpi(lpUserMdSpi);

		//维护订阅列表
		/*
		lpUserMdSpi->m_ppInstrumentID = NULL;
		lpUserMdSpi->m_ppInstrumentID = (char**)calloc(32, sizeof(char*));

		for (int i = 0; i < 32; ++i)
		{
			lpUserMdSpi->m_ppInstrumentID[i] = (char*)calloc(32, sizeof(char));
		}
		lpUserMdSpi->m_nInsNum = 0;*/


		//订阅证券行情
		lpUserMdApi->RegisterH5(pathstr1, pathstr2,false);

		lpUserMdApi->Init(HUNDSUN_FTDC_H5);

		if( strcmp(ip,"") != 0 )
		{
			lpUserMdApi->RegisterFutu("license.dat");

			lpUserMdApi->RegisterFront(ip.GetBuffer(0));

			lpUserMdApi->Init(HUNDSUN_FTDC_Futu);
		}
		
	}
	else
	{
		lpUserMdApi->ReleaseApi();

		delete lpUserMdSpi;

		//hundsun接口
		lpUserMdApi = CHundsunMdCenterApi::CreateHundsunMdCenterApi();

		lpUserMdSpi = new CMdSpi(lpUserMdApi);

		lpUserMdSpi->m_MdStrategy = this;

		lpUserMdApi->RegisterSpi(lpUserMdSpi);


		//维护订阅列表
		lpUserMdSpi->m_ppInstrumentID = NULL;
		lpUserMdSpi->m_ppInstrumentID = (char**)calloc(32, sizeof(char*));

		for (int i = 0; i < 32; ++i)
		{
			lpUserMdSpi->m_ppInstrumentID[i] = (char*)calloc(32, sizeof(char));
		}
		lpUserMdSpi->m_nInsNum = 0;


		//订阅证券行情
		lpUserMdApi->RegisterH5(pathstr1, pathstr2);

		lpUserMdApi->Init(HUNDSUN_FTDC_H5);


		if( strcmp(ip,"") != 0 )
		{
			lpUserMdApi->RegisterFutu("license.dat");

			lpUserMdApi->RegisterFront(ip.GetBuffer(0));

			lpUserMdApi->Init(HUNDSUN_FTDC_Futu);
		}


	}



}

void CStrategyManager::SaveModels()
{
	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_arModelRun[i];

		//pt->saveModel();

	}


}

void CStrategyManager::ReLoad()
{

	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_arModelRun[i];

		//pt->reload();

	}


}

void CStrategyManager::SetAccountInfo(char *account,CAccData *data)
{
	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_arModelRun[i];

		if(strcmp(pt->m_fundAccount,account)==0)
		{
			pt->data = data;

			pt->SetWndHwnd(this->m_hMsgWnd);
		}

	}
}
void CStrategyManager::OnTest1Min(CStrategyBase* pStrategy)
{

	char msg[256];
	if (pStrategy)
	{
		CStrategyManager* pStrategyManager = pStrategy->GetStrategyManager();
		if (pStrategyManager)
		{

			CTime tmTestBegin;
			tmTestBegin = CTime::GetCurrentTime();
			pStrategy->m_pStatistic->SetStartTime(tmTestBegin);

			CStringArray &arCodes = pStrategyManager->m_arTestCodes;
			CTime tmBegin(pStrategyManager->m_lTestTimeBegin);
			CTime tmEnd(pStrategyManager->m_lTestTimeEnd);

			CTimeSpan tm_spCount = tmEnd - tmBegin;
			int iTotalCount = tm_spCount.GetDays();
			int iTotalStep = 0;
			CString strBeginTime = tmBegin.Format("%Y%m%d");
			CString strEndTime = tmEnd.Format("%Y%m%d");
			pStrategyManager->m_oKManagerEx.PrepareData(arCodes, tmBegin, tmEnd);

			CTime tmKData = tmBegin;
			while (tmKData < tmEnd)
			{
				CString strDate = tmKData.Format("%Y%m%d");

				CKDataOneDay* pKData = NULL;
				pStrategyManager->m_oKManagerEx.GetKDataListByDate(strDate.GetBuffer(0), &pKData);

				if (pKData)
				{

					//sprintf_s(msg,256,"日线个数:%d...",pKData->m_mapKData.size());
					//pStrategy->AddLog(msg);

					int iSubCount = pKData->m_mapKData.size() * 240;
					std::map<string, CKDataEx*>::const_iterator citKData = pKData->m_mapKData.begin();
					int iSubStep = 0;
					CString strCode;
					while (citKData != pKData->m_mapKData.end())
					{
						CKDataEx* pData = citKData->second;

						std::vector<CKStrdata>::const_iterator citMinData = pData->m_kMin1.begin();

						strCode = citKData->first.c_str();


						CTime tmKDay;
						bool bCloseAll = false;


#ifdef _DEBUG

						if(strDate.Compare("20160219") == 0 )
						{
							//pStrategy->CloseAll(oTick.szInstrument, oTick.lastprice);
							int xx = 0;
							xx++;
							printf("%d",xx);

						}
#endif

						//if (pStrategyManager->m_oKManager.GetLastKDataDate(strCode, tmKDay) && 
						//	tmKDay < tmKData) //日线数据不足, 后面的数据不再回测
						//{
						//	if (TRADE_STATUS_INIT == pStrategy->GetSignalStatus(strCode))//已没有持仓
						//	{
						//		citKData++;
						//		continue;
						//	}
						//	else
						//	{
						//		bCloseAll = true;//wait to close...
						//	}
						//	
						//}

						pStrategy->SetParam("交易合约", strCode);


						//sprintf_s(msg,256,"合约:%s,K线个数:%d...",strCode.GetBuffer(0),pData->m_kMin1.size());
						//pStrategy->AddLog(msg);

						while (citMinData != pData->m_kMin1.cend())
						{

							if (0 == iSubStep % 20)//every 20 kdatas notifying one event to outer app in order to save time
							{
								St_BackTest_Step * pStep = new St_BackTest_Step();

								pStep->m_sCode = strCode;
								pStep->m_iTotalCount = iTotalCount;
								pStep->m_iTotalStep = iTotalStep;
								pStep->m_iSubCount = iSubCount;
								pStep->m_iSubStep = iSubStep;

								::PostMessage(pStrategyManager->m_hMsgWnd, WM_MESSAGE_BACKTEST_STEP, 0, (LPARAM)pStep);

							}
							CTickData oTick;

							strcpy(oTick.szInstrument, strCode.GetBuffer());
							strCode.ReleaseBuffer();
							char szTime[32] = {0};
							strcpy(szTime, citMinData->curDate);
							strcat(szTime, " ");
							strcat(szTime, citMinData->curTime);
							strcpy(oTick.szTime, szTime);
							strcpy(oTick.szDate,citMinData->curDate);

							oTick.lastprice = citMinData->priceS;
							oTick.highestprice = citMinData->priceH;

							strcpy(pStrategy->m_szTickTime, szTime);

							//昨收盘价
							CKStrdata oKData;
							if (pStrategyManager->m_oKManager.getNDayKData(oTick.szInstrument, citMinData->curDate, oKData, -1))
							{
								oTick.preClosePrice = oKData.priceS;
							}

							int iYear, iMonth, iDay, iHour, iMin;
							char szYear[5]= {0};
							char szMonth[3] = {0};
							char szDay[3] = {0};
							char szHour[3] = {0}; 
							char szMin[3] = {0};
							strncpy(szYear, citMinData->curDate, 4);
							iYear = atoi(szYear);
							strncpy(szMonth, citMinData->curDate + 4, 2);
							iMonth = atoi(szMonth);
							strncpy(szDay, citMinData->curDate + 6, 2);
							iDay = atoi(szDay);

							if(strlen(citMinData->curTime)==4)
							{
								strncpy(szHour, citMinData->curTime, 2);
								iHour = atoi(szHour);
								strncpy(szMin, citMinData->curTime + 2, 2);
								iMin = atoi(szMin);
							}
							else if(strlen(citMinData->curTime)==3)
							{
								strncpy(szHour, citMinData->curTime, 1);
								iHour = atoi(szHour);
								strncpy(szMin, citMinData->curTime + 1, 2);
								iMin = atoi(szMin);
							}
							CTime tmTick(iYear, iMonth, iDay, iHour, iMin, 0);



							if (tmTick >= tmBegin && tmTick <= tmEnd)
							{
								pStrategy->TickPeek(&oTick);

							}

							citMinData++;


							if (pData->m_kMin1.cend() == citMinData )
							{
								if (pData->IsLastKMin())
								{
									pStrategy->CloseAll(oTick.szInstrument, oTick.lastprice);
								}

								//CKStrdata oKData;
								//if (pStrategyManager->m_oKManager.getNDayKData(oTick.szInstrument, oTick.szDate, oKData, 1))
								//{
								//	CTime tmNextTrade;
								//	if (0 == CFunctionPub::String2Time(oKData.curDate, tmNextTrade))
								//	{
								//		//last tick, close all postions
								//		if (pStrategyManager->m_oKManagerEx.IsLastKData(oTick.szInstrument, oTick.szDate)
								//			|| tmNextTrade > tmEnd)
								//		{
								//			pStrategy->CloseAll(oTick.szInstrument, oTick.lastprice);
								//		}
								//		
								//	}
								//}
								//else
								//{
								//	if (bCloseAll)
								//	{
								//		pStrategy->CloseAll(oTick.szInstrument, oTick.lastprice);
								//	}

								//}




							}


							iSubStep++;


						}


						citKData++;
					}

					St_BackTest_Step * pStep = new St_BackTest_Step();
					pStep->m_sCode = strCode;
					pStep->m_iTotalCount = iTotalCount;
					pStep->m_iTotalStep = iTotalStep;
					pStep->m_iSubCount = iSubCount;
					pStep->m_iSubStep = iSubCount;

					::PostMessage(pStrategyManager->m_hMsgWnd, WM_MESSAGE_BACKTEST_STEP, 0, (LPARAM)pStep);


					pStrategyManager->m_oKManagerEx.Recycle(pKData);

					tmKData += CTimeSpan(1,0,0,0);
					iTotalStep++;
				}

				Sleep(50); //wait kmin data producing


				
				CStockEquity oEquity = pStrategy->m_pPolicy->GetEveryDayEquity(pStrategy, strDate);
				pStrategy->m_pStatistic->SetEveryDayEquity(strDate, oEquity);
			}

			if (arCodes.GetCount() > 0)
			{
				St_BackTest_Step * pStep = new St_BackTest_Step();
				CString strCode = arCodes.GetAt(arCodes.GetCount() - 1);
				if (strCode.Find("#") >= 0 && strCode.GetLength() > 6)
				{
					strCode = strCode.Right(6);
				}
				pStep->m_sCode = strCode;
				pStep->m_iTotalCount = 0xffff;
				pStep->m_iTotalStep = 0xffff;
				pStep->m_iSubCount = 0xffff;
				pStep->m_iSubStep = 0xffff;

				::PostMessage(pStrategyManager->m_hMsgWnd, WM_MESSAGE_BACKTEST_STEP, 0, (LPARAM)pStep);
			}

			pStrategy->m_pStatistic->Statistic("", true);

			for (int i = 0; i < arCodes.GetCount(); i++)
			{
				CString strCode = arCodes.GetAt(i);
				pStrategyManager->m_oKManager.UnLoadData(strCode);
			}

		}
	}
}
void CStrategyManager::OnTest1Day(CStrategyBase* pStrategy)
{
	if (pStrategy)
	{
		CTime tmTestBegin;
		tmTestBegin = CTime::GetCurrentTime();
		pStrategy->m_pStatistic->SetStartTime(tmTestBegin);

		CStringArray &arCodes = m_arTestCodes;
		CTime tmBegin(m_lTestTimeBegin);
		CTime tmEnd(m_lTestTimeEnd);

		CTimeSpan tm_spCount = tmEnd - tmBegin;
		int iTotalCount = tm_spCount.GetDays();
		int iTotalStep = 0;
		CString strBeginTime = tmBegin.Format("%Y%m%d");
		CString strEndTime = tmEnd.Format("%Y%m%d");
		

		CTime tmKData = tmBegin;
		while (tmKData < tmEnd)
		{
			CString strCurDate = tmKData.Format("%Y%m%d");
			int iSubCount = arCodes.GetSize() * 240;
			
			int iSubStep = 0;
			for (int i = 0; i < arCodes.GetSize(); i++)
			{
				CString strCode = arCodes.GetAt(i);
				if (strCode.Find("#") > 0)
				{
					strCode = strCode.Right(6);
				}
				pStrategy->SetParam("交易合约", strCode);
				CKStrdata oKData;
				if (m_oKManager.getDayKData(strCode, strCurDate, oKData))
				{
					if (0 == iSubStep % 20)//every 20 kdatas notifying one event to outer app in order to save time
					{
						St_BackTest_Step * pStep = new St_BackTest_Step();

						pStep->m_sCode = strCode;
						pStep->m_iTotalCount = iTotalCount;
						pStep->m_iTotalStep = iTotalStep;
						pStep->m_iSubCount = iSubCount;
						pStep->m_iSubStep = iSubStep;

						::PostMessage(m_hMsgWnd, WM_MESSAGE_BACKTEST_STEP, 0, (LPARAM)pStep);

					}
					
					int iYear, iMonth, iDay;
					char szYear[5]= {0};
					char szMonth[3] = {0};
					char szDay[3] = {0};

					strncpy(szYear, oKData.curDate, 4);
					iYear = atoi(szYear);
					strncpy(szMonth, oKData.curDate + 4, 2);
					iMonth = atoi(szMonth);
					strncpy(szDay, oKData.curDate + 6, 2);
					iDay = atoi(szDay);

					strcpy(pStrategy->m_szTickTime, oKData.curDate);
					CTime tmTick(iYear, iMonth, iDay, 0, 0, 0);


					if (tmTick >= tmBegin && tmTick <= tmEnd)
					{
						pStrategy->BackTest(&oKData, strCode);

					}
				}
				iSubStep++;
			}
			St_BackTest_Step * pStep = new St_BackTest_Step();
			pStep->m_sCode = "";
			pStep->m_iTotalCount = iTotalCount;
			pStep->m_iTotalStep = iTotalStep;
			pStep->m_iSubCount = iSubCount;
			pStep->m_iSubStep = iSubCount;

			::PostMessage(m_hMsgWnd, WM_MESSAGE_BACKTEST_STEP, 0, (LPARAM)pStep);

			CStockEquity oEquity = pStrategy->m_pPolicy->GetEveryDayEquity(pStrategy, strCurDate);
			pStrategy->m_pStatistic->SetEveryDayEquity(strCurDate, oEquity);

			tmKData += CTimeSpan(1,0,0,0);
			iTotalStep++;
		}

		if (arCodes.GetCount() > 0)
		{
			St_BackTest_Step * pStep = new St_BackTest_Step();
			CString strCode = arCodes.GetAt(arCodes.GetCount() - 1);
			if (strCode.Find("#") >= 0 && strCode.GetLength() > 6)
			{
				strCode = strCode.Right(6);
			}
			pStep->m_sCode = strCode;
			pStep->m_iTotalCount = 0xffff;
			pStep->m_iTotalStep = 0xffff;
			pStep->m_iSubCount = 0xffff;
			pStep->m_iSubStep = 0xffff;

			::PostMessage(m_hMsgWnd, WM_MESSAGE_BACKTEST_STEP, 0, (LPARAM)pStep);
		}
		pStrategy->m_pStatistic->Statistic("", true);

		for (int i = 0; i < arCodes.GetCount(); i++)
		{
			CString strCode = arCodes.GetAt(i);
			m_oKManager.UnLoadData(strCode);
		}

	}
}
void CStrategyManager::ProcessBackTest(void * lpData)
{
	CStrategyBase* pStrategy = (CStrategyBase*) lpData;
	if (pStrategy)
	{
		CStrategyManager* pStrategyManager = pStrategy->GetStrategyManager();
		if (pStrategyManager)
		{
			switch (pStrategyManager->m_eBackTestMode)
			{
			case E_K_1_MIN_TEST:
				pStrategyManager->OnTest1Min(pStrategy);
				break;
			case E_K_DAY_TEST:
				pStrategyManager->OnTest1Day(pStrategy);
				break;
			default:
				break;
			}
		}
		
	}
	
	
}

#if 0
void CStrategyManager::ProcessBackTest(void * lpData)
{
	CStrategyBase* pStrategy = (CStrategyBase*) lpData;
	if (pStrategy)
	{
		CStrategyManager* pStrategyManager = pStrategy->GetStrategyManager();
		if (pStrategyManager)
		{
			CStringArray &arCodes = pStrategy->GetStrategyManager()->m_arTestCodes;
			
			for(int i = 0; i < arCodes.GetCount(); i++)
			{
				
				CString strCode = arCodes.GetAt(i);

				if (strCode.Find("#") >= 0 && strCode.GetLength() > 6)
				{
					strCode = strCode.Right(6);
				}

				pStrategy->SetParam("交易合约", strCode);
				pStrategyManager->ReadHistoryData(pStrategy, strCode,
					pStrategyManager->m_lTestTimeBegin, 
					pStrategyManager->m_lTestTimeEnd, arCodes.GetCount(), i);

			}
			if (arCodes.GetCount() > 0)
			{
				St_BackTest_Step * pStep = new St_BackTest_Step();
				CString strCode = arCodes.GetAt(arCodes.GetCount() - 1);
				if (strCode.Find("#") >= 0 && strCode.GetLength() > 6)
				{
					strCode = strCode.Right(6);
				}
				pStep->m_sCode = strCode;
				pStep->m_iTotalCount = 0xffff;
				pStep->m_iTotalStep = 0xffff;
				pStep->m_iSubCount = 0xffff;
				pStep->m_iSubStep = 0xffff;

				::PostMessage(pStrategyManager->m_hMsgWnd, WM_MESSAGE_BACKTEST_STEP, 0, (LPARAM)pStep);
			}
			
			pStrategy->m_pStatistic->Statistic("", true);
		}
		
	
	}
	
}
#endif

void CStrategyManager::StragegyBackTest(LPCTSTR sName, LPCTSTR sParamList, LONG lTimeBegin, LONG lTimeEnd, LPCTSTR sCodeRanger, LPCTSTR sCycle)
{
	CString strlog;
	if (m_thdBackTest.IsRunning())
	{
		strlog.Format("当前回测任务没有结束");
		WRITE_LOG(1,strlog.GetBuffer(0));
		return;

	}
	
	
	
	CStrategyBase* pStrategy = GetStrategyInstance(sName);
	if (!pStrategy)
	{
		strlog.Format("策略初始化失败,%s",sName);
		WRITE_LOG(1,strlog.GetBuffer(0));
		return;
	}
	pStrategy->SetStrategyManager(this);
	pStrategy->m_pStrategyCodeInfo = m_pGetCodeInfo;
	pStrategy->m_pKManager = &m_oKManager;
	pStrategy->SetWndHwnd(m_hMsgWnd);
	pStrategy->m_bBackTest = true;



	pStrategy->SetParam(sParamList);

	CFunctionPub pub;

	if (m_arTestCodes.GetCount())
	{
		m_arTestCodes.RemoveAll();
	}
	pub.ParseParam(sCodeRanger,m_arTestCodes,'$');
	m_lTestTimeBegin = lTimeBegin;
	m_lTestTimeEnd = lTimeEnd;

	CString strCycle(sCycle);
	if (0 == strCycle.CompareNoCase("1分钟"))
	{
		m_eBackTestMode = E_K_1_MIN_TEST;
	}
	else if (0 == strCycle.CompareNoCase("5分钟"))
	{
		m_eBackTestMode = E_K_5_MIN_TEST;
	}
	else if (0 == strCycle.CompareNoCase("15分钟"))
	{
		m_eBackTestMode = E_K_15_MIN_TEST;
	}
	else if (0 == strCycle.CompareNoCase("1小时"))
	{
		m_eBackTestMode = E_K_1_HOUR_TEST;
	}
	else if (0 == strCycle.CompareNoCase("1日"))
	{
		m_eBackTestMode = E_K_DAY_TEST;
	}


	m_thdBackTest.Start(ProcessBackTest, 0, pStrategy);

	

}

int CStrategyManager::ReadHistoryData(CStrategyBase* pStrategy, CString strCode, long lTimeBegin, long lTimeEnd, int iTotalCount, int iTotalStep)
{

	char szCode[7] = {0};
	strcpy(szCode, strCode.GetBuffer());
	strCode.ReleaseBuffer();

	std::vector<CKStrdata>* pvKData = NULL;

	CTime tmBegin(lTimeBegin);
	CTime tmEnd(lTimeEnd);
	

	if (m_oKManager.getMinKData(szCode, &pvKData))
	{
		int iSubCount = pvKData->size();
		int iSubStep = 0;
		std::vector<CKStrdata>::const_iterator itK = pvKData->cbegin();
		
		while (itK != pvKData->cend())
		{
			iSubStep++;
			if (0 == iSubStep % 20)//every 20 kdatas notifying one event to outer app in order to save time
			{
				St_BackTest_Step * pStep = new St_BackTest_Step();
				pStep->m_sCode = strCode;
				pStep->m_iTotalCount = iTotalCount;
				pStep->m_iTotalStep = iTotalStep;
				pStep->m_iSubCount = iSubCount;
				pStep->m_iSubStep = iSubStep;

				::PostMessage(m_hMsgWnd, WM_MESSAGE_BACKTEST_STEP, 0, (LPARAM)pStep);

			}
			CTickData oTick;

			//strcpy(oTick.szInstrument, itK->code);
			char szTime[32] = {0};
			strcpy(szTime, itK->curDate);
			strcat(szTime, " ");
			strcat(szTime, itK->curTime);
			strcpy(oTick.szTime, szTime);
			strcpy(oTick.szDate,itK->curDate);

			oTick.lastprice = itK->priceS;
			oTick.highestprice = itK->priceH;

			strcpy(pStrategy->m_szTickTime, szTime);

			//昨收盘价
			CKStrdata oKData;
			if (m_oKManager.getNDayKData(oTick.szInstrument, itK->curDate, oKData, -1))
			{
				oTick.preClosePrice = oKData.priceS;
			}

			int iYear, iMonth, iDay, iHour, iMin;
			char szYear[5]= {0};
			char szMonth[3] = {0};
			char szDay[3] = {0};
			char szHour[3] = {0}; 
			char szMin[3] = {0};
			strncpy(szYear, itK->curDate, 4);
			iYear = atoi(szYear);
			strncpy(szMonth, itK->curDate + 4, 2);
			iMonth = atoi(szMonth);
			strncpy(szDay, itK->curDate + 6, 2);
			iDay = atoi(szDay);

			if(strlen(itK->curTime)==4)
			{
				strncpy(szHour, itK->curTime, 2);
				iHour = atoi(szHour);
				strncpy(szMin, itK->curTime + 2, 2);
				iMin = atoi(szMin);
			}
			else if(strlen(itK->curTime)==3)
			{
				strncpy(szHour, itK->curTime, 1);
				iHour = atoi(szHour);
				strncpy(szMin, itK->curTime + 1, 2);
				iMin = atoi(szMin);
			}
			CTime tmTick(iYear, iMonth, iDay, iHour, iMin, 0);
			if (tmTick >= tmBegin && tmTick <= tmEnd)
			{
				pStrategy->TickPeek(&oTick);

			}

			itK++;

			if (pvKData->cend() == itK)
			{
				//last tick, close all postions
				pStrategy->CloseAll(oTick.szInstrument, oTick.lastprice);
			}
		}

		St_BackTest_Step * pStep = new St_BackTest_Step();
		pStep->m_sCode = strCode;
		pStep->m_iTotalCount = iTotalCount;
		pStep->m_iTotalStep = iTotalStep;
		pStep->m_iSubCount = iSubCount;
		pStep->m_iSubStep = iSubCount;

		::PostMessage(m_hMsgWnd, WM_MESSAGE_BACKTEST_STEP, 0, (LPARAM)pStep);
	}

	m_oKManager.UnLoadData(szCode);


	return 0;
}
void CStrategyManager::UnLoad()
{
	/*
	std::vector<HINSTANCE>::const_iterator itDll = m_vDll.cbegin();
	while (itDll != m_vDll.cend())
	{
		HINSTANCE dll = m_vDll.pop_back();

		if(dll)
		{
			::FreeLibrary(m_vDll.pop_back());
		}
	}
	*/
}

void CStrategyManager::Task_Aux(void *lpData)
{
	CStrategyManager *pManager = (CStrategyManager*)lpData;
	char szMsg[128];
	for (;;)
	{
		
		if (pManager)
		{

#ifdef _DEBUG
			strcpy(szMsg, "REQ#queryarbit");

			//::SendMessage(pManager->m_hMsgWnd, WM_MESSAGE_SUBCODE, 0, (LPARAM)szMsg);
			//break;
#endif	

			pManager->m_mtxModelRemove.Lock();
			if (pManager->m_arModelRemove.GetCount() > 0)
			{
				CStrategyBase *pt = (CStrategyBase*)pManager->m_arModelRemove[0];
				pManager->m_arModelRemove.RemoveAt(0);

				pt->m_Exit = true;

				pt->m_bRun = false;

				pt->JoinAllTask();

				delete pt;

				pt = NULL;
			}
			
			pManager->m_mtxModelRemove.Unlock();

			CTime tmNow = CTime::GetCurrentTime();
			pManager->m_mtxSubCheck.Lock();
			if (9 == tmNow.GetHour() && 1 == tmNow.GetMinute())
			{
				
				pManager->m_bSubcodeEveryday = false;
				strcpy(szMsg, "重置订阅行情标记");

				::SendMessage(pManager->m_hMsgWnd,WM_MESSAGE_TRADE,0,(long)szMsg);
			}
			pManager->m_mtxSubCheck.Unlock();


			if (tmNow.GetHour() == 8 && tmNow.GetMinute() <= 59 && !pManager->m_bSubcodeEveryday)
			{
				if (!pManager->m_pthdSubcode)
				{
					pManager->m_pthdSubcode = new CEasyThread();
					
				}
				else
				{
					delete pManager->m_pthdSubcode;
					pManager->m_pthdSubcode = new CEasyThread();
				}
				if (!pManager->m_pthdSubcode->IsRunning())
				{
					pManager->m_pthdSubcode->Start(Task_Subcode, 0, pManager);
					strcpy(szMsg, "开始每日行情订阅");
					::SendMessage(pManager->m_hMsgWnd,WM_MESSAGE_TRADE,0,(long)szMsg);
					Sleep(2000 * 60);

					

					
				}

			}

		}

		Sleep(1000);
	}
}

void CStrategyManager::Task_Subcode(void *lpData)
{
	CStrategyManager *pManager = (CStrategyManager*)lpData;
	if (pManager)
	{
		pManager->m_mtxSubCheck.Lock();
		pManager->m_bSubcodeEveryday = true;
		pManager->m_mtxSubCheck.Unlock();

		for(int i=0; i < pManager->m_arModelRun.GetSize(); i++)
		{
			CStrategyBase * pt = (CStrategyBase *)pManager->m_arModelRun[i];

			if(pt->m_bRun==false)
				continue;

			pt->StartSubcode();
		}

	}
}


CString CStrategyManager::GetStrategyContext(CString sStrategyId)
{
	//param format: "StrategyName#Params#PostionInfo"
	//PostionInfo format: "code1;name;postion;price;dir;time$code2;name;postion;price;dir;time"
	CString strContext = "";
	
	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_arModelRun[i];
		if (0 == pt->GetStrategyId().Compare(sStrategyId))
		{
			strContext = sStrategyId;
			strContext += "#";
			strContext += pt->GetParamValue();
			strContext += "#";
			strContext += pt->GetPostionInfo();
		}

	}

	return strContext;
}

void CStrategyManager::SetStrategyContext(CString strStrategyId, CString strContext)
{
	for(int i=0;i<m_arModelRun.GetSize();i++)
	{
		CStrategyBase * pt = (CStrategyBase *)m_arModelRun[i];
		if (0 == pt->GetStrategyId().Compare(strStrategyId))
		{
			pt->ResumePostionInfo(strContext);
		}

	}
}