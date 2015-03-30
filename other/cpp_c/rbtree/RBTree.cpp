//file RBTree.cpp   
//written by saturnman，20101008。   
//updated by July，20110329。   
  
//所有的头文件都已补齐，现在您可以直接复制此份源码上机验证了（版权所有，侵权必究）。   
//July、updated，2011.05.06。   
#include<iostream>   
#include<algorithm>   
#include<iterator>   
#include<vector>   
#include<sstream>   
//#include"RBTree.h"    //如果.h文件，和cpp文件放在一个文件里，此句去掉   
using namespace std;   
  
int main()   
{   
    RB_Tree<int,int> tree;   
  
    tree.Insert(12, 12);   
    tree.Insert(1, 1);   
    tree.Insert(9, 9);   
    tree.Insert(2, 2);   
    tree.Insert(0, 0);   
    tree.Insert(11, 11);   
    tree.Insert(7, 7);   
  
  
    tree.Delete(9);   
  
    tree.PrintTree();   
    /*vector<int> v;  
      
    for(int i=0;i<20;++i)  
    {  
        v.push_back(i);  
    }  
    random_shuffle(v.begin(),v.end());  
    copy(v.begin(),v.end(),ostream_iterator<int>(cout," "));  
    cout<<endl;  
    stringstream sstr;  
    for(i=0;i<v.size();++i)  
    {  
        tree.Insert(v[i],i);  
        cout<<"insert:"<<v[i]<<endl;   //添加结点  
    }  
    for(i=0;i<v.size();++i)  
    {  
        cout<<"Delete:"<<v[i]<<endl;  
        tree.Delete(v[i]);             //删除结点  
        tree.InOrderTraverse();  
    }  
    cout<<endl;  
    tree.InOrderTraverse();*/  
    return 0;   
}  
