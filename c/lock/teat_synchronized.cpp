#include "Mutex.h"
#include "lock.h"

Mutex mutex1;//我的互斥量
into thread_count = 0;
DWORD CALLBACK thread_proc(LPVOID params)
{
    for(int i = 0; i < 10; ++i)
    {
            synchronized(mutex1)//这里同步！
            {
                for(char c = 'A'; c <= 'Z'; ++c)
                {
                    printf("%c",c);
                    Sleep(2);
                }
                printf("\n");
            }
    }
    thread_count--;
    return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{
    thread_count = 4;
    CreateThread(0, 0, thread_proc, 0, 0, 0);
    CreateThread(0, 0, thread_proc, 0, 0, 0);
    CreateThread(0, 0, thread_proc, 0, 0, 0);
    CreateThread(0, 0, thread_proc, 0, 0, 0);
    while (thread_count) 
        Sleep(0);
    getchar();
    DeleteCriticalSection(&g_cs);
    return 0;

｝

