#ifndef OBTUSE_SHARED_PTR
#define OBTUSE_SHARED_PTR
#include<algorithm>
#include<type_traits>
#include<iostream>
#include"unique_ptr.h"
namespace obtuse{
/*
|-Counter
|-shared_ptr
|-make_shared
|-weak_ptr
|-enable_shared_from_this
\_________________________
 |-need c20
 |-未实现线程安全, make_shared只调用了构造函数...
 |-测试代码在smart_ptr中
 |-删除了所有的SFINAE
 |-增加了用于enabl_weak_this的enable_if,其它类型萃取删除
 |-省略删除器...
*/
typedef unsigned long long int ull_t;
using   std::remove_extent_t;
using   std::remove_cv_t;
using   std::enable_if_t;
using   std::is_convertible_v;
template<class Tp> class weak_ptr;
template<class Tp> class enable_shared_from_this;


//////////////////////////////////////////////////////////////////////////
//Counter
///////////////
struct Counter{
private:	
	ull_t shared_count = 1;
	ull_t weak_count = 0;
public:
	constexpr Counter   () noexcept {}

	void add_shared     () { ++shared_count; }
	void add_weak       () { ++weak_count; }
	
	bool release_shared () { return --shared_count > 0 ? false : true; }
	bool release_weak   () { return --weak_count > 0 ? false : true; }

	ull_t use_count     () { return shared_count; }
	ull_t use_weak_count() { return weak_count; }
};

//////////////////////////////////////////////////////////////////////////
//shared_ptr
///////////////
//忽略类型转换检查
template<class Tp>
class shared_ptr{
public:
	//template<class Up> friend class std::shared_ptr;
	template<class Up> friend class shared_ptr;
	template<class Up> friend class weak_ptr;
	typedef            remove_extent_t<Tp> Ep;
	//template<class T > using IS_CONVERTIBLE = enable_if_t<is_convertible_v<remove_extent_t<T>*,Ep*>>;
private:
	Counter* cnt;
	Ep     * ptr;
public:
//constructor
	//default
	constexpr shared_ptr()               noexcept :ptr(nullptr),cnt(nullptr) {}

	constexpr shared_ptr(std::nullptr_t) noexcept :ptr(nullptr),cnt(nullptr) {}
	
	//new
	explicit shared_ptr(Ep* e) :ptr(e),cnt(new Counter) { enable_weak_this(e,e); }
	template<class Up>
	explicit shared_ptr(Up* u) :ptr(u),cnt(new Counter) { enable_weak_this(u,u); }
	
	//copy
	shared_ptr(const shared_ptr& sp)     noexcept :ptr(sp.ptr),cnt(sp.cnt){
		if(cnt) cnt->add_shared();
	}
	template<class Up>
	shared_ptr(const shared_ptr<Up>& sp) noexcept :ptr(sp.ptr),cnt(sp.cnt){
		if(cnt) cnt->add_shared();
	}
	template<class Up>
	shared_ptr(const shared_ptr<Up>& sp, Ep* p) noexcept :ptr(p),cnt(sp.cnt){
		if(cnt) cnt->add_shared();
	}

	shared_ptr& operator = (const shared_ptr& sp) noexcept{
		//赋值运算符都以调用原构造函数的方式生成临时值, 这个过程就是一次复制
		//将复制后的结果与当前对象交换, 所以这里不需要增加计数器
		shared_ptr(sp).swap(*this);
		return *this;
	}
	template<class Up>
	shared_ptr<Up>& operator = (const shared_ptr<Up>& sp) noexcept{
		shared_ptr(sp).swap(*this);
		return *this;
	}

	//move
	shared_ptr(shared_ptr&& sp) noexcept :ptr(sp.ptr),cnt(sp.cnt){
		sp.ptr = nullptr, sp.cnt = nullptr;
	}
	template<class Up>
	shared_ptr(shared_ptr<Up>&& sp) noexcept :ptr(sp.ptr),cnt(sp.cnt){
		sp.ptr = nullptr, sp.cnt = nullptr;
	}

