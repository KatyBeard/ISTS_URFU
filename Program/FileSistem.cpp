//---------------------------------------------------------------------------

#pragma hdrstop

#include "FileSistem.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// �������� ���� �������� �������
FSType DetermineFileSystemType(HANDLE FileHandle) {
	char dataBuffer[512];         // ���������� ������� �������
	// ���������������� � �����
	LARGE_INTEGER sectorOffset;           // LARGE_INTEGER - 64-��������� ����� �������� �� ������
	// �������� �� ������ ��������
	sectorOffset.QuadPart = 0;

	// ����� �������
	DWORD currentPosition = SetFilePointer(
		FileHandle,    //  ���������� �����
		sectorOffset.LowPart,   //  ����� ����������� ���������
		&sectorOffset.HighPart,
		FILE_BEGIN    //  ����� ������� ��������� ���� ��� ��� ������ �����
	);

	// �������� �� ������ ��� ��������
	if (currentPosition != sectorOffset.LowPart) {
		ShowMessage(L"������ ��� ��������");
		return FSType::NULL_FS;
	}

	DWORD bytesToRead = 512;				// ������� ������ ����� ���������
	DWORD bytesRead; 	    				// ������� ������ ������� �������

	// ������ ������
	bool readResult = ReadFile(
		FileHandle,
		dataBuffer,
		bytesToRead,
		&bytesRead,
		NULL
	);

	// ������� �� ������ ��� ������ ������
	if (!(readResult && bytesRead == bytesToRead)) {
		ShowMessage(L"������ ��� ������ ������");
		return FSType::NULL_FS;
	}

	// ��������� ������� ��������� ����� � ����������� �������
	if (strstr(dataBuffer, "NTFS") != 0) {
		return FSType::NTFS_FS;
	}

	// ���� ��� �� NTFS, ������� NULL
	ShowMessage(L"�� NTFS");
	return FSType::NULL_FS;
}

ClassFS* CreateFileSystem(HANDLE FileHandle) {
	ClassFS* FileSystem;
	FileSystem = new NTFS(FileHandle);
	return FileSystem;
}