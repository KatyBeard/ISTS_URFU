//---------------------------------------------------------------------------

#pragma hdrstop

#include "NTFS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

NTFS::NTFS(HANDLE FileHandle):ClassFS(FileHandle)
{
	char dataBuffer[512];
	// Позиционируемся в файле
	LARGE_INTEGER sectorOffset;
	// Смещение от начала кластера
	sectorOffset.QuadPart = 0;

	// Задаём позицию
	DWORD currentPosition = SetFilePointer(
		FileHandle,
		sectorOffset.LowPart,
		&sectorOffset.HighPart,
		FILE_BEGIN
	);

	// Проверка на ошибку при смещении
	if (currentPosition != sectorOffset.LowPart) {
		return;
	}

	DWORD bytesToRead = 512;				// Сколько байтов нужно прочитать
	DWORD bytesRead; 	    				// Сколько байтов удалось считать

	// Чтение данных
	bool readResult = ReadFile(
		FileHandle,
		dataBuffer,
		bytesToRead,
		&bytesRead,
		NULL
	);

	// Ошибки при смещении нет
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

