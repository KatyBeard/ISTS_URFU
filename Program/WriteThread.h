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
	DWORD bufferSize; // DWORD - 32-битное беззнаковое целое. Аналоги: unsigned long int, UINT.
	class TForm1* Form1;

	char* copyBuffer; // для работы с данными необходимо их скопировать
	ClusterTypeFile clusterTypeFile;  // определеяем сигнатуру кластера
	bool useTypeFileSearch; // нужно ли искать сигнатуры?

	// функция по добавлению кластера в БД
	bool addClusterToDB();
protected:
	void __fastcall Execute();
public:
	__fastcall WriteThread(bool CreateSuspended, TEvent* MyEvent, DWORD bufferSize, TForm1* Form1, bool useTypeFileSearch);
	void __fastcall addClustersToVST();  // по завершению вывод информации в VirtualStringTree

	char* receivedBuffer; // для БД
    DWORD clusterNumber;
};
//---------------------------------------------------------------------------
#endif
