#include "MarketDataCenter_impl.h"
#include "MarketDataSpi.h"

CHundsunMdCenterApi *CHundsunMdCenterApi::CreateHundsunMdCenterApi(const char *pszFlowPath)
{
    return new CHundsunMdCenterApi_impl(pszFlowPath);
}

CHundsunMdCenterApi_impl::CHundsunMdCenterApi_impl(const char *pszFlowPath)
{
    lpMdSpi = NULL;
	m_StockMDType = 0;
	m_FutuMDType = 0;

	memset(szDBFPath, 0, sizeof(szDBFPath));
	memset(szH5Path_LicFile, 0, sizeof(szH5Path_LicFile));
	memset(szH5Path_CfgFile, 0, sizeof(szH5Path_CfgFile));
	bH5_SubLevel2 = false;
	memset(szFutu_LicFile, 0, sizeof(szFutu_LicFile));
	memset(szFutu_Pwd, 0, sizeof(szFutu_Pwd));
	memset(szFutu_SslFile, 0, sizeof(szFutu_SslFile));
	memset(szFutu_SslPwd, 0, sizeof(szFutu_SslPwd));
	bFutu_Connected = false;
	bFutu_Logined = false;
	memset(szFrontAddress, 0, sizeof(szFrontAddress));

	//DBF
	lp_DBF_Impl = new CDBF_Impl();
	//H5sdk
	lp_H5sdk_Impl = CHundsunH5sdkApi::CreateHundsunH5sdkApi();
	//Futu
	lpFutuMD = CHundsunFutuMdApi::CreateHundsunFutuMdApi();
	//回调对象
	lpMarketDataSpi = new CMarketDataSpi(this);

	lp_DBF_Impl->RegisterDFBSpi(lpMarketDataSpi);
	m_H5_Impl.GetInstance()->RegisterH5Spi(lpMarketDataSpi);
	lp_H5sdk_Impl->RegisterSpi(lpMarketDataSpi);
	lpFutuMD->RegisterSpi(lpMarketDataSpi);

    m_localip = "";
    m_localMac = "";
    m_bHeartKeepStop = false;
}

CHundsunMdCenterApi_impl::~CHundsunMdCenterApi_impl()
{
    m_bHeartKeepStop = true;
    if(m_HeartKeepTrd.IsRunning())
    {
        m_HeartKeepTrd.Join();
    }
    //StopTrd();

    hJoinEvent.Notify();

	m_H5_Impl.ReleaseH5();
}

void CHundsunMdCenterApi_impl::ReleaseApi()
{
	lpMdSpi = NULL;
    delete this;
}

void CHundsunMdCenterApi_impl::Init(THundsunMarkerDataType MDType)
{
	switch (MDType)
	{
	case HUNDSUN_FTDC_DBF:
		{
			m_StockMDType = MDType;
			if(strcmp(szDBFPath, "") == 0)
				return;

			if(lp_DBF_Impl->InitDBF(szDBFPath) == 0)
			{

			}
		}
		break;
	case HUNDSUN_FTDC_H5:
		{
			m_StockMDType = MDType;
			if(strcmp(szH5Path_LicFile, "") == 0 || strcmp(szH5Path_CfgFile, "") == 0)
				return;

			if(m_H5_Impl.GetInstance()->InitH5(szH5Path_LicFile, szH5Path_CfgFile, bH5_SubLevel2) == 0)
			{

			}
		}
		break;
	case HUNDSUN_FTDC_H5SDK:
		{
			m_StockMDType = MDType;

		}
		break;
	case HUNDSUN_FTDC_Futu:
		{
			m_FutuMDType = MDType;
			if(strcmp(szFutu_LicFile, "") == 0 || strcmp(szFrontAddress, "") == 0)
				return;

			lpFutuMD->RegisterFront(szFrontAddress);
			lpFutuMD->Init(szFutu_LicFile, szFutu_Pwd, szFutu_SslFile, szFutu_SslPwd);
		}
		break;
	default:
		break;
	}
	
    return;
}

int CHundsunMdCenterApi_impl::Join()
{
    hJoinEvent.Wait(-1);
    return 0;
}

void CHundsunMdCenterApi_impl::RegisterDBFPath(const char *pszFilePath)
{
	if(!(pszFilePath))
		return;

	hs_strncpy(szDBFPath, pszFilePath, sizeof(szDBFPath));
}

