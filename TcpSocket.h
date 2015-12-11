#ifndef __TCP_SOCKET_H__
#define __TCP_SOCKET_H__

#include "SocketPacket.h"
#include "SocketBuffer.h"

class TCPSocket
{
public:
    TCPSocket();
    ~TCPSocket();

private:
    int m_Socket;
    bool m_bThreadTCPRecv;
    bool m_bThreadTCPSend;
    stuThreadParam m_parRecv;
    stuThreadParam m_parSend;

    CSocketBuffer m_buffer;

    static void* ThreadTCPRecv(void* param);
    static void* ThreadTCPSend(void* param);

    bool SetServerAddr(char* pIP, int nPort);
    bool MaskSignal();
    bool CreateSocket();
    bool ConnectSocket(char* pIP, int nPort);

public:
    bool Init();
    bool Run();
    bool Destory();
};

#endif // TCPSOCKET_H
