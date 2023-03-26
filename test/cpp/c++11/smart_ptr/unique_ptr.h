#ifndef OBTUSE_UNIQUE_PTR
#define OBTUSE_UNIQUE_PTR
#include<algorithm>
#include<type_traits>
#include<iostream>
namespace obtuse{
	//所有都不支持比较运算符
/*
|-default_deleter
|-default_deleter<T[]>
|-unique_ptr
|-unique_ptr<T[]>
|-make_unique(Args...)
|-make_unique(size_t)
|-make_unique with c17,20
|-测试代码在smart_ptr中
*/
template<class Tp>
struct default_deleter{
	constexpr default_deleter() noexcept = default;

	template<class U>
	default_deleter(const default_deleter<U>&) noexcept{}

	void operator()(Tp* ptr) const noexcept{
		delete ptr;
	}
};

template<class Tp>
struct default_deleter<Tp[]>{
	constexpr default_deleter() noexcept = default;

	template<class U>
	default_deleter(const default_deleter<U[]>&) noexcept{}
	
	template<class U>
	void operator()(U* ptr) const noexcept{
		delete []ptr;
	}
};

//省略自定义指针pointer
//省略空间优化compress_pair
//忽略auto_ptr
//省略一部分类型萃取判断
template<class Tp, class Dp = default_deleter<Tp>>
class unique_ptr{
private:	
    Tp* ptr;
	Dp  deleter;
public:
//construstor
	//default
    constexpr unique_ptr() noexcept :ptr(nullptr) {}

    constexpr unique_ptr(std::nullptr_t) noexcept :ptr(nullptr) {}
	//new
	explicit unique_ptr (Tp* p) noexcept :ptr(p) {}
	//new with deleter
	unique_ptr (Tp* p, Dp d) noexcept :ptr(p),deleter(d) {}
	//move
	unique_ptr(unique_ptr&& u) noexcept :ptr(u.release()),deleter(u.get_deleter()) {}
	template<class U, class E>
	unique_ptr(unique_ptr<U,E>&& u) noexcept :ptr(u.release()),deleter(u.get_deleter()) {}

	unique_ptr& operator =(unique_ptr&& u) noexcept{
        reset(u.release());
		deleter=u.get_deleter();
        return *this;
    }
	template<class U, class E>
	unique_ptr& operator =(unique_ptr<U,E>&& u) noexcept{
        reset(u.release());
		deleter=u.get_deleter();
        return *this;
    }

//usual function
	Tp* get() const noexcept{
		return ptr;
	}

	Tp* release() noexcept{
		Tp* tmp = ptr;
		ptr = nullptr;
		return tmp;
	}

	void reset(Tp* p = nullptr) noexcept{
		Tp* tmp = ptr;
		ptr = p;
		if(tmp) deleter(tmp);
	}

	Dp get_deleter() const noexcept{
		return deleter;
	}
	
	void swap(unique_ptr& u) noexcept{
		std::swap(u.ptr, ptr);
	}

    ~unique_ptr()noexcept{
        reset();
    }

//operator overload
	unique_ptr& operator = (std::nullptr_t) noexcept{
		reset();
		return *this;
	}
    Tp* operator ->()const{
        return ptr;
    }
    Tp& operator *()const{
        return *ptr;
    }
    bool operator !()const{
        return ptr==nullptr;
    }
    explicit operator bool()const{
        return ptr!=nullptr;
    }

	

    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator =(const unique_ptr&) = delete;
};

//数组特化版本
template<class Tp, class Dp>
class unique_ptr<Tp[],Dp>{
private:	
    Tp* ptr;
	Dp  deleter;
public:
    constexpr unique_ptr() noexcept :ptr(nullptr) {}

    constexpr unique_ptr(std::nullptr_t) noexcept :ptr(nullptr) {}

	explicit unique_ptr (Tp* p) noexcept :ptr(p) {}

	unique_ptr (Tp* p, Dp d) noexcept :ptr(p),deleter(d) {}

	unique_ptr(unique_ptr&& u) noexcept :ptr(u.release()),deleter(u.get_deleter()) {}

