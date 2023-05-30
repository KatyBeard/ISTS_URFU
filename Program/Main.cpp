//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VirtualTrees"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
// ����� ���������� (�����) + ����� ���������� � �� (�������� �������)
void __fastcall TForm1::ButtonSelectVolumeClick(TObject *Sender)
{
	UnicodeString volumeName = L"";
	// ����� ����������� ���� ������ ����������
	if (!SelectDirectory("�������� ����", "", volumeName)) {
		return;
	}
	// �������� �����
	// ����� ������� CreateDeviceHandle (�������� �����������)
	// HANDLE - ���������� (���������), �.�. ����� (������), � ������� �������� ����� ���������������� ������.
	//HANDLE FileHandle = CreateDeviceHandle(volumeName);
    wchar_t deviceName[20];
	// �������� �� path ������ ������ ������ (����� �����)
	swprintf(deviceName, L"\\\\.\\%c:", volumeName.w_str()[0]);

	// ������ handle ��� �����
	// HANDLE - ����������, �.�. �����, � ������� �������� ����� ���������������� ������.
	// � ������� ������������ ����� ��������� �� ����, ������� ����, ����������� ������� � �.�.
	HANDLE FileHandle = CreateFileW(
		deviceName,      // ��������� ����
		GENERIC_READ,    //  ������ ������� � ����� ��� ������
		FILE_SHARE_READ | FILE_SHARE_WRITE, //  ����� ����������� �������������
		NULL,    //  �������� ������������ - �� ���������
		OPEN_EXISTING, //  �������� �����, ������ ���� �� ����������
		FILE_ATTRIBUTE_NORMAL,  //  �������� �� ���������
		NULL    //  �� ���������� �������� �����
	);

	// ������ �������� �����
	if (FileHandle == INVALID_HANDLE_VALUE) {
		ShowMessage(L"������ ������ �����! ���������� ������� ��������� � ������� ��������������.");
		return;
	}
	// �������� ������ NTFS?
	FSType Type = DetermineFileSystemType(FileHandle);

	// ���� �� �� NTFS, ����� ���������
	if (static_cast<int>(Type) == 0) {
		ShowMessage(L"�������� ������� �� ��������������");
		return;
	}

	// ���������� ���������� ������
	FileSystem = CreateFileSystem(FileHandle);

    // ����� ���������� ��� ��
	FSSizeLabel->Caption = FileSystem->GetFileSystemSize()/1024;  // ������ � ��
	ClusterCountLabel->Caption = FileSystem->GetClusterCount();   // ���-�� ���������
	ClusterSizeLabel->Caption = FileSystem->GetClusterSize();     // ������ ��������

	// ����� � Edit ����� ��
	EditNameVolume->Text = volumeName;
}

void __fastcall TForm1::ButtonSelectBDClick(TObject *Sender)
{
	// ������� ��, ���� ��� ��� ���� �������
	if (Database != NULL) {
		sqlite3_close(Database);
	}

	// ������ ������������ ������� ���� � ��
	// ��������� � "���������" ������ OpenTextFileDialog1
	OpenTextFileDialog1->Execute();

	// �������� ��
	int openResult = sqlite3_open16(OpenTextFileDialog1->FileName.w_str(), &Database);
	if (openResult != 0) { // �����-���� ������
		ShowMessage(L"�� ������� ������� ���� ������!");
		return;
	}

	// ��������� ���� � �� � ������� �� � EditBD ��� ����������� � ����
	EditBD->Text = OpenTextFileDialog1->FileName;

}
//---------------------------------------------------------------------------
// ��������� ������� ������� �� ToggleSwitch
void __fastcall TForm1::ToggleSwitchSearchTypeClick(TObject *Sender)
{
   if (ToggleSwitchSearchType->State == tssOn) { // ��������
		ToggleSwitchSearchType->ThumbColor = clGreen;    // ���� - �������
		CheckListBoxType->Enabled = true;      // ���-���� � ������� �������
		LabelTypeFile->Enabled = true;

	} else {                                    // ���������
		ToggleSwitchSearchType->ThumbColor = clRed;   // ���� - �������
		CheckListBoxType->Enabled = false;    // ���-���� � ������� �� �������
		LabelTypeFile->Enabled = false;
	}
}

//---------------------------------------------------------------------------

// ��������� ������� ������ ���� ����� � ���-�����
void __fastcall TForm1::CheckListBoxTypeClickCheck(TObject *Sender)
{
	// ������� ���������� �������
	SelectedTypes.clear();

	// ���������� ������ � ���������� ������
	for (int i = 0; i < CheckListBoxType->Count; i++) {
		// ���� ������� "�����", �� ������� ��� � ������ CheckedTypes
		if (CheckListBoxType->Checked[i]) {
			// ��������� � ����� ������ �������� ���� (������ ������� +1)
			SelectedTypes.push_back( // ������� push_back() ��������� �������� ����� ������� � ����� �������
					CheckListBoxType->Items->Strings[i]
			);
		}
	}
}

//---------------------------------------------------------------------------
// ��������� ������� ��������� ������ ��� VirtualStringTree1
void __fastcall TForm1::VirtualStringTree1GetText(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText)
{
	if (Node == NULL) return;
	ScansTableStruct* nodeData = (ScansTableStruct*) VirtualStringTree1->GetNodeData(Node);
	// ���� ��������� � typedef struct � main.h
	switch (Column)
	{
		case 0:
		{
			CellText = nodeData->Id;
			break;
		}
		case 1:
		{
			CellText = nodeData->ClusterNumber;
			break;
		}
		case 2:
		{
			CellText = nodeData->Type;
			break;
		}
	}
}
//---------------------------------------------------------------------------

// ������� ������ �����������
void __fastcall TForm1::ButtonScanClick(TObject *Sender)
{
	if (EditBD->Text != L"���� � ��" && EditNameVolume->Text != L"����� ����") {
		// ������ ������
		IteratorFS* Iterator = FileSystem->GetIterator();

		// ����� �� ����� �� ���� �����?  true or false
		bool typeSearch = ToggleSwitchSearchType->State == tssOn;

        // ������ ������ ������
		// false - �������� �� ��, ��� �� ����������� �����, ��� ������ ��������
		// TForm1 (this) - ��������� �� ����� ����  , �������� � ����� ����� ����
		ReadThreadPtr = new ReadThread(false, Iterator, this, typeSearch);
	} else {
		ShowMessage(L"�� ������ ���� � �� �/��� ����� ����");
		return;
	}
}

//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonScanStopClick(TObject *Sender)
{
     // ��������� ������
	 ReadThreadPtr->Terminate();
}
//---------------------------------------------------------------------------


