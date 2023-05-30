//---------------------------------------------------------------------------

#ifndef ClassFSH
#define ClassFSH
#include <vcl.h>
//---------------------------------------------------------------------------
class IteratorFS {
	protected:
		class ClassFS* FileSystem;
		DWORD currentPosition;   // текущая позиция
		char* clusterBuffer;     // Целочисленный (символьный) тип данных
		// буффер, в котором записаны данные
		bool done;               // флаг о завершении работы, true or false
	public:
		IteratorFS(ClassFS* FileSystem);

		virtual void First();   // Отсроченный метод, так как используются виртуальные функции
		virtual void Next();
		virtual bool IsDone();
		virtual char* GetCurrent();
		virtual DWORD GetCurrentPosition();
		virtual ClassFS* GetFileSystem();

		~IteratorFS();   //  Деструктор
};


class ClassFS {
	private:
		HANDLE FileHandle;
	protected:    // поля/переменные доступны из наследников (применяем, чтобы избежать перезаписи значений)
		UINT64 fileSystemSize; // UINT64 = unsigned __int64
		DWORD clusterCount;   // DWORD - 32-битное беззнаковое целое
		DWORD clusterSize;

		// Конструктор
		// protected, потому что ФС на данном этапе только абстрактная
		ClassFS(HANDLE FileHandle);

	public:   //   public для свободного обращения из других кодов
		// Размер файловой системы (в байтах)
		UINT64 GetFileSystemSize();
		// Количество кластеров в файловой системе
		DWORD GetClusterCount();
		// Размер кластера файловой системы (в байтах)
		DWORD GetClusterSize();

        // Метод создания итератора (чистый метод)
		virtual IteratorFS* GetIterator() = 0; // 0, потому что нет текущей реализации (0 - pure-specifier, чистый)
		// IteratorFS* (указатель именно на объект),
		// а не IteratorFS, чтобы не создавать копирующий конструктор ClassFS()

		// Абстрактный метод чтения кластера
		virtual bool ReadCluster(char* dataBuffer, DWORD clusterNumber); // нет 0, так как реализуется в AbstractFileSystem.cpp

		// Деструктор
		~ClassFS();
};

#endif
