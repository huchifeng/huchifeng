#include "myapp.h"
#include <QMessageBox>

void MyApp::alert(QString s){
	QMessageBox::warning(NULL, " ", s);
}