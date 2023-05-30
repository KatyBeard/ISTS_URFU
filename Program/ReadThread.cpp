//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "ReadThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall ReadThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall ReadThread::ReadThread(bool CreateSuspended, IteratorFS* Iterator, TForm1* Form1, bool useTypeFileSearch)
	: TThread(CreateSuspended), Iterator(Iterator), Form1(Form1)
{   // ��� �������� ������ �������� ��������,
	// ����� ��� �������� ������ ���� ��������������� ��������
	FreeOnTerminate = true;
	// this ���������� ��� ������ �� ����� ����, ��� ������������ �����������
	this->MyEvent = new TEvent(NULL, true, false, "", false);
	// true - �������� �� ��, ��� ������� ����� ������� �� ������� WriteThreadPtr->Start()
	// Iterator->GetFileSystem()->GetClusterSize() - �������� ������ ��������
	// Form1 - ��� ����� � ������
	// useTypeFileSearch  - �������� �� ������, ����� �� ������ ���������� �����
	this->WriteThreadPtr = new WriteThread(true, MyEvent, Iterator->GetFileSystem()->GetClusterSize(), Form1, useTypeFileSearch);
}
//---------------------------------------------------------------------------
void __fastcall ReadThread::Execute()
{
	Synchronize(&OnStart);
	WriteThreadPtr->Start(); // ��������� �����, ������� ����� ������������ ������

	// ������� ������ ��������� - ������� � ����� ����������,
	// ��� ���� ��� ������ ������ ������� � WriteThread
	// ����� ����������� ������� "�� ��������� �� ������?",
	// ������ ��������� - ���� ������ ���������, �� ��� ���������� ��� ��������� ��������
	for (Iterator->First(); !Iterator->IsDone(); Iterator->Next()) {
		if (Terminated) break;

		// ������� ������� ������ ��������� �������
		WriteThreadPtr->receivedBuffer = Iterator->GetCurrent();
		// clusterNumber - ������� �������
		WriteThreadPtr->clusterNumber = Iterator->GetCurrentPosition();

		// ��������� ������ (������ ����� �������� ������������ ���������� �������)
		MyEvent->SetEvent();

		// ���� ���� - ������� ������ ������ (MyEvent->ResetEvent) �� ������� ������
		while (MyEvent->WaitFor(0) == wrSignaled)
		{
			// ���� ������ ������ ��������� ������������� �� �����
			if(Terminated) break;
		}
	}

	WriteThreadPtr->Terminate();   // ��������� ����� ��������

	Synchronize(&OnStop);
	WriteThreadPtr->WaitFor();  // ����, ���� ProcessThread �� ����������

	delete WriteThreadPtr;   // ������� ��������� �� �����
}
//---------------------------------------------------------------------------

void __fastcall ReadThread::OnStart() {
	Form1->LabelScanStart->Enabled = true; // ������ �����������
	Form1->LabelScanEnd->Enabled = false;  // ������ �������� ������������
}

void __fastcall ReadThread::OnStop() {
	Form1->LabelScanStart->Enabled = false;    // ������ �����������
	Form1->LabelScanEnd->Enabled = true;       // ������ �������� ������������
}

