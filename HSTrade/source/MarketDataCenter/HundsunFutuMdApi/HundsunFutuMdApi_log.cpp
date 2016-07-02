#include "HundsunFutuMdApi_log.h"
#include "HundsunFutuUserApiStruct.h"

void CHundsunFutuMdApi_log::HundsunFutuMdLog(void* lpvoid)
{
    CHundsunFutuMdApi_log *helper = (CHundsunFutuMdApi_log*)lpvoid;
    CMdApiLog* lpLog;
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
                    case FuncClientLogin:
                        {
                            CHundsunFutuReqFuncClientLoginField* lpReqFuncClientLogin = (CHundsunFutuReqFuncClientLoginField*)lpLog->pBuffer;
                            sprintf(szMsg, "[%d %s] ReqFuncClientLogin", lpLog->ndate, lpLog->sztime);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                     OpBranchNo : %d", lpReqFuncClientLogin->OpBranchNo);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[  char]                   OpEntrustWay : %c", lpReqFuncClientLogin->OpEntrustWay);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[string]                      OpStation : %s", lpReqFuncClientLogin->OpStation);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                       BranchNo : %d", lpReqFuncClientLogin->BranchNo);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[string]                       Password : %s", lpReqFuncClientLogin->Password);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[  char]                   PasswordType : %c", lpReqFuncClientLogin->PasswordType);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[  char]                   InputContent : %c", lpReqFuncClientLogin->InputContent);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[string]                 AccountContent : %s", lpReqFuncClientLogin->AccountContent);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[string]                    ContentType : %s", lpReqFuncClientLogin->ContentType);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[  char]                      AssetProp : %c", lpReqFuncClientLogin->AssetProp);
                            helper->WriteLog(szMsg);
                            helper->WriteLog("");
                        }
                        break;
                    case FuncFutuContractbailQry:
                        {
                            CHundsunFutuReqFuncFutuContractbailQryField* lpReqFuncFutuContractbailQry = (CHundsunFutuReqFuncFutuContractbailQryField*)lpLog->pBuffer;
                            sprintf(szMsg, "[%d %s] ReqFuncFutuContractbailQry", lpLog->ndate, lpLog->sztime);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[string]                      UserToken : %s", lpReqFuncFutuContractbailQry->UserToken);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[string]                   FutuExchType : %s", lpReqFuncFutuContractbailQry->FutuExchType);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[string]                       FutuCode : %s", lpReqFuncFutuContractbailQry->FutuCode);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[   int]                     RequestNum : %d", lpReqFuncFutuContractbailQry->RequestNum);
                            helper->WriteLog(szMsg);
                            sprintf(szMsg, "[string]                    PositionStr : %s", lpReqFuncFutuContractbailQry->PositionStr);
                            helper->WriteLog(szMsg);
                            helper->WriteLog("");
                        }
                        break;
                }
            }
            else if(lpLog->logType == LOGINFO_Response)
            {
                CHundsunFutuRspInfoField* lpRspInfo = (CHundsunFutuRspInfoField*)lpLog->pReturnBuf;
                if(lpRspInfo->ErrorNo == 0)
                {
                    switch(lpLog->nFunction)
                    {
                        case FuncClientLogin:
                            {
                                CHundsunFutuRspFuncClientLoginField* lpRspFuncClientLogin = (CHundsunFutuRspFuncClientLoginField*)lpLog->pBuffer;
                                sprintf(szMsg, "[%d %s] OnRspFuncClientLogin", lpLog->ndate, lpLog->sztime);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                       InitDate : %d", lpRspFuncClientLogin->InitDate);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]                      SysStatus : %c", lpRspFuncClientLogin->SysStatus);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                    CompanyName : %s", lpRspFuncClientLogin->CompanyName);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                    ContentType : %s", lpRspFuncClientLogin->ContentType);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                 AccountContent : %s", lpRspFuncClientLogin->AccountContent);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                       BranchNo : %d", lpRspFuncClientLogin->BranchNo);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                       ClientId : %s", lpRspFuncClientLogin->ClientId);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                     ClientName : %s", lpRspFuncClientLogin->ClientName);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                CorpClientGroup : %d", lpRspFuncClientLogin->CorpClientGroup);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                  CorpRiskLevel : %d", lpRspFuncClientLogin->CorpRiskLevel);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                  CorpBeginDate : %d", lpRspFuncClientLogin->CorpBeginDate);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                    CorpEndDate : %d", lpRspFuncClientLogin->CorpEndDate);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]                      ValidFlag : %c", lpRspFuncClientLogin->ValidFlag);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]               FundaccountCount : %d", lpRspFuncClientLogin->FundaccountCount);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                    FundAccount : %s", lpRspFuncClientLogin->FundAccount);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                   ClientRights : %s", lpRspFuncClientLogin->ClientRights);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]               LastOpEntrustWay : %c", lpRspFuncClientLogin->LastOpEntrustWay);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                  LastOpStation : %s", lpRspFuncClientLogin->LastOpStation);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                      SysnodeId : %d", lpRspFuncClientLogin->SysnodeId);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                      UserToken : %s", lpRspFuncClientLogin->UserToken);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                        ErrorNo : %d", lpRspFuncClientLogin->ErrorNo);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                      ErrorInfo : %s", lpRspFuncClientLogin->ErrorInfo);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]                      AssetProp : %c", lpRspFuncClientLogin->AssetProp);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]                    ProductFlag : %c", lpRspFuncClientLogin->ProductFlag);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]                    MessageFlag : %c", lpRspFuncClientLogin->MessageFlag);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]                 TabconfirmFlag : %c", lpRspFuncClientLogin->TabconfirmFlag);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                       LastDate : %d", lpRspFuncClientLogin->LastDate);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                      SessionNo : %d", lpRspFuncClientLogin->SessionNo);
                                helper->WriteLog(szMsg);
                            }
                            break;
                        case FuncFutuContractbailQry:
                            {
                                CHundsunFutuRspFuncFutuContractbailQryField* lpRspFuncFutuContractbailQry = (CHundsunFutuRspFuncFutuContractbailQryField*)lpLog->pBuffer;
                                sprintf(szMsg, "[%d %s] OnRspFuncFutuContractbailQry", lpLog->ndate, lpLog->sztime);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                   FutuExchType : %s", lpRspFuncFutuContractbailQry->FutuExchType);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                       FutuCode : %s", lpRspFuncFutuContractbailQry->FutuCode);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                       FutuName : %s", lpRspFuncFutuContractbailQry->FutuName);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                   FutucodeType : %s", lpRspFuncFutuContractbailQry->FutucodeType);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                      MoneyType : %s", lpRspFuncFutuContractbailQry->MoneyType);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]                       StopFlag : %c", lpRspFuncFutuContractbailQry->StopFlag);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                  AmountPerHand : %d", lpRspFuncFutuContractbailQry->AmountPerHand);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]                 FutuReportUnit : %c", lpRspFuncFutuContractbailQry->FutuReportUnit);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]                      PriceUnit : %c", lpRspFuncFutuContractbailQry->PriceUnit);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                  FutuPriceStep : %f", lpRspFuncFutuContractbailQry->FutuPriceStep);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                       OpenUnit : %f", lpRspFuncFutuContractbailQry->OpenUnit);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                       DropUnit : %f", lpRspFuncFutuContractbailQry->DropUnit);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                MaxMarketAmount : %d", lpRspFuncFutuContractbailQry->MaxMarketAmount);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]               MaxLimitedAmount : %d", lpRspFuncFutuContractbailQry->MaxLimitedAmount);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                      OpenRatio : %f", lpRspFuncFutuContractbailQry->OpenRatio);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                     OpenMargin : %f", lpRspFuncFutuContractbailQry->OpenMargin);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                     FoptExpiry : %s", lpRspFuncFutuContractbailQry->FoptExpiry);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                FoptStrikePrice : %f", lpRspFuncFutuContractbailQry->FoptStrikePrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                       PreDelta : %f", lpRspFuncFutuContractbailQry->PreDelta);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]                FutuProductType : %c", lpRspFuncFutuContractbailQry->FutuProductType);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]                       FoptType : %c", lpRspFuncFutuContractbailQry->FoptType);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                 UnderlyingCode : %s", lpRspFuncFutuContractbailQry->UnderlyingCode);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                     ExpireDate : %d", lpRspFuncFutuContractbailQry->ExpireDate);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                     StrikeDate : %d", lpRspFuncFutuContractbailQry->StrikeDate);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]             HighSidePreferFlag : %c", lpRspFuncFutuContractbailQry->HighSidePreferFlag);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                  PreDeltaValue : %f", lpRspFuncFutuContractbailQry->PreDeltaValue);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                    PositionStr : %s", lpRspFuncFutuContractbailQry->PositionStr);
                                helper->WriteLog(szMsg);
                            }
                            break;
                    }
                }
                else
                {
                    switch(lpLog->nFunction)
                    {
                        case FuncClientLogin:
                            sprintf(szMsg, "[%d %s] OnRspFuncClientLogin", lpLog->ndate, lpLog->sztime);
                            helper->WriteLog(szMsg);
                            break;
                        case FuncFutuContractbailQry:
                            sprintf(szMsg, "[%d %s] OnRspFuncFutuContractbailQry", lpLog->ndate, lpLog->sztime);
                            helper->WriteLog(szMsg);
                            break;
                    }
                    sprintf(szMsg, "[   int]                        ErrorNo : %d", lpRspInfo->ErrorNo);
                    helper->WriteLog(szMsg);
                    sprintf(szMsg, "[string]                      ErrorInfo : %s", lpRspInfo->ErrorInfo);
                    helper->WriteLog(szMsg);
                }
            }
            else if(lpLog->logType == LOGINFO_Return)
            {
                switch(lpLog->nIssueType)
                {
                    case 33100:
                        {
                                CHundsunFutuRtnFuncQhSingleHqPushField* lpRtnFuncQhSingleHqPush = (CHundsunFutuRtnFuncQhSingleHqPushField*)lpLog->pBuffer;
                                sprintf(szMsg, "[%d %s] OnRtnFuncQhSingleHqPush", lpLog->ndate, lpLog->sztime);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]                      QuoteType : %c", lpRtnFuncQhSingleHqPush->QuoteType);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                    QuoteLength : %d", lpRtnFuncQhSingleHqPush->QuoteLength);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                     UpdateTime : %d", lpRtnFuncQhSingleHqPush->UpdateTime);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                     QuoteIndex : %d", lpRtnFuncQhSingleHqPush->QuoteIndex);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                   FutuExchType : %s", lpRtnFuncQhSingleHqPush->FutuExchType);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                  CommodityType : %s", lpRtnFuncQhSingleHqPush->CommodityType);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                       FutuCode : %s", lpRtnFuncQhSingleHqPush->FutuCode);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]             PreSettlementPrice : %f", lpRtnFuncQhSingleHqPush->PreSettlementPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                  PreClosePrice : %f", lpRtnFuncQhSingleHqPush->PreClosePrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                PreOpenInterest : %f", lpRtnFuncQhSingleHqPush->PreOpenInterest);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                       PreDelta : %f", lpRtnFuncQhSingleHqPush->PreDelta);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                  FutuOpenPrice : %f", lpRtnFuncQhSingleHqPush->FutuOpenPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                  FutuLastPrice : %f", lpRtnFuncQhSingleHqPush->FutuLastPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                   BuyHighPrice : %f", lpRtnFuncQhSingleHqPush->BuyHighPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                  BuyHighAmount : %f", lpRtnFuncQhSingleHqPush->BuyHighAmount);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                   LowSellPrice : %f", lpRtnFuncQhSingleHqPush->LowSellPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                  LowSellAmount : %f", lpRtnFuncQhSingleHqPush->LowSellAmount);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                  FutuHighPrice : %f", lpRtnFuncQhSingleHqPush->FutuHighPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                   FutuLowPrice : %f", lpRtnFuncQhSingleHqPush->FutuLowPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                 UplimitedPrice : %f", lpRtnFuncQhSingleHqPush->UplimitedPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]               DownlimitedPrice : %f", lpRtnFuncQhSingleHqPush->DownlimitedPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                   AveragePrice : %f", lpRtnFuncQhSingleHqPush->AveragePrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                ChangeDirection : %f", lpRtnFuncQhSingleHqPush->ChangeDirection);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                 BusinessAmount : %f", lpRtnFuncQhSingleHqPush->BusinessAmount);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                   OpenInterest : %f", lpRtnFuncQhSingleHqPush->OpenInterest);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                BusinessBalance : %f", lpRtnFuncQhSingleHqPush->BusinessBalance);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                 FutuClosePrice : %f", lpRtnFuncQhSingleHqPush->FutuClosePrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                    SquarePrice : %f", lpRtnFuncQhSingleHqPush->SquarePrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[   int]                       CurrDate : %d", lpRtnFuncQhSingleHqPush->CurrDate);
                                helper->WriteLog(szMsg);
                        }
                        break;
                    case 33102:
                        {
                                CHundsunFutuRtnFuncQhCombinePushField* lpRtnFuncQhCombinePush = (CHundsunFutuRtnFuncQhCombinePushField*)lpLog->pBuffer;
                                sprintf(szMsg, "[%d %s] OnRtnFuncQhCombinePush", lpLog->ndate, lpLog->sztime);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                      ArbitCode : %s", lpRtnFuncQhCombinePush->ArbitCode);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                   FutuExchType : %s", lpRtnFuncQhCombinePush->FutuExchType);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                      FirstCode : %s", lpRtnFuncQhCombinePush->FirstCode);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[string]                     SecondCode : %s", lpRtnFuncQhCombinePush->SecondCode);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[  char]                      WeaveType : %c", lpRtnFuncQhCombinePush->WeaveType);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                   FutuBuyPrice : %f", lpRtnFuncQhCombinePush->FutuBuyPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                      BuyAmount : %f", lpRtnFuncQhCombinePush->BuyAmount);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                 TotalBuyAmount : %f", lpRtnFuncQhCombinePush->TotalBuyAmount);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                  FutuSellPrice : %f", lpRtnFuncQhCombinePush->FutuSellPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                     SellAmount : %f", lpRtnFuncQhCombinePush->SellAmount);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                TotalSaleAmount : %f", lpRtnFuncQhCombinePush->TotalSaleAmount);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                  FutuHighPrice : %f", lpRtnFuncQhCombinePush->FutuHighPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                   FutuLowPrice : %f", lpRtnFuncQhCombinePush->FutuLowPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]                 UplimitedPrice : %f", lpRtnFuncQhCombinePush->UplimitedPrice);
                                helper->WriteLog(szMsg);
                                sprintf(szMsg, "[double]               DownlimitedPrice : %f", lpRtnFuncQhCombinePush->DownlimitedPrice);
                                helper->WriteLog(szMsg);
                        }
                        break;
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

