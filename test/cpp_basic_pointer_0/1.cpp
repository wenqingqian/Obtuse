#include"../head/all.h"

void getMemory(int *p) {
	cout<<p<<"old p"<<endl;
    p = new int[100];
	cout<<p<<"new p"<<endl;
}

int main() 
{
    int *str = 0;
	cout<<str<<"old str"<<endl;
    getMemory(str);
	cout<<str<<"new str"<<endl;

	/*
	0			old str
	0			old p
	0x137e04f80 new p
	0			new str
	*/
    delete [] str;
}