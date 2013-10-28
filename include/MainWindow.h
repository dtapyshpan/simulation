#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "FileExx.h"
#include "DrawWidget.h"
#include "SimulationWorker.h"
#include "CustomiseFormSingleton.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:

  MainWindow();
  ~MainWindow();

private slots:

  void loadMap();
  void startSimulation();
  void stopSimulation();
  void exit();
  void about();
  void customiseMap();

private:

  QAction *loadMapAction;
  QAction *startSimAction;
  QAction *stopSimAction;
  QAction *exitAction;
  QAction *customiseMapAction;
  QAction *aboutAction;
	
  QMenu *topMenu;
  
  QThread *simthread;
  SimulationWorker *simwrk;
  
  DrawWidget *drawMain;
  
  ModelData data;
};

#endif
