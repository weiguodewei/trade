#include "MarketDataSpi.h"

void CMarketDataSpi::OnDBFConnected()
{
	if(lpApi)
	{
		if(lpApi->GetMdCenterSpi())
			lpApi->GetMdCenterSpi()->OnFrontConnected();
	}
}

void CMarketDataSpi::OnDBFRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(lpApi && lpApi->GetMdCenterSpi())
	{
		lpApi->GetMdCenterSpi()->OnRspSubMarketData(pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
}

void CMarketDataSpi::OnDBFRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData)
{
	if(lpApi && lpApi->GetMdCenterSpi())
	{
		lpApi->GetMdCenterSpi()->OnRtnDepthMarketData(pDepthMarketData);
	}
}

void CMarketDataSpi::OnH5Connected(int iMktType)
{
	if(lpApi)
	{
		if(lpApi->GetMdCenterSpi())
			lpApi->GetMdCenterSpi()->OnFrontConnected();
	}
}

void CMarketDataSpi::OnH5RspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(lpApi && lpApi->GetMdCenterSpi())
	{
		lpApi->GetMdCenterSpi()->OnRspQryInstrument(pInstrument, pRspInfo, nRequestID, bIsLast);
	}
}

void CMarketDataSpi::OnH5RspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(lpApi && lpApi->GetMdCenterSpi())
	{
		lpApi->GetMdCenterSpi()->OnRspSubMarketData(pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}
}


void CMarketDataSpi::OnH5RspUnSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	if(lpApi && lpApi->GetMdCenterSpi())
	{
		lpApi->GetMdCenterSpi()->OnRspUnSubMarketData(pSpecificInstrument, pRspInfo, nRequestID, bIsLast);
	}

}


void CMarketDataSpi::OnH5RtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData)
{
	if(lpApi && lpApi->GetMdCenterSpi())
	{
		lpApi->GetMdCenterSpi()->OnRtnDepthMarketData(pDepthMarketData);
	}
}

void CMarketDataSpi::OnH5sdkFrontConnected()
{
	if(lpApi)
	{
		if(lpApi->GetMdCenterSpi())
			lpApi->GetMdCenterSpi()->OnFrontConnected();
	}
}

void CMarketDataSpi::OnFrontConnected()
{
	if(lpApi && lpApi->GetMdCenterSpi())
	{
		lpApi->SetFutuConnected(true);
		lpApi->GetMdCenterSpi()->OnFrontConnected();
	}
}

void CMarketDataSpi::OnRspFuncClientLogin(CHundsunFutuRspFuncClientLoginField *pRspFuncClientLogin, CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(lpApi && lpApi->GetMdCenterSpi())
	{
		CHundsunFtdcRspInfoField m_RspInfo;
		memset(&m_RspInfo, 0, sizeof(CHundsunFtdcRspInfoField));
		m_RspInfo.ErrorID = pRspInfo->ErrorNo;
		hs_strncpy(m_RspInfo.ErrorMsg, pRspInfo->ErrorInfo, sizeof(m_RspInfo.ErrorMsg));

		CHundsunFtdcRspUserLoginField m_RspUserLogin;
		memset(&m_RspUserLogin, 0, sizeof(CHundsunFtdcRspUserLoginField));
		m_RspUserLogin.TradingDay = pRspFuncClientLogin->InitDate;
		hs_strncpy(m_RspUserLogin.FundAccount, pRspFuncClientLogin->FundAccount, sizeof(m_RspUserLogin.FundAccount)); 
		m_RspUserLogin.MarkerDataType = HUNDSUN_FTDC_Futu;
		m_RspUserLogin.SessionID = pRspFuncClientLogin->SessionNo;
		hs_strncpy(m_RspUserLogin.UserToken, pRspFuncClientLogin->UserToken, sizeof(m_RspUserLogin.UserToken));

		lpApi->SetFutuLogined(true);
		lpApi->GetMdCenterSpi()->OnRspUserLogin(&m_RspUserLogin, &m_RspInfo, nRequestID, bIsLast);
	}
}

