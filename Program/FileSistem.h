//---------------------------------------------------------------------------

#ifndef FileSistemH
#define FileSistemH
#include <vcl.h>
#include "ClassFS.h"
#include "NTFS.h"
//---------------------------------------------------------------------------

// ������������ ����� �������� ������
enum class FSType {
	NULL_FS=0,
	NTFS_FS=1        // ��������� FS ��� ������������ �����������
};

// ������������������� ��������� ����� �������� ��
ClassFS* CreateFileSystem(HANDLE FileHandle);

// ������� ����������� ��
FSType DetermineFileSystemType(HANDLE FileHandle);
#endif
