#include <iostream>
using namespace std;

class outer
{
private:
    static int i;
public:
    void test_outer()
    {
        static int i_outer1 = 1;

        class inner
        {
        public:
            void test_inner(void)
            {
                cout << "outer::inner" << outer::i << i_outer1 << endl;
            };
        };

        inner inner_o;
        inner_o.test_inner();
    }

    class inner_1
    {
    public:
        void test_inner(void)
        {
            cout << "outer::inner_1::test_inner" << outer::i <<endl;
        }
    };
};

int outer::i = 7;

int main()
{
    outer outer_o;
    outer_o.test_outer();
    outer::inner_1 inner_1_o;
    inner_1_o.test_inner();
    return 0;
}
