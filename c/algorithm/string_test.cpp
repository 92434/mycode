#include "stdio.h"
#include <iostream>
#include <string>
#include <iterator>
#include <algorithm>
using namespace std;

int main()
{
    cout << "hello" << endl;

    string strSrc("xiaofeiskjf;adkfjlfjsklfj\\sfj\\sdfj\\fsakdfj\\sdf");

    string str;

    unsigned int uiPos;

    str = strSrc;
    cout << str << endl;
    while(str.npos != (uiPos = str.rfind('\\')))
    {
        str.erase(str.begin() + uiPos, str.end());
        cout << str << endl;
    }

    str = strSrc;
    cout << str << endl;
    while(str.npos != (uiPos = str.find_last_not_of('s')))
    {
        str.erase(str.begin() + uiPos, str.end());
        cout << str << endl;
    }

    str = strSrc;
    replace(str.begin(), str.end(), '\\', '\r');

    return 0;
}
