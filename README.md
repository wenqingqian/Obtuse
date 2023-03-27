# Obtuse
.-.. --- ...- . .- -. -.. .--. . .- -.-. .

个人学习笔记及资料整理.

## 部分代码实现

&nbsp; [unique_ptr](./test/cpp/c%2B%2B11/smart_ptr/unique_ptr.h)

- 实现了自定义删除器和make_unique
- 省略自定义指针pointer
	省略空间优化compress_pair
	忽略auto_ptr
	忽略一部分类型萃取判断

&nbsp; [shared_ptr](./test/cpp/c%2B%2B11/smart_ptr/shared_ptr.h)

- 简单计数模块, shared_ptr及weak_ptr, 测试用make_shared, enable_shared_from_this
- 省略删除器
	主要实现拷贝,析构,enable_weak_this


