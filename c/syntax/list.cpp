#include <iostream>
using namespace std;

typedef struct _list_node
{
    struct _list_node *stpNext;
    int iData;
} list_node_t;

const int ciSize = 30;
const int ciCount = 30;
const int ciDuration = 9;

void test_list(void)
{
    list_node_t stsz[ciSize] = {};
    list_node_t *stpItem = 0;
    list_node_t *stpPrevItem = &stsz[ciSize - 1];

    for(int i = 0; i < ciSize; i++)
    {
        stsz[i].iData = i + 1;
        if(i < ciSize - 1)
        {
            stsz[i].stpNext = &stsz[i + 1];
        }
        else
        {
            stsz[i].stpNext = &stsz[0];
        }
    }

    for(int i = 0; i < ciCount; i++)
    {
        int j = 0;

        while(1)
        {
            stpItem = stpPrevItem->stpNext;
            j++;

            if(j < ciDuration)
            {
                stpPrevItem = stpItem;
            }
            else
            {
                break;
            }
        }

        //find it and remove it
        stpPrevItem->stpNext = stpItem->stpNext;
        cout
                << stpItem->iData
                << " ";
    }
    cout
            << endl;
}

class list_node{
	int idata;
	list_node *next_node;
public:
	list_node(int i) : idata(i)
	{

	}
	list_node *get_next_node(void)
	{
		return next_node;
	}
	int get_data(void)
	{
		return idata;
	}
	void set_data(int i)
	{
		idata = i;
	}
};


int main()
{
    test_list();
    return 0;
}
