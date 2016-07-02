#include "h5_impl.h"

CH5_Impl* lpH5_Obj = NULL;

inline int GetMarkDataExchangeID(int iMkt)
{
	switch (iMkt)
	{
	case MKT_SH_L1:
	case MKT_SH_L2:
		return HUNDSUN_FTDC_SH;
		break;
	case MKT_SZ_L1:
	case MKT_SZ_L2:
		return HUNDSUN_FTDC_SZ;
		break;
	case MKT_GGQQ:	
		return HUNDSUN_FTDC_GQ_SH;
		break;
	case MKT_GGQQ_SZ:
		return HUNDSUN_FTDC_GQ_SZ;
		break;
	default:
		return 0;
		break;
	}
}

inline int GetUpdateTime(int iTimes, int iSecond)
{
	int iUpdateTime;
	CDate now;
	if(now.GetTime() >= 130000)
	{
		if(iTimes >= 180)
			iUpdateTime = 140000 + (iTimes - 180) * 100 + iSecond;
		else
			iUpdateTime = 130000 + (iTimes - 120) * 100 + iSecond;
	}
	else
	{
		if(iTimes >= 60)
			iUpdateTime = 103000 + (iTimes - 60) * 100 + iSecond;
		else
			iUpdateTime = 93000 + iTimes * 100 + iSecond;
	}
	iUpdateTime -= 100;

	return iUpdateTime;
}

// 市场状态变化的回调函数
int FUNCTION_CALL_MODE OnMktStatus(int iMktType, int iConnectID, enumMktStatu eMktStatus)
{
	if(eMktStatus == MKT_INITIALIZED)
	{
		if(lpH5_Obj)
			lpH5_Obj->GetH5MarketInfo(iMktType);
	}

	return 0;
}

// 此函数旨在通知用户授权到期状态，用户只需要关注到期日
int FUNCTION_CALL_MODE OnExpire(enumExpireStatus eExpireStatus, unsigned int nExpireDate, const char *szExpireTip)
{
	return 0;
}

//	处理行情变化主推消息的回调函数
int FUNCTION_CALL_MODE OnReqRecv(int iRegID, IStockInfo* lpStockInfo, int iResults, int* lpResultid, int iConnectID)
{
	if(lpH5_Obj)
		lpH5_Obj->DealRecv(lpStockInfo, iResults, lpResultid);

	return 0;
}

CH5_Impl* CH5_Impl::GetInstance()
{
	if(!lpH5_Obj)
	{
		lpH5_Obj = new CH5_Impl();
		lpH5Spi = NULL;
	}

	return lpH5_Obj;
}

void CH5_Impl::ReleaseH5()
{
	if(lpH5_Obj)
	{
		UnInitApi();
		delete lpH5_Obj;
	}
}

int CH5_Impl::InitH5(const char *pszLicFile, const char *pszCfgFile, bool bSubLevel2)
{
	if(AuthorizeApi(pszLicFile, &OnExpire) == 0)
	{
		m_SubLevel2 = bSubLevel2;
		return InitApi(pszCfgFile, &OnMktStatus, 10000);
	}
	return -1;
}

void CH5_Impl::RegisterH5Spi(CHundsunH5Spi *pSpi)
{
	if(pSpi)
		lpH5Spi = pSpi;
}

