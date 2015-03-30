#include <iostream>
#include <string.h>
#include <map>
using namespace std;

class A{
public:
  A(){ 
  }
};

class B : public A{
public:
  B(){ 
  }
};


int main(){
  string str = "xiaofei";
  
  cout << str << endl;


  str.erase(str.length() - 3);
  cout << str << endl;

  map<int,int> map_map;
  
  map_map[1] = 1;
  //map_map[2] = 2;
  map_map[3] = 3;
  map_map[4] = 4;
  
  cout << (map_map.lower_bound(4) != map_map.end()) << endl;
  cout << (map_map.lower_bound(2) != map_map.end()) << endl;
  cout << map_map.lower_bound(2)->second << endl;
}
