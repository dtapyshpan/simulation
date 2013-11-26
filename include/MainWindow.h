#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "FileExx.h"
#include "DrawWidgetMW.h"
#include "SimulationWorker.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  MainWindow();
  ~MainWindow();

public slots:

  void showScale( const int );

private slots:

  void loadMap();
  void startSimulation();
  void stopSimulation();
  void exit();
  void about();

private:

  QLabel scaleLabel;
  QLabel isSpring;

  QSpinBox groundSB;
  QSpinBox waterSB;
	
  QMenu *topMenu;
  
  QThread *simthread;
  SimulationWorker *simwrk;
  
  DrawWidgetMW *drawMain;
  
  ModelData data;

  char buf[256];

  void createDockWidget();
  QToolBar* createToolBar();
};

#endif
