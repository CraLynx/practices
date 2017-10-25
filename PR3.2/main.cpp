#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
using namespace std;

bool isFileForDelete(char *filename)
{
	string sogl = "bcdfghjklmnpqrstvwxz";
	int j = 0;

	for (int i = 0; (i < strlen(filename)) && (j < 2); i++)
	{
		if (sogl.find(tolower((char)filename[i])) != -1) j++;
	}

	if (j >= 2) return true;
	else return false;

}

void search(char *path)
{
	DIR *dp;
	dirent *dir;

	dp = opendir(path);
	if (dp != NULL)
	{
		errno = 0;
		while((dir = readdir(dp)) != 0)
		{
			if(((strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0)) && (dir->d_type == DT_DIR))
			{
				char *newPath = new char[4096];				
				if (newPath != NULL)
				{
					strcpy(newPath, path);
					strcat(newPath, "/");
					strcat(newPath, dir->d_name);
					
					search(newPath);
				}
				else cout << "No memory" << '\n';

			}
			else if (dir->d_type == DT_REG)
			{			
				if (isFileForDelete(dir->d_name))
				{
					char *newPath = new char[4096];				
					if (newPath != NULL)
					{
						strcpy(newPath, path);
						strcat(newPath, "/");
						strcat(newPath, dir->d_name);

						remove(newPath);
					}
					else cout << "No memory" << '\n';
				}
			}
			
		}
		if (errno != 0) cout << strerror(errno) << '\n';
	}

	if (closedir(dp) == -1) cout << strerror(errno) << '\n';

}

int main()
{
	char path[] = "test";
	search(path);

	return 0;
}

