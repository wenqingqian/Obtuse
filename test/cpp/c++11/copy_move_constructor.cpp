#include<iostream>
using namespace std;


//--------------------浅拷贝------------------//
class Test1{
public:
	Test1(int s):size(s) {
		p = new char[size];
	}
	~Test1()	{
		if(p) {
			delete []p;
			p = nullptr;
		}
	}
	int size;
	char* p;
};

//-------------------深拷贝------------------//

class Test2 {
public:
	Test2(int s):size(s){
		p = new char[size];
	}
	Test2(const Test& t):size(t.size){
		p = new char[size];
		memcpy(p, t.p, size);
	}
	~Test2(){
		if(p)
		{
			delete []p;
			p = nullptr;
		}
	}
	int size;
	char* p;
};

//-------------------移动语义------------------//

class Test3 {
public:
	int size;
	char* p;
	Test3(Test3& t,bool bMove = false):size(t.size){
		if(bMove){//转移构造
			p = t.p;
			t.p = nullptr;
		}else{//拷贝构造
			p = new char[size];
			memcpy(p, t.p, size);
		}
	}
	Test3(const Test3& t){//拷贝构造
		p = new char[size];
		memcpy(p, t.p, size);
	}
};
/*
class Test {
public:
	......
	Test(Test&& t):size(t.size),p(t.p){
		t.p = nullptr;
	}
};
*/