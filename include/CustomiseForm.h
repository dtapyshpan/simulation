#ifndef CUSTOMISEFORM_H
#define CUSTOMISEFORM_H

#include <QtGui>
#include <QWidget>
#include "DrawWidgetCF.h"
#include "ModelData.h"
#include "Constants.h"

class CustomiseForm : public QMainWindow
{
  Q_OBJECT
	
public:

  CustomiseForm();
  ~CustomiseForm();

  int checkPntr();
  void saveMWPntr( QMainWindow *const );
  void copyModelData( const ModelData & );
  
private slots:

  void exit();
	
private:

  QMainWindow *mwpntr;
  QAction *exitAction;
  QMenu *custMenu;

  ModelData data;
  DrawWidgetCF *drawCust;
 
  void closeEvent();
  void closeEvent( QCloseEvent * );
  void paintEvent( QPaintEvent * );
};

#endif
