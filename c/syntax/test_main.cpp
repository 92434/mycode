
/*
|| Using the STL stable_partition algorithm
|| Takes any number of flags on the command line and
|| four filenames in order.
*/
#include <string>
#include <list>
#include <algorithm>
#include <iostream>
using namespace std;
#define abc xiaofei
#define def is hero!
#define _str(x) #x
#define str(x) _str(x)
#define con(x,y) x" "y
int main (int argc, char *argv[])
{
    cout << str(xiaofei) << str(is hero!) << endl;
    cout << con(str(abc),str(def)) << endl;
    for(int i = 0; i < 100; i++)
        __asm__("nop"::);
    return 0;
}
