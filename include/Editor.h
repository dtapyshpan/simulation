#ifndef EDITOR_H
#define EDITOR_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

#include <cstdio>
#include <cmath>

class Editor : public QDialog
{
  Q_OBJECT

public:

  Editor();
  
  void setData( const double, const double, const double );

signals:
  
  void sendChangedData( double, double, double );

private slots:

  void okPress();

private:

  QLineEdit *hEdit, *wEdit, *sEdit;

  double toDouble( const char * );
};

#endif