void CMarketDataSpi::OnRspFuncFutuContractbailQry(CHundsunFutuRspFuncFutuContractbailQryField *pRspFuncFutuContractbailQry, CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(lpApi && lpApi->GetMdCenterSpi())
	{
		CHundsunFtdcRspInfoField m_RspInfo;
		memset(&m_RspInfo, 0, sizeof(CHundsunFtdcRspInfoField));

		if(pRspFuncFutuContractbailQry)
		{
			CHundsunFtdcInstrumentField m_Instrument;
			memset(&m_Instrument, 0, sizeof(CHundsunFtdcInstrumentField));
			if(pRspFuncFutuContractbailQry->FutuExchType == "F1")
				m_Instrument.ExchangeID = HUNDSUN_FTDC_CZCE;
			else if(pRspFuncFutuContractbailQry->FutuExchType == "F2")
				m_Instrument.ExchangeID = HUNDSUN_FTDC_DCE;
			else if(pRspFuncFutuContractbailQry->FutuExchType == "F3")
				m_Instrument.ExchangeID = HUNDSUN_FTDC_SHFE;
			else if(pRspFuncFutuContractbailQry->FutuExchType == "F4")
				m_Instrument.ExchangeID = HUNDSUN_FTDC_CFFEX;
			hs_strncpy(m_Instrument.Instrument, pRspFuncFutuContractbailQry->FutuCode, sizeof(m_Instrument.Instrument));
			hs_strncpy(m_Instrument.InstrumentName, pRspFuncFutuContractbailQry->FutuName, sizeof(m_Instrument.InstrumentName));

			lpApi->GetMdCenterSpi()->OnRspQryInstrument(&m_Instrument, &m_RspInfo, nRequestID, bIsLast);
		}
		else
			lpApi->GetMdCenterSpi()->OnRspQryInstrument(NULL, &m_RspInfo, nRequestID, bIsLast);
	}
}

void CMarketDataSpi::OnRspFuncQhSingleHqSubscribe(CHundsunFutuRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	CHundsunFtdcRspInfoField RspInfo;
	RspInfo.ErrorID = pRspInfo->ErrorNo;
	hs_strncpy(RspInfo.ErrorMsg, pRspInfo->ErrorInfo, sizeof(RspInfo.ErrorMsg));

	if(lpApi && lpApi->GetMdCenterSpi())
	{
		lpApi->GetMdCenterSpi()->OnRspSubMarketData(NULL, &RspInfo, nRequestID, bIsLast);
	}
}

void CMarketDataSpi::OnRtnFuncQhSingleHqPush(CHundsunFutuRtnFuncQhSingleHqPushField *pFuncQhSingleHqPush)
{
	if(!pFuncQhSingleHqPush)
		return;

	CHundsunDepthMarketDataField m_DepthMarketData = {};

	m_DepthMarketData.UpdateTime = 0;
	m_DepthMarketData.UpdateMillisec = 0;
	//hs_strncpy(m_DepthMarketData.ExchangeID, pFuncQhSingleHqPush->FutuExchType, sizeof(m_DepthMarketData.ExchangeID));
	hs_strncpy(m_DepthMarketData.Instrument, pFuncQhSingleHqPush->FutuCode, sizeof(m_DepthMarketData.Instrument));
	hs_strncpy(m_DepthMarketData.InstrumentName, pFuncQhSingleHqPush->FutuCode, sizeof(m_DepthMarketData.InstrumentName));
	m_DepthMarketData.LastPrice = pFuncQhSingleHqPush->FutuLastPrice;
	m_DepthMarketData.PreSettlementPrice = pFuncQhSingleHqPush->PreSettlementPrice;
	m_DepthMarketData.PreClosePrice = pFuncQhSingleHqPush->PreClosePrice;
	m_DepthMarketData.PreOpenInterest = pFuncQhSingleHqPush->PreOpenInterest;
	m_DepthMarketData.OpenPrice = pFuncQhSingleHqPush->FutuOpenPrice;
	m_DepthMarketData.HighestPrice = pFuncQhSingleHqPush->FutuHighPrice;
	m_DepthMarketData.LowestPrice = pFuncQhSingleHqPush->FutuLowPrice;
	m_DepthMarketData.Turnover = pFuncQhSingleHqPush->BusinessBalance;
	m_DepthMarketData.Volume = pFuncQhSingleHqPush->BusinessAmount;
	m_DepthMarketData.OpenInterest = pFuncQhSingleHqPush->OpenInterest;
	m_DepthMarketData.ClosePrice = pFuncQhSingleHqPush->FutuClosePrice;
	m_DepthMarketData.SettlementPrice = pFuncQhSingleHqPush->SquarePrice;
	m_DepthMarketData.UpperLimitPrice = pFuncQhSingleHqPush->UplimitedPrice;
	m_DepthMarketData.LowerLimitPrice = pFuncQhSingleHqPush->DownlimitedPrice;
	m_DepthMarketData.BuyPrice1 = pFuncQhSingleHqPush->BuyHighPrice;
	m_DepthMarketData.BuyAmount1 = pFuncQhSingleHqPush->BuyHighAmount;
	m_DepthMarketData.SalePrice1 = pFuncQhSingleHqPush->LowSellPrice;
	m_DepthMarketData.SaleAmount1 = pFuncQhSingleHqPush->LowSellAmount;

	if(lpApi && lpApi->GetMdCenterSpi())
	{
		lpApi->GetMdCenterSpi()->OnRtnDepthMarketData(&m_DepthMarketData);
	}
}