#include <iostream>
using namespace std;

static inline long long align(long long x, int align)
{
  int sign = x < 0;

  if (sign)
    x = -x;

  x -= x % align;

  if (sign)
    x = -x;

  return x;
}

unsigned short get_byte(char c){
  c = c << 8;
  return c;
  //return c << 8;
}

void test_ull(void){
	unsigned long long ull;
	ull = 0x10000000000000000;
	cout << std::hex;
	cout << ull << endl;
	int i = (~0);
	cout << i << endl;
}


int main(){
	test_ull();
}
