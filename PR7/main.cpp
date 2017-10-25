#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <vector>
using namespace std;

const int MAX_PATH = 4096;
unsigned int min_file_name = 0;

void Read(char *dirname)
{
	DIR *dp;
	dirent *dir;

	dp = opendir(dirname);
	if(dp != NULL)
	{
		errno = 0;
		while((dir = readdir(dp)) != 0)
		{
			if((strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0))
			{
				if(dir->d_type == DT_DIR)
				{
					char *newPath = new char[MAX_PATH];
					if(newPath != NULL)
					{
						strcpy(newPath, dirname);
						strcat(newPath, "/");
						strcat(newPath, dir->d_name);

						Read(newPath);
						delete newPath;
					}
				}
				else
				{
					if(dir->d_type == DT_REG)
					{
						char *path = new char[MAX_PATH];
						if(path != NULL)
						{
							strcpy(path, dirname);
							strcat(path, "/");
							strcat(path, dir->d_name);
							
							
								if(min_file_name  == 0) min_file_name = strlen(dir->d_name);
								else if(min_file_name > strlen(dir->d_name)) min_file_name = strlen(dir->d_name);
							
							
							delete path;
						}
					}
				}
			}
		}
		if(errno != 0) cout << strerror(errno) << '\n';
	} else cout << strerror(errno) << '\n';	

	if(closedir(dp) == -1) cout << strerror(errno) << '\n';

}

int main()
{	
	char dn[] = "test";
	Read(dn);
	
	cout << "Maximum file name: " << min_file_name << '\n';
}