int CH5_Impl::ReqQryInstrumentH5(CHundsunFtdcQryInstrumentField *pQryInstrument, int nRequestID)
{
	if(!lpH5Spi)
		return 0;

	CHundsunFtdcInstrumentField m_Instrument;
	CHundsunFtdcRspInfoField RspInfo = {};
	map<pair<int, string>, CHundsunFtdcInstrumentField*>::iterator it;
	if(pQryInstrument->ExchangeID == 0)
	{
		for(it = m_InstrumentMap.begin(); it != m_InstrumentMap.end(); it++)
		{
			memcpy(&m_Instrument, it->second, sizeof(CHundsunFtdcInstrumentField));
			m_Instrument.ExchangeID = GetMarkDataExchangeID(m_Instrument.ExchangeID);
			lpH5Spi->OnH5RspQryInstrument(&m_Instrument, &RspInfo, nRequestID, false);
		}
	}
	else
	{
		for(it = m_InstrumentMap.begin(); it != m_InstrumentMap.end(); it++)
		{
			memcpy(&m_Instrument, it->second, sizeof(CHundsunFtdcInstrumentField));
			m_Instrument.ExchangeID = GetMarkDataExchangeID(m_Instrument.ExchangeID);
			if(pQryInstrument->ExchangeID == m_Instrument.ExchangeID)
				lpH5Spi->OnH5RspQryInstrument(&m_Instrument, &RspInfo, nRequestID, false);
		}
	}

	if(m_InstrumentMap.size() > 0)
		lpH5Spi->OnH5RspQryInstrument(NULL, &RspInfo, nRequestID, true);

	return 0;
}

int CH5_Impl::SubscribeH5(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount)
{
	CHundsunSpecificInstrumentField SpecificInstrument;
	CHundsunFtdcRspInfoField RspInfo;

	int ret = 0;
	int iBizid[2];
	iBizid[0] = RESULT_SIMPLE_QUOTE;
	iBizid[1] = RESULT_BIDOFF;

	for(int i = 0; i < nCount; i++)
	{
		switch (ExchangeID)
		{
		case HUNDSUN_FTDC_SH:
			if(m_SubLevel2)
				ret = SubStock(SUB_ADD, 100, MKT_SH_L2, ppInstrumentID[i], 2, iBizid, &OnReqRecv);
			else
				ret = SubStock(SUB_ADD, 100, MKT_SH_L1, ppInstrumentID[i], 2, iBizid, &OnReqRecv);
			break;
		case HUNDSUN_FTDC_SZ:
			if(m_SubLevel2)
				ret = SubStock(SUB_ADD, 100, MKT_SZ_L2, ppInstrumentID[i], 2, iBizid, &OnReqRecv);
			else
				ret = SubStock(SUB_ADD, 100, MKT_SZ_L1, ppInstrumentID[i], 2, iBizid, &OnReqRecv);
			break;
		case HUNDSUN_FTDC_GQ_SH:
			ret = SubStock(SUB_ADD, 100, MKT_GGQQ, ppInstrumentID[i], 2, iBizid, &OnReqRecv);
			break;
		case HUNDSUN_FTDC_GQ_SZ:
			ret = SubStock(SUB_ADD, 100, MKT_GGQQ_SZ, ppInstrumentID[i], 2, iBizid, &OnReqRecv);
			break;
		default:
			break;
		}

		if(lpH5Spi)
		{
			if(ret == 0)
			{
				SpecificInstrument.ExchangeID = ExchangeID;
				hs_strncpy(SpecificInstrument.Instrument, ppInstrumentID[i], sizeof(SpecificInstrument.Instrument));
				RspInfo.ErrorID = ret;
				hs_strncpy(RspInfo.ErrorMsg, "No Error.", sizeof(RspInfo.ErrorMsg));
				if(i == nCount - 1)
					lpH5Spi->OnH5RspSubMarketData(&SpecificInstrument, &RspInfo, 0, true);
				else
					lpH5Spi->OnH5RspSubMarketData(&SpecificInstrument, &RspInfo, 0, false);
			}
			else
			{
				SpecificInstrument.ExchangeID = ExchangeID;
				hs_strncpy(SpecificInstrument.Instrument, ppInstrumentID[i], sizeof(SpecificInstrument.Instrument));
				RspInfo.ErrorID = ret;
				hs_strncpy(RspInfo.ErrorMsg, "订阅失败，市场不存在或代码不存在", sizeof(RspInfo.ErrorMsg));
				if(i == nCount - 1)
					lpH5Spi->OnH5RspSubMarketData(&SpecificInstrument, &RspInfo, 0, true);
				else
					lpH5Spi->OnH5RspSubMarketData(&SpecificInstrument, &RspInfo, 0, false);
			}
		}
	}
	return 0;
}


