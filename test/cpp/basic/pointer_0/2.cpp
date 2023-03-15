#include<iostream>
using namespace std;

char *getMemory() {
    char p[12] = "Hello world";
    return p;
}

int main()
{
	char *p = 0;
	cout<<"old p:"<<reinterpret_cast<int*>(p)<<endl;
    p = getMemory();
	cout<<"new p:"<<reinterpret_cast<int*>(p)<<endl; 
	//ostream有char指针的重载直接输出字符串, 转成int*, 或者使用printf.
	/*
	old p:0
	new p:0
	*/

}