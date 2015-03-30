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
    static void insert_sort(T sz[], int size);//麻将排序，当前量往前面的有序数组里插
    template <typename T>
    static void shell_sort(T sz[], int size);//步长从大到小的在等步长的一组数中插入排序
    template <typename T>
    static void select_sort(T sz[], int size);//依次从第一个数开始，与后面数比较，较小的数与自己交换
    template <typename T>
    static void bubble_sort(T sz[], int size);//反复元素数-1次，每次都将相邻两数比较，按规则调整位置
    template <typename T>
    static void quick_sort(T sz[], int size);//找第一个数为key.从第一数开始向后，找大于等于key的数，从最后一数向前找小于key的数，找到的数不是同一个数，交换。一直到将key放到中部，然后，以key数位置以左，以右，分组进行同样操作。
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
void sort::insert_sort(T sz[], int size)//麻将排序，当前量往前面的有序数组里插
{
    int i;

    for(i = 0; i < size; i++)
    {
        T temp = sz[i];//当前值抓出来
        int j;

        for(j = i; j - 1 >= 0 && temp < sz[j - 1]; j--)//当前值以前的值需要向后移动
        {
            sz[j] = sz[j - 1];
        }
        sz[j] = temp;//当前值填入
    }
}

template <typename T>
void sort::shell_sort(T sz[], int size)//步长从大到小的在等步长的一组数中插入排序
{
    int i = size/2;//步长

    while(i > 0)
    {
        for(int j = i; j< size; j++)
        {
            T temp = sz[j];//当前值抓出来
            int k;

            for(k = j; k - i >= 0 && temp < sz[k - i]; k -= i)//当前值以前的值需要向后移动
            {
                sz[k] = sz[k-i];
            }
            sz[k] = temp;//当前值填入
        }
        i /= 2;
    }
}

template <typename T>
void sort::select_sort(T sz[], int size)//依次从第一个数开始，与后面数比较，较小的数与自己交换
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
void sort::bubble_sort(T sz[], int size)//反复元素数-1次，每次都将相邻两数比较，按规则调整位置
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
void sort::quick_sort(T sz[], int size)//找第一个数为key.从第一数开始向后，找大于等于key的数，从最后一数向前找小于key的数，找到的数不是同一个数，交换。一直到将key放到中部，然后，以key数位置以左，以右，分组进行同样操作。
{
    if(size > 0)
    {
        int i = 0;
        int j = size - 1;

        T key = sz[i];//key 值选择比较重要，选i，从右边开始，选j，从左边开始，保证key的位置不被忽略掉.否则会出错哦!

        //sort::print_array(sz, size, ">: ");

        while(i < j)
        {
            while(i < j)
            {
                if(sz[j] < key)//从后面找一个小值，与前面的值交换
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
                if(sz[i] > key)//从前面找一个大值，与后面的值交换
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

		//此时i == j!
        if(i > 0)//i左边要不要递归?
        {
            quick_sort(&sz[0], i);
        }

        if(i + 1 < size)//i右边要不要递归?
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
