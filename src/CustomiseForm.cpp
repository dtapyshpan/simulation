//ololololo

#include "../include/CustomiseForm.h"

CustomiseForm::CustomiseForm()
{
  mwpntr = NULL;

  statusBar();
  statusBar()->addPermanentWidget( &scaleLabel );
  
  exitAction = new QAction( tr("Exit"), this );
  connect( exitAction, SIGNAL(triggered()), this, SLOT(exit()) );

  custMenu = menuBar()->addMenu( tr("File") );
  custMenu->addAction( exitAction );
  
  drawCust = new DrawWidgetCF();
  setCentralWidget( drawCust );

  makePropWidget();
  
  setWindowTitle( tr("Map Customisation") );

  connect( drawCust, SIGNAL(changedScale( int )), this, SLOT(printScale( const int )) );
  connect( drawCust, SIGNAL(sendMousePosition( int, int )), this, SLOT(getMousePosition( const int, const int )) );
}

CustomiseForm::~CustomiseForm()
{
}

int CustomiseForm::checkPntr()
{
  return mwpntr ? 1 : 0;
}

void CustomiseForm::saveMWPntr( QMainWindow *const pntr )
{
  mwpntr = pntr;
}

void CustomiseForm::closeEvent()
{
  this->hide();
  mwpntr->show();
}

void CustomiseForm::closeEvent( QCloseEvent * )
{
  closeEvent();
}

void CustomiseForm::exit()
{
  closeEvent();
}

void CustomiseForm::copyModelData( const ModelData &arg )
{
  data = arg;
}

void CustomiseForm::printScale( const int arg )
{
  sprintf( buf, "%d%%", arg );
  scaleLabel.setText( buf );
}

void CustomiseForm::getMousePosition( const int x, const int y )
{
  int h = data.groundCell( x, y );
  int w = data.waterCell( x, y );

  sprintf( buf, "h = %d, w = %d", h, w );
  statusBar()->showMessage( buf );
}

void CustomiseForm::calcPicture()
{
  drawCust->drawData( data );
}

void CustomiseForm::makePropWidget()
{
  //propWidget = new QDockWidget( tr("test"), this );
  //addDockWidget( Qt::RightDockWidgetArea, propWidget );

  /*
  propWidget.setWindowFlags( Qt::FramelessWindowHint | Qt::Tool );
  propWidget.setAttribute( Qt::WA_TranslucentBackground );
  propWidget.setAutoFillBackground( false );
  propWidget.setStyleSheet( "background-color: #1b1b1b; border: 1px solid; border-radius: 7px; border-color: #303030;" );
  propWidget.setGeometry( 5, 5, 50, 50 );
  */

  
  propWidget = new QWidget();
  propWidget.setWindowFlags( Qt::FramelessWindowHint | Qt::Tool );
  propWidget.setAttribute( Qt::WA_TranslucentBackground );
  propWidget.setAutoFillBackground( false );
  propWidget.setStyleSheet( "background-color: #1b1b1b; border: 1px solid; border-radius: 7px; border-color: #303030;" );
  propWidget.setGeometry( 5, 5, 50, 50 );
}
