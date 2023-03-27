#include"shared_ptr.h"
//#include<memory>
using namespace obtuse;

struct node{
	weak_ptr<node> next;
};


int main(){
	shared_ptr<node> head = make_shared<node>();
	head->next = head;

	head.reset();

	std::cout<<"End\n";
}