void CHundsunMdCenterApi_impl::RegisterH5(const char *pszLicFile, const char *pszCfgFile, bool bSubLevel2)
{
	if(!(pszLicFile && pszCfgFile))
		return;

	hs_strncpy(szH5Path_LicFile, pszLicFile, sizeof(szH5Path_LicFile));
	hs_strncpy(szH5Path_CfgFile, pszCfgFile, sizeof(szH5Path_CfgFile));
	bH5_SubLevel2 = bSubLevel2;
}

void CHundsunMdCenterApi_impl::RegisterFutu(const char *pszLicFile, const char *pszPwd, const char *pszSslFile, const char *pszSslPwd)
{
	if(!pszLicFile)
		return;

	hs_strncpy(szFutu_LicFile, pszLicFile, sizeof(szFutu_LicFile));
	hs_strncpy(szFutu_Pwd, pszPwd, sizeof(szFutu_Pwd));
	hs_strncpy(szFutu_SslFile, pszSslFile, sizeof(szFutu_SslFile));
	hs_strncpy(szFutu_SslPwd, pszSslPwd, sizeof(szFutu_SslPwd));
}

void CHundsunMdCenterApi_impl::RegisterFront(const char *pszFrontAddress)
{
	if(!szFrontAddress)
		return;

	hs_strncpy(szFrontAddress, pszFrontAddress, sizeof(szFrontAddress));

    return;
}

void CHundsunMdCenterApi_impl::RegisterSpi(CHundsunMdCenterSpi *pSpi)
{
    lpMdSpi = pSpi;
    if(pSpi)
    {
        //StartTrd();
    }
    else
    {
        //StopTrd();
    }
}

int CHundsunMdCenterApi_impl::ReqUserLogin(CHundsunFtdcReqUserLoginField *pReqUserLoginField, int nRequestID)
{
	switch (pReqUserLoginField->MarkerDataType)
	{
	case HUNDSUN_FTDC_Futu:
		{
			if(!bFutu_Connected)
			{
				CHundsunFtdcRspInfoField m_RspInfo;
				m_RspInfo.ErrorID = -1;
				hs_strncpy(m_RspInfo.ErrorMsg, "未连上服务器...", sizeof(CHundsunFtdcRspInfoField));

				CHundsunFtdcRspUserLoginField m_RspUserLogin;
				memset(&m_RspUserLogin, 0, sizeof(CHundsunFtdcRspUserLoginField));
				hs_strncpy(m_RspUserLogin.FundAccount, pReqUserLoginField->FundAccount, sizeof(m_RspUserLogin.FundAccount)); 
				m_RspUserLogin.MarkerDataType = pReqUserLoginField->MarkerDataType;

				if(lpMdSpi)
				{
					lpMdSpi->OnRspUserLogin(&m_RspUserLogin, &m_RspInfo, nRequestID, true);
				}

				return -1;
			}

			CHundsunFutuReqFuncClientLoginField m_FuncClientLogin;
			memset(&m_FuncClientLogin, 0, sizeof(CHundsunFutuReqFuncClientLoginField));
			m_FuncClientLogin.OpEntrustWay = '7';
			m_FuncClientLogin.InputContent = '1';
			hs_strncpy(m_FuncClientLogin.AccountContent, pReqUserLoginField->FundAccount, sizeof(m_FuncClientLogin.AccountContent));
			m_FuncClientLogin.PasswordType = '2';
			hs_strncpy(m_FuncClientLogin.Password, pReqUserLoginField->Password, sizeof(m_FuncClientLogin.Password));

			return lpFutuMD->ReqFuncClientLogin(&m_FuncClientLogin, nRequestID);
		}
	default:
		break;
	}

	return 0;
}

