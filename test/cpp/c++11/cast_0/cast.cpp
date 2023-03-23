//强制类型转换
//@wenqingqian 2023.2.27

#include<iostream>
using namespace std;

class A{
public:
	float a;
};
class B{
public:
	int b;
};
class C:public A,public B{
public:
	int c;
};

//----------static_cast----------//
void fstatic(){
	//内置类型转换
	double a = 2.2;
 	int    b = static_cast<int>(a);
	cout<<b<<endl;
	//void*与其它累型指针转换
	void* p  = &b;
	int*  ip = static_cast<int*>(p);
	cout<<(*ip)<<endl;
}
//----------const_cast----------//
void fconst(){
	      int  n  = 1;
	const int* cp = &n;
	//(*cp)++; error
	      int* p  = const_cast<int*>(cp);
	(*p)++;
	cout<<n<<endl;
	//未定义行为, 本环境未改变cn
	const int  cn  = 1;
	const int* cp2 = &cn;
	      int* p2  = const_cast<int*>(cp2);
	(*p2)++;
	cout<<cn<<endl;
}

//----------reinterpret_cast----------//
void freinterpret(){
	int     a  = 1;
	int*    p  = &a;
	float*  dp = reinterpret_cast<float*>(p);
	cout<<*dp<<endl;//1.4013e-45
	//float和int都是32位但是编码格式不同, 比特层面上相同, 不同的解释方式得到不同的结果
}

//----------diff----------//
void fdiff(){
	C* pc  = new C;
	pc->a  = 1;
	pc->b  = 2;
	pc->c  = 3;
	B* pb  = (B*)pc;
	B* pbs = static_cast<B*>(pc);
	B* pbr = reinterpret_cast<B*>(pc);
	B* pbd = dynamic_cast<B*>(pc);
	cout<<"pc:" <<pc <<endl;
	cout<<"pb:" <<pb <<'-'<<pb->b <<endl;
	cout<<"pbs:"<<pbs<<'-'<<pbs->b<<endl;
	cout<<"pbr:"<<pbr<<'-'<<pbr->b<<endl;
	cout<<"pbd:"<<pbd<<'-'<<pbd->b<<endl;
}
/*
reinterpret_cast转换后的B类指针指向原先C类指针pc指向的类内首地址也是类A的首地址.

对得到的这个pbr指针按B类的方式获取类内对象b, 按照类内分布进行地址偏移得到b, 然而此时pbr实际指向地址还是C的地址
	, 即A的首地址, 三个类内分布又相同, 偏移后得到的是类A中的a, 并且按照b的属性int去解释, 得到例子中的结果.
*/

int main(){
	//fstatic();
	//fconst();
	//freinterpret();
	fdiff();
}