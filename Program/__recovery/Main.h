//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.FileCtrl.hpp>
#include "sqlite3.h"
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtDlgs.hpp>
#include "VirtualTrees.hpp"
#include <Vcl.CheckLst.hpp>
#include <Vcl.WinXCtrls.hpp>
#include <vector>
#include "FileSistem.h"
#include "ReadThread.h"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TButton *ButtonSelectVolume;
	TLabel *Label2;
	TEdit *EditBD;
	TButton *ButtonSelectBD;
	TEdit *EditNameVolume;
	TOpenTextFileDialog *OpenTextFileDialog1;
	TLabel *ClusterSizeLabel;
	TLabel *Label7;
	TLabel *Label6;
	TLabel *ClusterCountLabel;
	TLabel *FSSizeLabel;
	TLabel *Label3;
	TLabel *Label12;
	TLabel *LabelTypeFile;
	TButton *ButtonScan;
	TToggleSwitch *ToggleSwitchSearchType;
	TCheckListBox *CheckListBoxType;
	TVirtualStringTree *VirtualStringTree1;
	TButton *ButtonScanStop;
	void __fastcall ButtonSelectVolumeClick(TObject *Sender);
	void __fastcall ButtonSelectBDClick(TObject *Sender);
    void __fastcall ButtonScanClick(TObject *Sender);
	void __fastcall CheckListBoxTypeClickCheck(TObject *Sender);
	void __fastcall ToggleSwitchSearchTypeClick(TObject *Sender);
	void __fastcall VirtualStringTree1GetText(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText);
	void __fastcall ButtonScanStopClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
    // Ссылка на БД
	sqlite3* Database;
    // Список выбранных типов файлов
	// (всё что было отмечено в CheckListBox)
	// вектор, состоящий из кол-ва выбранных сигнатур, типа UnicodeString
	std::vector<UnicodeString> SelectedTypes;

	// Ссылка на файловую систему
	class ClassFS* FileSystem;     // В данной реализации может быть только NTFS
	class ReadThread* ReadThreadPtr;
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------


// Неоходимая структура таблицы в БД:
/*
CREATE TABLE "scans" (
	"id"	INTEGER NOT NULL UNIQUE,
	"cluster_number"	INTEGER,
	"type"	TEXT,
	PRIMARY KEY("id" AUTOINCREMENT)
);
*/
typedef struct {
	DWORD Id;
	DWORD ClusterNumber;
	UnicodeString Type;
} ScansTableStruct;

#endif
