#include <cassert>

#include "../include/MainWindow.h"

MainWindow::MainWindow()
{
	loadMapAction = new QAction( tr("Load Map"), this );
	saveMapAction = new QAction( tr("Save Map"), this );
	startSimAction = new QAction( tr("Start Simulation"), this );
	stopSimAction = new QAction( tr("Stop Simulation"), this );
	exitAction = new QAction( tr("Exit"), this );
	aboutAction = new QAction( tr("About"), this );

	connect( loadMapAction, SIGNAL(triggered()), this, SLOT(loadMap()) );
	connect( saveMapAction, SIGNAL(triggered()), this, SLOT(saveMap()) );
	connect( startSimAction, SIGNAL(triggered()), this, SLOT(startSimulation()) );
	connect( stopSimAction, SIGNAL(triggered()), this, SLOT(stopSimulation()) );
	connect( exitAction, SIGNAL(triggered()), this, SLOT(exit()) );
	connect( aboutAction, SIGNAL(triggered()), this, SLOT(about()) );

	topMenu = menuBar()->addMenu( tr("File") );
	topMenu->addAction( loadMapAction );
	//topMenu->addAction( saveMapAction );
	topMenu->addSeparator();
	topMenu->addAction( startSimAction );
	topMenu->addAction( stopSimAction );
	topMenu->addSeparator();
	topMenu->addAction( exitAction );

	customiseMapAction = new QAction( tr("Customise Map"), this );

	connect( customiseMapAction, SIGNAL(triggered()), this, SLOT(customiseMap()) );

	//topMenu = menuBar()->addMenu( tr("Options") );
	//topMenu->addAction( customiseMapAction );

	topMenu = menuBar()->addMenu( tr("Help") );
	topMenu->addAction( aboutAction );

	statusBar();
	//statusBar()->showMessage( tr("Load Map") );

	showMaximized();
	setWindowTitle( tr("Water Movement Simulation") );

	drawMain = new DrawWidget();
	this->setCentralWidget( drawMain );

	simthread = new QThread();
	simwrk = new SimulationWorker( drawMain );

	simwrk->moveToThread( simthread );

	connect( simthread, SIGNAL(started()), simwrk, SLOT(startModelling()) );
	connect( simwrk, SIGNAL(finished()), simthread, SLOT(quit()) );
}

MainWindow::~MainWindow()
{
	//CustomiseFormSingleton::dispose();
	//if( simwrk ) delete simwrk;
	//if( drawMain ) delete drawMain;
	printf("main window has been deleted\n");
}

void MainWindow::about()
{
	QMessageBox::about
	( 
		this, tr("About"), 
		tr("Water Simulation Tool.\n Test Version.\n\n\n(c)Tuvinian Institute for Exploration of Natural Resources(SB RAS).") 
	);
}

void MainWindow::loadMap()
{
	QString fileName = QFileDialog::getOpenFileName( this, "", "", "" );

	if( fileName.isEmpty() ) return;
	
	try
	{
		data.readDataFromFile( fileName.toStdString().c_str() );
		drawMain->drawData( data );
		statusBar()->showMessage( fileName );
	}
	catch( FileExx &arg )
	{
		arg.debugPrint();
	}
}

void MainWindow::saveMap()
{
}

void MainWindow::startSimulation()
{
	simwrk->setSimulationEnabled( 1 );
	simwrk->setModelData( data );
	simthread->start();
}

void MainWindow::stopSimulation()
{
	simwrk->setSimulationEnabled( 0 );
}

void MainWindow::exit()
{
	qApp->quit();
}

void MainWindow::customiseMap()
{
	if( !CustomiseFormSingleton::instance()->checkPntr() )
	{
		CustomiseFormSingleton::instance()->saveMWPntr( this );
	}
	
	CustomiseFormSingleton::instance()->saveImage( drawMain->getImage() );
	CustomiseFormSingleton::instance()->show();
	this->hide();
}
