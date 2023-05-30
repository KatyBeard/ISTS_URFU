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
// Выбор директории (диска) + вывод информации о ФС (файловой системе)
void __fastcall TForm1::ButtonSelectVolumeClick(TObject *Sender)
{
	UnicodeString volumeName = L"";
	// Вывод диалогового окна выбора директории
	if (!SelectDirectory("Выберите диск", "", volumeName)) {
		return;
	}
	// Открытие диска
	// Вызов функции CreateDeviceHandle (создание дискриптора)
	// HANDLE - дескриптор (указатель), т.е. число (индекс), с помощью которого можно идентифицировать ресурс.
	//HANDLE FileHandle = CreateDeviceHandle(volumeName);
    wchar_t deviceName[20];
	// Копируем из path только первый символ (букву диска)
	swprintf(deviceName, L"\\\\.\\%c:", volumeName.w_str()[0]);

	// Создаём handle для диска
	// HANDLE - дескриптор, т.е. число, с помощью которого можно идентифицировать ресурс.
	// С помощью дескприторов можно ссылаться на окна, объекты ядра, графические объекты и т.п.
	HANDLE FileHandle = CreateFileW(
		deviceName,      // Открываем диск
		GENERIC_READ,    //  Запрос доступа к диску для чтения
		FILE_SHARE_READ | FILE_SHARE_WRITE, //  Режим совместного использования
		NULL,    //  Атрибуты безопасности - по умолчанию
		OPEN_EXISTING, //  Открытие диска, только если он существует
		FILE_ATTRIBUTE_NORMAL,  //  Атрибуты по умолчанию
		NULL    //  Не копировать атрибуты диска
	);

	// Ошибка открытия диска
	if (FileHandle == INVALID_HANDLE_VALUE) {
		ShowMessage(L"Ошибка чтения диска! Попробуйте открыть программу с правами администратора.");
		return;
	}
	// Файловая ситема NTFS?
	FSType Type = DetermineFileSystemType(FileHandle);

	// Если ФС не NTFS, вывод сообщения
	if (static_cast<int>(Type) == 0) {
		ShowMessage(L"Файловая система не поддерживается");
		return;
	}

	// Применение фабричного метода
	FileSystem = CreateFileSystem(FileHandle);

    // Вывод информации про ФС
	FSSizeLabel->Caption = FileSystem->GetFileSystemSize()/1024;  // размер в Мб
	ClusterCountLabel->Caption = FileSystem->GetClusterCount();   // кол-во кластеров
	ClusterSizeLabel->Caption = FileSystem->GetClusterSize();     // размер кластера

	// Вывод в Edit имени ФС
	EditNameVolume->Text = volumeName;
}

void __fastcall TForm1::ButtonSelectBDClick(TObject *Sender)
{
	// Закрыть БД, если она уже была открыта
	if (Database != NULL) {
		sqlite3_close(Database);
	}

	// Просим пользователя выбрать файл с БД
	// обращение к "фантомной" кнопке OpenTextFileDialog1
	OpenTextFileDialog1->Execute();

	// Открытие БД
	int openResult = sqlite3_open16(OpenTextFileDialog1->FileName.w_str(), &Database);
	if (openResult != 0) { // Какая-либо ошибка
		ShowMessage(L"Не удалось открыть базу данных!");
		return;
	}

	// Сохраняем путь к БД и выводим ее в EditBD для отображения в окне
	EditBD->Text = OpenTextFileDialog1->FileName;

}
//---------------------------------------------------------------------------
// Обработка события нажатия на ToggleSwitch
void __fastcall TForm1::ToggleSwitchSearchTypeClick(TObject *Sender)
{
   if (ToggleSwitchSearchType->State == tssOn) { // Включено
		ToggleSwitchSearchType->ThumbColor = clGreen;    // Цвет - зеленый
		CheckListBoxType->Enabled = true;      // Чек-лист и надпись активны
		LabelTypeFile->Enabled = true;

	} else {                                    // Выключено
		ToggleSwitchSearchType->ThumbColor = clRed;   // Цвет - красный
		CheckListBoxType->Enabled = false;    // Чек-лист и надпись не активны
		LabelTypeFile->Enabled = false;
	}
}

//---------------------------------------------------------------------------

// Обработка события выбора типа файла в чек-листе
void __fastcall TForm1::CheckListBoxTypeClickCheck(TObject *Sender)
{
	// Удаляем содержимое вектора
	SelectedTypes.clear();

	// Обновление списка с выбранными типами
	for (int i = 0; i < CheckListBoxType->Count; i++) {
		// Если элемент "нажат", то добавим его в вектор CheckedTypes
		if (CheckListBoxType->Checked[i]) {
			// Вставляем в конец списка название типа (размер вектора +1)
			SelectedTypes.push_back( // Функция push_back() позволяет вставить новый элемент в конец вектора
					CheckListBoxType->Items->Strings[i]
			);
		}
	}
}

//---------------------------------------------------------------------------
// Обработка события получения текста для VirtualStringTree1
void __fastcall TForm1::VirtualStringTree1GetText(TBaseVirtualTree *Sender, PVirtualNode Node,
		  TColumnIndex Column, TVSTTextType TextType, UnicodeString &CellText)
{
	if (Node == NULL) return;
	ScansTableStruct* nodeData = (ScansTableStruct*) VirtualStringTree1->GetNodeData(Node);
	// Поля прописаны в typedef struct в main.h
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

// Нажатие кнопки Сканировать
void __fastcall TForm1::ButtonScanClick(TObject *Sender)
{
	if (EditBD->Text != L"Путь к БД" && EditNameVolume->Text != L"Метка тома") {
		// Запуск потока
		IteratorFS* Iterator = FileSystem->GetIterator();

		// Нужен ли поиск по типу файла?  true or false
		bool typeSearch = ToggleSwitchSearchType->State == tssOn;

        // Запуск потока чтения
		// false - отвечает за та, что он запускается сразу, как только создался
		// TForm1 (this) - ссылаемся на самих себя  , передаем в поток самих себя
		ReadThreadPtr = new ReadThread(false, Iterator, this, typeSearch);
	} else {
		ShowMessage(L"Не выбран путь к БД и/или метка тома");
		return;
	}
}

//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonScanStopClick(TObject *Sender)
{
     // Остановка потока
	 ReadThreadPtr->Terminate();
}
//---------------------------------------------------------------------------


