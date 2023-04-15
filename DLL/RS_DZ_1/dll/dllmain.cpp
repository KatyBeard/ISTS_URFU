// dllmain.cpp : Defines the entry point for the DLL application.
#include "dllmain.h"
#include "iostream"



extern "C" int _libmain(unsigned long reason)
{
    return 1;
}
int __stdcall Group()
{
	static int Group = 581220;
	return Group;
}

//const char* __stdcall FIO()
int __stdcall FIO(char *ptrToBuffer)
{
//	setlocale(LC_ALL, "rus");
//	const char* ch = "Бородашкина Екатерина Алексеевна";
//	return ch;
	char fio[] = "Бородашкина Екатерина Алексеевна";
	int count = strlen(fio);
	if (ptrToBuffer != NULL)
	{
		strcpy(ptrToBuffer, fio);
	}
	return count + 1;
}
