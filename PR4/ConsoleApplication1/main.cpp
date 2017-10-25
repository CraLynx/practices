#include <iostream>
#include <fstream>
using namespace std;

/*
	Практическая работа №4

	Напишите программу mytail, имеющую два аргумента: имя файла и число n – и выводящую содер-
	жимое последних n строк файла. Пример вызова: ./mytail test.txt 10

*/

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "RUS");
	if (argc == 3)
	{
		char *filename = new char[4096];
		char buffer[255];
		int count_lines = atoi(argv[2]);

		if (count_lines < 0) count_lines *= -1;

		if (filename != NULL)
		{
			strcpy(filename, argv[1]);
			ifstream read_file(filename);
			
			if (read_file.is_open())
			{
				while (count_lines > 0)
				{
					read_file >> buffer;
					if(strcmp(buffer, "") != 0) cout << buffer << '\n';
					else break;

					count_lines--;
				}
			}

			read_file.close();

			delete filename;
		}
		else cout << "No memory" << '\n';
	}
	else cout << "No arguments" << '\n';
	system("pause");
	
}