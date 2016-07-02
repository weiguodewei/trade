/**********************************************
 * Filename : TradeCommHeaders.h
 * Author	: Haowc10553
 * Date		: 2015.10.09
 *  
 * Desc		: 交易通讯协议头定义
 **********************************************/
#ifndef _TRADE_COMM_HEADER_H_
#define _TRADE_COMM_HEADER_H_

#include <string>
#include <windows.h>

using namespace std;

////////////////////////////////////////////////////////
const char HEARTBEAT_PACKET[]	= "<root Version=\"1.0\" FuncID=\"999\"/>";
const char QUERY_FUND_ACCOUNT[] = "<root Version=\"1.0\" FuncID=\"1000\"/>";
const char ENTRUST_INFO[] = "<root Version=\"1.0\" FuncID=\"1004\" SessionID=\"1\">\
										<item Code=\"600570\" Market=\"Stock\" Account=\"10000013\" BS=\"Buy\" Dir=\"Open\" Amount=\"100\" Price=\"66.55\" Prop=\"0\"/>\
									</root>";
////////////////////////////////////////////////////////

#define MAX_BUF_LEN     (1024 * 100)

/**********************************************************
*				协议报文总体结构：
***********************************************************
*      报文头     +     协议头      +     协议数据包
* （PacketHeader） （ProtocolHeader）    （ProtocolData）
**********************************************************/

#pragma pack(1)
/***********************************************
 * 交易报文头
 ***********************************************/
struct PackerHeader 
{
	short	HeaderFlag;		// 长度2，包头标志，指定为7611
	int		DataLength;		// 长度4，协议包长度（不包含报文头）
};

/***********************************************
 * 交易协议头
 ***********************************************/
struct ProtocolHeader
{
	int		Action;			// 长度4，功能号，指定为8888
	int		Data1;			// 长度4，保留
	int		Data2;			// 长度4，保留
	int		Data3;			// 长度4，保留
	int		Data4;			// 长度4，保留
	int		DataLength;		// 长度4，协议包数据长度
	int		Data5;			// 长度4，保留
};
#pragma pack()

#endif
