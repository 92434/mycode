#include <stdio.h>
#include <iostream>
using namespace std;
static int i = 0;

void f_ex(void)
{
    i += 4;
    cout << "&i(globle2) = " << hex << &i << endl;
}

void g_ex(void)
{
    static int i = 0;
    i += 5;
    cout << "&i(local3) = " << hex << &i << endl;
}

static void h(void)
{
    cout << "static void h() 2" <<endl;
}

void call_h(void)
{
    h();
}
