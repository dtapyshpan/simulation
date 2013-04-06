#include <cstdio>
#include <cstring>

#include "../include/FileExx.h"

FileExx::FileExx( const char *const arg )
{
	strcpy( file, arg );
}

void FileExx::debugPrint()
{
	printf("File %s cannot be open\n", file);
}
