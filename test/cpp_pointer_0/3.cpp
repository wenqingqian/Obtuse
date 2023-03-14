#include"../head/all.h"

void getMemory(char **p, int n) {
    *p = new char[n];
}

int main()
{
    char *str = NULL;
	printf("old str:%d\n",str);
    getMemory(&str, 100);
	printf("new str:%d\n",str);
	/*
	old str:0
	new str:854605952
	*/
    delete [] str;
}

