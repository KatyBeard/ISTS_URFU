//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "dllmain.h"


#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
	int group = Group();
	Edit1->Text = group;

	char *ptrToBuffer;
	int strSize = FIO(NULL);
	ptrToBuffer = new char[strSize];
	FIO(ptrToBuffer);

//	const char* fio = FIO();
	Edit2->Text = ptrToBuffer;
	delete[] ptrToBuffer;
}
//---------------------------------------------------------------------------

