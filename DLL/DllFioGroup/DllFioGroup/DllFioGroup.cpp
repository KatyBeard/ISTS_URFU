﻿// DllFioGroup.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "DllFioGroup.h"
#include <iostream>
using namespace std;

string Fio() {
	return "Borodashkina Ekaterina Alekseevna";
}

int Group() {
	return 581220;
}

//// This is an example of an exported variable
//DLLFIOGROUP_API int nDllFioGroup=0;
//
//// This is an example of an exported function.
//DLLFIOGROUP_API int fnDllFioGroup(void)
//{
//    return 0;
//}
//
//// This is the constructor of a class that has been exported.
//CDllFioGroup::CDllFioGroup()
//{
//    return;
//}