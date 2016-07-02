#ifndef _MG_SERVER_SOCKET_H_
#define _MG_SERVER_SOCKET_H_

#include <winsock2.h> 
#include <iostream>
#include <list>

#pragma comment(lib,"ws2_32.lib")

#define PORT 5150

class CMgServerSocket
{
public:
	CMgServerSocket();
	virtual ~CMgServerSocket();

	bool	CreateSockListen();
	bool	Bind();
	bool	Bind(int port);
	bool	Listen(int nBacklog);
// 	bool	Accept();
#ifdef WIN32
	SOCKET	Accept();
	int		Read(SOCKET sock, void* buf, int buflen);
	bool	Send(SOCKET sock, const void* buf, int buflen);
	SOCKET	GetSockListen() { return m_SockListen; }
	SOCKET	GetSockAccept() { return m_SockAccept; }
	
#else
	int		Accept();
	int		Read(int sock, void* buf, int buflen);
	bool	Send(int sock, const void* buf, int buflen);
	int		GetSockListen() { return m_SockListen; }
	int		GetSockAccept() { return m_SockAccept; }
#endif
	void	CloseSockAccept();
	void	ClostSockListen();

protected:
	int					m_nLastError;
	bool				m_bBind;
	bool				m_bAccepted;
	bool				m_bSockListenValid;
	bool				m_bSockAcceptValid;

private:
#ifdef WIN32
	SOCKET				m_SockListen;
	SOCKET				m_SockAccept;
#else
	int					m_SockListen;
	int					m_SockAccept;
#endif
	struct sockaddr_in	m_addrServer;
};

#endif