//---------------------------------------------------------------------------

#ifndef NTFSH
#define NTFSH
#include <vcl.h>
#include "ClassFS.h"
#include "TypeFile.h"
//---------------------------------------------------------------------------
#pragma pack(push, 1)
// NTFS
typedef struct {
	char Padding1[3];                           // Смещение
	char OEM[8];                           		// Сигнатура ФС
	unsigned short SectorSize;					// Размер сектора в байтах
	unsigned char ClusterSize;                  // Кол-во секторов в одном кластере
	char Padding2[7];                           // Смещение
	char DeviceType;							// Тип носителя данных
	char Padding3[18];                          // Смещение
	unsigned long SectorsVolumeCount;  			// Общее количество секторов в разделе
} NTFS_BootRecord;
#pragma pack(pop)

// Класс-наследник от ClassFS
class NTFS : public ClassFS {
	public:
		NTFS(HANDLE FileHandle);
		IteratorFS* GetIterator();
};

// Декоратор итератора
class NTFSIterator : public IteratorFS {
	public:
		NTFSIterator(ClassFS* FileSystem);

};


#endif
