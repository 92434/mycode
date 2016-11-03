#include <iostream>
#include <stdio.h>
using namespace std;

#define my_print0(cp, var...) printf(cp, ##var)

#define my_print1(cp, var...) my_print0(cp, ##var)

int main()
{
    my_print0("xiaofei %d\n", 3);
    my_print1("xiaofei\n");
    my_print0("xiaofei %d\n", 3);

    my_print0("xiaofei %s\n", XIAOFEI);
    return 0;
}

static char xiaofei[] = "xiaofei";
