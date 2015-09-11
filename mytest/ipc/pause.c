#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int ret ;
    ret=alarm(3) ; /*调用alarm定时器函数*/
    pause() ;
    printf("I have been waken up.\n") ;
    return 0 ;
}