int CH5_Impl::UnSubscribeH5(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount)
{
	CHundsunSpecificInstrumentField SpecificInstrument;
	CHundsunFtdcRspInfoField RspInfo;

	int ret = 0;
	int iBizid[2];
	iBizid[0] = RESULT_SIMPLE_QUOTE;
	iBizid[1] = RESULT_BIDOFF;

	for(int i = 0; i < nCount; i++)
	{
		switch (ExchangeID)
		{
		case HUNDSUN_FTDC_SH:
			if(m_SubLevel2)
				ret = SubStock(SUB_CANCEL, 100, MKT_SH_L2, ppInstrumentID[i], 2, iBizid, &OnReqRecv);
			else
				ret = SubStock(SUB_CANCEL, 100, MKT_SH_L1, ppInstrumentID[i], 2, iBizid, &OnReqRecv);
			break;
		case HUNDSUN_FTDC_SZ:
			if(m_SubLevel2)
				ret = SubStock(SUB_CANCEL, 100, MKT_SZ_L2, ppInstrumentID[i], 2, iBizid, &OnReqRecv);
			else
				ret = SubStock(SUB_CANCEL, 100, MKT_SZ_L1, ppInstrumentID[i], 2, iBizid, &OnReqRecv);
			break;
		case HUNDSUN_FTDC_GQ_SH:
			ret = SubStock(SUB_CANCEL, 100, MKT_GGQQ, ppInstrumentID[i], 2, iBizid, &OnReqRecv);
			break;
		case HUNDSUN_FTDC_GQ_SZ:
			ret = SubStock(SUB_CANCEL, 100, MKT_GGQQ_SZ, ppInstrumentID[i], 2, iBizid, &OnReqRecv);
			break;
		default:
			break;
		}

		if(lpH5Spi)
		{
			if(ret == 0)
			{
				SpecificInstrument.ExchangeID = ExchangeID;
				hs_strncpy(SpecificInstrument.Instrument, ppInstrumentID[i], sizeof(SpecificInstrument.Instrument));
				RspInfo.ErrorID = ret;
				hs_strncpy(RspInfo.ErrorMsg, "No Error.", sizeof(RspInfo.ErrorMsg));
				if(i == nCount - 1)
					lpH5Spi->OnH5RspUnSubMarketData(&SpecificInstrument, &RspInfo, 0, true);
				else
					lpH5Spi->OnH5RspUnSubMarketData(&SpecificInstrument, &RspInfo, 0, false);
			}
			else
			{
				SpecificInstrument.ExchangeID = ExchangeID;
				hs_strncpy(SpecificInstrument.Instrument, ppInstrumentID[i], sizeof(SpecificInstrument.Instrument));
				RspInfo.ErrorID = ret;
				hs_strncpy(RspInfo.ErrorMsg, "取消订阅失败...", sizeof(RspInfo.ErrorMsg));
				if(i == nCount - 1)
					lpH5Spi->OnH5RspUnSubMarketData(&SpecificInstrument, &RspInfo, 0, true);
				else
					lpH5Spi->OnH5RspUnSubMarketData(&SpecificInstrument, &RspInfo, 0, false);
			}
		}
	}
	return 0;

}



