#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
using namespace std;

const int MAX_PATH = 4096;

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

int main()
{
    DIR *dp;
    dirent *element;

    char path[] = "myFolder";

    dp = opendir(path);
    if(dp != NULL)
    {
        errno = 0;
        while((element = readdir(dp)) != 0)
        {
            if((strcmp(element->d_name, ".") != 0) && (strcmp(element->d_name, "..") != 0))
            {
                if(element->d_type == DT_REG)
                {
									if(isFileForDelete(element->d_name))
									{
										char *dPath = new char[MAX_PATH];
										if(dPath != NULL)
										{
											strcpy(dPath, path);
											strcpy(dPath, element->d_name);
											if(unlink(dPath) != 0) cout << strerror(errno) << '\n';

											delete dPath;
										}
										else cout << "No memory" << '\n';
									}
                }
            }
        }
        if(errno != 0) cout << strerror(errno) << '\n';
    }
    else cout << strerror(errno) << '\n';

    if(closedir(dp) == -1) cout << strerror(errno) << '\n';
}
