//---------------------------------------------------------------------------

#ifndef WriteThreadH
#define WriteThreadH
#include <System.Classes.hpp>
#include <vcl.h>
#include <cstring>
#include "TypeFile.h"
#include "Main.h"
#include "sqlite3.h"
//---------------------------------------------------------------------------
class WriteThread : public TThread
{
private:
	TEvent* MyEvent;
	DWORD bufferSize; // DWORD - 32-������ ����������� �����. �������: unsigned long int, UINT.
	class TForm1* Form1;

	char* copyBuffer; // ��� ������ � ������� ���������� �� �����������
	ClusterTypeFile clusterTypeFile;  // ����������� ��������� ��������
	bool useTypeFileSearch; // ����� �� ������ ���������?

	// ������� �� ���������� �������� � ��
	bool addClusterToDB();
protected:
	void __fastcall Execute();
public:
	__fastcall WriteThread(bool CreateSuspended, TEvent* MyEvent, DWORD bufferSize, TForm1* Form1, bool useTypeFileSearch);
	void __fastcall addClustersToVST();  // �� ���������� ����� ���������� � VirtualStringTree

	char* receivedBuffer; // ��� ��
    DWORD clusterNumber;
};
//---------------------------------------------------------------------------
#endif
