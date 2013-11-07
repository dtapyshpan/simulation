#ifndef DRAWWIDGETCF_H
#define DRAWWIDGETCF_H

#include <QtGui>
#include <QWidget>
#include "ModelData.h"
#include "DrawWidget.h"

class DrawWidgetCF : public DrawWidget
{
  Q_OBJECT

public:
  
  DrawWidgetCF();
  ~DrawWidgetCF();

signals:
  
  void changedScale( int );
  
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
  int imageH, imageW;

  QImage imageData;
  QPixmap pixmapData;

  void drawGround( const ModelData & );
  void drawWater( const ModelData & );
  void paintEvent( QPaintEvent * );

  void initValues();
};

#endif
