#include "stdafx.h"
#include "MgServerSocket.h"
#include <Winsock2.h>

const int gReceiveTimeOut = 5;

CMgServerSocket::CMgServerSocket(void)
{
#ifdef WIN32
	WSADATA wsadata;
	WSAStartup( MAKEWORD( 2, 0 ), &wsadata );
#endif
	m_bSockListenValid = false;
	m_bSockAcceptValid = false;
	m_bBind = false;
	m_bAccepted = false;
	m_nLastError = 0;
}

CMgServerSocket::~CMgServerSocket(void)
{
	ClostSockListen();
}

bool CMgServerSocket::CreateSockListen()
{
#ifdef WIN32
	m_SockListen = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if ( m_SockListen == INVALID_SOCKET )
	{
#else
	m_SockListen = socket( AF_INET, SOCK_STREAM, 0 );
	if ( m_SockListen == -1 ) 
	{
#endif
		m_nLastError = 0x01;
		return false;
	}
	else 
	{
		m_bSockListenValid = true;
		return true;
	}
}

bool CMgServerSocket::Bind( void )
{
	m_addrServer.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons( PORT );
	

	if (SOCKET_ERROR == bind( m_SockListen, ( struct sockaddr* ) & m_addrServer, sizeof( struct sockaddr ) ) )
	{
		//printf("Error at socket(): %ld\n", WSAGetLastError());
		return false;
	}
	else
	{
		m_bBind = true;
		return true;
	}
}

bool CMgServerSocket::Bind(int port)
{
	m_addrServer.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	m_addrServer.sin_family = AF_INET;
	m_addrServer.sin_port = htons( port );
	

	if (SOCKET_ERROR == bind( m_SockListen, ( struct sockaddr* ) & m_addrServer, sizeof( struct sockaddr ) ) )
	{
		//printf("Error at socket(): %ld\n", WSAGetLastError());
		return false;
	}
	else
	{
		m_bBind = true;
		return true;
	}

}

bool CMgServerSocket::Listen(int nBacklog)
{
	if ( SOCKET_ERROR == listen( m_SockListen, nBacklog ) )
	{
		//printf("Error at socket(): %ld\n", WSAGetLastError());
		return false;
	}
	return true;
}

#ifdef WIN32
SOCKET CMgServerSocket::Accept()
#else
int CMgServerSocket::Accept()
#endif
{
	int len;
	len = sizeof( struct sockaddr );
	m_SockAccept = accept( m_SockListen, ( struct sockaddr* ) & m_addrServer, &len );
	if ( m_SockAccept == INVALID_SOCKET )
	{
		//printf("Error at socket(): %ld\n", WSAGetLastError());
		return 0;
	}
	else
	{
		m_bAccepted = true;
		return m_SockAccept;
	}
}

//return bytes
#ifdef WIN32
int CMgServerSocket::Read(SOCKET sock, void* buf, int buflen)
#else
int CMgServerSocket::Read(int sock, const void* buf, int buflen)
#endif
{
	if ( buflen <= 0 || buf == NULL ) 
	{
		m_nLastError = 0x0C;
		return -1;
	}

#ifdef WIN32
	int nret = recv(sock, (char*)buf, buflen, 0);
#else
	int nret = recv(sock, buf, buflen, 0);
#endif
	if (nret < 0) 
	{
		m_nLastError = 0x0D;
		CloseSockAccept();
		return -1;
	}
	else if ( nret == 0 ) 
	{
		m_nLastError = 0x0E;
		CloseSockAccept();
		return 0;
	}
	return nret;
}

#ifdef WIN32
bool CMgServerSocket::Send(SOCKET sock, const void* buf, int buflen)
#else
bool CMgServerSocket::Send(int sock, const void* buf, int buflen)
#endif
{
	int pos = 0;
	int nRet = 0;
	char* movehead = (char*)buf;

	while (pos < buflen) 
	{

#ifdef WIN32
		nRet = send(sock, (char*)(movehead + pos), buflen - pos, 0 /*flags*/ );
#else
		nRet = send(sock, (void*)(movehead + pos), buflen - pos, 0 /*flags*/ );
#endif
		if (nRet == 0) 
		{
			m_nLastError = 0x08;
			std::cout<<"0x08: send find connection closed;"<<std::endl;
			CloseSockAccept();
			return false;
		}
		else if (nRet < 0) 
		{
			m_nLastError = 0x09;
			std::cout<<"0x09: send find network error;"<<std::endl;
			CloseSockAccept();
			return false;
		}
		else 
		{
			pos += nRet;
			if ( pos != buflen ) 
			{
				continue;
			}
			else 
			{
				return true;
			}
		}
	}
	m_nLastError = 0x0A;
	return false;
}

void CMgServerSocket::CloseSockAccept()
{
	if ( m_bSockAcceptValid ) 
	{
// 		if ( m_bAccepted )
			shutdown( m_SockAccept, 0 );
#ifdef WIN32
		closesocket( m_SockAccept );
#else
		close( m_SockAccept );
#endif
		m_bSockAcceptValid = false;
		m_bAccepted = false;
	}
}

void CMgServerSocket::ClostSockListen()
{
	if ( m_bSockListenValid ) 
	{
		if ( m_bBind )
		{
			shutdown( m_SockListen, 0 );
			m_bBind = false;
		}
#ifdef WIN32
		closesocket( m_SockListen );
#else
		close( m_SockListen );
#endif
	}
#ifdef WIN32
	WSACleanup();
#endif
	m_bSockListenValid = false;
}
