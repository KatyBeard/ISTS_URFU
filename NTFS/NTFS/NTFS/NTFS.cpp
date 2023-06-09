﻿// NTFS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include "NTFSDriver.h"

int main()
{
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
        printf("\nDisk read ERROR :(\n\nTry to run as an administrator.\n");
        system("pause");
        return 0;
    }

    LARGE_INTEGER sectorOffset;
    sectorOffset.QuadPart = 0;

    unsigned long currentPosition = SetFilePointer(
        cFile,  //  Дескриптор файла
        sectorOffset.LowPart,   //  Байты перемещения указателя
        & sectorOffset.HighPart,
        FILE_BEGIN  //  Точка отсчета равняется нулю или это начало файла
    );

    // Проверка на ошибку при смещении
    if (currentPosition == INVALID_SET_FILE_POINTER && (currentPosition = GetLastError()) != NO_ERROR || currentPosition != sectorOffset.LowPart)
    {
        printf("\nOffset ERROR :(\n\nTry to run the app again.\n");
        system("pause");
        return 0;
    }

    BYTE dataBuffer[512];
    NTFSBootRecord *currentRecord = (NTFSBootRecord*)dataBuffer;

    DWORD bytesToRead = 512;
    DWORD bytesRead;

    bool readResult = ReadFile(
        cFile,
        dataBuffer,
        bytesToRead,
        &bytesRead,
        NULL);

    if (readResult == false || bytesRead != bytesToRead)
    {
        printf("File reading ERROR or there is a discrepancy in the number of bytes read.\n\nTry to run the app again.\n");
        system("pause");
        return 0;
    }

    UINT nameLowest = currentRecord->nameLowest;
    UINT nameHighest = currentRecord->nameHighest;
    USHORT bytesPerSector = currentRecord->bytesPerSector;
    BYTE sectorsPerCluster = currentRecord->sectorsPerCluster;
    ULONGLONG numSectors = currentRecord->numSectors;
    ULONGLONG mftCluster = currentRecord->mftCluster;
    ULONGLONG mftClusterCopy = currentRecord->mftClusterCopy;
    UINT mftClusterSize = currentRecord->mftClusterSize;
    UINT volumeIdLowest = currentRecord->volumeIdLowest;
    UINT volumeIdHighest = currentRecord->volumeIdHighest;
    USHORT signature = currentRecord->signature;

    if (nameLowest != 1397118030 && nameHighest != 538976288 && signature != 43605) {
        printf("Error! The transition to the bootstrap program was not detected or the signature not detected.\n\nTry to run the app again.\n");
        system("pause");
        return 0;
    }

    setlocale(LC_ALL, "rus");
    printf("\nРазрабатывала Екатерина Бородашкина, студентка группы РИ-581220.\n");
    std::cout << "Количество байт в секторе: " << bytesPerSector 
        << "\nКоличество секторов в кластере: " << static_cast<int>(sectorsPerCluster)
        << "\nКоличество секторов в блоке: " << numSectors
        << "\nНачальный кластер MFT таблицы: " << mftCluster
        << "\nНачальный кластер копии MFT таблицы: " << mftClusterCopy
        << "\nРазмер записи MFT в кластерах: " << mftClusterSize
        << "\nНаименьшее слово в загрузочном диске: " << volumeIdLowest
        << "\nНаибольшее слово в загрузочном диске: " << volumeIdHighest << "\n";

    system("pause");
    CloseHandle(cFile);
    return 0;
}


