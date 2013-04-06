#include <cassert>

#include "../include/CustomiseForm.h"

CustomiseForm::CustomiseForm()
{
	mwpntr = NULL;
	widthImage = heightImage = -1;

	setMapSizeAction = new QAction( tr("Set Size"), this );
	exitAction = new QAction( tr("Exit"), this );
	randomMapAction = new QAction( tr("Random Map"), this );
	clearMapAction = new QAction( tr("Clear Map"), this );

	connect( setMapSizeAction, SIGNAL(triggered()), this, SLOT(setMapSize()) );
	connect( exitAction, SIGNAL(triggered()), this, SLOT(exit()) );
	connect( randomMapAction, SIGNAL(triggered()), this, SLOT(randomMap()) );
	connect( clearMapAction, SIGNAL(triggered()), this, SLOT(clearMap()) );

	custMenu = menuBar()->addMenu( tr("File") );
	custMenu->addAction( setMapSizeAction );
	custMenu->addAction( exitAction );
	custMenu = menuBar()->addMenu( tr("Options") );
	custMenu->addAction( randomMapAction );
	custMenu->addAction( clearMapAction );

	statusBar();
	
	showMaximized();
	setWindowTitle( tr("Map Customisation") );

	drawCust = new DrawWidget();
	this->setCentralWidget( drawCust );
	
	createSettings();

	connect( SizeDialogSingleton::instance(), SIGNAL(returnImageSize(int, int)), this, SLOT(saveImageSize(int, int)) );
}

CustomiseForm::~CustomiseForm()
{
}

int CustomiseForm::checkPntr()
{
	if( mwpntr ) return 1;
	return 0;
}

void CustomiseForm::saveMWPntr( QMainWindow *const pntr1 )
{
	mwpntr = pntr1;
}

void CustomiseForm::setMapSize()
{
	SizeDialogSingleton::instance()->show();
}

void CustomiseForm::randomMap()
{
}

void CustomiseForm::clearMap()
{
}

void CustomiseForm::exit()
{
	closeEvent();
}

void CustomiseForm::saveImageSize( int w, int h )
{
	widthImage = w;
	heightImage = h;
	statusBar()->showMessage( tr("got image size") );
}

void CustomiseForm::saveImage( const QImage &arg )
{
	drawCust->setImage( arg );
}

/*
this event is called when user has closed CustomiseForm Window
*/
void CustomiseForm::closeEvent()
{
	this->hide();
	mwpntr->show();
}

void CustomiseForm::closeEvent( QCloseEvent * )
{
	closeEvent();
}

void CustomiseForm::createSettings()
{
	settings = new QDockWidget( tr(""), this );
	settings->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
	settings->setFeatures( QDockWidget::NoDockWidgetFeatures );

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addStretch( 0 );
	layout->setDirection( QBoxLayout::BottomToTop );

	/*
	QLabel *labelM = new QLabel( "Image Size:" );
	QLabel *labelW = new QLabel( "Width:" );
	QLabel *labelH = new QLabel( "Height:" );
	*/

	gh = new QRadioButton( tr("Ground Height"), this );
	wd = new QRadioButton( tr("Water Depth"), this );
	sbg = new QSpinBox();
	sbw = new QSpinBox();

	sbg->setMaximum( 8848 );
	sbg->setMinimum( 0 );
	sbw->setMaximum( 230 );
	sbw->setMinimum( 0 );

	layout->addWidget( sbw );
	layout->addWidget( wd );
	layout->addWidget( sbg );
	layout->addWidget( gh );

	/*
	layout->addWidget( labelH );
	layout->addWidget( labelW );
	layout->addWidget( labelM );
	*/

	QWidget *tmpWidget = new QWidget();
	tmpWidget->setLayout( layout );
	settings->setWidget( tmpWidget );

	this->addDockWidget( Qt::RightDockWidgetArea, settings );
}

void CustomiseForm::mousePressEvent( QMouseEvent *event )
{
	if( event->buttons() & Qt::LeftButton )
	{
		if( gh->isDown() )
		{
			//change the ground height
			assert(0);
		}
		else if( wd->isDown() )
		{
			//change the water depth
			assert(0);
		}
	}
}
