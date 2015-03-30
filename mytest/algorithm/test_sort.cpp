#include <iostream>
#include <vector>
#include <iterator>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctime> // To seed random generator
using namespace std;

class sort
{
public:
    template <typename T>
    static void print_array(T sz[], int size, char *cp = "");
    template <typename T>
    static void test_sort(T sz[], int size);

    template <typename T>
    static void insert_sort(T sz[], int size);//�齫���򣬵�ǰ����ǰ��������������
    template <typename T>
    static void shell_sort(T sz[], int size);//�����Ӵ�С���ڵȲ�����һ�����в�������
    template <typename T>
    static void select_sort(T sz[], int size);//���δӵ�һ������ʼ����������Ƚϣ���С�������Լ�����
    template <typename T>
    static void bubble_sort(T sz[], int size);//����Ԫ����-1�Σ�ÿ�ζ������������Ƚϣ����������λ��
    template <typename T>
    static void quick_sort(T sz[], int size);//�ҵ�һ����Ϊkey.�ӵ�һ����ʼ����Ҵ��ڵ���key�����������һ����ǰ��С��key�������ҵ���������ͬһ������������һֱ����key�ŵ��в���Ȼ����key��λ���������ң��������ͬ��������
};

template <typename T>
void sort::print_array(T sz[], int size, char *cp)
{
    cout << cp;
    vector<T> v(sz, sz + size);
    copy(v.begin(), v.end(), ostream_iterator<T> (cout, " "));
    cout << endl;
}

template <typename T>
void sort::test_sort(T sz[], int size)
{
    bool bAsc = true;
    bool bDec = true;

    if(size)
    {
        for(int i = 1; i < size; i++)
        {
            if(sz[i - 1] > sz[i])
            {
                bAsc = false;
                break;
            }
        }

        for(int i = 1; i < size; i++)
        {
            if(sz[i - 1] < sz[i])
            {
                bDec = false;
                break;
            }
        }

        if(bAsc && bDec)
        {
            cout
                << "logic error!"
                << endl;
        }
        else if(bAsc || bDec)
        {
            cout
                << "sort ok!"
                << endl;
        }
        else
        {
            cout
                << "sort error!"
                << endl;
        }
    }
    else
    {
        cout
            << "size = "
            << size
            << endl;
    }

}

template <typename T>
void sort::insert_sort(T sz[], int size)//�齫���򣬵�ǰ����ǰ��������������
{
    int i;

    for(i = 0; i < size; i++)
    {
        T temp = sz[i];//��ǰֵץ����
        int j;

        for(j = i; j - 1 >= 0 && temp < sz[j - 1]; j--)//��ǰֵ��ǰ��ֵ��Ҫ����ƶ�
        {
            sz[j] = sz[j - 1];
        }
        sz[j] = temp;//��ǰֵ����
    }
}

template <typename T>
void sort::shell_sort(T sz[], int size)//�����Ӵ�С���ڵȲ�����һ�����в�������
{
    int i = size/2;//����

    while(i > 0)
    {
        for(int j = i; j< size; j++)
        {
            T temp = sz[j];//��ǰֵץ����
            int k;

            for(k = j; k - i >= 0 && temp < sz[k - i]; k -= i)//��ǰֵ��ǰ��ֵ��Ҫ����ƶ�
            {
                sz[k] = sz[k-i];
            }
            sz[k] = temp;//��ǰֵ����
        }
        i /= 2;
    }
}

template <typename T>
void sort::select_sort(T sz[], int size)//���δӵ�һ������ʼ����������Ƚϣ���С�������Լ�����
{
    int i;
    for(i = 0; i < size; i++)
    {
        int j;

        for(j = i + 1; j < size; j++)
        {
            if(sz[j] < sz[i])
            {
                T temp = sz[i];
                sz[i] = sz[j];
                sz[j] = temp;
            }
        }
    }
}

template <typename T>
void sort::bubble_sort(T sz[], int size)//����Ԫ����-1�Σ�ÿ�ζ������������Ƚϣ����������λ��
{
    int i;
    int j;

    for(i = 0; i < size - 1; i++)
    {
        for(j = 0; j < size - 1; j++)
        {
            if(sz[j + 1] < sz[j])
            {
                T temp = sz[j];
                sz[j] = sz[j + 1];
                sz[j + 1] = temp;
            }
        }
    }
}

template <typename T>
void sort::quick_sort(T sz[], int size)//�ҵ�һ����Ϊkey.�ӵ�һ����ʼ����Ҵ��ڵ���key�����������һ����ǰ��С��key�������ҵ���������ͬһ������������һֱ����key�ŵ��в���Ȼ����key��λ���������ң��������ͬ��������
{
    if(size > 0)
    {
        int i = 0;
        int j = size - 1;

        T key = sz[i];//key ֵѡ��Ƚ���Ҫ��ѡi�����ұ߿�ʼ��ѡj������߿�ʼ����֤key��λ�ò������Ե�.��������Ŷ!

        //sort::print_array(sz, size, ">: ");

        while(i < j)
        {
            while(i < j)
            {
                if(sz[j] < key)//�Ӻ�����һ��Сֵ����ǰ���ֵ����
                {
                    T temp;
                    temp = sz[i];
                    sz[i] = sz[j];
                    sz[j] = temp;

                    //sort::print_array(sz, size);

                    break;
                }
                else
                {
                    j--;
                }
            }

            while(i < j)
            {
                if(sz[i] > key)//��ǰ����һ����ֵ��������ֵ����
                {
                    T temp;
                    temp = sz[i];
                    sz[i] = sz[j];
                    sz[j] = temp;

                    //sort::print_array(sz, size);

                    break;
                }
                else
                {
                    i++;
                }
            }
        }

		//��ʱi == j!
        if(i > 0)//i���Ҫ��Ҫ�ݹ�?
        {
            quick_sort(&sz[0], i);
        }

        if(i + 1 < size)//i�ұ�Ҫ��Ҫ�ݹ�?
        {
            quick_sort(&sz[i + 1], size - i - 1);
        }
    }
}

int main()
{
    srand(time(0)); // Seed random number generator
    const static int count = rand() % 10000;
    const static int range = 26;

    cout
        << "count: "
        << count
        << endl;

    int isz[count];

    char csz[count];

    for(int i = 0; i < count; i++)
    {
        isz[i] = rand() % range;

        csz[i] = rand() % range + 'a';
    }

    cout
        << "time(0): "
        << time(0)
        << endl;

    cout
        << "rand(): "
        << rand()
        << endl;

    sort::print_array(isz, sizeof(isz) / sizeof(int));

    sort::quick_sort(isz, sizeof(isz) / sizeof(int));

    sort::print_array(isz, sizeof(isz) / sizeof(int));

    sort::test_sort(isz, sizeof(isz) / sizeof(int));

#if 0
    sort::print_array(csz, sizeof(csz) / sizeof(char));

    sort::quick_sort(csz, sizeof(csz) / sizeof(char));

    sort::print_array(csz, sizeof(csz) / sizeof(char));

    sort::test_sort(csz, sizeof(csz) / sizeof(char));
#endif

    return 0;
}
