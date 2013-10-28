#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QtGui>
#include <QWidget>
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
	
protected:

  void mousePressEvent( QMouseEvent * );
  void mouseMoveEvent( QMouseEvent * );
  void wheelEvent( QWheelEvent * );

private:

  double cScale;
  QPoint lastPos, delta, cMousePos;

  static const int boxsize;
  static const double EPS;
  int imageHeight, imageWidth;

  QImage imageData;
  QPixmap pixmapData;

  void drawGround( const ModelData & );
  void drawWater( const ModelData & );
  void paintEvent( QPaintEvent * );

  void initValues();
};

#endif
