#include <iostream>
using namespace std;

class c1
{

public:
    int i;

    c1() : i(7)
    {
        cout
                << "c1() constructor"
                << endl;
    }

    c1(int ip) : i(ip)
    {
        cout
                << "c1(int) constructor"
                <<endl;
    }

    c1(c1& oc1) : i(7)
    {
        cout
                << "c1(c1&) constructor"
                << endl;
    }

    void operator()()
    {
        i = 7;
        cout
                << "operator()"
                << endl;
    }

    c1& operator=(c1& oc1)
    {
        i = 7;
        cout
                << "operator="
                << endl;
        return oc1;
    }

    virtual void f()
    {
        cout << "c1 f"
             << " "
             << "i="
             << i
             << endl;
    }

    virtual void pv() = 0;

    void f(int ip)
    {
        cout << "c1 f int"
             << endl;
    }

    void f(int ip1, int ip2)
    {
        cout << "c1 f int int"
             <<endl;
    }

    void f(float fp)
    {
        cout << "c1 f float"
             << endl;
    }
};

class c2 : public c1
{
public:
    int i;
    int i1;
    int i2;
    static int si;
    static int sisz[20];
    const int ci;

    c2() : ci(6)
    {
        i = 2;
        si = 66;

        cout
                << "c2() constructor"
                << endl;
    }

    c2(int ip1) : i1(ip1), ci(ip1)
    {
        i = 2;
        si = 66;
        cout
                << "c2(int) constructor"
                << endl;
    }

    c2(int ip1, int ip2) : i1(ip1), i2(ip2), ci(ip2)
    {
        i = 2;
        si = 66;
        cout
                << "c2(int, int) constructor"
                << endl;
    }
    void pv()
    {
        cout
            << "c2 pv()"
            << endl;
    }

//    void f() const
//    {
//        cout
//                << "c2 f"
//                << " "
//                << "i="
//                << i
//                << endl;
//    }


    class c3
    {
    public:
        c3()
        {
            cout
                    << "c2::c3() constructor"
                    << endl;
        }
    };
};

class c4 : public c2
{
public:
    c4()
    {
        cout
                << "c4() constructor"
                << endl;
    }
    void f()
    {
        cout
                << "c4 f"
                << endl;
    }

};

int c2::si;
int c2::sisz[];

///////////////////////////////////////////////////////////////////////
// Define a class which can't be derived from
///////////////////////////////////////////////////////////////////////
class MakeFinal
{
    friend class FinalClass2;

private :
    MakeFinal() {}
    ~MakeFinal() {}
};

class FinalClass2 : virtual public MakeFinal
{
public :
    FinalClass2() {}
    ~FinalClass2() {}
};

int main()
{
    c2 c2o(3, 4);
    //c1 c1o;

    c2o.f();
    return 0;
}