void CH5_Impl::GetH5MarketInfo(int iMktType)
{
	int iErrno = 0;
	//获取对应合约信息
	IMarketOverview* lpMarket = GetSupportMarketInfo(iMktType, 20, iErrno);
	IStockInitInfo* lpStkInf;
	IStockInfo* lpStockInfo;
	CHundsunFtdcInstrumentField m_Instrument;
	CHundsunDepthMarketDataField m_DepthMarketData;
	if(lpMarket)
	{
		for (int i = 0; i < lpMarket->GetTotalCount(); i++)
		{
			lpStkInf = lpMarket->GetStockInitInfo(i);

			memset(&m_Instrument, 0, sizeof(CHundsunFtdcInstrumentField));
			m_Instrument.ExchangeID = iMktType;
			hs_strncpy(m_Instrument.Instrument, lpStkInf->GetStockCode(), sizeof(m_Instrument.Instrument));
			hs_strncpy(m_Instrument.InstrumentName, lpStkInf->GetStockName(), sizeof(m_Instrument.InstrumentName));
			UpdateInstrument(&m_Instrument);

			//lpStockInfo = GetStockInfo(iMktType, m_Instrument.Instrument);
			//if(lpStockInfo)
			//{
			//	memset(&m_DepthMarketData, 0, sizeof(CHundsunDepthMarketDataField));
			//	GetH5StockInfo(lpStockInfo, &m_DepthMarketData);
			//	UpdateDepthMD(&m_DepthMarketData);
			//}
		}
	}

	if(lpH5Spi)
		lpH5Spi->OnH5Connected(iMktType);
}

void CH5_Impl::DealRecv(IStockInfo* lpStockInfo, int iResults, int* lpResultid)
{
	bool bOnReturn = false;
	CHundsunDepthMarketDataField* lpDepthMD = GetDepthMD(GetMarkDataExchangeID(lpStockInfo->GetInItInfo()->GetMkt()), lpStockInfo->GetInItInfo()->GetStockCode());
	if(!lpDepthMD)
	{
		CHundsunDepthMarketDataField m_DepthMarketData = {};

		for(int i=0; i<iResults; i++)
		{
			GetH5StockInfo(lpStockInfo, &m_DepthMarketData, lpResultid[i]);
			UpdateDepthMD(&m_DepthMarketData);
			if(lpResultid[i] == RESULT_BIDOFF)
				bOnReturn = true;
		}
		if(bOnReturn && lpH5Spi)
			lpH5Spi->OnH5RtnDepthMarketData(&m_DepthMarketData);
	}
	else
	{
		for(int i=0; i<iResults; i++)
		{
			GetH5StockInfo(lpStockInfo, lpDepthMD, lpResultid[i]);
			UpdateDepthMD(lpDepthMD);
			if(lpResultid[i] == RESULT_BIDOFF)
				bOnReturn = true;
		}
		if(bOnReturn && lpH5Spi)
			lpH5Spi->OnH5RtnDepthMarketData(lpDepthMD);
	}
}

bool CH5_Impl::IsSpecialInstrument(THundsunFtdcInstrument pszInstrument)
{
	char szBuf[8] = {};
	memcpy(szBuf, pszInstrument, 2);
	if(strcmp(szBuf, "1A") == 0 || strcmp(szBuf, "1B") == 0 || strcmp(szBuf, "2A") == 0 || strcmp(szBuf, "2C") == 0)
		return true;

	return false;
}

void CH5_Impl::GetH5StockInfo(IStockInfo* pStockInfo, CHundsunDepthMarketDataField *pDepthMarketData, int iSetID)
{
	switch (iSetID)
	{
	case -1:
		{
			UpdateSimpleQuote(pStockInfo, pDepthMarketData);
			UpdateBidoff(pStockInfo, pDepthMarketData);
		}
		break;
	case RESULT_SIMPLE_QUOTE:
		UpdateSimpleQuote(pStockInfo, pDepthMarketData);
		break;
	case RESULT_BIDOFF:
		UpdateBidoff(pStockInfo, pDepthMarketData);
		break;
	default:
		break;
	}
}

