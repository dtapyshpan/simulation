#include "../include/CustomiseForm.h"

char buf[256];

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
  sprintf( buf, "%d%%\0", arg );
  scaleLabel.setText( buf );
}

void CustomiseForm::getMousePosition( const int x, const int y )
{
  int h = data.groundCell( x, y );
  int w = data.waterCell( x, y );

  sprintf( buf, "h = %d, w = %d\0", h, w );
  statusBar()->showMessage( buf );
}

void CustomiseForm::calcPicture()
{
  drawCust->drawData( data );
}
