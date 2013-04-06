#ifndef FILEEXX
#define FILEEXX

class FileExx
{
private:

	char file[256];

public:

	FileExx( const char *const );
	virtual void debugPrint();
};

#endif
