#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <tchar.h>
#include <string.h>
#include <windows.h>
#include <vector>
using namespace std;

/*
	
	Практическая работа №5.1

	Удалить обычные файлы (regular ﬁle), имена которых имеют в своем составе ровно 2 согласных ла-
	тинских буквы.

*/

bool isFileForDelete(TCHAR *filename)
{
	string sogl = "bcdfghjklmnpqrstvwxz";
	int j = 0;

	for (int i = 0; (i < _tcslen(filename)) && (j <= 2); i++)
	{
		if (sogl.find(tolower((char)filename[i])) != -1) j++;
	}

	if (j == 2) return true;
	else return false;
}

void Error()
{
	LPVOID lpMsgBuff;
	DWORD dw = GetLastError();
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dw, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuff, 0, NULL);

	_tprintf((LPCTSTR)lpMsgBuff);
	_tprintf(_T("\n"));

	LocalFree(lpMsgBuff);
}

DWORD WINAPI readDir(const LPVOID lpParameter)
{
	vector<HANDLE> pids; //Вектор id дочерних потоков

	TCHAR path[MAX_PATH];
	_tcscpy(path, (TCHAR*)lpParameter);
	TCHAR findDir[MAX_PATH];
	_tcscpy(findDir, path);
	_tcscat(findDir, _T("\\"));
	_tcscat(findDir, _T("*"));

	HANDLE fh;
	WIN32_FIND_DATA file;

	fh = FindFirstFile(findDir, &file);
	if (fh != INVALID_HANDLE_VALUE)
	{
		while (FindNextFile(fh, &file))
		{
			if ((_tccmp(file.cFileName, _T(".")) != 0) && (_tccmp(file.cFileName, _T("..")) != 0))
			{
				if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					TCHAR newPath[MAX_PATH];
					_tcscpy(newPath, path);
					_tcscat(newPath, _T("\\"));
					_tcscat(newPath, file.cFileName);

					HANDLE hThread = CreateThread(NULL, 0, &readDir, newPath, 0, NULL);
					pids.push_back(hThread);

				}
				else
				{
					TCHAR filePath[MAX_PATH];
					_tcscpy(filePath, path);
					_tcscat(filePath, _T("\\"));
					_tcscat(filePath, file.cFileName);

					if (isFileForDelete(file.cFileName))
						if (!DeleteFile(filePath)) Error();

				}
			}
		}
		if (GetLastError() != ERROR_NO_MORE_FILES) Error();
	}
	else if (GetLastError() != ERROR_FILE_NOT_FOUND) Error();

	if (!FindClose(fh)) Error();

	if (!pids.empty())
	{
		for (int i = 0; i < pids.size(); i++)
		{
			if (WaitForSingleObject(pids[i], INFINITE) == WAIT_FAILED) Error();
			else cout << "Success" << '\n'
		}
	}

	return 0;
}

int main()
{
	HANDLE hMain = CreateThread(NULL, 0, &readDir, _T("C:\\Test"), 0, NULL);
	if (WaitForSingleObject(hMain, INFINITE) == WAIT_FAILED) Error();
	else cout << "Success" << '\n';
	system("pause");
}