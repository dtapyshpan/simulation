#ifndef DRAWWIDGET_H
#define DRAWWIDGET_H

#include <QtGui>
#include <QWidget>
#include "ModelData.h"

class DrawWidget : public QWidget
{
  Q_OBJECT
 
public slots: 

  virtual void drawData( const ModelData & ) = 0;
	
public:

  virtual void mousePressEvent( QMouseEvent * ) = 0;
  virtual void mouseMoveEvent( QMouseEvent * ) = 0;
  virtual void wheelEvent( QWheelEvent * ) = 0;
};

#endif
