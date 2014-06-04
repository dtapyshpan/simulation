#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "FileExx.h"
#include "DrawWidgetCF.h"
#include "SimulationWorker.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  MainWindow();
  ~MainWindow();

public slots:

  void showScale( const int );
  void getMousePosition( const int, const int, double*, double*, double* );
  void saveChangedData( int, int, double, double, double );

private slots:

  void loadMap();
  void saveMap();
  void startSimulation();
  void stopSimulation();
  void editMap();
  void exit();
  void about();

private:

  QToolBar* createToolBar();

  QLabel scaleLabel;
  QThread *simthread;
  SimulationWorker *simwrk;
  DrawWidgetCF *drawMain;
  ModelData *data;
  char buf[25];
};

#endif
