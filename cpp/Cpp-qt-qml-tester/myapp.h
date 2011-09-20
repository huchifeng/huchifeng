#pragma once

#include <QtGui/QApplication>


class MyApp : public QApplication
{
	Q_OBJECT;
	//Q_PROPERTY(QWidget* qmlView READ qmlView);
public:
	MyApp(int argc, char** argv):QApplication(argc, argv){
	}
	public slots:
		void alert(QString s);

};
