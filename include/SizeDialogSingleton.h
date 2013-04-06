#ifndef SIZEDIALOGSINGLETON_H
#define SIZEDIALOGSINGLETON_H

#include "SizeDialog.h"

class SizeDialogSingleton
{
public:

	static SizeDialog* instance();
	static void dispose();

private:

	static SizeDialog *sdsingleton;
};

#endif
