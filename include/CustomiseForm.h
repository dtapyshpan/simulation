#ifndef CUSTOMISEFORM_H
#define CUSTOMISEFORM_H

#include <QtGui>

#include "DrawWidget.h"
#include "SizeDialogSingleton.h"
#include "Constants.h"

class CustomiseForm : public QMainWindow
{
	Q_OBJECT

public:

	CustomiseForm();
	~CustomiseForm();

	int checkPntr();
	void saveMWPntr( QMainWindow *const );
	void saveImage( const QImage & );

public slots:

	void saveImageSize( int, int );
 
private slots:

	void setMapSize();
  void randomMap();
	void clearMap();
	void exit();
	
private:

	DrawWidget *drawCust;
	QMainWindow *mwpntr;

	QAction *setMapSizeAction;
	QAction *exitAction;
	QAction *randomMapAction;
	QAction *clearMapAction;

	QMenu *custMenu;
	QDockWidget *settings;

	QRadioButton *gh;
	QRadioButton *wd;
	QSpinBox *sbg;
	QSpinBox *sbw;

	int widthImage, heightImage;

	void closeEvent();
	void createSettings();

	void closeEvent( QCloseEvent * );
	void mousePressEvent( QMouseEvent * );
};

#endif
