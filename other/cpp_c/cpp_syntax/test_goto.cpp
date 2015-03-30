#include <iostream>
#include <stdio.h>
using namespace std;

#define my_print0(cp, var...) printf(cp, ##var)

#define my_print1(cp, var...) my_print0(cp, ##var)



int main()
{
    //int i;
    my_print0("xiaofei %d\n", 3);
    my_print1("xiaofei\n");
    my_print0("xiaofei %d\n", 3);

    cout << 0UL << endl;

    cout << hex << ~0UL << endl;
    cout << ~0UL << endl;

//    i = 0;
//
//    while(i)
//    {
//label1:
//        cout << 0UL << endl;
//    }
//
//    i = 1;
//    goto label1;

    unsigned char uc1 = 1,uc2 = 253;
    unsigned char uc3 =  uc1 - uc2;
    cout << hex << uc3 << endl;

    unsigned int ui1 = 8,ui2 = 2, ui3 = 0;

    ui3 = ui1 ^ ui2;

    cout << hex << ui3 << endl;

    return 0;
}
