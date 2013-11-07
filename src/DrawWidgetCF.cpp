#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

#include "../include/DrawWidgetCF.h"

const int DrawWidgetCF::boxsize = 5;
const double DrawWidgetCF::EPS = 1e-7;

const double defaultZoom = 0.9;
const double defaultScale = 1.0;

DrawWidgetCF::DrawWidgetCF()
{
  setMouseTracking( true );
  initValues();
}

DrawWidgetCF::~DrawWidgetCF()
{
  printf("DrawWidgetCF has been deleted\n");
}

void DrawWidgetCF::initValues()
{
  imageH = imageW = -1;
  cScale = defaultScale;
  delta = lastPos = cMousePos = QPoint( 0, 0 );
}

void DrawWidgetCF::drawGround( const ModelData &data )
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

void DrawWidgetCF::drawWater( const ModelData &data )
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
	tmp.setHsvF( 0.7, color, 1 - color );
	painter.fillRect( x, y, boxsize, boxsize, tmp );
      }
    }
  }
}

void DrawWidgetCF::drawData( const ModelData &data )
{
  if( data.getHeight() == -1 || data.getWidth() == -1 ) return;

  drawGround( data );
  drawWater( data );
  emit changedScale( int( cScale * 100.0 ) );

  pixmapData = QPixmap::fromImage( imageData );
  update();
}

void DrawWidgetCF::paintEvent( QPaintEvent * )
{
  if( pixmapData.isNull() ) return;

  int x = int( boxsize * imageH * cScale );
  int y = int( boxsize * imageW * cScale );
  QPixmap layer = pixmapData.scaled( x, y );

  int boxwh = int( boxsize * cScale );
  
  QPainter painter( this );
  painter.drawPixmap( delta, layer );
  painter.setPen( Qt::red );
  painter.setBrush( Qt::NoBrush );
  painter.drawRect( cMousePos.x(), cMousePos.y(), boxwh, boxwh );

  if( fabs( cScale - defaultScale ) <= EPS ) 
  {
    int x = cMousePos.x() - delta.x();
    int y = cMousePos.y() - delta.y();

    if( x < 0 || y < 0 ) return;
    if( x > imageH * boxsize || y > imageW * boxsize ) return;

    
  }
}

void DrawWidgetCF::mousePressEvent( QMouseEvent *event )
{
  if( event->buttons() == Qt::LeftButton ) 
  {
    lastPos = event->pos();
  }
}

void DrawWidgetCF::mouseMoveEvent( QMouseEvent *event )
{
  if( event->buttons() == Qt::LeftButton )
  {
    delta += event->pos() - lastPos;
    lastPos = event->pos();
  }
  cMousePos = event->pos();
  update();
}

void DrawWidgetCF::wheelEvent( QWheelEvent *event ) 
{
  int numDegrees = -1 * event->delta() / 8;
  int numSteps = numDegrees / 15;

  cScale *= pow( defaultZoom, numSteps );
  emit changedScale( int( cScale * 100.0 ) );
  update();
}
