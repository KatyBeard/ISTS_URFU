//---------------------------------------------------------------------------

#ifndef ReadThreadH
#define ReadThreadH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <vcl.h>
#include "WriteThread.h"
#include "Main.h"
//---------------------------------------------------------------------------
class ReadThread : public TThread
{
private:
	class IteratorFS* Iterator;
	class WriteThread* WriteThreadPtr;   // ���� ������ �� ������ �����-�������
	class TForm1* Form1;
	TEvent* MyEvent;  // ���� ������ �� MyEvent ��� ����, ����� "��������" � WriteThread
protected:
	void __fastcall Execute();
public:
	__fastcall ReadThread(bool CreateSuspended, IteratorFS* Iterator, TForm1* Form1, bool useTypeFileSearch);
};
//---------------------------------------------------------------------------
#endif
