#ifndef CTIMEVAL_H
#define CTIMEVAL_H

#include <time.h>
#include <sys/time.h>               // 时间
#include "global.h"

// (32bit)2147483647 / 31536000 = 68       1970 + 68 = 2038year
class CTimeVal
{
public:
    CTimeVal();
    CTimeVal(struct timeval tm);
    CTimeVal(signed long tv_sec, signed long tv_usec);
    ~CTimeVal();

private:

public:
    signed long m_tv_sec;
    signed long m_tv_usec;
    CTimeVal operator + (CTimeVal &tm);
    CTimeVal operator - (CTimeVal &tm);

    bool getHereTime();
    signed long getNowSec();
    signed long getNowUSec();
    signed long getTotalMSec();
    signed long getTotalUSec();
};

#endif // CTIMEVAL_H
