﻿// NTFS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include "NTFSDriver.h"

int main()
{
    BYTE dataBuffer[512];
    NTFSBootRecord* pNTFSBootRecord = (NTFSBootRecord*)dataBuffer;

    HANDLE cFile = CreateFileW(
        L"\\\\.\\C:",    // Открываем диск С
        GENERIC_READ,   //  Запрос доступа к диску для чтения
        FILE_SHARE_READ | FILE_SHARE_WRITE, //  Режим совместного использования
        NULL,   //  Атрибуты безопасности - по умолчанию
        OPEN_EXISTING,  //  Открытие диска, только если он существует
        FILE_ATTRIBUTE_NORMAL,  //  Атрибуты по умолчанию
        NULL  //  Не копировать атрибуты диска
    );

    // Ошибка открытия диска
    if (cFile == INVALID_HANDLE_VALUE) {
        printf("\nDisk read ERROR :(\n\nTry to run as an administrator\n");
        system("pause");
        return 0;
    }
    else { printf("It's okey ^_^\n"); system("pause"); return 0;
    }
}


