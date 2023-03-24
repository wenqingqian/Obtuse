#include<iostream>
using namespace std;

void getMemory1(int *p) {
	cout<<p<<"old p"<<endl;
    p = new int[100];
	cout<<p<<"new p"<<endl;
}

char *getMemory2() {
    char p[12] = "Hello world";
    return p;
}

void getMemory3(char **p, int n) {
    *p = new char[n];
}

void getMemory4(char *&p, int n) {
    p = new char[n];
}

int main() 
{
    int *p1 = 0;
	cout<<p1<<"old p1"<<endl;
    getMemory1(p1);
	cout<<p1<<"new p1"<<endl;
	/*
	0			old p1
	0			old p
	0x137e04f80 new p
	0			new p1
	*/
    delete [] p1;
////////////////////////////////////////////////////////////
	char *p2 = 0;
	cout<<"old p2:"<<reinterpret_cast<int*>(p2)<<endl;
    p2 = getMemory2();
	cout<<"new p2:"<<reinterpret_cast<int*>(p2)<<endl; 
	//ostream有char指针的重载直接输出字符串, 转成int*, 或者使用printf.
	/*
	old p2:0
	new p2:0
	*/
////////////////////////////////////////////////////////////
	char *p3 = NULL;
	printf("old p3:%d\n",p3);
    getMemory3(&p3, 100);
	printf("new p3:%d\n",p3);
	/*
	old p3:0
	new p3:854605952
	*/
    delete [] p3;
////////////////////////////////////////////////////////////
	char *p4 = NULL;
	printf("old p4:%d\n",p4);
    getMemory4(p4, 100);
	printf("new p4:%d\n",p4);
	/*
	old p4:0
	new p4:1415604464
	*/
    delete [] p4;
}