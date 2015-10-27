#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

typedef void (*pFunc)(void);
typedef std::vector<pFunc> vFunc;
typedef std::map<pFunc, string> mFunc;


vFunc vec_func;
mFunc map_func;

void test1(){
	cout << __FUNCTION__ << endl;
}


void test2(){
	cout << __FUNCTION__ << endl;
}

void test3(){
	cout << __FUNCTION__ << endl;
}

#define add_func(vec,x) \
do{\
	cout << #x << ":\t\t" << (void *)x << endl;\
	map_func[x] = #x;\
	vec.push_back(x);\
}while(0)

void prepare(){
	add_func(vec_func, test1);
	add_func(vec_func, test2);
	add_func(vec_func, test3);
}

void exec_func(void){
	vFunc::iterator it = vec_func.begin();
	for(it; it != vec_func.end(); it++){
		mFunc::iterator cur_func = map_func.find(*it);
		if(cur_func != map_func.end()){
			cout << cur_func->second << ":\t\t" << (void *)(cur_func->first) << endl;
		}
		(*it)();
	}
}

void test_macro_and_vector_func(){
	prepare();
	cout << "_____________________________" << endl;
	exec_func();
}

void test_macro_define(){
#define TEST 1
	cout << TEST << endl;
	{
#define TEST 2
		cout << TEST << endl;
	}
	cout << TEST << endl;
}
#define PARA_INFO(file, func){#func}
void get_para(std::string str){
	cout << str << endl;
}

void test_constant_para(){
	get_para(PARA_INFO(1,test_constant_para));
}

int main(){
	struct st {
		int i;
	};
#ifdef XIAOFEI
	cout << "xiaofei defined" << XIAOFEI  << 1 << endl;
#endif
	//test_constant_para();
	cout << (void *)test_constant_para << endl;
	cout << (void *)get_para << endl;
	struct st st = {0};
	struct st* stp = &st;
}
