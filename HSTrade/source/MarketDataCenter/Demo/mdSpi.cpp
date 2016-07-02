#include "mdSpi.h"
#include "DBSpi.h"

bool CMdSpi::IsErrorRspInfo(CHundsunFtdcRspInfoField *pRspInfo)
{
	bool ret = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (ret)
	{
		printf("IsErrorRspInfo\n");
		cerr<<" 响应 | "<<pRspInfo->ErrorMsg<<endl;
	}
	return ret;
}

void CMdSpi::SubAll()
{
	char* Instrument[1];
	char sTemp[1024] = {};
	map<pair<int, string>, CHundsunFtdcInstrumentField*>::iterator it;
	for(it = m_InstrumentMap.begin(); it != m_InstrumentMap.end(); it++)
	{
		memcpy(sTemp, it->second->Instrument, 2);

		if(it->second->ExchangeID = HUNDSUN_FTDC_SH)
		{	
			if(strcmp(sTemp, "60") != 0)
				continue;

			Instrument[0] = it->second->Instrument;
			lpUserApi->SubscribeMarketData(it->second->ExchangeID, Instrument, sizeof(Instrument)/sizeof(char*));
		}
		else if(it->second->ExchangeID = HUNDSUN_FTDC_SZ)
		{
			if(!(strcmp(sTemp, "00") == 0 || strcmp(sTemp, "30") == 0))
				continue;

			Instrument[0] = it->second->Instrument;
			lpUserApi->SubscribeMarketData(it->second->ExchangeID, Instrument, sizeof(Instrument)/sizeof(char*));
		}
	}
}

CHundsunFtdcInstrumentField* CMdSpi::GetInstrument(THundsunExchangeType ExchangeID, THundsunFtdcInstrument pszInstrument)
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

void CMdSpi::UpdateInstrument(CHundsunFtdcInstrumentField *pInstrument)
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

void CMdSpi::OnFrontConnected()
{
	printf("[行情]连接成功...\n");
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	printf("[行情]连接断开...\n");
}

void CMdSpi::OnRspUserLogin(CHundsunFtdcRspUserLoginField *pRspUserLogin, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		cout<<pRspUserLogin->MarkerDataType<<"|"<<pRspUserLogin->FundAccount<<"登录成功..."<<endl;
	}
}

void CMdSpi::OnRspQryInstrument(CHundsunFtdcInstrumentField *pInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(pInstrument)
	{
		UpdateInstrument(pInstrument);
		cout<<pInstrument->ExchangeID<<"|"<<pInstrument->Instrument << "|" <<  pInstrument->InstrumentName<<endl;

		if( strcmp(pInstrument->Instrument,"10000380")==0 )
		{
			int i = 0;
			i++;
		}
	}



	if(bIsLast)
		cout<<"合约查询成功"<<endl;
}

void CMdSpi::OnRspSubMarketData(CHundsunSpecificInstrumentField *pSpecificInstrument, CHundsunFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(!IsErrorRspInfo(pRspInfo))
	{
		if(pSpecificInstrument)
			cout<<"["<<pSpecificInstrument->Instrument<<"] 订阅成功"<<endl;
	}
}

void CMdSpi::OnRtnDepthMarketData(CHundsunDepthMarketDataField *pDepthMarketData)
{
	cout<<"["<<pDepthMarketData->Instrument<<"] LastPrice:"<<pDepthMarketData->LastPrice<<endl;
	cout<<"卖5价："<<pDepthMarketData->SalePrice5<<"    卖5量："<<pDepthMarketData->SaleAmount5/100<<endl;
	cout<<"卖4价："<<pDepthMarketData->SalePrice4<<"    卖4量："<<pDepthMarketData->SaleAmount4/100<<endl;
	cout<<"卖3价："<<pDepthMarketData->SalePrice3<<"    卖3量："<<pDepthMarketData->SaleAmount3/100<<endl;
	cout<<"卖2价："<<pDepthMarketData->SalePrice2<<"    卖2量："<<pDepthMarketData->SaleAmount2/100<<endl;
	cout<<"卖1价："<<pDepthMarketData->SalePrice1<<"    卖1量："<<pDepthMarketData->SaleAmount1/100<<endl;
	cout<<"买1价："<<pDepthMarketData->BuyPrice1<<"    买1量："<<pDepthMarketData->BuyAmount1/100<<endl;
	cout<<"买2价："<<pDepthMarketData->BuyPrice2<<"    买2量："<<pDepthMarketData->BuyAmount2/100<<endl;
	cout<<"买3价："<<pDepthMarketData->BuyPrice3<<"    买3量："<<pDepthMarketData->BuyAmount3/100<<endl;
	cout<<"买4价："<<pDepthMarketData->BuyPrice4<<"    买4量："<<pDepthMarketData->BuyAmount4/100<<endl;
	cout<<"买5价："<<pDepthMarketData->BuyPrice5<<"    买5量："<<pDepthMarketData->BuyAmount5/100<<endl;

	if(lpUserDBSpi)
	{
		lpUserDBSpi->MarkdataInsert(pDepthMarketData);
	}
}