void CH5_Impl::UpdateSimpleQuote(IStockInfo* pStockInfo, CHundsunDepthMarketDataField *pDepthMarketData)
{
	ISimpleQuote* lpQuote;
	lpQuote = pStockInfo->GetSimpleQuoteInfo();
	pDepthMarketData->ExchangeID = GetMarkDataExchangeID(pStockInfo->GetInItInfo()->GetMkt());
	hs_strncpy(pDepthMarketData->Instrument, pStockInfo->GetInItInfo()->GetStockCode(), sizeof(pDepthMarketData->Instrument));
	hs_strncpy(pDepthMarketData->InstrumentName, pStockInfo->GetInItInfo()->GetStockName(), sizeof(pDepthMarketData->InstrumentName));
	if(lpQuote)
	{
		pDepthMarketData->LastPrice = lpQuote->GetLastPrice();
		pDepthMarketData->PreSettlementPrice = lpQuote->GetPreJieSuanPrice();
		pDepthMarketData->PreClosePrice = lpQuote->GetPrevPrice();
		pDepthMarketData->OpenPrice = lpQuote->GetOpenPrice();
		pDepthMarketData->HighestPrice = lpQuote->GetHighPrice();
		pDepthMarketData->LowestPrice = lpQuote->GetLowPrice();
		pDepthMarketData->Turnover = lpQuote->GetVolum();
		pDepthMarketData->Volume = lpQuote->GetAmount();
		pDepthMarketData->Count = 0;
		pDepthMarketData->OpenInterest = lpQuote->GetPosition();
		pDepthMarketData->ClosePrice = 0;
		pDepthMarketData->SettlementPrice = lpQuote->GetJieSuanPrice();
		pDepthMarketData->UpperLimitPrice = lpQuote->GetUpPrice();
		pDepthMarketData->LowerLimitPrice = lpQuote->GetDownPrice();
		pDepthMarketData->UpdateTime = GetUpdateTime(lpQuote->GetTimes(), lpQuote->GetSecond());
	}
}

