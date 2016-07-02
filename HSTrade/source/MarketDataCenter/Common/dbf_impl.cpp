#include "dbf_impl.h"

char sTempFiled[1024];

inline void ParseValue(int &val, int size, unsigned char* &p)
{
	memcpy(sTempFiled, p, size);
	sTempFiled[size] = '\0';
	val = atoi(sTempFiled);
	p = p + size;
}

inline void ParseValue(long &val, int size, unsigned char* &p)
{
	memcpy(sTempFiled, p, size);
	sTempFiled[size] = '\0';
	val = atol(sTempFiled);
	//istringstream strValue(sTempFiled);
	//strValue >> val;
	p = p + size;
}

inline void ParseValue(double &val, int size, unsigned char* &p)
{
	memcpy(sTempFiled, p, size);
	sTempFiled[size] = '\0';
	val = atof(sTempFiled);
	p = p + size;
}

inline void ParseValue(char *val, int size , unsigned char* &p)
{
	memcpy(sTempFiled, p, size);
	sTempFiled[size] = '\0';
	strncpy(val, sTempFiled, size);
	p = p + size;
}

int CDBF_Impl::InitDBF(const char *pszDBFFile)
{
	sprintf_s(szPath_show2003, "%s/show2003.dbf", pszDBFFile);
	sprintf_s(szPath_sjshq, "%s/sjshq.dbf", pszDBFFile);

	m_ReadDBFTrd.Start(ReadDBF,0,this);

	if(lpDBFSpi)
		lpDBFSpi->OnDBFConnected();

	return 0;
}

void CDBF_Impl::RegisterDFBSpi(CHundsunDBFSpi *pSpi)
{
	if(pSpi)
		lpDBFSpi = pSpi;
}

int CDBF_Impl::SubscribeDBF(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount)
{
	CHundsunSpecificInstrumentField SpecificInstrument;
	CHundsunFtdcRspInfoField RspInfo;

	for(int i = 0; i < nCount; i++)
	{
		pair<int, string> pr(ExchangeID, ppInstrumentID[i]);
		if( SubInfoSet.find(pr) == SubInfoSet.end())
		{
			SubInfoSet.insert(pr);
			if(lpDBFSpi)
			{
				SpecificInstrument.ExchangeID = ExchangeID;
				hs_strncpy(SpecificInstrument.Instrument, ppInstrumentID[i], sizeof(SpecificInstrument.Instrument));
				RspInfo.ErrorID = 0;
				hs_strncpy(RspInfo.ErrorMsg, "No Error.", sizeof(RspInfo.ErrorMsg));
				if(i == nCount - 1)
					lpDBFSpi->OnDBFRspSubMarketData(&SpecificInstrument, &RspInfo, 0, true);
				else
					lpDBFSpi->OnDBFRspSubMarketData(&SpecificInstrument, &RspInfo, 0, false);

				CHundsunDepthMarketDataField* lpDepthMarketData = GetHQ(ExchangeID, ppInstrumentID[i]);
				if(lpDepthMarketData != NULL)
				{
					lpDBFSpi->OnDBFRtnDepthMarketData(lpDepthMarketData);
				}
			}
		}
		else
		{
			if(lpDBFSpi)
			{
				SpecificInstrument.ExchangeID = ExchangeID;
				hs_strncpy(SpecificInstrument.Instrument, ppInstrumentID[i], sizeof(SpecificInstrument.Instrument));
				RspInfo.ErrorID = 0;
				hs_strncpy(RspInfo.ErrorMsg, "Repeat Sub.", sizeof(RspInfo.ErrorMsg));
				if(i == nCount - 1)
					lpDBFSpi->OnDBFRspSubMarketData(&SpecificInstrument, &RspInfo, 0, true);
				else
					lpDBFSpi->OnDBFRspSubMarketData(&SpecificInstrument, &RspInfo, 0, false);
			}
		}
	}

	return 0;
}

