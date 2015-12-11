#include "Lock.h"

CLock::CLock(void)
{
    pthread_mutex_init(&m_Lock,NULL);

}

CLock::~CLock(void)
{
    pthread_mutex_destroy(&m_Lock);

}

bool CLock::Lock()
{
    pthread_mutex_lock(&m_Lock);
    return true;
}

bool CLock::UnLock()
{
    pthread_mutex_unlock(&m_Lock);
    return true;
}
