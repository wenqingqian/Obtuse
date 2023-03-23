//范围循环
//@wenqingqian 2023.2.26
#include<iostream>
using namespace std;

//----------自定义范围循环对象----------//
class test1{
private:
	int a[10]={0,1,2,3,4,5,6,7,8,9};
public:
	test1(){}
	int* pbegin = a;
	int* pend   = a+10;
	int* begin() {return pbegin;}
	int* end()   {return pend;}
};
void ftest1(){
	test1 t1;
	for(auto& n:t1){
		cout<<n;
	}
}

//----------数组指针----------//
void f2(int (&s)[3]){cout<<1;}
void f2(int *p){cout<<2;}
void ftest2(){
	int a[3]={1,2,3};
	int*p = a;
	f2(p);
	//f2(a);//error
}
void ftest3(){
	int a[3][3]={1,2,3,4,5,6,7,8,9};
	//for(int(&c)[3]:a)
	//外层循环得到三个一维数组的引用
	for(auto&c:a){
		for(int i:c){
			cout<<i;
		}
	}
	//for(int*c :a)
	//外层循环得到三个一维数组退化后的指针
	//即指向三个一维数组首元素地址的指针
	for(auto c:a){
		for(int i=0;i<3;++i){
			cout<<c[i];
		}
	}
	auto ftest3_p = [](int p[][3]){
		//= [](int(*p)[3]) 两种写法相同
		for(int i=0;i<3;++i){
			for(int j=0;j<3;++j){
				//(*(p+i))解引用得到一级指针,指向含有三个元素的数组头
				cout<<(*(p+i))[j];
			}
		}
	};
	ftest3_p(a);
}


int main(){
	//ftest1();
	//ftest2();
	ftest3();
}