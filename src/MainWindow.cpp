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

  topMenu = menuBar()->addMenu( tr("Help") );
  topMenu->addAction( aboutAction );
  
  showMaximized();
	
  drawMain = new DrawWidgetMW();
  setCentralWidget( drawMain );

  simthread = new QThread();
  simwrk = new SimulationWorker( drawMain );

  simwrk->moveToThread( simthread );

  createDockWidget();

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

void MainWindow::showScale( const int arg )
{
  sprintf( buf, "%d%%", arg );
  scaleLabel.setText( buf );
}

void MainWindow::createDockWidget()
{
  QDockWidget *editWidget = new QDockWidget();
  editWidget->setAllowedAreas( Qt::RightDockWidgetArea );
  
  QGridLayout *gLayout = new QGridLayout();

  //----------------

  QLabel *hLabel = new QLabel("Height:");
  gLayout->addWidget( hLabel, 1, 0 );
  gLayout->addWidget( &groundSB, 1, 1 );

  QLabel *wLabel = new QLabel("Water:");
  gLayout->addWidget( wLabel, 2, 0 );
  gLayout->addWidget( &waterSB, 2, 1 );

  QLabel *sLabel = new QLabel( "Spring:" );
  gLayout->addWidget( sLabel, 3, 0 );
  gLayout->addWidget( &isSpring, 3, 1 );
  isSpring.setText( tr( "No" ) );

  //----------------

  QWidget *tmpW = new QWidget();
  tmpW->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
  tmpW->setLayout( gLayout );

  editWidget->setWidget( tmpW );

  addDockWidget( Qt::RightDockWidgetArea, editWidget );
}
