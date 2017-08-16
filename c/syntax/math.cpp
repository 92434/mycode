#include <iostream>
#include <stdio.h>

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
	ull = 0x1000000000000000;
	std::cout << std::hex;
	std::cout << ull << std::endl;
	int i = (~0);
	std::cout << i << std::endl;
}


int main(){
	test_ull();
	printf("-0.1 > 0:%s\n", -0.1 > 0 ? "true" : "false");
	printf("-0.1 < 0:%s\n", -0.1 < 0 ? "true" : "false");
	printf("0.0 < 1e-10:%s\n", 0.0 < 1e-10 ? "true" : "false");
	printf("-0.0 == 0.0:%s\n", -0.0 == 0.0 ? "true" : "false");
}
