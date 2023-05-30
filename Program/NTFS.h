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
	char Padding1[3];                           // ��������
	char OEM[8];                           		// ��������� ��
	unsigned short SectorSize;					// ������ ������� � ������
	unsigned char ClusterSize;                  // ���-�� �������� � ����� ��������
	char Padding2[7];                           // ��������
	char DeviceType;							// ��� �������� ������
	char Padding3[18];                          // ��������
	unsigned long SectorsVolumeCount;  			// ����� ���������� �������� � �������
} NTFS_BootRecord;
#pragma pack(pop)

// �����-��������� �� ClassFS
class NTFS : public ClassFS {
	public:
		NTFS(HANDLE FileHandle);
		IteratorFS* GetIterator();
};

// ��������� ���������
class NTFSIterator : public IteratorFS {
	public:
		NTFSIterator(ClassFS* FileSystem);

};


#endif
