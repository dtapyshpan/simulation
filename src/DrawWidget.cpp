#include <cstdlib>
#include <ctime>
#include <cassert>

#include "../include/DrawWidget.h"

const int DrawWidget::boxsize = 5;
const double DrawWidget::EPS = 1e-7;

DrawWidget::DrawWidget()
{
	imageHeight = imageWidth = -1;
}

DrawWidget::~DrawWidget()
{
	printf("DrawWidget has been deleted\n");
}

const QImage & DrawWidget::getImage()
{
	return imageData;
}

void DrawWidget::setImage( const QImage &newImage )
{
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
	drawGround( data );
	drawWater( data );

	pixmapData = QPixmap::fromImage( imageData );
	update();
}

void DrawWidget::paintEvent( QPaintEvent * )
{
	QPainter painter( this );

	painter.fillRect( rect(), Qt::white );
	if( !pixmapData.isNull() ) painter.drawPixmap( QPoint( 0, 0 ), pixmapData );
}
