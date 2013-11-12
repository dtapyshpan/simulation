#include <cassert>
#include "../include/MainWindow.h"

MainWindow::MainWindow()
{
  loadMapAction = new QAction( tr("Load Map"), this );
  startSimAction = new QAction( tr("Start Simulation"), this );
  stopSimAction = new QAction( tr("Stop Simulation"), this );
  exitAction = new QAction( tr("Exit"), this );
  aboutAction = new QAction( tr("About"), this );

  connect( loadMapAction, SIGNAL(triggered()), this, SLOT(loadMap()) );
  connect( startSimAction, SIGNAL(triggered()), this, SLOT(startSimulation()) );
  connect( stopSimAction, SIGNAL(triggered()), this, SLOT(stopSimulation()) );
  connect( exitAction, SIGNAL(triggered()), this, SLOT(exit()) );
  connect( aboutAction, SIGNAL(triggered()), this, SLOT(about()) );

  topMenu = menuBar()->addMenu( tr("File") );
  topMenu->addAction( loadMapAction );
  topMenu->addSeparator();
  topMenu->addAction( startSimAction );
  topMenu->addAction( stopSimAction );
  topMenu->addSeparator();
  topMenu->addAction( exitAction );

  customiseMapAction = new QAction( tr("Customise Map"), this );
  connect( customiseMapAction, SIGNAL(triggered()), this, SLOT(customiseMap()) );
  
  topMenu = menuBar()->addMenu( tr("Options") );
  topMenu->addAction( customiseMapAction );
	
  topMenu = menuBar()->addMenu( tr("Help") );
  topMenu->addAction( aboutAction );
  
  showMaximized();
	
  drawMain = new DrawWidgetMW();
  setCentralWidget( drawMain );

  simthread = new QThread();
  simwrk = new SimulationWorker( drawMain );

  simwrk->moveToThread( simthread );

  statusBar();
  statusBar()->addPermanentWidget( &scaleLabel );

  connect( simthread, SIGNAL(started()), simwrk, SLOT(startModeling()) );
  connect( simwrk, SIGNAL(finished()), simthread, SLOT(quit()) );
  connect( drawMain, SIGNAL( sendScale( int ) ), this, SLOT( showScale( const int ) ) );

  setWindowTitle( tr("Water Movement Simulation Tool") );
}

MainWindow::~MainWindow()
{
  printf("main window has been deleted\n");
}

void MainWindow::about()
{
  QMessageBox::about
  ( 
    this, tr("About"), 
    tr("Water Simulation Tool.\n\n\n(c)Tuvinian Institute for Exploration of Natural Resources(SB RAS).")
  );
}

void MainWindow::loadMap()
{
  QString fileName = QFileDialog::getOpenFileName( this, "", "/home/denis/WaterModeling/data/", "" );
  
  if( fileName.isEmpty() ) return;

  try
  {
    data.readDataFromFile( fileName.toStdString().c_str() );
    drawMain->drawData( data );
  }
  catch( FileExx &arg )
  {
    arg.debugPrint();
  }
}

void MainWindow::startSimulation()
{
  if( data.getHeight() == -1 || data.getWidth() == -1 ) return;
  simwrk->setSimulationEnabled( true );
  simwrk->setModelData( &data );
  simthread->start();
}

void MainWindow::stopSimulation()
{
  simwrk->setSimulationEnabled( false );
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

  if( data.getHeight() != -1 && data.getWidth() != -1 )
  {
    CustomiseFormSingleton::instance()->copyModelData( data );
    CustomiseFormSingleton::instance()->calcPicture();
  }
 
  this->hide();
  CustomiseFormSingleton::instance()->show();
}

void MainWindow::showScale( const int arg )
{
  sprintf( buf, "%d%%", arg );
  scaleLabel.setText( buf );
}
