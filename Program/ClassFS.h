//---------------------------------------------------------------------------

#ifndef ClassFSH
#define ClassFSH
#include <vcl.h>
//---------------------------------------------------------------------------
class IteratorFS {
	protected:
		class ClassFS* FileSystem;
		DWORD currentPosition;   // ������� �������
		char* clusterBuffer;     // ������������� (����������) ��� ������
		// ������, � ������� �������� ������
		bool done;               // ���� � ���������� ������, true or false
	public:
		IteratorFS(ClassFS* FileSystem);

		virtual void First();   // ����������� �����, ��� ��� ������������ ����������� �������
		virtual void Next();
		virtual bool IsDone();
		virtual char* GetCurrent();
		virtual DWORD GetCurrentPosition();
		virtual ClassFS* GetFileSystem();

		~IteratorFS();   //  ����������
};


class ClassFS {
	private:
		HANDLE FileHandle;
	protected:    // ����/���������� �������� �� ����������� (���������, ����� �������� ���������� ��������)
		UINT64 fileSystemSize; // UINT64 = unsigned __int64
		DWORD clusterCount;   // DWORD - 32-������ ����������� �����
		DWORD clusterSize;

		// �����������
		// protected, ������ ��� �� �� ������ ����� ������ �����������
		ClassFS(HANDLE FileHandle);

	public:   //   public ��� ���������� ��������� �� ������ �����
		// ������ �������� ������� (� ������)
		UINT64 GetFileSystemSize();
		// ���������� ��������� � �������� �������
		DWORD GetClusterCount();
		// ������ �������� �������� ������� (� ������)
		DWORD GetClusterSize();

        // ����� �������� ��������� (������ �����)
		virtual IteratorFS* GetIterator() = 0; // 0, ������ ��� ��� ������� ���������� (0 - pure-specifier, ������)
		// IteratorFS* (��������� ������ �� ������),
		// � �� IteratorFS, ����� �� ��������� ���������� ����������� ClassFS()

		// ����������� ����� ������ ��������
		virtual bool ReadCluster(char* dataBuffer, DWORD clusterNumber); // ��� 0, ��� ��� ����������� � AbstractFileSystem.cpp

		// ����������
		~ClassFS();
};

#endif
