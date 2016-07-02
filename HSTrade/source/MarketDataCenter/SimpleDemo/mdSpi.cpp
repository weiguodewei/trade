#include "mdSpi.h"

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

void CMdSpi::OnFrontConnected()
{
	printf("[行情]连接成功...\n");
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	printf("[行情]连接断开...\n");
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
	cout<<"卖10价："<<pDepthMarketData->SalePrice10<<"  卖10量："<<pDepthMarketData->SaleAmount10/100<<endl;
	cout<<"卖9价："<<pDepthMarketData->SalePrice9<<"    卖9量："<<pDepthMarketData->SaleAmount9/100<<endl;
	cout<<"卖8价："<<pDepthMarketData->SalePrice8<<"    卖8量："<<pDepthMarketData->SaleAmount8/100<<endl;
	cout<<"卖7价："<<pDepthMarketData->SalePrice7<<"    卖7量："<<pDepthMarketData->SaleAmount7/100<<endl;
	cout<<"卖6价："<<pDepthMarketData->SalePrice6<<"    卖6量："<<pDepthMarketData->SaleAmount6/100<<endl;
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
	cout<<"买6价："<<pDepthMarketData->BuyPrice6<<"    买6量："<<pDepthMarketData->BuyAmount6/100<<endl;
	cout<<"买7价："<<pDepthMarketData->BuyPrice7<<"    买7量："<<pDepthMarketData->BuyAmount7/100<<endl;
	cout<<"买8价："<<pDepthMarketData->BuyPrice8<<"    买8量："<<pDepthMarketData->BuyAmount8/100<<endl;
	cout<<"买9价："<<pDepthMarketData->BuyPrice9<<"    买9量："<<pDepthMarketData->BuyAmount9/100<<endl;
	cout<<"买10价："<<pDepthMarketData->BuyPrice10<<"  买10量："<<pDepthMarketData->BuyAmount10/100<<endl;
}
