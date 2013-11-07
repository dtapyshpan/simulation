#ifndef DRAWWIDGETMW_H
#define DRAWWIDGETMW_H

#include <QtGui>
#include <QWidget>
#include "ModelData.h"
#include "DrawWidget.h"

class DrawWidgetMW : public DrawWidget
{
  Q_OBJECT

public:
  
  DrawWidgetMW();
  ~DrawWidgetMW();
  
public slots: 

  void drawData( const ModelData & );
	
protected:

  void mousePressEvent( QMouseEvent * );
  void mouseMoveEvent( QMouseEvent * );
  void wheelEvent( QWheelEvent * );

private:

  double cScale;
  QPoint lastPos, delta;

  static const int boxsize;
  int imageH, imageW;

  QImage imageData;
  QPixmap pixmapData;

  void drawGround( const ModelData & );
  void drawWater( const ModelData & );
  void paintEvent( QPaintEvent * );

  void initValues();
};

#endif
