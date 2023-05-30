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
	FreeOnTerminate = true; // очистка при завершении
	receivedBuffer = new char[bufferSize];  // выделение памяти
	copyBuffer = new char[bufferSize];
}
//---------------------------------------------------------------------------
void __fastcall WriteThread::Execute()
{
	while (!Terminated)
	{
		// Проверяем установлен ли флаг (с ожиданием в 0 мс)
		if (MyEvent->WaitFor(0) == wrSignaled)   // Ожидание 0 сек. wrsignaled, если произошла активизация события, wrTimeout, если за время тайм-аута ничего не произошло.
		{
			// Копируем буфер
			memcpy(copyBuffer, receivedBuffer, bufferSize);

			// Сбрасываем флаг события, чтобы ReadThread продолжил читать
			MyEvent->ResetEvent();

			// Работаем с copyBuffer
			if (useTypeFileSearch) {
				clusterTypeFile = FindTypeFile(copyBuffer, Form1->SelectedTypes); // ищем сигнатуры по отмеченным в форме
				if (static_cast<int>(clusterTypeFile) == 0) {
					continue; // если сигнатура не найдена, то работа продолжается
				}
			}

			// Вставка данных в БД
			addClusterToDB();
		}
	}  // Так как флаг события уже сброшен, то условие на установку флага не будет выполняться.
    // Из-за этого цикл повторяется "в холостую" до тех пор, пока ReadThread снова не поднимет флажок MyEvent->SetEvent();

	Synchronize(&addClustersToVST);

	delete[] receivedBuffer, copyBuffer;
}
//---------------------------------------------------------------------------

bool WriteThread::addClusterToDB() {
	// Шаг 1 (подготовка запроса)
	const char* errmsg;  // Переменная ошибки
	sqlite3_stmt* pStatement; // Запись ответа в БД (ответ в виде данных)

	// Формируем запросы в виде строки
	wchar_t sql[] = L"INSERT INTO scans (cluster_number, type) VALUES (?, ?);";

	int result = sqlite3_prepare16_v2(Form1->Database, sql, -1, &pStatement, NULL);
	if (result != SQLITE_OK) {
		errmsg = sqlite3_errmsg(Form1->Database);
		ShowMessage(L"Ошибка при выполнении INSERT для таблицы scans: " + UnicodeString(errmsg));
		// Шаг 3 (завершение обработки запроса)
		sqlite3_finalize(pStatement);
		return false;
	}

	// Подставляем значения вместо ?, ?
	result = sqlite3_bind_int64(pStatement, 1, clusterNumber);
	if (result != SQLITE_OK) {
		errmsg = sqlite3_errmsg(Form1->Database);
		ShowMessage(L"Ошибка при выполнении INSERT для таблицы scans: " + UnicodeString(errmsg));
		// Шаг 3 (завершение обработки запроса)
		sqlite3_finalize(pStatement);
		return false;
	}

	result = sqlite3_bind_text(pStatement, 2, GetTypeFileName(clusterTypeFile), -1, SQLITE_STATIC);
	if (result != SQLITE_OK) {
		errmsg = sqlite3_errmsg(Form1->Database);
		ShowMessage(L"Ошибка при выполнении INSERT для таблицы scans: " + UnicodeString(errmsg));
		// Шаг 3 (завершение обработки запроса)
		sqlite3_finalize(pStatement);
		return false;
	}

	// Шаг 2 (выполнение запроса)
	result = sqlite3_step(pStatement);

	// Проверяем результат
	if (result != SQLITE_DONE) {
		errmsg = sqlite3_errmsg(Form1->Database);
		ShowMessage(L"Ошибка при выполнении INSERT для таблицы scans: " + UnicodeString(errmsg));

		// Шаг 3 (завершение обработки запроса)
		sqlite3_finalize(pStatement);
		return false;
	}

	// Шаг 3 (завершение обработки запроса)
	sqlite3_finalize(pStatement);

	return true;
}

void  __fastcall WriteThread::addClustersToVST() {
	TVirtualStringTree* VST = Form1->VirtualStringTree1;

	// Шаг 1 (подготовка запроса)
	const char *errmsg;
	sqlite3_stmt *pStatement;

	// Формируем запрос с фильтрацией кластера по сигнатуре
	wchar_t sql[] = L"SELECT id, cluster_number, type FROM scans;";

	int result = sqlite3_prepare16_v2(Form1->Database, sql, -1, &pStatement, NULL);

	if (result == SQLITE_OK) {
		VST->Clear();
		VST->BeginUpdate();

		// Шаг 2 (выполнение запроса и получение результата для множества строк)
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
		ShowMessage(L"Ошибка при выполнении SELECT для таблицы scans: " + UnicodeString(errmsg));
	}

	// Шаг 3 (завершение обработки запроса)
	sqlite3_finalize(pStatement);
}
