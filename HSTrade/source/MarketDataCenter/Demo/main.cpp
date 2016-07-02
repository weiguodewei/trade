#include "HundsunMdCenterApi.h"
#include "MarkDataTraderApi.h"
#include "mdSpi.h"
#include "DBSpi.h"
#include "public.h"

int main()
{
	//启动回库
	CMarkDataTraderApi* lpUserDBApi = CMarkDataTraderApi::CreateMarkDataTraderApi();
	CDBSpi* lpUserDBSpi = new CDBSpi(lpUserDBApi);
	lpUserDBApi->RegisterSpi(lpUserDBSpi);
	lpUserDBApi->RegisterFront("192.168.54.157:9138");
	//lpUserDBApi->Init("license/hs_2.0_157/license.dat");

	/*
	//DBF
	CHundsunMdCenterApi* lpUserMdApi = CHundsunMdCenterApi::CreateHundsunMdCenterApi();
	CMdSpi* lpUserMdSpi = new CMdSpi(lpUserMdApi);
	lpUserMdApi->RegisterSpi(lpUserMdSpi);
	lpUserMdApi->RegisterDBFPath("D:/dbf");
	lpUserMdApi->Init(HUNDSUN_FTDC_DBF);

	cerr<<"------------------------------------------------"<<endl;
	cerr<<" [1]  订阅                                      "<<endl;
	cerr<<" [0]  Exit                                      "<<endl;
	cerr<<"------------------------------------------------"<<endl;

	while(1)
	{
		int cmd;  
		cin>>cmd;
		switch(cmd){
			case 1:
				char* Instrument[1];
				Instrument[0] = "600570";
				lpUserMdApi->SubscribeMarketData(HUNDSUN_FTDC_SH, Instrument, sizeof(Instrument)/sizeof(char*));
				break;
			case 0: exit(0);
		}
	}

	lpUserMdApi->ReleaseApi();
	*/

	/*
	//H5
	CHundsunMdCenterApi* lpUserMdApi = CHundsunMdCenterApi::CreateHundsunMdCenterApi();
	CMdSpi* lpUserMdSpi = new CMdSpi(lpUserMdApi);
	lpUserMdApi->RegisterSpi(lpUserMdSpi);
	lpUserMdApi->RegisterH5("h5apilicense.xml", "h5config.xml");
	lpUserMdApi->Init(HUNDSUN_FTDC_H5);

	cerr<<"------------------------------------------------"<<endl;
	cerr<<" [1]  订阅                                      "<<endl;
	cerr<<" [0]  Exit                                      "<<endl;
	cerr<<"------------------------------------------------"<<endl;

	while(1)
	{
		int cmd;  
		cin>>cmd;
		switch(cmd){
		case 1:
			char* Instrument[1];
			Instrument[0] = "600570";
			lpUserMdApi->SubscribeMarketData(HUNDSUN_FTDC_SH, Instrument, sizeof(Instrument)/sizeof(char*));
			break;
		case 0: exit(0);
		}
	}

	lpUserMdApi->ReleaseApi();
	*/

	/*
	//Futu
	CHundsunMdCenterApi* lpUserMdApi = CHundsunMdCenterApi::CreateHundsunMdCenterApi();
	CMdSpi* lpUserMdSpi = new CMdSpi(lpUserMdApi);
	lpUserMdApi->RegisterSpi(lpUserMdSpi);
	lpUserMdApi->RegisterFutu("license/yaqh_hs_sim_2.0/license.dat");
	lpUserMdApi->RegisterFront("115.238.53.140:9006");
	lpUserMdApi->Init(HUNDSUN_FTDC_Futu);

	cerr<<"------------------------------------------------"<<endl;
	cerr<<" [1]  订阅                                      "<<endl;
	cerr<<" [0]  Exit                                      "<<endl;
	cerr<<"------------------------------------------------"<<endl;

	while(1)
	{
		int cmd;  
		cin>>cmd;
		switch(cmd){
		case 1:
			char* Instrument[1];
			Instrument[0] = "IF1511";
			lpUserMdApi->SubscribeMarketData(HUNDSUN_FTDC_CFFEX, Instrument, sizeof(Instrument)/sizeof(char*));
			break;
		case 0: exit(0);
		}
	}

	lpUserMdApi->ReleaseApi();
	*/

	//H5 && Futu 一起上
	CHundsunMdCenterApi* lpUserMdApi = CHundsunMdCenterApi::CreateHundsunMdCenterApi();
	CMdSpi* lpUserMdSpi = new CMdSpi(lpUserMdApi, lpUserDBSpi);
	lpUserMdApi->RegisterSpi(lpUserMdSpi);
	//lpUserMdApi->RegisterDBFPath("D:/dbf");
	lpUserMdApi->RegisterH5("h5apilicense.xml", "h5config.xml");
	lpUserMdApi->RegisterFutu("license/yaqh_hs_real_2.0/license.dat");
	//lpUserMdApi->RegisterFront("122.224.221.235:9106");
	//lpUserMdApi->RegisterFront("112.124.211.5:9999");
	//lpUserMdApi->Init(HUNDSUN_FTDC_DBF);
	lpUserMdApi->Init(HUNDSUN_FTDC_H5);
	//lpUserMdApi->Init(HUNDSUN_FTDC_Futu);
	//lpUserMdApi->Init(HUNDSUN_FTDC_H5SDK);

	char* Instrument[1];

	cerr<<"------------------------------------------------"<<endl;
	cerr<<" [1]  订阅证券                                  "<<endl;
	cerr<<" [2]  订阅期货                                  "<<endl;
	cerr<<" [3]  查询证券合约信息                          "<<endl;
	cerr<<" [4]  订阅证券所有合约                          "<<endl;
	cerr<<" [5]  期货登录                                  "<<endl;
	cerr<<" [6]  查询期货合约信息                          "<<endl;
	cerr<<" [7]  订阅个股期权合约信息                          "<<endl;
	cerr<<" [0]  Exit                                      "<<endl;
	cerr<<"------------------------------------------------"<<endl;

	while(1)
	{
		int cmd;  
		cin>>cmd;
		switch(cmd){
		case 1:
			Instrument[0] = "600570";
			lpUserMdApi->SubscribeMarketData(HUNDSUN_FTDC_SH, Instrument, sizeof(Instrument)/sizeof(char*));
			break;
		case 2:
			Instrument[0] = "IF1511";
			lpUserMdApi->SubscribeMarketData(HUNDSUN_FTDC_CFFEX, Instrument, sizeof(Instrument)/sizeof(char*));
			break;
		case 3:
			{
				CHundsunFtdcQryInstrumentField m_Instrument;
				memset(&m_Instrument, 0, sizeof(CHundsunFtdcQryInstrumentField));

				m_Instrument.ExchangeID = HUNDSUN_FTDC_SH;
				lpUserMdApi->ReqQryInstrument(&m_Instrument, 0);
				//m_Instrument.ExchangeID = HUNDSUN_FTDC_SZ;
				//lpUserMdApi->ReqQryInstrument(&m_Instrument, 0);
			}
			break;
		case 4:
			lpUserMdSpi->SubAll();
			break;
		case 5:
			{
				CHundsunFtdcReqUserLoginField m_UserLogin;
				memset(&m_UserLogin, 0, sizeof(CHundsunFtdcReqUserLoginField));
				m_UserLogin.MarkerDataType = HUNDSUN_FTDC_Futu;
				hs_strncpy(m_UserLogin.FundAccount, "1111", sizeof(m_UserLogin.FundAccount));
				hs_strncpy(m_UserLogin.Password, "123456", sizeof(m_UserLogin.Password));

				lpUserMdApi->ReqUserLogin(&m_UserLogin, 0);
			}
			break;
		case 6:
			{
				CHundsunFtdcQryInstrumentField m_Instrument;
				memset(&m_Instrument, 0, sizeof(CHundsunFtdcQryInstrumentField));

				m_Instrument.ExchangeID = HUNDSUN_FTDC_SHFE;
				lpUserMdApi->ReqQryInstrument(&m_Instrument, 0);
				m_Instrument.ExchangeID = HUNDSUN_FTDC_CZCE;
				lpUserMdApi->ReqQryInstrument(&m_Instrument, 0);
				m_Instrument.ExchangeID = HUNDSUN_FTDC_DCE;
				lpUserMdApi->ReqQryInstrument(&m_Instrument, 0);
				m_Instrument.ExchangeID = HUNDSUN_FTDC_CFFEX;
				lpUserMdApi->ReqQryInstrument(&m_Instrument, 0);
			}
			break;
		case 7:
			{
				Instrument[0] = "10000380";
				lpUserMdApi->SubscribeMarketData(HUNDSUN_FTDC_GQ_SH, Instrument, sizeof(Instrument)/sizeof(char*));
			}
		case 8:
			{
				Instrument[0] = "510050";
				lpUserMdApi->SubscribeMarketData(HUNDSUN_FTDC_SH, Instrument, sizeof(Instrument)/sizeof(char*));
			}
		case 9:
			{
				CHundsunFtdcQryInstrumentField m_Instrument;
				memset(&m_Instrument, 0, sizeof(CHundsunFtdcQryInstrumentField));

				m_Instrument.ExchangeID = HUNDSUN_FTDC_GQ_SH;
				lpUserMdApi->ReqQryInstrument(&m_Instrument, 0);
			}
			break;
		case 0: exit(0);
		}
	}
	lpUserMdApi->ReleaseApi();

	return 0;
}