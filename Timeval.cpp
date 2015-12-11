#include "Timeval.h"

CTimeVal::CTimeVal()
{
    m_tv_sec = 0;
    m_tv_usec = 0;
}

CTimeVal::CTimeVal(timeval tm)
{
    m_tv_sec = tm.tv_sec;
    m_tv_usec = tm.tv_usec;
}

CTimeVal::CTimeVal(signed long tv_sec, signed long tv_usec)
{
    m_tv_sec = tv_sec;
    m_tv_usec = tv_usec;
}

CTimeVal::~CTimeVal()
{

}

////////////////////////////////////////////////////////////////////////////////
/// 名   称：CTimeVal::operator +
/// 功   能：对CTimeVal类型数据重载加号的处理
/// 参   数：CTimeVal &tm
/// 返回 值：CTimeVal                   public
////////////////////////////////////////////////////////////////////////////////
CTimeVal CTimeVal::operator + (CTimeVal &tm)
{
    CTimeVal tmp;
    tmp.m_tv_sec = m_tv_sec + tm.m_tv_sec;
    tmp.m_tv_usec = m_tv_usec + tm.m_tv_usec;
    if (tmp.m_tv_usec >= 1000000)
    {
        tmp.m_tv_sec += 1;
        tmp.m_tv_usec -= 1000000;
    }
    return tmp;
}

////////////////////////////////////////////////////////////////////////////////
/// 名   称：CTimeVal::operator -
/// 功   能：对CTimeVal类型数据重载减号的处理
/// 参   数：CTimeVal &tm
/// 返回 值：CTimeVal                   public
////////////////////////////////////////////////////////////////////////////////
CTimeVal CTimeVal::operator - (CTimeVal &tm)
{
    CTimeVal tmp;
    tmp.m_tv_sec = m_tv_sec - tm.m_tv_sec;
    tmp.m_tv_usec = m_tv_usec - tm.m_tv_usec;
    if (tmp.m_tv_usec < 0)
    {
        tmp.m_tv_sec -= 1;
        tmp.m_tv_usec += 1000000;
    }
    return tmp;
}

////////////////////////////////////////////////////////////////////////////////
/// 名   称：CTimeVal::getHereTime
/// 功   能：获取当前系统时间，为1970年到现在的秒数、及零头微秒
/// 参   数：void
/// 返回 值：bool                       public
////////////////////////////////////////////////////////////////////////////////
bool CTimeVal::getHereTime(void)
{
    struct timeval tm;
    gettimeofday(&tm, NULL);
    m_tv_sec = tm.tv_sec;
    m_tv_usec = tm.tv_usec;

    return true;
}

////////////////////////////////////////////////////////////////////////////////
/// 名   称：CTimeVal::getNowSec
/// 功   能：存储时间秒数
/// 参   数：void
/// 返回 值：signed long                 public
////////////////////////////////////////////////////////////////////////////////
signed long CTimeVal::getNowSec(void)
{
    getHereTime();
    return m_tv_sec;
}

////////////////////////////////////////////////////////////////////////////////
/// 名   称：CTimeVal::getNowUSec
/// 功   能：存储时间秒数的零头微秒
/// 参   数：void
/// 返回 值：signed long                 public
////////////////////////////////////////////////////////////////////////////////
signed long CTimeVal::getNowUSec(void)
{
    getHereTime();
    return m_tv_usec;
}

////////////////////////////////////////////////////////////////////////////////
/// 名   称：CTimeVal::getTotalMSec
/// 功   能：存储时间的总共毫秒
/// 参   数：void
/// 返回 值：signed long                 public
////////////////////////////////////////////////////////////////////////////////
signed long CTimeVal::getTotalMSec()
{
    getHereTime();
    return (m_tv_sec * 1000) + (long)(m_tv_usec / 1000);
}

////////////////////////////////////////////////////////////////////////////////
/// 名   称：CTimeVal::getNowUSec
/// 功   能：存储时间的总共微秒
/// 参   数：void
/// 返回 值：signed long                 public
////////////////////////////////////////////////////////////////////////////////
signed long CTimeVal::getTotalUSec()
{
    getHereTime();
    return (m_tv_sec * 1000000) + m_tv_usec;
}
