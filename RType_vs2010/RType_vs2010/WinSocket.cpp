#include "WinSocket.h"

WinSocket::WinSocket(void)
{
}

WinSocket::~WinSocket(void)
{
}

bool	WinSocket::initUDP(int port)
{
	if ((this->sock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0)) == INVALID_SOCKET) 
		return false;
	this->sin.sin_addr.s_addr = inet_addr(INADDR_ANY);
	this->sin.sin_family = AF_INET;
	this->sin.sin_port = htons(port);
	if (bind(this->sock, (SOCKADDR *)&this->sin, sizeof(this->sin)) == SOCKET_ERROR)
	{
		printf("Bind error\n");
		return false;
	}
	return true;
}

bool	WinSocket::initServer(int port)
{
	if ((this->sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0)) == INVALID_SOCKET)
	{
		std::cout << "Error creating socket" << std::endl;
		return false;
	}
	this->sin.sin_addr.s_addr = INADDR_ANY;
	this->sin.sin_family = AF_INET;
	this->sin.sin_port = htons(port);
	if (bind(this->sock, (SOCKADDR *)&this->sin, sizeof(this->sin)) == SOCKET_ERROR)
			{
		std::cout << "Error binding socket" << std::endl;
		return false;
	}
	return true;
}

bool WinSocket::connectToServer(std::string const & host, short port)
{
	if ((this->sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0)) == INVALID_SOCKET) 
		return false;
	this->sin_s.sin_addr.s_addr = inet_addr(host.c_str());
	this->sin_s.sin_family = AF_INET;
	this->sin_s.sin_port = htons(port);
	if (WSAConnect(this->sock, (SOCKADDR *)&this->sin_s, sizeof(this->sin_s), NULL ,NULL ,NULL ,NULL) == SOCKET_ERROR)
		return false;
	return true;
}


bool WinSocket::listenSocket(int backlog)
{
	if (listen(this->sock, backlog) == -1)
		return (false);
	return true;
}

int WinSocket::recData(std::string & buffer , int blocksize){	WSABUF Databuf;	DWORD receive;	DWORD lflag = 0;	Databuf.len = blocksize + 1;	char buf[1024];	Databuf.buf = buf;	receive = blocksize;	int err;	int len;	if (((len = WSARecv(this->sock, &Databuf, 1, &receive, &lflag, NULL, NULL)) ==  SOCKET_ERROR ) 			&& (WSA_IO_PENDING != (err = WSAGetLastError())))			{		wprintf(L"recvfailed with error %u\n", WSAGetLastError());		return (-1);	}	buffer = Databuf.buf;	return len;}int WinSocket::sendData(std::string const & data)
{
	WSABUF Databuf;
	Databuf.len = data.length();
	Databuf.buf = (CHAR * )data.c_str();
	std::cout << "Sending =" << Databuf.buf << std::endl;
	if (WSASend(this->sock, &Databuf, 1, &Databuf.len, 0,0,0) == SOCKET_ERROR)
					{		wprintf(L"send failed with error %u\n", WSAGetLastError());		return (-1);	}
	return 0;
}


bool WinSocket::closeSocket()
{
	if (closesocket(this->sock) == 0)
		return true;
	return false;
}

int WinSocket::recDataFrom(std::string & buffer , int blocksize){	WSABUF Databuf;	DWORD receive;	struct sockaddr_in in;	char buf[1024];	int len;	Databuf.len = blocksize + 1;	Databuf.buf = buf;	receive = blocksize;	int size = sizeof(in);	if ((len = WSARecvFrom(this->sock, &Databuf, 1, &receive, 0, (SOCKADDR *)& in, &size, 0, 0)) ==  SOCKET_ERROR)		return (-1);	buffer = Databuf.buf;	return len;}int WinSocket::sendDataTo(std::string const & data,std::string const &host, int port)
{
	struct sockaddr_in that;
	WSABUF Databuf;
	
	Databuf.len = data.length();
	Databuf.buf = (CHAR * )data.c_str();
	that.sin_family = AF_INET;
	that.sin_port = htons(port);
	that.sin_addr.s_addr = inet_addr(host.c_str());
	int size = sizeof(that);
	if (WSASendTo(this->sock, &Databuf, 1, &Databuf.len, 0,  (SOCKADDR *)& that, size, 0, 0) == SOCKET_ERROR)
		return -1;
	return 0;
}

void	WinSocket::setSocket(int socket)
{
	this->sock = socket;
}

ISocket * WinSocket::acceptedConnection()
{
	SOCKET sock;
	sockaddr_in saClient;
	int iClientSize = sizeof(saClient);

	if ((sock = WSAAccept(this->sock, (SOCKADDR*) &saClient, &iClientSize, NULL, NULL)) == INVALID_SOCKET)
		return (NULL);
	ISocket *nsocket = new WinSocket;
	nsocket->setSocket(sock);
	return (nsocket);
}

int			WinSocket::getSocket() const
{
	return this->sock;
};