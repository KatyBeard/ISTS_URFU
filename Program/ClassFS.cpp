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
	// Позиционируемся в файле
	LARGE_INTEGER sectorOffset;
	// Смещение от начала кластера
	sectorOffset.QuadPart = (LONGLONG)(clusterNumber*GetClusterSize());

	// Задаём позицию
	DWORD currentPosition = SetFilePointer(
		FileHandle,
		sectorOffset.LowPart,
		&sectorOffset.HighPart,
		FILE_BEGIN
	);
	// Проверка позиционирования
	if (currentPosition != sectorOffset.LowPart) {
		return false;
	}

	DWORD bytesToRead = GetClusterSize();	// Сколько байтов нужно прочитать
	DWORD bytesRead; 	    				// Сколько байтов удалось считать

	// Чтение данных
	bool readResult = ReadFile(
		FileHandle,
		dataBuffer,
		bytesToRead,
		&bytesRead,
		NULL
	);

    // Если успешно, то проверка кол-ва считанных байт
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
	: FileSystem(FileSystem)      // помещаем переданную переменную
	// FileSystem в class ClassFS* FileSystem в .h
{
	// Объявляем буфер для кластера с нужным размером
	clusterBuffer = new char[FileSystem->GetClusterSize()];

	done = false;   // работа не закончена
}

// Деструктор
IteratorFS::~IteratorFS() {
	// Очистка буфера при удалении итератора
	delete[] clusterBuffer;
}

void IteratorFS::First() {
	// Начальная позиция чтения
	currentPosition = 0;

	// Читаем нулевой кластер
	if (!FileSystem->ReadCluster(clusterBuffer, 0)) {
		done = true;    // если ошибка чтения, то флаг - завершили работу
	}
}

void IteratorFS::Next() {
	currentPosition++;    //увеличивает текущую позицию на 1
	// Читаем кластер по номеру currentPosition
	if (!FileSystem->ReadCluster(clusterBuffer, currentPosition)) {
		done = true;
	}
}

// Проверка завершения работы
// Если не звершили работу, то смотрим "currentPosition + 1",
// выходит ли это значение за границы кластеров?
bool IteratorFS::IsDone() {
	return done || currentPosition + 1 >= FileSystem->GetClusterCount();
}

char* IteratorFS::GetCurrent() {       // Возвращаем текущий прочитанный кластер для дальнейшей работы
	return clusterBuffer;
}

// Служебные методы для декоратора
DWORD IteratorFS::GetCurrentPosition() {      // Текущая позиция
	return currentPosition;
}

ClassFS* IteratorFS::GetFileSystem() {     // Текущая ФС
	return FileSystem;
}