#include <stdio.h>
#include <malloc.h>

extern int end, etext, edata;
extern int _end, _etext, _edata;
int main(void)
{
    void *p = 0;
    unsigned int i;

    if(!p)
    {
        printf("1\n");
    }

    if((p = malloc(100)) == 0)
    {
        printf("!");
    }
    printf("%08x\n", (unsigned int)p);
    if(!p)
    {
        printf("2\n");
    }
    i = !p;
    printf("%08x\n", i);
    free(p);


    printf("%p, %p, %p\n", end, etext, edata);
    //printf("%p, %p, %p\n", _end, _etext, _edata);
    return 0;
}
