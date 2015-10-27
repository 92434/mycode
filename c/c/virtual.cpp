#include <iostream>
#include <vector>
using namespace std;

class C{
public:
	C(){}
	virtual void exec_fun(void) = 0;//{cout << "C::exec_func" << endl;}
};

class E{
	static std::vector<C *> vec;
public:
	static void push_item(C *c){
		vec.push_back(c);
	}

	static void exec_all(){
		std::vector<C *>::iterator it = vec.begin();
		for(it; it != vec.end(); it++){
			(*it)->exec_fun();
		}
	}
};

template<class T>
class D : public C{
typedef void (T::*fn)(void);
private:
	T *m_pT;
	fn m_func;
public:
	D(T *pT = 0, fn func = 0) : m_pT(pT), m_func(func){}

	void make(T *pT = 0, fn func = 0){
		m_pT =  pT;
		m_func = (fn)func;
		E::push_item(this);
		cout << "[D::make] = " << (void *)(m_pT->*m_func) << endl;
	}

	virtual void exec_fun(void){
		((*m_pT).*m_func)();
		cout << "[D::rexec_fun] = " << (void *)(m_pT->*m_func) << endl;
	}
};

class A{
public:
	D<A> d;
	A(){ 
		cout << "[A::A()]" << endl;
		d.make(this, &A::func);
	}

	virtual void func(void){
		cout << "[A::func()] = " << (void *)(&A::func) << endl;
	}
};

class B : public A{
public:
	D<B> d;
	B(){ 
		cout << "[B::B()]" << endl;
		d.make(this, &B::func);
	}

	void func(void){
		cout << "[B::func()] = " << (void *)(&B::func) << endl;
	}
};


std::vector<C *> E::vec;

int main(){
	B b;
	E::exec_all();
	//结论：这个规律适用于多态
}
