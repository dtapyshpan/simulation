#include <cstdlib>
#include <cmath>
#include <cassert>

#include "../include/DrawWidgetMW.h"

const int DrawWidgetMW::boxsize = 5;

const double defaultZoom = 0.9;
const double defaultScale = 1.0;
const double EPS = 1e-7;

DrawWidgetMW::DrawWidgetMW()
{
  initValues();
}

DrawWidgetMW::~DrawWidgetMW()
{
  printf("DrawWidgetMW has been deleted\n");
}

void DrawWidgetMW::initValues()
{
  imageH = imageW = -1;
  cScale = defaultScale;
  delta = lastPos = QPoint( 0, 0 );
}

void DrawWidgetMW::drawGround( const ModelData &data )
{
  imageH = data.getHeight();
  imageW = data.getWidth();
  int hI = imageH * boxsize, wI = imageW * boxsize;
  QImage ret( hI, wI, QImage::Format_RGB32 );
  QPainter painter( &ret );
  QColor tmp;
  
  for( int i = 0; i < imageH; ++i )
  {
    int x = i * boxsize;
    for( int j = 0; j < imageW; ++j )
    {
      int y = j * boxsize;
      double color = data.getGroundColor( i, j );
      tmp.setHsvF( 0.3, color, 1.0 - color );
      painter.fillRect( x, y, boxsize, boxsize, tmp );
    }
  }

  imageData = ret;
}

void DrawWidgetMW::drawWater( const ModelData &data )
{
  QPainter painter( &imageData );
  QColor tmp;

  for( int i = 0; i < imageH; ++i )
  {
    int x = i * boxsize;
    for( int j = 0; j < imageW; ++j ) 
    {
      const int wl = data.waterCell( i, j );
      if( wl > 0 )
      {
	double color = data.getWaterColor( i, j );
	int y = j * boxsize;
	tmp.setHsvF( 0.7, color, 1.0 - color );
	painter.fillRect( x, y, boxsize, boxsize, tmp );
      }
    }
  }
}

void DrawWidgetMW::drawData( const ModelData &data )
{
  if( data.getHeight() == -1 || data.getWidth() == -1 ) return;

  initValues();
  drawGround( data );
  drawWater( data );
  emit sendScale( int( cScale * 100.0 ) );

  pixmapData = QPixmap::fromImage( imageData );
  layer = pixmapData;
  update();
}

void DrawWidgetMW::paintEvent( QPaintEvent * )
{
  if( pixmapData.isNull() ) return;
  
  QPainter painter( this );
  painter.drawPixmap( delta, layer );
}

void DrawWidgetMW::mousePressEvent( QMouseEvent *event )
{
  if( event->buttons() == Qt::LeftButton ) 
  {
    lastPos = event->pos();
  }
}

void DrawWidgetMW::mouseMoveEvent( QMouseEvent *event )
{
  if( event->buttons() == Qt::LeftButton )
  {
    delta += event->pos() - lastPos;
    lastPos = event->pos();
    update();
  }
}

void DrawWidgetMW::wheelEvent( QWheelEvent *event ) 
{
  int numDegrees = -1 * event->delta() / 8;
  int numSteps = numDegrees / 15;

  cScale *= pow( defaultZoom, numSteps );

  int x = int( boxsize * imageH * cScale );
  int y = int( boxsize * imageW * cScale );
  layer = pixmapData.scaled( x, y );

  emit sendScale( int( cScale * 100.0 ) );

  update();
}
