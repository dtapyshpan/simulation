#include "../include/SizeDialogSingleton.h"

SizeDialog *SizeDialogSingleton::sdsingleton = NULL;

SizeDialog* SizeDialogSingleton::instance()
{
	if( !sdsingleton )
	{
		sdsingleton = new SizeDialog();
	}
	return sdsingleton;
}

void SizeDialogSingleton::dispose()
{
	if( sdsingleton ) delete sdsingleton;
}
