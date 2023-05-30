//---------------------------------------------------------------------------

#ifndef FileSistemH
#define FileSistemH
#include <vcl.h>
#include "ClassFS.h"
#include "NTFS.h"
//---------------------------------------------------------------------------

// Перечисление типов файловых систем
enum class FSType {
	NULL_FS=0,
	NTFS_FS=1        // Добавляем FS для однозначного определения
};

// Параметризированный фабричный метод создания ФС
ClassFS* CreateFileSystem(HANDLE FileHandle);

// Функция определения ФС
FSType DetermineFileSystemType(HANDLE FileHandle);
#endif
