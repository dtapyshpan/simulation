#include <iostream>
#include "../include/Editor.h"

Editor::Editor()
{
  setModal( true );

  QHBoxLayout *groundL = new QHBoxLayout;
  QLabel *hLabel = new QLabel( tr("Height:"), this );
  hEdit = new QLineEdit( this );
  groundL->addWidget( hLabel );
  groundL->addWidget( hEdit );

  QHBoxLayout *waterL = new QHBoxLayout;
  QLabel *wLabel = new QLabel( tr("Water:"), this );
  wEdit = new QLineEdit( this );
  waterL->addWidget( wLabel );
  waterL->addWidget( wEdit );

  QHBoxLayout *springL = new QHBoxLayout;
  QLabel *sLabel = new QLabel( tr("Spring:"), this );
  sEdit = new QLineEdit( this );
  springL->addWidget( sLabel );
  springL->addWidget( sEdit );

  QHBoxLayout *buttonsL = new QHBoxLayout;
  QPushButton *okB = new QPushButton( tr("Ok") );
  QPushButton *cancelB = new QPushButton( tr("Cancel") );
  connect( okB, SIGNAL(clicked()), this, SLOT(okPress()) );
  connect( cancelB, SIGNAL(clicked()), this, SLOT(close()) );
  buttonsL->addWidget( okB );
  buttonsL->addWidget( cancelB );

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout( groundL );
  mainLayout->addLayout( waterL );
  mainLayout->addLayout( springL );
  mainLayout->addLayout( buttonsL );
  setLayout( mainLayout );

  setWindowTitle( tr("Config Dialog") );
}

double Editor::toDouble( const char *s )
{
  printf("s = %s\n", s);
  double x1 = 0.0, x2 = 0.0;
  int i = 0;
  while( i < strlen( s ) )
  {
    if( s[i] == '.' )
    {
      ++i;
      break;
    }
    x1 = x1 * 10 + int(s[i]) - int('0');
    ++i;
  }
  int d = 10;
  while( i < strlen( s ) )
  {
    x2 += double(int(s[i]) - int('0')) / d;
    d *= 10;
    ++i;
  }
  return x1 + x2;
}

void Editor::okPress()
{
  double h = toDouble( hEdit->text().toUtf8().constData() );
  double w = toDouble( wEdit->text().toUtf8().constData() );
  double s = toDouble( sEdit->text().toUtf8().constData() );
  emit sendChangedData( h, w, s );
  close();
}

void Editor::setData( const double dH, const double dW, const double dS )
{
  char s1[25], s2[25], s3[25];
  sprintf( s1, "%.8lf", dH );
  sprintf( s2, "%.8lf", dW );
  sprintf( s3, "%.8lf", dS );
  hEdit->setText( s1 );
  wEdit->setText( s2 );
  sEdit->setText( s3 );
}
