//file RBTree.cpp   
//written by saturnman��20101008��   
//updated by July��20110329��   
  
//���е�ͷ�ļ����Ѳ��룬����������ֱ�Ӹ��ƴ˷�Դ���ϻ���֤�ˣ���Ȩ���У���Ȩ�ؾ�����   
//July��updated��2011.05.06��   
#include<iostream>   
#include<algorithm>   
#include<iterator>   
#include<vector>   
#include<sstream>   
//#include"RBTree.h"    //���.h�ļ�����cpp�ļ�����һ���ļ���˾�ȥ��   
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
        cout<<"insert:"<<v[i]<<endl;   //��ӽ��  
    }  
    for(i=0;i<v.size();++i)  
    {  
        cout<<"Delete:"<<v[i]<<endl;  
        tree.Delete(v[i]);             //ɾ�����  
        tree.InOrderTraverse();  
    }  
    cout<<endl;  
    tree.InOrderTraverse();*/  
    return 0;   
}  