void CH5_Impl::UpdateBidoff(IStockInfo* pStockInfo, CHundsunDepthMarketDataField *pDepthMarketData)
{
	IBidOffer* lpBidOffer;
	LevelInfo* lpLevel;

	lpBidOffer = pStockInfo->GetBidOffer();
	if(lpBidOffer)
	{
		lpLevel=lpBidOffer->GetBidInfo(0);
		if(lpLevel)
		{
			pDepthMarketData->BuyPrice1 = lpLevel->price;
			pDepthMarketData->BuyAmount1 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetBidInfo(1);
		if(lpLevel)
		{
			pDepthMarketData->BuyPrice2 = lpLevel->price;
			pDepthMarketData->BuyAmount2 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetBidInfo(2);
		if(lpLevel)
		{
			pDepthMarketData->BuyPrice3 = lpLevel->price;
			pDepthMarketData->BuyAmount3 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetBidInfo(3);
		if(lpLevel)
		{
			pDepthMarketData->BuyPrice4 = lpLevel->price;
			pDepthMarketData->BuyAmount4 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetBidInfo(4);
		if(lpLevel)
		{
			pDepthMarketData->BuyPrice5 = lpLevel->price;
			pDepthMarketData->BuyAmount5 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetBidInfo(5);
		if(lpLevel)
		{
			pDepthMarketData->BuyPrice6 = lpLevel->price;
			pDepthMarketData->BuyAmount6 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetBidInfo(6);
		if(lpLevel)
		{
			pDepthMarketData->BuyPrice7 = lpLevel->price;
			pDepthMarketData->BuyAmount7 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetBidInfo(6);
		if(lpLevel)
		{
			pDepthMarketData->BuyPrice7 = lpLevel->price;
			pDepthMarketData->BuyAmount7 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetBidInfo(7);
		if(lpLevel)
		{
			pDepthMarketData->BuyPrice8 = lpLevel->price;
			pDepthMarketData->BuyAmount8 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetBidInfo(8);
		if(lpLevel)
		{
			pDepthMarketData->BuyPrice9 = lpLevel->price;
			pDepthMarketData->BuyAmount9 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetBidInfo(9);
		if(lpLevel)
		{
			pDepthMarketData->BuyPrice10 = lpLevel->price;
			pDepthMarketData->BuyAmount10 = lpLevel->Amount;
		}

		lpLevel=lpBidOffer->GetOfferInfo(0);
		if(lpLevel)
		{
			pDepthMarketData->SalePrice1 = lpLevel->price;
			pDepthMarketData->SaleAmount1 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetOfferInfo(1);
		if(lpLevel)
		{
			pDepthMarketData->SalePrice2 = lpLevel->price;
			pDepthMarketData->SaleAmount2 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetOfferInfo(2);
		if(lpLevel)
		{
			pDepthMarketData->SalePrice3 = lpLevel->price;
			pDepthMarketData->SaleAmount3 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetOfferInfo(3);
		if(lpLevel)
		{
			pDepthMarketData->SalePrice4 = lpLevel->price;
			pDepthMarketData->SaleAmount4 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetOfferInfo(4);
		if(lpLevel)
		{
			pDepthMarketData->SalePrice5 = lpLevel->price;
			pDepthMarketData->SaleAmount5 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetOfferInfo(5);
		if(lpLevel)
		{
			pDepthMarketData->SalePrice6 = lpLevel->price;
			pDepthMarketData->SaleAmount6 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetOfferInfo(6);
		if(lpLevel)
		{
			pDepthMarketData->SalePrice7 = lpLevel->price;
			pDepthMarketData->SaleAmount7 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetOfferInfo(7);
		if(lpLevel)
		{
			pDepthMarketData->SalePrice8 = lpLevel->price;
			pDepthMarketData->SaleAmount8 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetOfferInfo(8);
		if(lpLevel)
		{
			pDepthMarketData->SalePrice9 = lpLevel->price;
			pDepthMarketData->SaleAmount9 = lpLevel->Amount;
		}
		lpLevel=lpBidOffer->GetOfferInfo(9);
		if(lpLevel)
		{
			pDepthMarketData->SalePrice10 = lpLevel->price;
			pDepthMarketData->SaleAmount10 = lpLevel->Amount;
		}
	}
}

CHundsunFtdcInstrumentField* CH5_Impl::GetInstrument(THundsunExchangeType ExchangeID, THundsunFtdcInstrument pszInstrument)
{
	pair<int, string> pr(ExchangeID, pszInstrument);
	map<pair<int, string>, CHundsunFtdcInstrumentField*>::iterator it = m_InstrumentMap.find(pr);
	if( it != m_InstrumentMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

CHundsunDepthMarketDataField* CH5_Impl::GetDepthMD(THundsunExchangeType ExchangeID, const char* pszInstrument)
{
	pair<int, string> pr(ExchangeID, pszInstrument);
	map<pair<int, string>, CHundsunDepthMarketDataField*>::iterator it = m_DepthMDMap.find(pr);
	if( it != m_DepthMDMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void CH5_Impl::UpdateInstrument(CHundsunFtdcInstrumentField *pInstrument)
{
	CHundsunFtdcInstrumentField* lpInstrument = GetInstrument(pInstrument->ExchangeID, pInstrument->Instrument);
	if(!lpInstrument)
	{
		lpInstrument = new CHundsunFtdcInstrumentField();
		pair<int, string> pr(pInstrument->ExchangeID, pInstrument->Instrument);
		m_InstrumentMap[pr] = lpInstrument;
	}
	memcpy(lpInstrument, pInstrument, sizeof(CHundsunFtdcInstrumentField));
}

void CH5_Impl::UpdateDepthMD(CHundsunDepthMarketDataField *pDepthMarketData)
{
	CHundsunDepthMarketDataField* lpDepthMarketData = GetDepthMD(pDepthMarketData->ExchangeID, pDepthMarketData->Instrument);
	if(!lpDepthMarketData)
	{
		lpDepthMarketData = new CHundsunDepthMarketDataField();
		pair<int, string> pr(pDepthMarketData->ExchangeID, pDepthMarketData->Instrument);
		m_DepthMDMap[pr] = lpDepthMarketData;
	}
	memcpy(lpDepthMarketData, pDepthMarketData, sizeof(CHundsunDepthMarketDataField));
}
