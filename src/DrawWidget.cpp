#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>

#include "../include/DrawWidget.h"

const int DrawWidget::boxsize = 5;
const double DrawWidget::EPS = 1e-7;

const double defaultZoom = 0.9;
const double defaultScale = 1.0;

DrawWidget::DrawWidget()
{
  setMouseTracking( true );
  imageHeight = imageWidth = -1;
  initValues();
}

DrawWidget::~DrawWidget()
{
  printf("DrawWidget has been deleted\n");
}

void DrawWidget::initValues()
{
  cScale = defaultScale;
  delta = lastPos = cMousePos = QPoint( 0, 0 );
}

const QImage & DrawWidget::getImage()
{
  return imageData;
}

void DrawWidget::setImage( const QImage &newImage )
{
  initValues();
  imageData = newImage;
  pixmapData = QPixmap::fromImage( newImage );
  update();
}

void DrawWidget::drawGround( const ModelData &data )
{
  imageHeight = data.getHeight();
  imageWidth = data.getWidth();
  int hI = imageHeight * boxsize, wI = imageWidth * boxsize;
  QImage ret( hI, wI, QImage::Format_RGB32 );
  QPainter painter( &ret );
  QColor tmp;
  
  for( int i = 0; i < imageHeight; ++i )
  {
    int x = i * boxsize;
    for( int j = 0; j < imageWidth; ++j )
    {
      int y = j * boxsize;
      double color = data.getGroundColor( i, j );
      tmp.setHsvF( 0.3, color, 1.0 - color );
      painter.fillRect( x, y, boxsize, boxsize, tmp );
    }
  }

  imageData = ret;
}

void DrawWidget::drawWater( const ModelData &data )
{
  QPainter painter( &imageData );
  QColor tmp;

  for( int i = 0; i < imageHeight; ++i )
  {
    int x = i * boxsize;
    for( int j = 0; j < imageWidth; ++j ) 
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

void DrawWidget::drawData( const ModelData &data )
{
  if( data.getHeight() == -1 || data.getWidth() == -1 ) return;

  drawGround( data );
  drawWater( data );

  pixmapData = QPixmap::fromImage( imageData );
  update();
}

void DrawWidget::paintEvent( QPaintEvent * )
{
  //if( pixmapData.isNull() ) return;
  
  /*
  QPainter pmpainter( &pixmapData );
  pmpainter.setPen( Qt::blue );
  pmpainter.drawPoint( ( 2 * defaultScale - cScale ) * QPointF( cMousePos - delta ) );
  */
  
  QPainter painter( this );
  //painter.setPen( Qt::blue );
  //painter.setBrush( Qt::NoBrush );
  //painter.drawPoint( cMousePos );

  //painter.scale( cScale, cScale );
  //painter.drawPixmap( delta, pixmapData );
  
  painter.setPen( Qt::red );
  painter.setBrush( Qt::NoBrush );
  painter.drawPoint( cMousePos );
}

void DrawWidget::mousePressEvent( QMouseEvent *event )
{
  if( event->buttons() == Qt::LeftButton ) 
  {
    lastPos = event->pos();
  }
}

void DrawWidget::mouseMoveEvent( QMouseEvent *event )
{
  if( event->buttons() == Qt::LeftButton )
  {
    delta += event->pos() - lastPos;
    lastPos = event->pos();
  }
  cMousePos = event->pos();
  update();
}

void DrawWidget::wheelEvent( QWheelEvent *event ) 
{
  int numDegrees = -1 * event->delta() / 8;
  int numSteps = numDegrees / 15;

  cScale *= pow( defaultZoom, numSteps );
  printf("cScale = %lf\n", cScale);
  update();
}
