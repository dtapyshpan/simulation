#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <QtGui>

class ContextMenu : public QTextEdit 
{
  Q_OBJECT

public:

  ContextMenu();
  void showContextMenu( QContextMenuEvent * );

public slots:

  void slotActivated( QAction * );

private:
 
  QMenu *m_pmnu;
};

#endif
