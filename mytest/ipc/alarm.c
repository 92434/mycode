#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void handler(int signum) {  /*信号处理函数*/
    printf("hello\n");
}

int main() {
    int i;
    int time ;

    signal(SIGALRM,handler);  /*注册SIGALRM信号处理方式*/

    alarm(3);

    for(i=1;i<5;i++){
        printf("sleep %d ...\n", i);
        sleep(1);
    }

    alarm(3);

    sleep(2);

    time=alarm(0) ; /*取消SIGALRM信号，返回剩余秒数*/
    printf("time=%d\n",time) ;

    for(i=1;i<3;i++){
        printf("sleep %d ...\n", i);
        sleep(1);
    }

    return 0 ;
}