int CDBF_Impl::UnSubscribeDBF(THundsunExchangeType ExchangeID, char *ppInstrumentID[], int nCount)
{
	return 0;
}

void CDBF_Impl::ReadDBF(void* lpvoid)
{
	CDBF_Impl *helper = (CDBF_Impl*)lpvoid;
	while(helper->bStopReadDBF == false)
	{
		helper->UpdateSH();
		helper->UpdateSZ();

		Sleep(100);
	}
}

void CDBF_Impl::UpdateSH()
{
	timeval tv1, tv2, tv3;

	tv1 = CurrentTimeTag();
	if(!m_dbf_show2003.InitDbf(szPath_show2003))
	{
		return;
	}

	unsigned char szBuf[1024] = {};
	char sTemp[1024] = {};
	long m_RecordCount = m_dbf_show2003.GetRecordCount();

	CHundsunDepthMarketDataField m_DepthMD;
	unsigned char *p = NULL;
	const char* pData = NULL;

	tv2 = CurrentTimeTag();

	for(int i = 0; i <= m_RecordCount; i++)
	{
		m_dbf_show2003.GetRecord(i, szBuf);
		p = &(szBuf[0]);
		memcpy(sTemp, p, 2);
		if(strcmp(sTemp, "60") != 0)
			continue;

		memset(&m_DepthMD, 0, sizeof(CHundsunDepthMarketDataField));
		m_DepthMD.ExchangeID = HUNDSUN_FTDC_SH;
		ParseValue(m_DepthMD.Instrument, 6, p);
		ParseValue(m_DepthMD.InstrumentName, 8, p);
		ParseValue(m_DepthMD.PreClosePrice, 8, p);
		ParseValue(m_DepthMD.OpenPrice, 8, p);
		ParseValue(m_DepthMD.Turnover, 12, p);
		ParseValue(m_DepthMD.HighestPrice, 8, p);
		ParseValue(m_DepthMD.LowestPrice, 8, p);
		ParseValue(m_DepthMD.LastPrice, 8, p);
		ParseValue(m_DepthMD.BuyPrice1, 8, p);
		ParseValue(m_DepthMD.SalePrice1, 8, p);
		ParseValue(m_DepthMD.Volume, 10, p);
		p = p + 8;
		ParseValue(m_DepthMD.BuyAmount1, 10, p);
		ParseValue(m_DepthMD.BuyPrice2, 8, p);
		ParseValue(m_DepthMD.BuyAmount2, 10, p);
		ParseValue(m_DepthMD.BuyPrice3, 8, p);
		ParseValue(m_DepthMD.BuyAmount3, 10, p);
		ParseValue(m_DepthMD.SaleAmount1, 10, p);
		ParseValue(m_DepthMD.SalePrice2, 8, p);
		ParseValue(m_DepthMD.SaleAmount2, 10, p);
		ParseValue(m_DepthMD.SalePrice3, 8, p);
		ParseValue(m_DepthMD.SaleAmount3, 10, p);
		ParseValue(m_DepthMD.BuyPrice4, 8, p);
		ParseValue(m_DepthMD.BuyAmount4, 10, p);
		ParseValue(m_DepthMD.BuyPrice5, 8, p);
		ParseValue(m_DepthMD.BuyAmount5, 10, p);
		ParseValue(m_DepthMD.SalePrice4, 8, p);
		ParseValue(m_DepthMD.SaleAmount4, 10, p);
		ParseValue(m_DepthMD.SalePrice5, 8, p);
		ParseValue(m_DepthMD.SaleAmount5, 10, p);

		UpdateHQ(m_DepthMD.ExchangeID, m_DepthMD.Instrument, &m_DepthMD);
	}

	tv3 = CurrentTimeTag();

	double m_RunDiff = (tv2.tv_sec - tv1.tv_sec) * 1000000 + (tv2.tv_usec - tv1.tv_usec);
	double m_RunDiff2 = (tv3.tv_sec - tv1.tv_sec) * 1000000 + (tv3.tv_usec - tv1.tv_usec);
	sprintf(sTemp, "总数：%d 耗时： %0.3f   ----   %0.3f", m_RecordCount, m_RunDiff / 1000, m_RunDiff2 / 1000);
	//cout<<sTemp<<endl;

	return;
}

