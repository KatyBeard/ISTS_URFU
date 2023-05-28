#pragma once
#include <windows.h>
#pragma pack(push,1)

typedef struct
{
	BYTE    jump[3];	//	JMP
	UINT nameLowest;	//	OEM lowest
	UINT nameHighest;	//	OEM highest
	USHORT bytesPerSector;	//	���������� ���� �� ������
	BYTE sectorsPerCluster;	//	���������� �������� � ��������
	BYTE    reserved0[7];
	BYTE    mediaDesc;
	UINT16  reserved2;
	UINT16  sectorsPerTrack;	//	���������� �������� �� �������
	UINT16  numHeads;	//	���������� ��������� �������
	BYTE    reserved3[8];
	UINT16  reserved4;
	UINT16  reserved5;
	ULONGLONG  numSectors;	//	���������� �������� � �������
	ULONGLONG  mftCluster;	//	����� �������� MFT
	ULONGLONG  mftClusterCopy;	//	����� �������� ����� MFT
	UINT  mftClusterSize;	//	������ MFT � ���������
	UINT  bufferSize;	//	������ ������ ������� � ���������
	////	�������� ����� �����
	UINT  volumeIdLowest;
	UINT volumeIdHighest;
	BYTE padding[0x1AE];
	USHORT signature;	//	���������
} NTFSBootRecord;
# pragma pack(pop)