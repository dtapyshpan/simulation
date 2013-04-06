#ifndef CUSTOMISEFORMSINGLETON_H
#define CUSTOMISEFORMSINGLETON_H

#include "CustomiseForm.h"

class CustomiseFormSingleton
{
public:
	static CustomiseForm* instance();
	static void dispose(); 

private:
	static CustomiseForm *cfsingleton;
};

#endif
