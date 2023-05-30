//---------------------------------------------------------------------------

#pragma hdrstop

#include "NTFS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

NTFS::NTFS(HANDLE FileHandle):ClassFS(FileHandle)
{
	char dataBuffer[512];
	// ��������������� � �����
	LARGE_INTEGER sectorOffset;
	// �������� �� ������ ��������
	sectorOffset.QuadPart = 0;

	// ����� �������
	DWORD currentPosition = SetFilePointer(
		FileHandle,
		sectorOffset.LowPart,
		&sectorOffset.HighPart,
		FILE_BEGIN
	);

	// �������� �� ������ ��� ��������
	if (currentPosition != sectorOffset.LowPart) {
		return;
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

	// ������ ��� �������� ���
	if (readResult && bytesRead == bytesToRead) {
		NTFS_BootRecord* ntfsBR = (NTFS_BootRecord*) dataBuffer;
		clusterSize = (DWORD)(ntfsBR->SectorSize * ntfsBR->ClusterSize); // 	DWORD = unsigned long
		clusterCount = (DWORD)(
				ntfsBR->SectorsVolumeCount
				* ntfsBR->SectorSize
				/ clusterSize
		);
		fileSystemSize = clusterSize*clusterCount; // UINT64 = unsigned __int64
	}
}

IteratorFS* NTFS::GetIterator() {
	return new NTFSIterator(this);
}

NTFSIterator::NTFSIterator(ClassFS* FileSystem)
	: IteratorFS(FileSystem)
{

}

