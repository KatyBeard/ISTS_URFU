// NTFS.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include "NTFSDriver.h"

int main()
{
    BYTE dataBuffer[512];
    NTFSBootRecord* pNTFSBootRecord = (NTFSBootRecord*)dataBuffer;
}


