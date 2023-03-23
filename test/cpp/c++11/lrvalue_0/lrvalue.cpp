#include<iostream>

using namespace std;


//----------myforward, test for if delete static_assert
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

//

template<class T1, class T2>
T1&& StaticCastFun(T1&& a1, T2&& a2) 
{
    return static_cast<T1&&>(a2);
}

template<class T1, class T2>
T1&& ForwardCastFun(T1&& a1, T2&& a2) 
{

    return forward<T1>(a2);
}


template<class T1, class T2>
T1&& MyForwardCastFun(T1&& a1, T2&& a2) 
{

    return MyForward<T1>(a2);
}


struct TypeB{};
struct TypeA
{
    TypeA() {
		cout << "Default  Create" << endl;
	}
    TypeA(const TypeA& ) { 
		cout << "From TypeA  Create" << endl; 
	}
    ~TypeA() { cout << "Del" <<endl;}
    TypeA(const TypeB&) { 
		cout << "From TypeB  Create" << endl;
	}
};
void ForwardTypeTest(const TypeA a,  const TypeB b)
{
    cout << "StaticCastFun" << endl;

    StaticCastFun(a, b);

    cout << "ForwardCastFun" << endl;

    //ForwardCastFun(a, b);//open this

	cout<<"forwardend"<<endl;

	MyForwardCastFun(a, b);
}


int main()
{
    TypeA ta1;
    TypeB tb1;
    ForwardTypeTest(ta1,tb1);

	return 0;
}