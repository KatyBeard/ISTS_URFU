// VS_Console.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>

// объявление прототипов библиотечных функций в виде указателей на функцию. Объявляются там где будут использоваться.
//typedef const char*(__stdcall* FIO)(); // typedef возвращаемый_тип (<соглашение о вызове функции (такое же как в библиотеке)> *имя_указателя) (типы_параметров);
typedef int (__stdcall* FIO)(char*);
typedef int(__stdcall* Group)();
int main() {
    HMODULE h = LoadLibraryW(L"C:\\Users\\Екатерина\\Desktop\\5 курс\\2 семестр\\ИБТС\\ISTS_URFU\\DLL\\RS_DZ_1\\dll\\Win64\\Debug\\dll.dll");
    if (h == NULL) { // проверить успешно ли подключилась библиотека
        printf("load lib error\n");
        system("pause");
        exit(1);
    }
    FIO ptr_to_fun1 = (FIO)GetProcAddress(h, "FIO"); // указатель на функцию (вызывать как обычную функцию)
    Group ptr_to_fun2 = (Group)GetProcAddress(h, "Group");

    // вызов функции: возвращаемый_тип имя_переменной = ptr_to_fun1(параметры);
    setlocale(LC_ALL, "rus");
    std::cout << "Домашняя работа номер 1, DLL\n\n";
    int group = ptr_to_fun2();
    if (!group) printf("unable to load Group\n");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char* ptrToBuffer;
    int strSize = ptr_to_fun1(NULL);
    ptrToBuffer = new char[strSize];
    ptr_to_fun1(ptrToBuffer);
    //const char* fio = ptr_to_fun1();
    if (!ptrToBuffer) printf("unable to load FIO\n");
    std::cout << "Номер группы: " << group << "\n";
    std::cout << "ФИО: " << ptrToBuffer << "\n";
    delete[] ptrToBuffer;

    FreeLibrary(h);
    system("pause");
    return 0;
}