	template<class U, class E>
	unique_ptr(unique_ptr<U,E>&& u) noexcept :ptr(u.release()),deleter(u.get_deleter()) {}

	unique_ptr& operator =(unique_ptr&& u) noexcept{
        reset(u.release());
		deleter=u.get_deleter();
        return *this;
    }

	template<class U, class E>
	unique_ptr& operator =(unique_ptr<U,E>&& u) noexcept{
        reset(u.release());
		deleter=u.get_deleter();
        return *this;
    }

	Tp* get() const noexcept{
		return ptr;
	}

	Tp* release() noexcept{
		Tp* tmp = ptr;
		ptr = nullptr;
		return tmp;
	}

	void reset(Tp* p = nullptr) noexcept{
		Tp* tmp = ptr;
		ptr = p;
		if(tmp) deleter(tmp);
	}

	Dp get_deleter() const noexcept{
		return deleter;
	}
	
	void swap(unique_ptr& u) noexcept{
		std::swap(u.ptr, ptr);
		std::swap(u.deleter,deleter);
	}

    ~unique_ptr()noexcept{
        reset();
    }

	unique_ptr& operator = (std::nullptr_t) noexcept{
		reset();
		return *this;
	}
    Tp* operator ->()const{
        return ptr;
    }
    Tp& operator *()const{
        return *ptr;
    }
    bool operator !()const{
        return ptr==nullptr;
    }
    explicit operator bool()const{
        return ptr!=nullptr;
    }
	Tp& operator [](size_t i)const{
		return ptr[i];
	}
	
    unique_ptr(const unique_ptr&) = delete;
    unique_ptr& operator =(const unique_ptr&) = delete;
};


//make_unique C11
//SFINAE
template<class Tp, class... Args, 
		 class = typename std::enable_if<
		    	!std::is_array<Tp>::value
				 >::type>
unique_ptr<Tp> make_unique(Args&&... args)
{
	std::cout<<"make_unique:  Tp constructor\n";
    return unique_ptr<Tp>(new Tp(std::forward<Args>(args)...));
}

template<class Tp, 
	 	 class = typename std::enable_if<
		    	 std::is_array<Tp>::value
		      	 >::type>
unique_ptr<Tp> make_unique(size_t n)
{
	std::cout<<"make_unique:  Tp[] constructor\n";
    return unique_ptr<Tp>(new typename std::remove_extent<Tp>::type[n]());
}






//C17
template<class Tp, class... Args, 
		 class = std::enable_if_t<
		    	 !std::is_array_v<Tp>
				 >>
unique_ptr<Tp> make_unique_C17(Args&&... args)
{
	std::cout<<"make_unique:  Tp constructor\n";
    return unique_ptr<Tp>(new Tp(std::forward<Args>(args)...));
}

template<class Tp, 
	 	 class = std::enable_if_t<
		    	 std::is_array_v<Tp>
		      	 >>
unique_ptr<Tp> make_unique_C17(size_t n)
{
	std::cout<<"make_unique:  Tp[] constructor\n";
    return unique_ptr<Tp>(new std::remove_extent_t<Tp>[n]());
}

//C20
template<class T> 
concept IS_ARRAY = std::is_array_v<T>;

template<class Tp, class... Args>
requires (!IS_ARRAY<Tp>)
unique_ptr<Tp> make_unique_C20(Args&&... args)
{
	std::cout<<"make_unique:  Tp constructor\n";
    return unique_ptr<Tp>(new Tp(std::forward<Args>(args)...));
}

template<IS_ARRAY Tp>
unique_ptr<Tp> make_unique_C20(size_t n)
{
	std::cout<<"make_unique:  Tp[] constructor\n";
    return unique_ptr<Tp>(new std::remove_extent_t<Tp>[n]());
}
/*
template<typename T>
concept IS_ARRAY = std::is_array_v<T>

template<class Tp, class... Args>
requires std::is_array_v<Tp>
unique_ptr<Tp> make_unique_C20(Args&&... args)

template<class Tp, class... Args>
requires IS_ARRAY<Tp>
unique_ptr<Tp> make_unique_C20(Args&&... args)
*/

}
#endif