void CDBF_Impl::UpdateSZ()
{
	timeval tv1, tv2, tv3;

	tv1 = CurrentTimeTag();
	if(!m_dbf_sjshq.InitDbf(szPath_sjshq))
	{
		return;
	}

	unsigned char szBuf[1024] = {};
	char sTemp[1024] = {};
	long m_RecordCount = m_dbf_sjshq.GetRecordCount();



	int FieldSize;
	CHundsunDepthMarketDataField m_DepthMD;
	unsigned char *p = NULL;
	const char* pData = NULL;

	tv2 = CurrentTimeTag();

	for(int i = 0; i <= m_RecordCount; i++)
	{
		m_dbf_sjshq.GetRecord(i, szBuf);
		p = &(szBuf[0]);
		memcpy(sTemp, p, 2);

		if(!(strcmp(sTemp, "00") == 0 || strcmp(sTemp, "30") == 0))
			continue;

		memset(&m_DepthMD, 0, sizeof(CHundsunDepthMarketDataField));
		m_DepthMD.ExchangeID = HUNDSUN_FTDC_SZ; 
		ParseValue(m_DepthMD.Instrument, 6, p);
		ParseValue(m_DepthMD.InstrumentName, 8, p);
		ParseValue(m_DepthMD.PreClosePrice, 9, p);
		ParseValue(m_DepthMD.OpenPrice, 9, p);
		ParseValue(m_DepthMD.LastPrice, 9, p);
		ParseValue(m_DepthMD.Volume, 12, p);
		ParseValue(m_DepthMD.Turnover, 17, p);
		ParseValue(m_DepthMD.Count, 9, p);
		ParseValue(m_DepthMD.HighestPrice, 9, p);
		ParseValue(m_DepthMD.LowestPrice, 9, p);
		p = p + 44;
		ParseValue(m_DepthMD.SalePrice5, 9, p);
		ParseValue(m_DepthMD.SaleAmount5, 12, p);
		ParseValue(m_DepthMD.SalePrice4, 9, p);
		ParseValue(m_DepthMD.SaleAmount4, 12, p);
		ParseValue(m_DepthMD.SalePrice3, 9, p);
		ParseValue(m_DepthMD.SaleAmount3, 12, p);
		ParseValue(m_DepthMD.SalePrice2, 9, p);
		ParseValue(m_DepthMD.SaleAmount2, 12, p);
		ParseValue(m_DepthMD.SalePrice1, 9, p);
		ParseValue(m_DepthMD.SaleAmount1, 12, p);
		ParseValue(m_DepthMD.BuyPrice1, 9, p);
		ParseValue(m_DepthMD.BuyAmount1, 12, p);
		ParseValue(m_DepthMD.BuyPrice2, 9, p);
		ParseValue(m_DepthMD.BuyAmount2, 12, p);
		ParseValue(m_DepthMD.BuyPrice3, 9, p);
		ParseValue(m_DepthMD.BuyAmount3, 12, p);
		ParseValue(m_DepthMD.BuyPrice4, 9, p);
		ParseValue(m_DepthMD.BuyAmount4, 12, p);
		ParseValue(m_DepthMD.BuyPrice5, 9, p);
		ParseValue(m_DepthMD.BuyAmount5, 12, p);

		UpdateHQ(m_DepthMD.ExchangeID, m_DepthMD.Instrument, &m_DepthMD);
	}

	tv3 = CurrentTimeTag();

	double m_RunDiff = (tv2.tv_sec - tv1.tv_sec) * 1000000 + (tv2.tv_usec - tv1.tv_usec);
	double m_RunDiff2 = (tv3.tv_sec - tv1.tv_sec) * 1000000 + (tv3.tv_usec - tv1.tv_usec);
	sprintf(sTemp, "总数：%d 耗时： %0.3f   ----   %0.3f", m_RecordCount, m_RunDiff / 1000, m_RunDiff2 / 1000);
	//cout<<sTemp<<endl;
}

