#include <iostream>
#include <windows.h>
#include <tchar.h>
using namespace std;

/*
	
	Практическая работа №1.1

	Удалить обычные файлы (regular ﬁle), имена которых имеют в своем составе ровно 2 согласных ла-
	тинских буквы.

*/


void Error()
{
	LPVOID lpMsgBuff;
	DWORD dw = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dw, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuff, 0, NULL);
	_tprintf((LPCTSTR)lpMsgBuff);
	LocalFree(lpMsgBuff);
}

bool isFileForDelete(TCHAR *filename)
{
	string sogl = "bcdfghjklmnpqrstvwxz";
	int j = 0;

	for (int i = 0; (i < _tcslen(filename)) && (j < 2); i++)
	{
		if (sogl.find(tolower((char)filename[i])) != -1) j++;
	}

	if (j >= 2) return true;
	else return false;
}

int main()
{
	TCHAR *root = new TCHAR[MAX_PATH];
	TCHAR openPath[MAX_PATH];
	_tcscpy(root, _T("C:\\Test\\"));
	_tcscpy(openPath, root);
	_tcscat(openPath, _T("*"));

	HANDLE hFind;
	WIN32_FIND_DATA file;

	if (root != NULL)
	{
		hFind = FindFirstFile(openPath, &file);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			while (FindNextFile(hFind, &file))
			{
				if (file.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
				{
					if (isFileForDelete(file.cFileName))
					{
						TCHAR *dPath = new TCHAR[MAX_PATH];
						if (dPath != NULL)
						{
							_tcscpy(dPath, root);
							_tcscat(dPath, file.cFileName);
							if (!DeleteFile(dPath)) Error();

							delete dPath;
						}
						else _tprintf(_T("Sorry, Aleksandr Igorevich, no memory"));
					}
				}
			}
			if (GetLastError() != ERROR_NO_MORE_FILES) Error();
		}
		else if (GetLastError() != ERROR_FILE_NOT_FOUND) Error();

		if (!FindClose(hFind)) Error();
		delete root;
	}

	system("pause");
}