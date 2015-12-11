#include "TcpSocket.h"

void* TCPSocket::ThreadTCPRecv(void* param)
{
    char buf[SIZE_PACKET_MAX];
    CTimeVal now;
    CSocketPacket packet;
    stuThreadParam* pStu = (stuThreadParam*)param;
    CSocketBuffer* pBuf = (CSocketBuffer*)pStu->pBuffer;

    int nPa1 = 0;
    int nPa2 = 0;
    int nResult = 0;
    char* pStr = 0x00;
    int nStr = 0;
    long lTimeDiff = 0;

    while (*pStu->pOnOff)
    {
        usleep(1000 * 10);
        int nRet = recv(*pStu->pSocket, buf, SIZE_PACKET_MAX, 0);
        if (nRet <= 0)
        {
            LOG("Recv Error.%s\n", strerror(errno));
        }
        else
        {
            pBuf->PushByte(buf, nRet);
            while(pBuf->PopPacket(packet.setTotalData()))
            {
                if (packet.isChecked())
                {
                    nPa1 = packet.pop_int();
                    nPa2 = packet.pop_int();
                    nResult = packet.pop_int();
                    packet.pop_byteArray(pStr, nStr);
                    lTimeDiff = now.getTotalMSec() - packet.pop_long();
                    LOG("Thr:%d,Soc:%d,nPa1:%d,nPa2:%d,nRes:%d,Text:%s,TimDiff:%d.\n",
                        pthread_self(), *pStu->pSocket, nPa1, nPa2, nResult, pStr, lTimeDiff);
                    //LOG("Thr:%d,Soc:%d,nPa1:%d,nPa2,bRes:.\n",
                    //    pthread_self(), *pStu->pSocket, nPa1, nPa2, nResult);
                }
                else
                {
                    LOG("Packet Recv is not Checked.\n");
                }
            }
        }
    }
}

void* TCPSocket::ThreadTCPSend(void* param)
{
    long lAdd = 0;
    CTimeVal now;
    CSocketPacket packet;
    stuThreadParam* pStu = (stuThreadParam*)param;

    char strTest[1024];
    strcpy(strTest, "hello epoll.this is a test, and I want to know you have more more buffer to save me..thank you ~..");

    while(*pStu->pOnOff)
    {
        usleep(1000 * 10);
        packet.formatData();

        packet.set_CMD(SERVER_CMD_TEST);
        //packet.push_int(rand() % 100);
        //packet.push_int(rand() % 100);
        //packet.push_long(now.getTotalMSec());
        packet.push_long(lAdd);
        packet.push_byteArray(strTest, strlen(strTest) + 1);
        lAdd++;

        packet.push_finish();

        if (packet.isCompleted())
        {
            int nRet = send(*pStu->pSocket, packet.getTotalData(), packet.get_Length(), 0);
            if (nRet <= 0)
            {
                LOG("Send Error.%s\n", strerror(errno));
            }
        }
        else
        {
            LOG("Packet Send is not completed.\n");
        }
    }
    LOG("[LAST]The Last Packet ID is : %d.\n", lAdd);
}

TCPSocket::TCPSocket()
    : m_Socket(0)
    , m_bThreadTCPRecv(false)
    , m_bThreadTCPSend(false)
{
    m_parRecv.pSocket = &m_Socket;
    m_parRecv.pOnOff = &m_bThreadTCPRecv;
    m_parRecv.pBuffer = &m_buffer;

    m_parSend.pSocket = &m_Socket;
    m_parSend.pOnOff = &m_bThreadTCPSend;
    m_parSend.pBuffer = &m_buffer;
}

TCPSocket::~TCPSocket()
{

}

bool TCPSocket::SetServerAddr(char* pIP, int nPort)
{

    return true;
}

bool TCPSocket::MaskSignal()
{
//    struct sigaction sa;
//    sa.sa_handler = SIG_IGN;
//    sa.sa_flags = 0;
//    if ((-1 == sigemptyset(&sa.sa_mask)) || (-1 == sigaction(SIGPIPE, &sa, 0)))
//    {
//        LOG("ignore signal pipe is error.\n");
//        return false;
//    }
    sigset_t signal_mask;
    sigemptyset(&signal_mask);
    sigaddset(&signal_mask, SIGPIPE);
    int rc = pthread_sigmask(SIG_BLOCK, &signal_mask, NULL);
    if (0 != rc)
    {
        LOG("mask signal pipe error.\n");
        return false;
    }
    return true;
}

bool TCPSocket::CreateSocket()
{
    m_Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == m_Socket)
    {
        LOG("[TCPSocket]Create socket error.\n");
        return false;
    }
    return true;
}

bool TCPSocket::ConnectSocket(char* pIP, int nPort)
{
    sockaddr_in addrServer;

    memset(&addrServer, 0, sizeof(addrServer));
    addrServer.sin_family = AF_INET;
    addrServer.sin_addr.s_addr = inet_addr(pIP);
    addrServer.sin_port = htons(nPort);

    /* 尝试连线*/
    if (connect(m_Socket, (struct sockaddr *)&addrServer,sizeof(sockaddr)) < 0)
    {
        LOG("[TCPSocket]Connect socket error.\n")  ;
        return false;
    }

    return true;
}

bool TCPSocket::Init()
{
    if (!MaskSignal())
    {
        return false;
    }
    if (!CreateSocket())
    {
        return false;
    }

    return true;
}

bool TCPSocket::Run()
{
    if (!ConnectSocket(SERVER_IP, SERVER_PORT))
    {
        return false;
    }

    m_bThreadTCPRecv = true;
    pthread_t ptdTCPRecv;
    pthread_create(&ptdTCPRecv, NULL, ThreadTCPRecv, &m_parRecv);

    m_bThreadTCPSend = true;
    pthread_t ptdTCPSend;
    pthread_create(&ptdTCPSend, NULL, ThreadTCPSend, &m_parSend);

    return true;
}

bool TCPSocket::Destory()
{
    m_bThreadTCPRecv = false;
    m_bThreadTCPSend = false;
    //睡眠2秒钟，等待线程结束
    usleep(1000 * 1000 * 2);

    close(m_Socket);

    return true;
}
