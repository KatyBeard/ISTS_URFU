#pragma once
#include <windows.h>
#pragma pack(push,1)

typedef struct
{
	BYTE    jump[3];	//	JMP
	UINT nameLowest;	//	OEM lowest
	UINT nameHighest;	//	OEM highest
	USHORT bytesPerSector;	//	Количество байт на сектор
	BYTE sectorsPerCluster;	//	Количество секторов в кластере
	BYTE    reserved0[7];
	BYTE    mediaDesc;
	UINT16  reserved2;
	UINT16  sectorsPerTrack;	//	Количество секторов на дорожке
	UINT16  numHeads;	//	Количество магнитных головок
	BYTE    reserved3[8];
	UINT16  reserved4;
	UINT16  reserved5;
	ULONGLONG  numSectors;	//	Количество секторов в разделе
	ULONGLONG  mftCluster;	//	Номер кластера MFT
	ULONGLONG  mftClusterCopy;	//	Номер кластера копии MFT
	UINT  mftClusterSize;	//	Размер MFT в кластерах
	UINT  bufferSize;	//	Размер буфера индекса в кластерах
	////	Серийный номер диска
	UINT  volumeIdLowest;
	UINT volumeIdHighest;
	BYTE padding[0x1AE];
	USHORT signature;	//	Сигнатура
} NTFSBootRecord;
# pragma pack(pop)