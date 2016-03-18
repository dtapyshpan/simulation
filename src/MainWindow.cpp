#include <cassert>
#include <QStatusBar>
#include <QToolBar>
#include <QMessageBox>
#include <QFileDialog>
#include "../include/MainWindow.h"

MainWindow::MainWindow()
{
  showMaximized();
  addToolBar( Qt::TopToolBarArea, createToolBar() );

  data = new ModelData();

  drawMain = new DrawWidgetCF();
  setCentralWidget( drawMain );

  simthread = new QThread();
  simwrk = new SimulationWorker( drawMain );

  simwrk->moveToThread( simthread );

  statusBar();
  statusBar()->addPermanentWidget( &scaleLabel );

  connect( simthread, SIGNAL(started()), simwrk, SLOT(startModeling()) );
  connect( simwrk, SIGNAL(finished()), simthread, SLOT(quit()) );
  connect( drawMain, SIGNAL(changedScale(int)), this, SLOT(showScale(const int)) );
  connect( drawMain, SIGNAL(sendMousePosition(int, int, double*, double*, double*)), this, SLOT(getMousePosition(const int, const int, double*, double*, double*)) );
  connect( drawMain, SIGNAL(sendToSaveChangedData(int, int, double, double, double)), this, SLOT(saveChangedData(int, int, double, double, double)) );
  
  setWindowTitle( tr("Water Movement Simulation Tool") );
}

MainWindow::~MainWindow()
{
  printf("main window has been deleted\n");
  if( simthread ) delete simthread;
  if( simwrk ) delete simwrk;
  if( drawMain ) delete drawMain;
  if( data ) delete data;
}

QToolBar* MainWindow::createToolBar()
{
  QToolBar *menuTB = new QToolBar();

  menuTB->addAction( QPixmap("icons/open_new"), "open map", this, SLOT(loadMap()) );
  menuTB->addAction( QPixmap("icons/save_new"), "save map", this, SLOT(saveMap()) );
  menuTB->addAction( QPixmap("icons/start_new"), "start modeling", this, SLOT(startSimulation()) );
  menuTB->addAction( QPixmap("icons/stop_new"), "stop modeling", this, SLOT(stopSimulation()) );
  menuTB->addAction( QPixmap("icons/edit_new"), "editMap", this, SLOT(editMap()) );
  menuTB->addAction( QPixmap("icons/about_new"), "about", this, SLOT(about()) );
  menuTB->setMovable( false );
  
  return menuTB;
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
    data->readDataFromFile( fileName.toStdString().c_str() );
    drawMain->drawData( data );
  }
  catch( FileExx &arg )
  {
    arg.debugPrint();
  }
}

void MainWindow::saveMap()
{
  if( data->getRow() <= 0 || data->getColumn() <= 0 ) return;

  QString fileName = QFileDialog::getSaveFileName( this, tr("Save File"), "", "" );
  if( fileName.isEmpty() ) return;
  
  data->saveToFile( fileName.toStdString().c_str() );
}

void MainWindow::startSimulation()
{
  if( data->getRow() == -1 || data->getColumn() == -1 ) return;
  simwrk->setSimulationEnabled( true );
  simwrk->setModelData( data );
  simthread->start();
}

void MainWindow::stopSimulation()
{
  simwrk->setSimulationEnabled( false );
}

void MainWindow::editMap()
{
  
  drawMain->setEdit();
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

void MainWindow::getMousePosition( const int x, const int y, double *dH, double *dW, double *dS )
{
  *dH = data->groundCell( x, y );                               
  *dW = data->waterCell( x, y );
  *dS = data->springCell( x, y );
  sprintf( buf, "h = %lf, w = %lf, s = %lf", *dH, *dW, *dS );
  statusBar()->showMessage( buf );
}

void MainWindow::saveChangedData( int x, int y, double h, double w, double s )
{
  data->rgroundCell( x, y ) = h;
  data->rwaterCell( x, y ) = w;
  data->rspringCell( x, y ) = s;
  drawMain->drawData( data );
}
