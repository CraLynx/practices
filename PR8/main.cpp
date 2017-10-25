#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <dirent.h>
using namespace std;

const int MAX_PATH = 4096;

struct SFile
{
	char filename[MAX_PATH];
	int size;
};

void Error()
{
	cout << strerror(errno) << '\n';
}

int main()
{
	vector<pid_t> pids;
	DIR *dp;
	dirent *dir;

	SFile file;
	char currentFile[MAX_PATH];
	int min_file_size = 0;

	bool isParentProcess = true;
	int pipefd[2];
	if(pipe(pipefd) == -1) Error();
	else
	{
		char *source_path = new char[MAX_PATH];
		if(source_path != NULL)
		{
			strcpy(source_path, "test");
			dp = opendir(source_path);
			if(dp != NULL)
			{
				errno = 0;
				while((dir = readdir(dp)) != 0)
				{
					if(((strcmp(dir->d_name, ".") != 0) && (strcmp(dir->d_name, "..") != 0)) && (dir->d_type == DT_DIR))
					{
						pid_t pid = fork();
						switch(pid)
						{
						case 0:
						{
							min_file_size = 0;
							isParentProcess = false;
							pids.clear();
							strcat(source_path, "/");
							strcat(source_path, dir->d_name);	

							if(closedir(dp) == 1) Error();
							dp = opendir(source_path);
							if(dp == NULL) Error();
						} break;
						case -1: Error(); break;
						default: pids.push_back(pid); break;
						}
					}
					else if(dir->d_type == DT_REG)
					{
						char *path = new char[MAX_PATH];
						if(path != NULL)
						{
							strcpy(path, source_path);
							strcat(path, "/");
							strcat(path, dir->d_name);
							int res = 0;


				
								if(min_file_size == 0)
								{
									min_file_size = strlen(dir->d_name);
									strcpy(currentFile, dir->d_name);
								}
								else if(strlen(dir->d_name) <= min_file_size)
								{
									min_file_size = strlen(dir->d_name);
									strcpy(currentFile, dir->d_name);
								}

							delete path;
						}
					}
				}
				if(errno != 0) Error();

				if(isParentProcess == false)
				{
					strcpy(file.filename, currentFile);
					file.size = min_file_size;

					if(close(pipefd[0]) == -1) Error();

					int res = 0;
					res = write(pipefd[1], &file, sizeof(file));
					if(res == -1) Error();
					else if(res != sizeof(file)) cout << "Error write" << '\n';
				}
				else
				{
					strcpy(file.filename, currentFile);
					file.size = min_file_size;
				}
			}
			else Error();

			if(closedir(dp) == -1) Error();

			delete source_path;
		}
		else cout << "No memory" << '\n';
	}
	
	if(pids.size() > 0)
	{
		int status = 0;	
		for(int i = 0; i < pids.size(); i++)
		{
			if(waitpid(pids[i], &status, 0)) if(!WIFEXITED(status)) Error();		
			else Error();
		}
	}

	if(isParentProcess)
	{
		SFile f;
		if(strcmp(file.filename, "") != 0) cout << "Filename: " << file.filename << " name len: " << file.size << '\n';
		if(close(pipefd[1]) == -1) Error();

		bool eof = true;
		int res = 0;
		while(eof)
		{
			res = read(pipefd[0], &f, sizeof(SFile));
			if(res == 0) eof = false;
			else if(res == -1)
			{
				Error();
				eof = false;
			}
			else if(res == sizeof(SFile)) cout << "Filename: " << f.filename << " name len: " << f.size << '\n';
			else cout << "Error read" << '\n';
		}		
		if(close(pipefd[0]) == -1) Error();
	}
}
