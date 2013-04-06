#include "../include/SizeDialog.h"

SizeDialog::SizeDialog()
{
	this->setWindowTitle( tr("Enter Map's Size") );
	wlabel = new QLabel( tr("Width:") );
	hlabel = new QLabel( tr("Height:") );

	wedit = new QLineEdit();
	hedit = new QLineEdit();

	wedit->clear();
	hedit->clear();

	okbut = new QPushButton( tr("Ok") );
	cnlbut = new QPushButton( tr("Cancel") );

	QVBoxLayout *mainLayout = new QVBoxLayout();

	QHBoxLayout *tmpLayout = new QHBoxLayout();
	QVBoxLayout *tmpLayout1 = new QVBoxLayout();
	tmpLayout1->addWidget( wlabel );
	tmpLayout1->addWidget( hlabel );
	tmpLayout->addLayout( tmpLayout1 );

	QVBoxLayout *tmpLayout2 = new QVBoxLayout();
	tmpLayout2->addWidget( wedit );
	tmpLayout2->addWidget( hedit );
	tmpLayout->addLayout( tmpLayout2 );

	mainLayout->addLayout( tmpLayout );

	QHBoxLayout *butLayout = new QHBoxLayout();
	butLayout->addWidget( cnlbut );
	butLayout->addWidget( okbut );

	mainLayout->addLayout( butLayout );
	
	this->setLayout( mainLayout );

	connect( okbut, SIGNAL( clicked() ), this, SLOT( onOkClick() ) );
	connect( cnlbut, SIGNAL( clicked() ), this, SLOT( onCancelClick() ) );

	this->setMinimumSize( 270, 138 );
	this->setMaximumSize( 270, 138 );
	wedit->setMaxLength( 10 );
	hedit->setMaxLength( 10 );
	wedit->setInputMask( "D999999999" );
	hedit->setInputMask( "D999999999" );
}

void SizeDialog::onOkClick()
{
	if( wedit->text().length() == 0 || hedit->text().length() == 0)
	{
		return;
	}

	int w, h;
	sscanf( wedit->text().toStdString().c_str(), "%d", &w );
	sscanf( hedit->text().toStdString().c_str(), "%d", &h );

	wedit->clear();
	hedit->clear();

	emit returnImageSize( w, h );
	this->hide();
}

void SizeDialog::onCancelClick()
{
	wedit->clear();
	hedit->clear();
	reject();
}
