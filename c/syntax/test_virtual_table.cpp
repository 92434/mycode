#include <iostream>

using namespace std;

class A
{
public:
    A()
    {
        cout << "A::A()" << endl;
    }

    A(A &a)
    {
        cout << "A::A(A &a)" << endl;
    }

    void a3(){
        cout << "A::a3" << endl;
    }

    virtual void a1()
    {
        cout << "A::a1" << endl;
    }

    virtual void a2()
    {
        cout << "A::a2" << endl;
    }
};

class B : public A
{
public:
    B()
    {
        cout << "B::B()" << endl;
    }

    B(B &b)
    {
        cout << "B::B(B &b)" << endl;
    }

    void b3(){
        cout << "B::b3" << endl;
    }

    virtual void b1()
    {
        cout << "B::b1" << endl;
    }

    virtual void b2()
    {
        cout << "B::b2" << endl;
    }
};

typedef void (*pFunc)(void);

int main()
{
    B b;
    A &a = b;

    pFunc func;

    for (int i = 0; i < 5; i++)
    {
        func = *((pFunc *)(int *)*(int *)&b + i);
    }
    cout << "Hello world!" << endl;
    return 0;
}
