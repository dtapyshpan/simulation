#include "../include/CustomiseFormSingleton.h"

CustomiseForm *CustomiseFormSingleton::cfsingleton = NULL;

CustomiseForm* CustomiseFormSingleton::instance()
{
	if( !cfsingleton ) cfsingleton = new CustomiseForm();
	return cfsingleton;
}

void CustomiseFormSingleton::dispose()
{
	if( cfsingleton ) delete cfsingleton;
}
