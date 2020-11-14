#ifndef lowlevel_io_h
#define lowlevel_io_h

int countDirectories(char*);
char* createPath(char*, char*);
void deletePath(char*);
char** createDirTable(int, char*);
void deleteDirTable(char**, int);
#endif