CHundsunDepthMarketDataField* CDBF_Impl::GetHQ(THundsunExchangeType ExchangeID, const char* pszInstrument)
{
	pair<int, string> pr(ExchangeID, pszInstrument);
	map<pair<int, string>, CHundsunDepthMarketDataField*>::iterator it = HQMap.find(pr);
	if( it != HQMap.end())
	{
		return it->second;
	}
	else
	{
		return NULL;
	}
}

void CDBF_Impl::UpdateHQ(THundsunExchangeType ExchangeID, const char* pszInstrument, CHundsunDepthMarketDataField *pDepthMarketData)
{
	CHundsunDepthMarketDataField* lpDepthMarketData = GetHQ(ExchangeID, pszInstrument);
	if(lpDepthMarketData == NULL)
	{
		lpDepthMarketData = new CHundsunDepthMarketDataField();
		pair<int, string> pr(ExchangeID, pszInstrument);
		HQMap[pr] = lpDepthMarketData;
	}

	//判断一下是不是有新的行情
	if(pDepthMarketData->Volume > lpDepthMarketData->Volume || pDepthMarketData->LastPrice != lpDepthMarketData->LastPrice ||
		pDepthMarketData->BuyPrice1 != lpDepthMarketData->BuyPrice1 || pDepthMarketData->BuyAmount1 != lpDepthMarketData->BuyAmount1 ||
		pDepthMarketData->SalePrice1 != lpDepthMarketData->SalePrice1 || pDepthMarketData->SaleAmount1 != lpDepthMarketData->SaleAmount1 ||
		pDepthMarketData->BuyPrice2 != lpDepthMarketData->BuyPrice2 || pDepthMarketData->BuyAmount2 != lpDepthMarketData->BuyAmount2 ||
		pDepthMarketData->SalePrice2 != lpDepthMarketData->SalePrice2 || pDepthMarketData->SaleAmount2 != lpDepthMarketData->SaleAmount2 ||
		pDepthMarketData->BuyPrice3 != lpDepthMarketData->BuyPrice3 || pDepthMarketData->BuyAmount3 != lpDepthMarketData->BuyAmount3 ||
		pDepthMarketData->SalePrice3 != lpDepthMarketData->SalePrice3 || pDepthMarketData->SaleAmount3 != lpDepthMarketData->SaleAmount3 ||
		pDepthMarketData->BuyPrice4 != lpDepthMarketData->BuyPrice4 || pDepthMarketData->BuyAmount4 != lpDepthMarketData->BuyAmount4 ||
		pDepthMarketData->SalePrice4 != lpDepthMarketData->SalePrice4 || pDepthMarketData->SaleAmount4 != lpDepthMarketData->SaleAmount4 ||
		pDepthMarketData->BuyPrice5 != lpDepthMarketData->BuyPrice5 || pDepthMarketData->BuyAmount5 != lpDepthMarketData->BuyAmount5 ||
		pDepthMarketData->SalePrice5 != lpDepthMarketData->SalePrice5 || pDepthMarketData->SaleAmount5 != lpDepthMarketData->SaleAmount5 ||
		pDepthMarketData->PreClosePrice != lpDepthMarketData->PreClosePrice)
	{
		if(lpDBFSpi)
		{
			pair<int, string> pr(ExchangeID, pszInstrument);
			if( SubInfoSet.find(pr) != SubInfoSet.end())
			{
				//触发主推上层
				lpDBFSpi->OnDBFRtnDepthMarketData(pDepthMarketData);
			}
		}

		memcpy(lpDepthMarketData, pDepthMarketData, sizeof(CHundsunDepthMarketDataField));
	}
}

