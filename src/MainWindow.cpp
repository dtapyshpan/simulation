#include <cassert>
#include "../include/MainWindow.h"

MainWindow::MainWindow()
{
  showMaximized();
  addToolBar( Qt::TopToolBarArea, createToolBar() );
	
  drawMain = new DrawWidgetCFOB();
  setCentralWidget( drawMain );

  simthread = new QThread();
  simwrk = new SimulationWorker( drawMain );

  simwrk->moveToThread( simthread );

  createDockWidget();

  statusBar();
  statusBar()->addPermanentWidget( &scaleLabel );

  connect( simthread, SIGNAL(started()), simwrk, SLOT(startModeling()) );
  connect( simwrk, SIGNAL(finished()), simthread, SLOT(quit()) );
  connect( drawMain, SIGNAL(changedScale( int )), this, SLOT(showScale( const int )) );
  connect( drawMain, SIGNAL(sendMousePosition( int, int )), this, SLOT(getMousePosition( const int, const int )) );

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

QToolBar* MainWindow::createToolBar()
{
  QToolBar *menuTB = new QToolBar();

  menuTB->addAction( QPixmap("icons/open"), "open map", this, SLOT(loadMap()) );
  menuTB->addAction( QPixmap("icons/start"), "start modeling", this, SLOT(startSimulation()) );
  menuTB->addAction( QPixmap("icons/stop"), "stop modeling", this, SLOT(stopSimulation()) );
  menuTB->addAction( QPixmap("icons/about"), "about", this, SLOT(about()) );

  return menuTB;
}

void MainWindow::getMousePosition( const int x, const int y )
{
  int h = data.groundCell( x, y );                               
  int w = data.waterCell( x, y );
  sprintf( buf, "h = %d, w = %d", h, w );
  statusBar()->showMessage( buf );
}
