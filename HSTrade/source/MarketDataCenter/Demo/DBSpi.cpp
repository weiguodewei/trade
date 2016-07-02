#include "DBSpi.h"

bool CDBSpi::IsErrorRspInfo(CMarkDataRspInfoField *pRspInfo)
{
	bool ret = ((pRspInfo) && (pRspInfo->ErrorNo != 0));
	if (ret)
	{
		printf("IsErrorRspInfo\n");
		cerr<<" 响应 | "<<pRspInfo->ErrorInfo<<endl;
	}
	return ret;
}

inline char* GetFutuExchange(int iExchangeID)
{
	if(iExchangeID == HUNDSUN_FTDC_SH)
		return "SH";
	if(iExchangeID == HUNDSUN_FTDC_SZ)
		return "SZ";
	if(iExchangeID == HUNDSUN_FTDC_SHFE)
		return "SHFE";
	if(iExchangeID == HUNDSUN_FTDC_CZCE)
		return "CZCE";
	if(iExchangeID == HUNDSUN_FTDC_DCE)
		return "DCE";
	if(iExchangeID == HUNDSUN_FTDC_CFFEX)
		return "CFFEX";
	if(iExchangeID == HUNDSUN_FTDC_INE)
		return "INE";
	else
		return "";
}

int CDBSpi::MarkdataInsert(CHundsunDepthMarketDataField *pDepthMarketData)
{
	if(!bConnected)
		return -1;

	CMarkDataReqFuncMarkdatainsertField m_Markdatainsert;
	memset(&m_Markdatainsert, 0, sizeof(CMarkDataReqFuncMarkdatainsertField));

	CDate now;
	m_Markdatainsert.UpdateTime = now.GetTime(); //pDepthMarketData->UpdateTime;
	hs_strncpy(m_Markdatainsert.FutuExchType, GetFutuExchange(pDepthMarketData->ExchangeID), sizeof(m_Markdatainsert.FutuExchType));
	hs_strncpy(m_Markdatainsert.FutuCode, pDepthMarketData->Instrument, sizeof(m_Markdatainsert.FutuCode));
	hs_strncpy(m_Markdatainsert.FutuName, pDepthMarketData->InstrumentName, sizeof(m_Markdatainsert.FutuName));
	m_Markdatainsert.LastPrice = pDepthMarketData->LastPrice;
	m_Markdatainsert.PreSettlementPrice = pDepthMarketData->PreSettlementPrice;
	m_Markdatainsert.PreClosePrice = pDepthMarketData->PreClosePrice;
	m_Markdatainsert.PreOpenInterest = pDepthMarketData->PreOpenInterest;
	m_Markdatainsert.OpenPrice = pDepthMarketData->OpenPrice;
	m_Markdatainsert.HighPrice = pDepthMarketData->HighestPrice;
	m_Markdatainsert.LowPrice = pDepthMarketData->LowestPrice;
	m_Markdatainsert.BusinessBalance = pDepthMarketData->Turnover;
	m_Markdatainsert.BusinessAmount = pDepthMarketData->Volume;
	m_Markdatainsert.BusinCount = pDepthMarketData->Count;
	m_Markdatainsert.OpenInterest = pDepthMarketData->OpenInterest;
	m_Markdatainsert.ClosePrice = pDepthMarketData->ClosePrice;
	m_Markdatainsert.SquarePrice = pDepthMarketData->SettlementPrice;
	m_Markdatainsert.UplimitedPrice = pDepthMarketData->UpperLimitPrice;
	m_Markdatainsert.DownlimitedPrice = pDepthMarketData->LowerLimitPrice;
	m_Markdatainsert.BuyPrice1 = pDepthMarketData->BuyPrice1;
	m_Markdatainsert.BuyAmount1 = pDepthMarketData->BuyAmount1;
	m_Markdatainsert.SalePrice1 = pDepthMarketData->SalePrice1;
	m_Markdatainsert.SaleAmount1 = pDepthMarketData->SaleAmount1;
	m_Markdatainsert.BuyPrice2 = pDepthMarketData->BuyPrice2;
	m_Markdatainsert.BuyAmount2 = pDepthMarketData->BuyAmount2;
	m_Markdatainsert.SalePrice2 = pDepthMarketData->SalePrice2;
	m_Markdatainsert.SaleAmount2 = pDepthMarketData->SaleAmount2;
	m_Markdatainsert.BuyPrice3 = pDepthMarketData->BuyPrice3;
	m_Markdatainsert.BuyAmount3 = pDepthMarketData->BuyAmount3;
	m_Markdatainsert.SalePrice3 = pDepthMarketData->SalePrice3;
	m_Markdatainsert.SaleAmount3 = pDepthMarketData->SaleAmount3;
	m_Markdatainsert.BuyPrice4 = pDepthMarketData->BuyPrice4;
	m_Markdatainsert.BuyAmount4 = pDepthMarketData->BuyAmount4;
	m_Markdatainsert.SalePrice4 = pDepthMarketData->SalePrice4;
	m_Markdatainsert.SaleAmount4 = pDepthMarketData->SaleAmount4;
	m_Markdatainsert.BuyPrice5 = pDepthMarketData->BuyPrice5;
	m_Markdatainsert.BuyAmount5 = pDepthMarketData->BuyAmount5;
	m_Markdatainsert.SalePrice5 = pDepthMarketData->SalePrice5;
	m_Markdatainsert.SaleAmount5 = pDepthMarketData->SaleAmount5;

	return lpUserApi->ReqFuncMarkdatainsert(&m_Markdatainsert);
}

void CDBSpi::OnFrontConnected()
{
	bConnected = true;
	printf("[回库]连接成功...\n");
}

void CDBSpi::OnFrontDisconnected(int nReason)
{
	bConnected = false;
	printf("[回库]连接断开...\n");
}

void CDBSpi::OnRspFuncMarkdatainsert(CMarkDataRspFuncMarkdatainsertField *pRspFuncMarkdatainsert, CMarkDataRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{

	}
}