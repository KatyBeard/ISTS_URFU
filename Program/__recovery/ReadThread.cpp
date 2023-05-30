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
	// чтобы при удалении потока было самостоятельное очищение
	FreeOnTerminate = true;
	// this используем для ссылки на самих себя, для однозначного определения
	this->MyEvent = new TEvent(NULL, true, false, "", false);
	// true - отвечает за то, что процесс будет запущен по команде WriteThreadPtr->Start()
	// Iterator->GetFileSystem()->GetClusterSize() - передаем размер кластера
	// Form1 - для связи с формой
	// useTypeFileSearch  - отвечаем за вопрос, нужно ли искать расширение файла
	this->WriteThreadPtr = new WriteThread(true, MyEvent, Iterator->GetFileSystem()->GetClusterSize(), Form1, useTypeFileSearch);
}
//---------------------------------------------------------------------------
void __fastcall ReadThread::Execute()
{
	WriteThreadPtr->Start(); // запускаем поток, который будет обрабатывать данные

	// сначала первое состояние - переход в режим готовности,
	// при этом уже считан первый кластер в WriteThread
	// далее проверяется условие "не закончена ли работа?",
	// третье состояние - если работа закончена, то для готовность для следующей итерации
	for (Iterator->First(); !Iterator->IsDone(); Iterator->Next()) {
		if (Terminated) break;

		// Передаём второму потоку считанный кластер
		WriteThreadPtr->receivedBuffer = Iterator->GetCurrent();
		// clusterNumber - текущий кластер
		WriteThreadPtr->clusterNumber = Iterator->GetCurrentPosition();

		// Поднимаем флажок (второй поток начинает обрабатывать полученный кластер)
		MyEvent->SetEvent();

		// Беск цикл - ожидаем снятия флажка (MyEvent->ResetEvent) от второго потока
		while (MyEvent->WaitFor(0) == wrSignaled)
		{
			// Если работу потока завершили принудительно из формы
			if(Terminated) break;
		}
	}

	WriteThreadPtr->Terminate();   // завершаем поток процесса

	WriteThreadPtr->WaitFor();  // ждем, пока ProcessThread не завершится

	delete WriteThreadPtr;   // очистка указателя на поток
}
//---------------------------------------------------------------------------


