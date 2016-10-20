#include <iomanip>
#include <iostream>

#include "xiaofei_debug.h"

using namespace std;

//precision(n) 	setprecision(n) 	设置实数的精度为n位
//width(n) 	setw(n) 	设置字段宽度为n位
//fill(c) 	setfill(c) 	设置填充宇符c
//setf() 	setiosflags() 	设置输出格式状态，括号中应给出格式状态，内容与控制符setiosflags括号中的内容相同，如表13.5所示
//unsetf() 	resetioflags() 	终止已设置的输出格式状态，在括号中应指定内容
int test1()
{
	mydebug();
	int a;
	cout << "input a:";
	cin >> a;
	cout << "dec:" << dec << a << endl; //以十进制形式输出整数
	cout << "hex:" << hex << a << endl; //以十六进制形式输出整数a
	cout << "oct:" << setbase(8) << a << endl; //以八进制形式输出整数a
	string pt = "China"; //pt指向字符串"China"
	cout << setw(10) << pt << endl; //指定域宽为,输出字符串
	cout << setfill('*') << setw(10) << pt << endl; //指定域宽,输出字符串,空白处以'*'填充
	double pi = 22.0 / 7.0; //计算pi值
	//按指数形式输出,8位小数
	cout << setiosflags(ios::scientific) << setprecision(8);
	cout << "pi=" << pi << endl; //输出pi值
	cout << "pi=" << setprecision(4) << pi << endl; //改为位小数
	cout << "pi=" << setiosflags(ios::fixed) << pi << endl; //改为小数形式输出

	return 0;
}

//ios::left 	输出数据在本域宽范围内向左对齐
//ios::right 	输出数据在本域宽范围内向右对齐
//ios::internal 	数值的符号位在域宽内左对齐，数值右对齐，中间由填充字符填充
//ios::dec 	设置整数的基数为10
//ios::oct 	设置整数的基数为8
//ios::hex 	设置整数的基数为16
//ios::showbase 	强制输出整数的基数(八进制数以0打头，十六进制数以0x打头)
//ios::showpoint 	强制输出浮点数的小点和尾数0
//ios::uppercase 	在以科学记数法格式E和以十六进制输出字母时以大写表示
//ios::showpos 	对正数显示“+”号
//ios::scientific 	浮点数以科学记数法格式输出
//ios::fixed 	浮点数以定点格式(小数形式)输出
//ios::unitbuf 	每次输出之后刷新所有的流
//ios::stdio 	每次输出之后清除stdout, stderr
int test2()
{
	int a = 21;
	mydebug();
	cout.setf(ios::showbase);//显示基数符号(0x或)
	cout << "dec:" << a << endl;   //默认以十进制形式输出a
	cout.unsetf(ios::dec);         //终止十进制的格式设置
	cout.setf(ios::hex);           //设置以十六进制输出的状态
	cout << "hex:" << a << endl;   //以十六进制形式输出a
	cout.unsetf(ios::hex);         //终止十六进制的格式设置
	cout.setf(ios::oct);           //设置以八进制输出的状态
	cout << "oct:" << a << endl;   //以八进制形式输出a
	cout.unsetf(ios::oct);
	string pt = "China";            //pt指向字符串"China"
	cout.width(10);                //指定域宽为
	cout << pt << endl;            //输出字符串
	cout.width(10);                //指定域宽为
	cout.fill('*');                //指定空白处以'*'填充
	cout << pt << endl;            //输出字符串
	double pi = 22.0 / 7.0;        //输出pi值
	cout.setf(ios::scientific);    //指定用科学记数法输出
	cout << "pi=";                 //输出"pi="
	cout.width(14);                //指定域宽为
	cout << pi << endl;            //输出pi值
	cout.unsetf(ios::scientific); //终止科学记数法状态
	cout.setf(ios::fixed);        //指定用定点形式输出
	cout.width(12);               //指定域宽为
	cout.setf(ios::showpos);      //正数输出“+”号
	cout.setf(ios::internal);     //数符出现在左侧
	cout.precision(6);            //保留位小数
	cout << pi << endl;           //输出pi,注意数符“+”的位置

	return 0;
}

int main(int argc, char **argv)
{
	int rtn = 0;
	test1();
	test2();
	return rtn;
}
