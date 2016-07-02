#include "MarkDataTraderApi_log.h"
#include "MarkDataUserApiStruct.h"

void CMarkDataTraderApi_log::MarkDataTraderLog(void* lpvoid)
{
    CMarkDataTraderApi_log *helper = (CMarkDataTraderApi_log*)lpvoid;
    CTraderApiLog* lpLog;
    char szMsg[1024];
    while(helper->bLogInfoStop == false)
    {
        lpLog = helper->msgPush.Pop(1000);

        if(lpLog != NULL)
        {
            if(lpLog->logType == LOGINFO_Request)
            {
                switch(lpLog->nFunction)
                {
                    case FuncMarkdatainsert:
                        {
                            CMarkDataReqFuncMarkdatainsertField* lpReqFuncMarkdatainsert = (CMarkDataReqFuncMarkdatainsertField*)lpLog->pBuffer;
                            sprintf(szMsg, "[%d %s] ReqFuncMarkdatainsert", lpLog->ndate, lpLog->sztime);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                     UpdateTime : %d", lpReqFuncMarkdatainsert->UpdateTime);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[string]                   FutuExchType : %s", lpReqFuncMarkdatainsert->FutuExchType);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[string]                       FutuCode : %s", lpReqFuncMarkdatainsert->FutuCode);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[string]                       FutuName : %s", lpReqFuncMarkdatainsert->FutuName);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                      LastPrice : %f", lpReqFuncMarkdatainsert->LastPrice);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]             PreSettlementPrice : %f", lpReqFuncMarkdatainsert->PreSettlementPrice);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                  PreClosePrice : %f", lpReqFuncMarkdatainsert->PreClosePrice);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                PreOpenInterest : %f", lpReqFuncMarkdatainsert->PreOpenInterest);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                      OpenPrice : %f", lpReqFuncMarkdatainsert->OpenPrice);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                      HighPrice : %f", lpReqFuncMarkdatainsert->HighPrice);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                       LowPrice : %f", lpReqFuncMarkdatainsert->LowPrice);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                BusinessBalance : %f", lpReqFuncMarkdatainsert->BusinessBalance);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                 BusinessAmount : %d", lpReqFuncMarkdatainsert->BusinessAmount);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                     BusinCount : %d", lpReqFuncMarkdatainsert->BusinCount);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                   OpenInterest : %f", lpReqFuncMarkdatainsert->OpenInterest);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                     ClosePrice : %f", lpReqFuncMarkdatainsert->ClosePrice);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                    SquarePrice : %f", lpReqFuncMarkdatainsert->SquarePrice);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                 UplimitedPrice : %f", lpReqFuncMarkdatainsert->UplimitedPrice);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]               DownlimitedPrice : %f", lpReqFuncMarkdatainsert->DownlimitedPrice);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                      BuyPrice1 : %f", lpReqFuncMarkdatainsert->BuyPrice1);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                     BuyAmount1 : %d", lpReqFuncMarkdatainsert->BuyAmount1);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                     SalePrice1 : %f", lpReqFuncMarkdatainsert->SalePrice1);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                    SaleAmount1 : %d", lpReqFuncMarkdatainsert->SaleAmount1);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                      BuyPrice2 : %f", lpReqFuncMarkdatainsert->BuyPrice2);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                     BuyAmount2 : %d", lpReqFuncMarkdatainsert->BuyAmount2);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                     SalePrice2 : %f", lpReqFuncMarkdatainsert->SalePrice2);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                    SaleAmount2 : %d", lpReqFuncMarkdatainsert->SaleAmount2);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                      BuyPrice3 : %f", lpReqFuncMarkdatainsert->BuyPrice3);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                     BuyAmount3 : %d", lpReqFuncMarkdatainsert->BuyAmount3);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                     SalePrice3 : %f", lpReqFuncMarkdatainsert->SalePrice3);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                    SaleAmount3 : %d", lpReqFuncMarkdatainsert->SaleAmount3);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                      BuyPrice4 : %f", lpReqFuncMarkdatainsert->BuyPrice4);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                     BuyAmount4 : %d", lpReqFuncMarkdatainsert->BuyAmount4);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                     SalePrice4 : %f", lpReqFuncMarkdatainsert->SalePrice4);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                    SaleAmount4 : %d", lpReqFuncMarkdatainsert->SaleAmount4);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                      BuyPrice5 : %f", lpReqFuncMarkdatainsert->BuyPrice5);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                     BuyAmount5 : %d", lpReqFuncMarkdatainsert->BuyAmount5);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[double]                     SalePrice5 : %f", lpReqFuncMarkdatainsert->SalePrice5);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                    SaleAmount5 : %d", lpReqFuncMarkdatainsert->SaleAmount5);
                            helper->WriteLog(szMsg);
                            helper->WriteLog("");
                        }
                        break;
                }
            }
            else if(lpLog->logType == LOGINFO_Response)
            {
                CMarkDataRspInfoField* lpRspInfo = (CMarkDataRspInfoField*)lpLog->pReturnBuf;
                if(lpRspInfo->ErrorNo == 0)
                {
                    switch(lpLog->nFunction)
                    {
                        case FuncMarkdatainsert:
                            {
                                CMarkDataRspFuncMarkdatainsertField* lpRspFuncMarkdatainsert = (CMarkDataRspFuncMarkdatainsertField*)lpLog->pBuffer;
                                sprintf(szMsg, "[%d %s] OnRspFuncMarkdatainsert", lpLog->ndate, lpLog->sztime);
                                helper->WriteLog(szMsg);
                                helper->WriteLog("");
                            }
                            break;
                    }
                }
                else
                {
                    switch(lpLog->nFunction)
                    {
                        case FuncMarkdatainsert:
                            sprintf(szMsg, "[%d %s] OnRspFuncMarkdatainsert", lpLog->ndate, lpLog->sztime);
                            helper->WriteLog(szMsg);
                            break;
                    }
                    sprintf(szMsg, "[   int]                        ErrorNo : %d", lpRspInfo->ErrorNo);
                    helper->WriteLog(szMsg);
                    sprintf(szMsg, "[string]                      ErrorInfo : %s", lpRspInfo->ErrorInfo);
                    helper->WriteLog(szMsg);
                    helper->WriteLog("");
                }
            }
            else if(lpLog->logType == LOGINFO_Return)
            {
                switch(lpLog->nIssueType)
                {
                }
            }

            if(lpLog->pReturnBuf != NULL)
                free(lpLog->pReturnBuf);
            if(lpLog->pBuffer != NULL)
                free(lpLog->pBuffer);
            delete lpLog;
        }
    }
}

