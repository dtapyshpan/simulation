#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <cstdio>
#include <cstdlib>

#include <QtGui>

#include "CustomiseFormSingleton.h"
#include "DrawWidget.h"
#include "SimulationWorker.h"
#include "FileExx.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	MainWindow();
	~MainWindow();

private slots:

	void loadMap();
	void saveMap();
	void startSimulation();
	void stopSimulation();
	void exit();
	void about();

	void customiseMap();

private:

	QAction *loadMapAction;
	QAction *saveMapAction;
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
