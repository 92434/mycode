#include <iostream>
#include <stdio.h>
#include "file_cache.h"
#include <vector>
#include <string>

#define do_exec(expr) \
	do { \
		std::cout << #expr << " is " << expr << std::endl; \
	} while (0)

void pflags() {
	std::ios::fmtflags flags = std::cout.flags();
	printf("%x\n", flags);
}

class Base{
public:
	virtual void foo()=0;
	Base() { call_foo();}
	void call_foo() { foo(); }
};

class Derived: Base{
	void foo() {  }
};

int main(int argc, char **argv) {
	do_exec(sizeof("xiaofei"));
	do_exec(printf("xiaofei"));

	Derived d;
	return 0;
}