int CHundsunMdCenterApi_impl::ReqQryInstrument(CHundsunFtdcQryInstrumentField *pQryInstrument, int nRequestID)
{
	switch (pQryInstrument->ExchangeID)
	{
	case HUNDSUN_FTDC_SH:
	case HUNDSUN_FTDC_SZ:
	case HUNDSUN_FTDC_GQ_SH:
	case HUNDSUN_FTDC_GQ_SZ:
		{
			switch (m_StockMDType)
			{
			case HUNDSUN_FTDC_DBF:
				//lp_DBF_Impl->SubscribeDBF(ExchangeID, ppInstrumentID, nCount);
				break;
			case HUNDSUN_FTDC_H5:
				m_H5_Impl.GetInstance()->ReqQryInstrumentH5(pQryInstrument, nRequestID);
				break;
			default:
				break;
			}
		}
		break;
	case HUNDSUN_FTDC_SHFE:
	case HUNDSUN_FTDC_CZCE:
	case HUNDSUN_FTDC_DCE:
	case HUNDSUN_FTDC_CFFEX:
		{
			if(!bFutu_Logined)
			{
				CHundsunFtdcRspInfoField m_RspInfo;
				m_RspInfo.ErrorID = -1;
				hs_strncpy(m_RspInfo.ErrorMsg, "期货未登录...", sizeof(CHundsunFtdcRspInfoField));

				if(lpMdSpi)
				{
					lpMdSpi->OnRspQryInstrument(NULL, &m_RspInfo, nRequestID, true);
				}

				return -1;
			}

			CHundsunFutuReqFuncFutuContractbailQryField m_req;
			memset(&m_req, 0, sizeof(CHundsunFutuReqFuncFutuContractbailQryField));
			switch (pQryInstrument->ExchangeID)
			{
			case HUNDSUN_FTDC_SHFE:
				hs_strncpy(m_req.FutuExchType, "F3", sizeof(m_req.FutuExchType));
				break;
			case HUNDSUN_FTDC_CZCE:
				hs_strncpy(m_req.FutuExchType, "F1", sizeof(m_req.FutuExchType));
				break;
			case HUNDSUN_FTDC_DCE:
				hs_strncpy(m_req.FutuExchType, "F2", sizeof(m_req.FutuExchType));
				break;
			case HUNDSUN_FTDC_CFFEX: 
				hs_strncpy(m_req.FutuExchType, "F4", sizeof(m_req.FutuExchType));
				break;
			default:
				break;
			}
			return lpFutuMD->ReqFuncFutuContractbailQry(&m_req, nRequestID);
		}
		break;
	default:
		break;
	}

	return 0;
}

int CHundsunMdCenterApi_impl::SubscribeMarketData(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount)
{
	switch (ExchangeID)
	{
	case HUNDSUN_FTDC_SH:
	case HUNDSUN_FTDC_SZ:
	case HUNDSUN_FTDC_GQ_SH:
	case HUNDSUN_FTDC_GQ_SZ:
		{
			switch (m_StockMDType)
			{
			case HUNDSUN_FTDC_DBF:
				lp_DBF_Impl->SubscribeDBF(ExchangeID, ppInstrumentID, nCount);
				break;
			case HUNDSUN_FTDC_H5:
				m_H5_Impl.GetInstance()->SubscribeH5(ExchangeID, ppInstrumentID, nCount);
				break;
			default:
				break;
			}
		}
		break;
	case HUNDSUN_FTDC_SHFE:
	case HUNDSUN_FTDC_CZCE:
	case HUNDSUN_FTDC_DCE:
	case HUNDSUN_FTDC_CFFEX:
		{
			switch (m_FutuMDType)
			{
			case HUNDSUN_FTDC_Futu:
				lpFutuMD->ReqFuncQhSingleHqSubscribe(ppInstrumentID, nCount);
				break;;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

int CHundsunMdCenterApi_impl::UnSubscribeMarketData(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount)
{
	switch (ExchangeID)
	{
	case HUNDSUN_FTDC_SH:
	case HUNDSUN_FTDC_SZ:
	case HUNDSUN_FTDC_GQ_SH:
	case HUNDSUN_FTDC_GQ_SZ:
		{
			switch (m_StockMDType)
			{
			case HUNDSUN_FTDC_DBF:
				lp_DBF_Impl->UnSubscribeDBF(ExchangeID, ppInstrumentID, nCount);
				break;
			case HUNDSUN_FTDC_H5:
				m_H5_Impl.GetInstance()->UnSubscribeH5(ExchangeID, ppInstrumentID, nCount);
				break;
			default:
				break;
			}
		}
		break;
	case HUNDSUN_FTDC_SHFE:
	case HUNDSUN_FTDC_CZCE:
	case HUNDSUN_FTDC_DCE:
	case HUNDSUN_FTDC_CFFEX:
		{
			switch (m_FutuMDType)
			{
			case HUNDSUN_FTDC_Futu:
				lpFutuMD->ReqFuncQhSingleHqCancel(ppInstrumentID, nCount);
				break;;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}

	return 0;
}

CHundsunMdCenterSpi* CHundsunMdCenterApi_impl::GetMdCenterSpi()
{
	return lpMdSpi;
}

void CHundsunMdCenterApi_impl::SetFutuConnected(bool bvl)
{
	bFutu_Connected = bvl;
}

void CHundsunMdCenterApi_impl::SetFutuLogined(bool bvl)
{
	bFutu_Logined = bvl;
}
