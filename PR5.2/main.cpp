#include <iostream>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>
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
	vector<pid_t> pids;
	
	char *selectedDir = new char[MAX_PATH];
	strcpy(selectedDir, "test");
	DIR *dp = opendir(selectedDir);
	dirent *dir;

	if(dp != NULL)
	{
		errno = 0;
		while((dir = readdir(dp)) != 0)
		{
			if((strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0) && (dir->d_type == DT_DIR))
			{
				pid_t pid = fork();
				
				if(pid == 0)
				{
					strcpy(selectedDir, selectedDir);	
					strcat(selectedDir, "/");
					strcat(selectedDir, dir->d_name);

					if(closedir(dp) == -1) cout << strerror(errno) << '\n';
					pids.clear();
					
					dp = opendir(selectedDir);
				}
				
				if(pid > 0) pids.push_back(pid);
				
				if(pid < 0) cout << strerror(errno) << '\n';
			}
			else
			{
				if(dir->d_type == DT_REG)
				{
					if(isFileForDelete(dir->d_name) == true)
					{
						char *path = new char[MAX_PATH];
						strcpy(path, selectedDir);
						strcat(path, "/");
						strcat(path, dir->d_name);
						cout << "Path for deleting file: " << path << '\n';
						if(unlink(path) == -1) cout << strerror(errno) << '\n';
						delete path;
					}
				}
			}
		}
		if(errno != 0) cout << strerror(errno) << '\n';
	} else cout << strerror(errno) << '\n';

	if(closedir(dp) == -1) cout << strerror(errno) << '\n';
	
	if(pids.size() != 0)
	{
		int status;
		for(int i = 0; i < pids.size(); i++)
		{
			if(waitpid(pids[i], &status, 0)) cout << "Proccess: " << strerror(errno) << '\n';
			else cout << "Proccess: " << strerror(errno) << '\n';
			if(WIFEXITED(status)) cout << "Proccess ended good " << '\n';
		}	
	}
	
	delete selectedDir;
}

