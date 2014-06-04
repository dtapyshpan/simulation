#include <cstdlib>
#include <ctime>
#include <cassert>

#include "../include/DrawWidgetCF.h"

const int DrawWidgetCF::boxsize = 5;
const double DrawWidgetCF::EPS = 1e-7;

const double defaultZoom = 0.9;
const double defaultScale = 1.0;

int boxwh;

DrawWidgetCF::DrawWidgetCF()
{
  setMouseTracking( true );
  initValues();
  connect( &edit, SIGNAL(sendChangedData(double, double, double)), this, SLOT(getChangedData(double, double, double)) );
}

DrawWidgetCF::~DrawWidgetCF()
{
  printf("DrawWidgetCF has been deleted\n");
}

void DrawWidgetCF::initValues()
{
  editF = false;
  dRow = dColumn = -1;
  cScale = defaultScale;
  boxwh = boxsize;
  delta = lastPos = cMousePos = QPoint( 0, 0 );
}

void DrawWidgetCF::drawGround( const ModelData *const data )
{
  int iRow = dRow * boxsize, iColumn = dColumn * boxsize;
  QImage ret( iColumn, iRow, QImage::Format_RGB32 ); //width, height
  QPainter painter( &ret );
  QColor tmp;

  for( int c = 0; c < dColumn; ++c )
  {
    int y = c * boxsize;
    for( int r = 0; r < dRow; ++r )
    {
      int x = r * boxsize;
      double color = data->getGroundColor( r, c );
      tmp.setHsvF( 0.3, color, 1.0 - color );
      painter.fillRect( y, x,  boxsize, boxsize, tmp );
    }
  }
  
  imageData = ret;
}

void DrawWidgetCF::drawWater( const ModelData *const data )
{
  QPainter painter( &imageData );
  QColor tmp;

  for( int c = 0; c < dColumn; ++c ) 
  {
    int y = c * boxsize;
    for( int r = 0; r < dRow; ++r )
    {
      const double wl = data->waterCell( r, c );
      if( wl > EPS )
      {
	int x = r * boxsize;
	double color = data->getWaterColor( r, c );
	tmp.setHsvF( 0.7, color, 1.0 - color );
	painter.fillRect( y, x, boxsize, boxsize, tmp );
      }
    }
  }
}

void DrawWidgetCF::drawSnow( const ModelData *const data )
{
  QPainter painter( &imageData );
  QColor tmp;

  for( int c = 0; c < dColumn; ++c ) 
  {
    int y = c * boxsize;
    for( int r = 0; r < dRow; ++r )
    {
      const double wl = data->snowCell( r, c );
      if( wl > EPS )
      {
	int x = r * boxsize;
	double color = data->getSnowColor( r, c );
	tmp.setHsvF( 0.5, color, 1.0 - color );
	painter.fillRect( y, x, boxsize, boxsize, tmp );
      }
    }
  }
}

void DrawWidgetCF::drawData( ModelData *data )
{
  if( data->getRow() == -1 || data->getColumn() == -1 ) return;

  initValues();

  dRow = data->getRow();
  dColumn = data->getColumn();

  drawGround( data );
  drawWater( data );
  drawSnow( data );
  emit changedScale( int( cScale * 100.0 ) );

  pixmapData = QPixmap::fromImage( imageData );
  layer = pixmapData;
  update();
}

void DrawWidgetCF::paintEvent( QPaintEvent * )
{
  if( pixmapData.isNull() ) return;

  QPainter painter( this );
  painter.drawPixmap( delta, layer );
  painter.setPen( Qt::red );
  painter.setBrush( Qt::NoBrush );
  painter.drawRect( cMousePos.x(), cMousePos.y(), boxwh, boxwh );
}

void DrawWidgetCF::mousePressEvent( QMouseEvent *event )
{
  if( event->buttons() == Qt::LeftButton ) 
    lastPos = event->pos();
}

void DrawWidgetCF::mouseMoveEvent( QMouseEvent *event )
{
  if( event->buttons() == Qt::LeftButton )
  {
    delta += event->pos() - lastPos;
    lastPos = event->pos();
  }

  cMousePos = event->pos();

  if( fabs( cScale - defaultScale ) <= EPS ) 
  {
    int c = ( cMousePos.x() - delta.x() ) / boxsize;
    int r = ( cMousePos.y() - delta.y() ) / boxsize;

    if( r < 0 || c < 0 ) return;
    if( r >= dRow || c >= dColumn ) return;

    emit sendMousePosition( r, c, &dataH, &dataW, &dataS );
  }

  update();
}

void DrawWidgetCF::wheelEvent( QWheelEvent *event ) 
{
  int numDegrees = -1 * event->delta() / 8;
  int numSteps = numDegrees / 15;

  cScale *= pow( defaultZoom, numSteps );

  boxwh = int( boxsize * cScale );
  int nRow = int( boxsize * dRow * cScale );
  int nColumn = int( boxsize * dColumn * cScale );
  layer = pixmapData.scaled( nColumn, nRow );

  emit changedScale( int( cScale * 100.0 ) );
  update();
}

void DrawWidgetCF::mouseReleaseEvent( QMouseEvent *event )
{
  if( editF && fabs( defaultScale - cScale ) <= EPS )
  {
    edit.setData( dataH, dataW, dataS );
    edit.show();
  }
}

void DrawWidgetCF::setEdit()
{
  if( dRow != -1 && dColumn != -1 ) editF = !editF;
}

void DrawWidgetCF::getChangedData( double h, double w, double s )
{
  int c = ( cMousePos.x() - delta.x() ) / boxsize;
  int r = ( cMousePos.y() - delta.y() ) / boxsize;
  emit sendToSaveChangedData( r, c, h, w, s );
}
