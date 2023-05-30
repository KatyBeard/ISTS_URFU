//---------------------------------------------------------------------------

#pragma hdrstop

#include "FileSistem.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// Проверка типа файловой системы
FSType DetermineFileSystemType(HANDLE FileHandle) {
	char dataBuffer[512];         // Считывание первего сектора
	// Позиционирование в файле
	LARGE_INTEGER sectorOffset;           // LARGE_INTEGER - 64-разрядное целое значение со знаком
	// Смещение от начала кластера
	sectorOffset.QuadPart = 0;

	// Задаём позицию
	DWORD currentPosition = SetFilePointer(
		FileHandle,    //  Дескриптор файла
		sectorOffset.LowPart,   //  Байты перемещения указателя
		&sectorOffset.HighPart,
		FILE_BEGIN    //  Точка отсчета равняется нулю или это начало файла
	);

	// Проверка на ошибку при смещении
	if (currentPosition != sectorOffset.LowPart) {
		ShowMessage(L"ошибку при смещении");
		return FSType::NULL_FS;
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

	// Проерка на ошибку при чтении данных
	if (!(readResult && bytesRead == bytesToRead)) {
		ShowMessage(L"ошибку при чтении данных");
		return FSType::NULL_FS;
	}

	// Проверяем наличие ключевого слова в загрузочном секторе
	if (strstr(dataBuffer, "NTFS") != 0) {
		return FSType::NTFS_FS;
	}

	// Если это не NTFS, выводим NULL
	ShowMessage(L"не NTFS");
	return FSType::NULL_FS;
}

ClassFS* CreateFileSystem(HANDLE FileHandle) {
	ClassFS* FileSystem;
	FileSystem = new NTFS(FileHandle);
	return FileSystem;
}