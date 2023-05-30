//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop

#include "WriteThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(&UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall ProcessThread::UpdateCaption()
//      {
//        Form1->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------

__fastcall WriteThread::WriteThread(bool CreateSuspended, TEvent* MyEvent, DWORD bufferSize, TForm1* Form1, bool useTypeFileSearch)
 : TThread(CreateSuspended), MyEvent(MyEvent), bufferSize(bufferSize), Form1(Form1), useTypeFileSearch(useTypeFileSearch)
{
	FreeOnTerminate = true; // ������� ��� ����������
	receivedBuffer = new char[bufferSize];  // ��������� ������
	copyBuffer = new char[bufferSize];
}
//---------------------------------------------------------------------------
void __fastcall WriteThread::Execute()
{
	while (!Terminated)
	{
		// ��������� ���������� �� ���� (� ��������� � 0 ��)
		if (MyEvent->WaitFor(0) == wrSignaled)   // �������� 0 ���. wrsignaled, ���� ��������� ����������� �������, wrTimeout, ���� �� ����� ����-���� ������ �� ���������.
		{
			// �������� �����
			memcpy(copyBuffer, receivedBuffer, bufferSize);

			// ���������� ���� �������, ����� ReadThread ��������� ������
			MyEvent->ResetEvent();

			// �������� � copyBuffer
			if (useTypeFileSearch) {
				clusterTypeFile = FindTypeFile(copyBuffer, Form1->SelectedTypes); // ���� ��������� �� ���������� � �����
				if (static_cast<int>(clusterTypeFile) == 0) {
					continue; // ���� ��������� �� �������, �� ������ ������������
				}
			}

			// ������� ������ � ��
			addClusterToDB();
		}
	}  // ��� ��� ���� ������� ��� �������, �� ������� �� ��������� ����� �� ����� �����������.
    // ��-�� ����� ���� ����������� "� ��������" �� ��� ���, ���� ReadThread ����� �� �������� ������ MyEvent->SetEvent();

	Synchronize(&addClustersToVST);

	delete[] receivedBuffer, copyBuffer;
}
//---------------------------------------------------------------------------

bool WriteThread::addClusterToDB() {
	// ��� 1 (���������� �������)
	const char* errmsg;  // ���������� ������
	sqlite3_stmt* pStatement; // ������ ������ � �� (����� � ���� ������)

	// ��������� ������� � ���� ������
	wchar_t sql[] = L"INSERT INTO scans (cluster_number, type) VALUES (?, ?);";

	int result = sqlite3_prepare16_v2(Form1->Database, sql, -1, &pStatement, NULL);
	if (result != SQLITE_OK) {
		errmsg = sqlite3_errmsg(Form1->Database);
		ShowMessage(L"������ ��� ���������� INSERT ��� ������� scans: " + UnicodeString(errmsg));
		// ��� 3 (���������� ��������� �������)
		sqlite3_finalize(pStatement);
		return false;
	}

	// ����������� �������� ������ ?, ?
	result = sqlite3_bind_int64(pStatement, 1, clusterNumber);
	if (result != SQLITE_OK) {
		errmsg = sqlite3_errmsg(Form1->Database);
		ShowMessage(L"������ ��� ���������� INSERT ��� ������� scans: " + UnicodeString(errmsg));
		// ��� 3 (���������� ��������� �������)
		sqlite3_finalize(pStatement);
		return false;
	}

	result = sqlite3_bind_text(pStatement, 2, GetTypeFileName(clusterTypeFile), -1, SQLITE_STATIC);
	if (result != SQLITE_OK) {
		errmsg = sqlite3_errmsg(Form1->Database);
		ShowMessage(L"������ ��� ���������� INSERT ��� ������� scans: " + UnicodeString(errmsg));
		// ��� 3 (���������� ��������� �������)
		sqlite3_finalize(pStatement);
		return false;
	}

	// ��� 2 (���������� �������)
	result = sqlite3_step(pStatement);

	// ��������� ���������
	if (result != SQLITE_DONE) {
		errmsg = sqlite3_errmsg(Form1->Database);
		ShowMessage(L"������ ��� ���������� INSERT ��� ������� scans: " + UnicodeString(errmsg));

		// ��� 3 (���������� ��������� �������)
		sqlite3_finalize(pStatement);
		return false;
	}

	// ��� 3 (���������� ��������� �������)
	sqlite3_finalize(pStatement);

	return true;
}

void  __fastcall WriteThread::addClustersToVST() {
	TVirtualStringTree* VST = Form1->VirtualStringTree1;

	// ��� 1 (���������� �������)
	const char *errmsg;
	sqlite3_stmt *pStatement;

	// ��������� ������ � ����������� �������� �� ���������
	wchar_t sql[] = L"SELECT id, cluster_number, type FROM scans;";

	int result = sqlite3_prepare16_v2(Form1->Database, sql, -1, &pStatement, NULL);

	if (result == SQLITE_OK) {
		VST->Clear();
		VST->BeginUpdate();

		// ��� 2 (���������� ������� � ��������� ���������� ��� ��������� �����)
		while (true) {
			result = sqlite3_step(pStatement);
			if(result != SQLITE_ROW) break;

			PVirtualNode entryNode = VST->AddChild(VST->RootNode);
			ScansTableStruct* nodeData = (ScansTableStruct*) VST->GetNodeData(entryNode);

			nodeData->Id = sqlite3_column_int64(pStatement, 0);
			nodeData->ClusterNumber = sqlite3_column_int64(pStatement, 1);
			nodeData->Type = UnicodeString((char*)sqlite3_column_text(pStatement, 2));
		}

		VST->EndUpdate();
	} else {
		errmsg = sqlite3_errmsg(Form1->Database);
		ShowMessage(L"������ ��� ���������� SELECT ��� ������� scans: " + UnicodeString(errmsg));
	}

	// ��� 3 (���������� ��������� �������)
	sqlite3_finalize(pStatement);
}
