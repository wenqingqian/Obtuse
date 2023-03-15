#include<iostream>
using namespace std;


struct TypeB{};

class test{
public:
	test(int size){
		this->size=size;
		v=new int[size];
	}
	test(const test& t){
		size=t.size;
		v=new int[t.size];
	}
	int *v;
	int size;
	void show(){cout<<size<<endl;}
	~test(){
		if(v) delete []v;
	}
};
void TestLValueOrRValue(test& rA)
{
    std::cout << "left Value " << rA.size << std::endl;
}
void TestLValueOrRValue(test&& rA)
{
    std::cout << "right Value " << rA.size << std::endl;
}
void TestFun()
{
    TestLValueOrRValue(test(1111));
    auto&& ra2222 = test(2222);
    TestLValueOrRValue(ra2222);
}
//----------------------------------------------//
class test2{
public:
	test2(int n){
		s=n;
		cout<<"..."<<s<<endl;
	}
	int s;
	test2(const test2& t){
		s=t.s;
		cout<<"lvalue"<<s<<endl;
	}
	test2(test2&& t){
		s=t.s;
		cout<<"rvalue"<<s<<endl;
	}
	~test2(){
		cout<<"disconstruct"<<s<<endl;
	}
};
template<typename T>
void funct(T a){
	cout<<a<<endl;
}

template<typename T>
using remove_reference_t = typename remove_reference<T>::type;

template <class _Ty>
constexpr _Ty&& MyForward(remove_reference_t<_Ty>& _Arg) noexcept {
	cout<<"forward&"<<endl;
    return static_cast<_Ty&&>(_Arg);
}

template <class _Ty>
constexpr _Ty&& MyForward(remove_reference_t<_Ty>&& _Arg) noexcept { 
    //static_assert(!is_lvalue_reference<_Ty>::value,"bad forward call");
	cout<<"forward&&"<<endl;
    return static_cast<_Ty&&>(_Arg);
}

template<class T1, class T2>
T1&& StaticCastFun(T1&& a1, T2&& a2) 
{
    return static_cast<T1&&>(a2);
}

template<class T1, class T2>
T1&& ForwardCastFun(T1&& a1, T2&& a2) 
{

    return MyForward<T1>(a2);
}

int gs = 0;
struct TypeA
{
    TypeA() {
		s=gs;
		++gs; 
		cout << "Default  Create" <<s<< endl;
	}
    TypeA(const TypeA& ) { 
		s=gs;
		++gs;
		cout << "From TypeA  Create" <<s<< endl; 
	}
    ~TypeA() { cout << "Del" <<s<<endl;}
    TypeA(const TypeB&) { 
		s=gs;
		++gs;
		cout << "From TypeB  Create" <<s<< endl;
	}
	
	int s;
};
void ForwardTypeTest(const TypeA a,  const TypeB b)
{
    cout << "StaticCastFun" << endl;
    StaticCastFun(a, b);
    cout << "ForwardCastFun" << endl;
    ForwardCastFun(a, b);
	cout<<"forwardend"<<endl;
}

TypeA fff(TypeB s){
	return static_cast<TypeA>(s);
}

class test3{
	public:
	explicit test3(string s):str(s){
		cout<<"default construct"<<endl;
	}
	test3(const test3& s):str(s.str){
		cout<<"copy construct"<<endl;
	}
	test3(test3&& s):str(s.str){
		cout<<"move construct"<<endl;
	}

	string str;
};


int main()
{

    TypeA ta1;
    TypeB tb1;
    //ForwardTypeTest(ta1,tb1);
	cout<<"end"<<endl;

	return 0;
}