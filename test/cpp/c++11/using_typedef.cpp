//using和typedef
//@wenqingqian 2023.2.25
#include<iostream>
using namespace std;
//------------------别名模板-----------------//
template<typename T>
struct _typedef{
	typedef T type;
};
template<typename T> using _using   = T;

template<typename T, class U> 
void func (T i){
			 _typedef<int>::type a0 = i;  //非从属名称
			 _using  <T  >       a1 = i;  //从属名称
			 _typedef<T  >       a2    ;  //从属名称
	typename _typedef<T  >::type a3 = i;  //嵌套从属名称
	typename U            ::type a4 = i;  //嵌套从属名称
	cout<<a0<<a1<<a3<<a4;
//using   可以接受模板   模板绑定在别名上
//typedef 不可以接受模板 模板绑定在结构体上
//		  在结构体内再声明与模板参数相同的类型成员, 形成了嵌套
//	      结构体_typedef可能在某个特化的情况下存在成员type不为类型的情况
}
template<typename T, template<class>class U>
void func2 (T i){
	         _typedef<int>::type a  = i;
			 _using  <T>         a2 = i;
	typename _typedef<T>  ::type a3 = i;
	typename U       <int>::type a4 = i;
	cout<<a<<a2<<a3<<a4;
}


//--------------using作用域--------------//
class A{
public:
	void func(){cout<<value<<pvalue<<endl;}
protected:
	int value;
private:
	int pvalue;
};

class B:public A{
public:
	using A::func;
	using A::value;
//public or private: //error	
//	using A::pvalue;
};

//-----------------嵌套从属不加typename特例-------------------//
template<typename T>
class C{
public:
	C(){}
	C(T x):val(x){}
	T val;
};
template<typename T>
struct Base{
	typedef C<T> type;
};

template<typename T>
class D:public Base<T>::type{
public:
	D(int x):Base<T>::type(x){
		typename Base<T>::type tmpC;
	}
};




int main(){
	func <int,_typedef<int>>(1);
	func2<int,_typedef     >(1);

	B b;
	b.value=1;
	b.func();

	D<int>(1);
}
