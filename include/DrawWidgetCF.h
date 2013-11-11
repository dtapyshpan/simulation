#ifndef DRAWWIDGETCF_H
#define DRAWWIDGETCF_H

#include <QtGui>
#include <QWidget>
#include <QMainWindow>
#include "ModelData.h"
#include "DrawWidget.h"
#include "ContextMenu.h"

class DrawWidgetCF : public DrawWidget
{
  Q_OBJECT

public:
  
  DrawWidgetCF();
  ~DrawWidgetCF();

signals:
  
  void changedScale( int );
  void sendMousePosition( int, int );
  
public slots: 

  void drawData( const ModelData & );
	
protected:

  void mousePressEvent( QMouseEvent * );
  void mouseMoveEvent( QMouseEvent * );
  void wheelEvent( QWheelEvent * );
  void contextMenuEvent( QContextMenuEvent * );

private:

  double cScale;
  QPoint lastPos, delta, cMousePos;

  static const int boxsize;
  static const double EPS;
  int imageH, imageW;

  QImage imageData;
  QPixmap pixmapData, layer;
  ContextMenu cMenu;

  void drawGround( const ModelData & );
  void drawWater( const ModelData & );
  void paintEvent( QPaintEvent * );

  void initValues();
};

#endif
