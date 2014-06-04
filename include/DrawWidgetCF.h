#ifndef DRAWWIDGETCF_H
#define DRAWWIDGETCF_H

#include <QtGui>
#include <QWidget>
#include <QMainWindow>

#include <cmath>

#include "ModelData.h"
#include "Editor.h"

class DrawWidgetCF : public QWidget
{
  Q_OBJECT

public:
  
  DrawWidgetCF();
  ~DrawWidgetCF();

  void setEdit();

public slots:

  void drawData( ModelData * );
  void getChangedData( double, double, double );

signals:
  
  void changedScale( int );
  void sendMousePosition( int, int, double*, double*, double* );
  void sendToSaveChangedData( int, int, double, double, double );

protected:

  void mousePressEvent( QMouseEvent * );
  void mouseMoveEvent( QMouseEvent * );
  void wheelEvent( QWheelEvent * );
  void mouseReleaseEvent( QMouseEvent * );

private:

  static const int boxsize;
  static const double EPS;

  bool editF;
  double cScale, dataH, dataW, dataS;
  QPoint lastPos, delta, cMousePos;
  Editor edit;

  int dRow, dColumn;

  QImage imageData;
  QPixmap pixmapData, layer;

  void drawGround( const ModelData *const );
  void drawWater( const ModelData *const );
  void drawSnow( const ModelData *const );
  void paintEvent( QPaintEvent * );
  void initValues();
};

#endif
