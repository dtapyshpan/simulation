#ifndef SIZEDIALOG_H
#define SIZEDIALOG_H

#include <QtGui>
#include <cstdio>

class SizeDialog : public QDialog
{
	Q_OBJECT

public:

	SizeDialog();

private slots:

 	void onOkClick();
	void onCancelClick();

signals:
	
	void returnImageSize( int, int );

private:

	QLabel *wlabel;
	QLabel *hlabel;

	QLineEdit *wedit;
	QLineEdit *hedit;

	QPushButton *okbut;
	QPushButton *cnlbut;
};

#endif
