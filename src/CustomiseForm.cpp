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

  setWindowTitle( tr("Map Customisation") );

  connect( drawCust, SIGNAL(changedScale( int )), this, SLOT(printScale( const int )) );
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

void CustomiseForm::paintEvent( QPaintEvent * )
{
  drawCust->drawData( data );
}

void CustomiseForm::printScale( const int arg )
{
  char buf[21];
  sprintf( buf, "%d%%", arg );
  scaleLabel.setText( buf );
}
