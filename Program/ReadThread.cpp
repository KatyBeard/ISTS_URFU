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
{   // при создании потока включаем параметр,
	// чтобы при удалении потока было самосто€тельное очищение
	FreeOnTerminate = true;
	// this используем дл€ ссылки на самих себ€, дл€ однозначного определени€
	this->MyEvent = new TEvent(NULL, true, false, "", false);
	// true - отвечает за то, что процесс будет запущен по команде WriteThreadPtr->Start()
	// Iterator->GetFileSystem()->GetClusterSize() - передаем размер кластера
	// Form1 - дл€ св€зи с формой
	// useTypeFileSearch  - отвечаем за вопрос, нужно ли искать расширение файла
	this->WriteThreadPtr = new WriteThread(true, MyEvent, Iterator->GetFileSystem()->GetClusterSize(), Form1, useTypeFileSearch);
}
//---------------------------------------------------------------------------
void __fastcall ReadThread::Execute()
{
	Synchronize(&OnStart);
	WriteThreadPtr->Start(); // запускаем поток, который будет обрабатывать данные

	// сначала первое состо€ние - переход в режим готовности,
	// при этом уже считан первый кластер в WriteThread
	// далее провер€етс€ условие "не закончена ли работа?",
	// третье состо€ние - если работа закончена, то дл€ готовность дл€ следующей итерации
	for (Iterator->First(); !Iterator->IsDone(); Iterator->Next()) {
		if (Terminated) break;

		// ѕередаЄм второму потоку считанный кластер
		WriteThreadPtr->receivedBuffer = Iterator->GetCurrent();
		// clusterNumber - текущий кластер
		WriteThreadPtr->clusterNumber = Iterator->GetCurrentPosition();

		// ѕоднимаем флажок (второй поток начинает обрабатывать полученный кластер)
		MyEvent->SetEvent();

		// Ѕеск цикл - ожидаем сн€ти€ флажка (MyEvent->ResetEvent) от второго потока
		while (MyEvent->WaitFor(0) == wrSignaled)
		{
			// ≈сли работу потока завершили принудительно из формы
			if(Terminated) break;
		}
	}

	WriteThreadPtr->Terminate();   // завершаем поток процесса

	Synchronize(&OnStop);
	WriteThreadPtr->WaitFor();  // ждем, пока ProcessThread не завершитс€

	delete WriteThreadPtr;   // очистка указател€ на поток
}
//---------------------------------------------------------------------------

void __fastcall ReadThread::OnStart() {
	Form1->LabelScanStart->Enabled = true; //  нопка сканировать
	Form1->LabelScanEnd->Enabled = false;  //  нопка прервать сканирование
}

void __fastcall ReadThread::OnStop() {
	Form1->LabelScanStart->Enabled = false;    //  нопка сканировать
	Form1->LabelScanEnd->Enabled = true;       //  нопка прервать сканирование
}