	shared_ptr& operator = (shared_ptr&& sp) noexcept{
		shared_ptr(std::move(sp)).swap(*this);
		return *this;
	}
	template<class Up>
	shared_ptr<Up>& operator = (shared_ptr<Up>&& sp) noexcept{
		shared_ptr(std::move(sp)).swap(*this);
		return *this;
	}

	//from unique_ptr
	template<class Up, class Dp>
	shared_ptr(unique_ptr<Up, Dp>&& up) :ptr(up.release()) {}
	template<class Up, class Dp>
	shared_ptr& operator=(unique_ptr<Up, Dp>&& up){
		shared_ptr(std::move(up)).swap(*this);
		return *this;
	}

	//from weak_ptr
	template<class Up>
	shared_ptr(const weak_ptr<Up>& wp) :ptr(wp.ptr),cnt(wp.cnt) { 
		//从一个weak_ptr复制过来本质就是从一个shared_ptr复制过来, 所以要增加
		if(cnt) cnt->add_shared(); 
	}
	template<class Up>
	shared_ptr<Up>& operator = (const weak_ptr<Up>& wp){
		shared_ptr(wp).swap(*this);
		return *this;
	}

	//
	~shared_ptr() noexcept{
		if(!cnt) return;
		if(cnt->release_shared()){
			std::cout<<"destory ptr\n";
			delete ptr;
			ptr = nullptr;
			if(cnt && cnt->use_weak_count()==0){
				std::cout<<"destory cnt\n";
				delete cnt;
				cnt = nullptr;
			}
		}
	}
//usual function
	template<class Up, class Ptr> 
	enable_if_t<is_convertible_v<
		Ptr*, const enable_shared_from_this<Up>*
		>,
	void> enable_weak_this(const enable_shared_from_this<Up>* ep, Ptr* p) noexcept{
		using rawUp = remove_cv_t<Up>;
		if(ep && ep->weak_this.expired()){
			ep->weak_this = shared_ptr<rawUp>(
				*this, const_cast<rawUp*>(p)
			);
		} 
	}//转换不成功则匹配下面这个版本, 接受任意参数, 不做任何行为
	void enable_weak_this(...) noexcept {}

	void swap(shared_ptr& sp) noexcept{
		std::swap(ptr,sp.ptr);
		std::swap(cnt,sp.cnt);
	}

	Ep* get() const noexcept { return ptr; }

	void reset() noexcept {
		shared_ptr().swap(*this);
	}
	template<class Up>
	void reset(Up* p){
		shared_ptr(p).swap(*this);
	}

	ull_t use_count() const noexcept { return cnt ? cnt->use_count() : 0; }

	bool unique() const noexcept { return use_count()==1; }

//operator overload
	Ep* operator ->()const{
        return ptr;
    }
    Ep& operator *()const{
        return *ptr;
    }
    bool operator !()const{
        return ptr==nullptr;
    }
    explicit operator bool()const{
        return ptr!=nullptr;
    }

};


//make_shared for test
template<class Tp, class ...Args>
requires (!IS_ARRAY<Tp>)
shared_ptr<Tp> make_shared(Args&& ...args){
	return shared_ptr<Tp>(new Tp(std::forward<Args>(args)...));
}


//////////////////////////////////////////////////////////////////////////
//weak_ptr
///////////////
template<class Tp>
class weak_ptr{
public:
	template<class Up> friend class shared_ptr;
	template<class Up> friend class weak_ptr;
	typedef            remove_extent_t<Tp> Ep;
	//template<class T > using IS_CONVERTIBLE = enable_if_t<is_convertible_v<remove_extent_t<T>*,Ep*>>;
private:	
	Counter* cnt;
	Ep     * ptr;
public:
//constructor
	//default
	constexpr weak_ptr() noexcept :ptr(nullptr),cnt(nullptr) {}

