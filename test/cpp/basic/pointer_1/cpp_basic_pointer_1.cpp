#include"head/all.h"
class test1{
public:
	int v   = 4;
	int *p  = &v;
	int *&r = p;
};

class test2{
public:
	int v  = 4;
	int &r = v;
	int *p = &r;
};
int main() {
	{
	test1 *p       = new test1;
	long long *ptr = (long long*) p;

	cout<< (long long)&p->v <<endl; //5022705584
	cout<< (long long)&p->p	<<endl; // 5022705592

	cout<< *(ptr)			<<endl; // 4
	cout<< *(ptr+1)			<<endl; // 5022705584
	cout<< *(ptr+2)			<<endl; // 5022705592
	}{
	test2 *p       = new test2;
	long long *ptr = (long long*) p;
	
	cout<< (long long)&p->v <<endl; // 5669676896

	cout<< *(ptr)           <<endl; // 4
	cout<< *(ptr+1)        	<<endl; // 5669676896
	cout<< *(ptr+2)        	<<endl; // 5669676896
	}
}