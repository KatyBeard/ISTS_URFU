//---------------------------------------------------------------------------

#pragma hdrstop

#include "ClassFS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

ClassFS::ClassFS(HANDLE FileHandle)
	: FileHandle(FileHandle)
{

}

ClassFS::~ClassFS()
{
	CloseHandle(FileHandle);
}

bool ClassFS::ReadCluster(char* dataBuffer, DWORD clusterNumber)
{
	// ��������������� � �����
	LARGE_INTEGER sectorOffset;
	// �������� �� ������ ��������
	sectorOffset.QuadPart = (LONGLONG)(clusterNumber*GetClusterSize());

	// ����� �������
	DWORD currentPosition = SetFilePointer(
		FileHandle,
		sectorOffset.LowPart,
		&sectorOffset.HighPart,
		FILE_BEGIN
	);
	// �������� ����������������
	if (currentPosition != sectorOffset.LowPart) {
		return false;
	}

	DWORD bytesToRead = GetClusterSize();	// ������� ������ ����� ���������
	DWORD bytesRead; 	    				// ������� ������ ������� �������

	// ������ ������
	bool readResult = ReadFile(
		FileHandle,
		dataBuffer,
		bytesToRead,
		&bytesRead,
		NULL
	);

    // ���� �������, �� �������� ���-�� ��������� ����
	return readResult && bytesRead == bytesToRead;
}

UINT64 ClassFS::GetFileSystemSize() {
	return fileSystemSize;
}

DWORD ClassFS::GetClusterCount() {
	return clusterCount;
}

DWORD ClassFS::GetClusterSize() {
	return clusterSize;
}

IteratorFS::IteratorFS(ClassFS* FileSystem)
	: FileSystem(FileSystem)      // �������� ���������� ����������
	// FileSystem � class ClassFS* FileSystem � .h
{
	// ��������� ����� ��� �������� � ������ ��������
	clusterBuffer = new char[FileSystem->GetClusterSize()];

	done = false;   // ������ �� ���������
}

// ����������
IteratorFS::~IteratorFS() {
	// ������� ������ ��� �������� ���������
	delete[] clusterBuffer;
}

void IteratorFS::First() {
	// ��������� ������� ������
	currentPosition = 0;

	// ������ ������� �������
	if (!FileSystem->ReadCluster(clusterBuffer, 0)) {
		done = true;    // ���� ������ ������, �� ���� - ��������� ������
	}
}

void IteratorFS::Next() {
	currentPosition++;    //����������� ������� ������� �� 1
	// ������ ������� �� ������ currentPosition
	if (!FileSystem->ReadCluster(clusterBuffer, currentPosition)) {
		done = true;
	}
}

// �������� ���������� ������
// ���� �� �������� ������, �� ������� "currentPosition + 1",
// ������� �� ��� �������� �� ������� ���������?
bool IteratorFS::IsDone() {
	return done || currentPosition + 1 >= FileSystem->GetClusterCount();
}

char* IteratorFS::GetCurrent() {       // ���������� ������� ����������� ������� ��� ���������� ������
	return clusterBuffer;
}

// ��������� ������ ��� ����������
DWORD IteratorFS::GetCurrentPosition() {      // ������� �������
	return currentPosition;
}

ClassFS* IteratorFS::GetFileSystem() {     // ������� ��
	return FileSystem;
}