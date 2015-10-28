#include <iostream>
using namespace std;

namespace xiaofei
{
class c1
{
    int i;
public:
    c1() : i(3)
    {
        cout << "init c1()"
             << " i="
             << i
             << endl;
    }

    c1(int i) : i(i)
    {
        cout
                << "init c1(int)"
                << " i="
                << i
                << endl;
    }

    c1(c1& oc1)
    {
        i = oc1.i;
        cout
                << "copy constructor"
                << endl;
    }

    void f() const
    {
        static int ii;
        cout
                << "f() "
                << endl;
    }
};
}

using namespace xiaofei;
int main()
{
    int iaa[3][3] = {1,2,3,4,5,6,7,8,9};

    int (*a)[3];//只知道3列
    int *b[3];//只知道3行
    int i = 1;
    int const *cip = &i;
    //(*cip)++;
    i++;

    a = iaa;
    b[0] = iaa[0];
    b[1] = iaa[1];
    cout
            << b[1][3]
            << endl;
    cout
            << iaa[1][3]
            << endl;

    c1 oc11;
    c1 oc12(oc11);

    oc11.f();
    return 0;
}

