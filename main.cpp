#include "TcpSocket.h"

#define MAX_THREAD 10

int main()
{
    LOG("hello tcp client.\n");
    TCPSocket tcp[50];

    for (int i = 0; i < MAX_THREAD; i++)
    {
        usleep(1000 * 200);
        tcp[i].Init();
        tcp[i].Run();

        LOG("Socket start:%d.\n", i);
    }

    getchar();
    usleep(1000 * 1000);

    for (int j = 0; j < MAX_THREAD; j++)
    {
        usleep(1000 * 200);
        tcp[j].Destory();

        LOG("Socket stop:%d.\n", j);
    }
    return 0;
}
