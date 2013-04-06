#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QtGui>

#include "ModelData.h"

class DrawWidget : public QWidget
{
	Q_OBJECT

public:

	DrawWidget();
	~DrawWidget();

	const QImage & getImage();
	void setImage( const QImage & );

public slots: 

	void drawData( const ModelData & );

private:

	static const int boxsize;
	static const double EPS;
	int imageHeight, imageWidth;

	QImage imageData;
	QPixmap pixmapData;

	void drawGround( const ModelData & );
	void drawWater( const ModelData & );
	void paintEvent( QPaintEvent * );
};

#endif