	//copy
	weak_ptr(const weak_ptr& wp) noexcept :ptr(wp.ptr),cnt(wp.cnt){
		if(cnt) cnt->add_weak();
	}
	template<class Up>
	weak_ptr(const weak_ptr<Up>& wp) noexcept :ptr(wp.ptr),cnt(wp.cnt){
		if(cnt) cnt->add_weak();
	}
	template<class Up>
	weak_ptr(const shared_ptr<Up>& sp) noexcept :ptr(sp.ptr),cnt(sp.cnt){
		if(cnt) cnt->add_weak();
	}

	weak_ptr& operator = (const weak_ptr& wp) noexcept{
		weak_ptr(wp).swap(*this);
		return *this;
	}
	template<class Up>
	weak_ptr<Up>& operator = (const weak_ptr<Up>& wp) noexcept{
		weak_ptr(wp).swap(*this);
		return *this;
	}
	template<class Up>
	weak_ptr<Up>& operator = (const shared_ptr<Up>& sp) noexcept{
		weak_ptr(sp).swap(*this);
		return *this;
	}

	//move
	weak_ptr(weak_ptr&& wp) noexcept :ptr(wp.ptr),cnt(wp.cnt){
		wp.ptr = nullptr, wp.cnt = nullptr;
	}
	template<class Up>
	weak_ptr(weak_ptr<Up>&& wp) noexcept :ptr(wp.ptr),cnt(wp.cnt){
		wp.ptr = nullptr, wp.cnt = nullptr;
	}

	weak_ptr& operator = (weak_ptr&& wp) noexcept{
		weak_ptr(std::move(wp)).swap(*this);
		return *this;
	}
	template<class Up>
	weak_ptr<Up>& operator = (weak_ptr<Up>&& wp) noexcept{
		weak_ptr(std::move(wp)).swap(*this);
		return *this;
	}

	~weak_ptr() noexcept{
		if(!cnt) return;
		if(cnt->release_weak() && cnt->use_count()==0){
			std::cout<<cnt->use_weak_count()<<std::endl;
			delete cnt;
			std::cout<<"destory cnt weak\n";
			cnt = nullptr;
		}
	}

//usual function
	void swap(weak_ptr& wp) noexcept{
		std::swap(ptr,wp.ptr);
		std::swap(cnt,wp.cnt);
	}

	void reset() noexcept {
		weak_ptr().swap(*this);
	}
	template<class Up>
	void reset(Up* p){
		weak_ptr(p).swap(*this);
	}

	ull_t use_count() const noexcept { return cnt ? cnt->use_count() : 0; }

	bool expired() const noexcept { return cnt == nullptr || cnt->use_count() == 0; }

	shared_ptr<Ep> lock() const noexcept{
		return expired() ? shared_ptr<Ep>() : shared_ptr<Ep>(*this);
	}


};


//////////////////////////////////////////////////////////////////////////
//enable_shared_from_this
///////////////
template<class Tp>
class enable_shared_from_this{   
public:
	template <class Up> friend class shared_ptr;
private:
    mutable weak_ptr<Tp> weak_this;
protected:
    enable_shared_from_this() noexcept {}

    enable_shared_from_this(const enable_shared_from_this&) noexcept {}
    enable_shared_from_this& operator=(const enable_shared_from_this&) noexcept { return *this; }

    ~enable_shared_from_this() {}
public:
    shared_ptr<Tp> shared_from_this() 
	{ return shared_ptr<Tp>(weak_this); }

    shared_ptr<const Tp> shared_from_this() const 
	{ return shared_ptr<const Tp>(weak_this); }


    weak_ptr<Tp> weak_from_this() noexcept
    { return weak_this; }

    weak_ptr<const Tp> weak_from_this() const noexcept
    { return weak_this; }
};
}//namespace
#endif