#include "SocketBuffer.h"

CSocketBuffer::CSocketBuffer(void)
    : m_nBufferSize(0x00)
    , m_pHead(0x00)
    , m_pRead(0x00)
    , m_pWrite(0x00)
{
    CLockAuto autolock(m_lock);

    m_pHead = (char*)malloc(sizeof(char) * SIZE_PACKET_MAX * 2);

    if (0x00 != m_pHead)
    {
        m_pRead = m_pHead;
        m_pWrite = m_pHead;
        m_nBufferSize = sizeof(char) * SIZE_PACKET_MAX * 2;
    }

}

CSocketBuffer::~CSocketBuffer(void)
{

}

int CSocketBuffer::GetFreeSize()
{
    int nTmp = m_pRead - m_pWrite;
    if (nTmp <= 0)
    {
        nTmp += m_nBufferSize;
    }

    return nTmp;
}

int CSocketBuffer::GetBusySize()
{
    int nTmp = m_pWrite - m_pRead;
    if (nTmp < 0)
    {
        nTmp += m_nBufferSize;
    }

    return nTmp;
}

bool CSocketBuffer::SetLimitByteSize(int nLimit)
{
    CLockAuto autolock(m_lock);

    if (0x00 != m_pHead)
    {
        free(m_pHead);
        m_pHead = 0x00;
    }
    m_pHead = (char*)malloc(sizeof(char) * nLimit);

    if (0x00 != m_pHead)
    {
        m_pRead = m_pHead;
        m_pWrite = m_pHead;
        m_nBufferSize = sizeof(char) * nLimit;
        return true;
    }
    else
    {
        return false;
    }
}

int CSocketBuffer::GetNextPackSize()
{
    CLockAuto autolock(m_lock);

    int nResult = 0;
    int nInt = sizeof(int);
    if (GetBusySize() < sizeof(int))
    {
        return 0;
    }
    cout << m_pRead - m_pHead << "%%%%" << (int)*m_pRead << "^^^" <<endl;

    int nRight = m_pHead + m_nBufferSize - m_pRead;
    if (nRight < nInt)
    {
        int nLeftSize = nInt - nRight;
        if (nRight != 0)
        {
            memcpy((char*)&nResult, m_pRead, nRight);
        }
        memcpy((char*)&nResult + nRight, m_pHead, nLeftSize);
    }
    else
    {
        memcpy((char*)&nResult, m_pRead, nInt);

    }

    return nResult;
}

bool CSocketBuffer::PushByte(char* pBuf, int nBufSize)
{
    CLockAuto autolock(m_lock);

    // 写入数据长度非法则返回
    if (nBufSize <= 0)
    {
        return false;
    }
    // 缓冲区数据不足读取长度则返回
    int nFreeSize = GetFreeSize();
    if (nFreeSize <= nBufSize)
    {
        return false;
    }
    // 写入数据
    int nRightSize = m_pHead + m_nBufferSize - m_pWrite;
    if (nRightSize < nBufSize)
    {
        int nLeftSize = nBufSize - nRightSize;
        if (nRightSize != 0)
        {
            memcpy(m_pWrite, pBuf, nRightSize);
        }
        memcpy(m_pHead, pBuf + nRightSize, nLeftSize);
        m_pWrite = m_pHead + nLeftSize;
    }
    else
    {
        memcpy(m_pWrite, pBuf, nBufSize);
        m_pWrite += nBufSize;

    }

    return true;
}

bool CSocketBuffer::PopByte(char* pBuf, int nBufSize)
{
    CLockAuto autolock(m_lock);

    // 读取数据长度非法则返回
    if (nBufSize <= 0)
    {
        return false;
    }
    // 缓冲区数据不足读取长度则返回
    int nBusySize = GetBusySize();
    if (nBusySize < nBufSize)
    {
        return false;
    }
    // 读取数据
    int nRightSize = m_pHead + m_nBufferSize - m_pRead;
    if (nRightSize < nBufSize)
    {
        int nLeftSize = nBufSize - nRightSize;
        if (nRightSize != 0)
        {
            memcpy(pBuf, m_pRead, nRightSize);
        }
        memcpy(pBuf + nRightSize, m_pHead, nLeftSize);
        m_pRead = m_pHead + nLeftSize;
    }
    else
    {
        memcpy(pBuf, m_pRead, nBufSize);
        m_pRead += nBufSize;

    }

    return true;
}

bool CSocketBuffer::PopPacket(char* pBuf)
{
    CLockAuto autolock(m_lock);

    // 制作下个包的长度保存在nResult中
    int nResult = 0;
    int nInt = sizeof(int);
    if (GetBusySize() < sizeof(int))
    {
        return 0;
    }
    //cout << m_pRead - m_pHead << "%%%%" << (int)*m_pRead << "^^^" <<endl;

    int nRight = m_pHead + m_nBufferSize - m_pRead;
    if (nRight < nInt)
    {
        int nLeftSize = nInt - nRight;
        if (nRight != 0)
        {
            memcpy((char*)&nResult, m_pRead, nRight);
        }
        memcpy((char*)&nResult + nRight, m_pHead, nLeftSize);
    }
    else
    {
        memcpy((char*)&nResult, m_pRead, nInt);

    }

    // 读取数据长度非法则返回
    if (nResult <= 0)
    {
        return false;
    }
    // 缓冲区数据不足读取长度则返回
    int nBusySize = GetBusySize();
    if (nBusySize < nResult)
    {
        return false;
    }
    // 读取数据
    int nRightSize = m_pHead + m_nBufferSize - m_pRead;
    if (nRightSize < nResult)
    {
        int nLeftSize = nResult - nRightSize;
        if (nRightSize != 0)
        {
            memcpy(pBuf, m_pRead, nRightSize);
        }
        memcpy(pBuf + nRightSize, m_pHead, nLeftSize);
        m_pRead = m_pHead + nLeftSize;
    }
    else
    {
        memcpy(pBuf, m_pRead, nResult);
        m_pRead += nResult;

    }

    return true;
}
