#include <iostream>
using namespace std;

class A
{
public:
    A()
    {
        cout
                << "A() constructor"
                << endl;
    }
    virtual void f()= 0;
    void g()
    {
        cout
                << "A::g()"
                << endl;
    }
};

class B : public A
{
	public:
    B()
    {
        cout
                << "B() constructor"
                << endl;
    }
    void f(void)
    {
        cout
                << "B::f()"
                << endl;
    }
};

int main()
{
    B b;
	b.f();
	b.g();

    return 0;
}
