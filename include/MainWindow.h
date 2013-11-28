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
  void getMousePosition( const int, const int );

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
  
  DrawWidgetCF *drawMain;
  
  ModelData data;

  char buf[256];

  void createDockWidget();
  QToolBar* createToolBar();
};

#endif
