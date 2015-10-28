#include <iostream>
using namespace std;

class A{
public:
	int i;

	A(){ 
		cout << "A()" << endl;
	}

	A(A &ia){
		cout << "A(A &)" << endl;
		this->i = ia.i;
	}

	operator A*(){//这个，定义的时候已经指定了返回值
		cout << "A*" << endl;
		return this;
	}

	A *operator ->(){//这个，重载了->运算符的涵义
		cout << "->" << endl;
		return this;
	}

	class B{
		
	};

	B get_obj(){
		return B();
	}
};

#define test(x) \
do {\
	cout << ">>>>>>" #x << endl;\
	cout << x << endl;\
} while(0)

A& get_obj(){
	A a;
	a.i = 23;
	return a;
}

void set_obj(A a0){
	A a1;
	a1 = a0;
}
void test_constructor(){
	//set_obj(get_obj());
	A a = get_obj();
	set_obj(a);
}

void test_unary(){
	A a;
	a.i = 23;

	test(((A *)(a))->i);
	test(a->i);
}

int main(){
	//test_unary();
	//test_constructor();
}
