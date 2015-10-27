#include <stdio.h>
#include <iostream>
using namespace std;
class base
{
    int i;
public:
    base()
    {
        i = 0;
    }
    base(int var) : i(var) {}
    int GetValue()
    {
        return i;
    }
    bool operator==(int var)
    {
        return i == var;
    }
    friend bool operator==(int var, base& b)
    {
        return var == b.i;
    }
    base operator++(int)
    {
        base b(i);
        i++;
        return b;
    }
    base& operator++()
    {
        ++i;
        return *this;
    }
    int& operator*()
    {
        return i;
    }
    friend ostream& operator<<(ostream& o, base b)
    {
        return o << b.i;
    }
};

static int i = 0;
int count = 0;

void f(void)
{
    static int i = 0;
    i += 1;
    cout << "&i(local1) = " << hex << &i << endl;
}

void g(void)
{
    static int i = 0;
    i += 2;
    cout << "&i(local2) = " << hex << &i << endl;
}

static void h(void)
{
    cout << "static void h() 1 " << endl;
}

static void (*p)(void) = h;


typedef struct _my_struct
{
    int id;
    char *str;
    int idbak;
    int attr;
} my_struct_t;

my_struct_t my_info[] =
{
    {1,"xiaofei1",0,1},
    {2,"xiaofei2",},
    {3,"xiaofei3"},
    {0,0},
};

int main(int argc, char *argv[])
{
    cout << *base(1) << endl;
    cout << base(5) <<endl;
    while(1)
    {
        extern void f_ex(void);
        extern void g_ex(void);
        extern void call_h(void);
        count++;
        i += 3;
        cout << "&i(globle1) = " << hex << &i << endl;
        f();
        g();
        f_ex();
        g_ex();
        p();
        call_h();
        my_struct_t *pmy_info = my_info;
        while(pmy_info->id)
        {
            cout << pmy_info->id << ": " << pmy_info->str << pmy_info->attr << endl;
            pmy_info++;
        }
        cout << ((-1) ? "1" : "2") << endl;
        cout << ((0) ? "1" : "2") << endl;
        cout << ((1) ? "1" : "2") << endl;
        cout << "ok!" << endl;
    }
    return 0;
}
