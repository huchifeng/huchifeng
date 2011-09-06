#include <QtGui/QApplication>
#include <QtDeclarative/QtDeclarative>

#ifdef _DEBUG
#pragma comment(lib, "QtDeclaratived4.lib")
#else
#pragma comment(lib, "QtDeclarative4.lib")
#endif


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QDir::setCurrent(QCoreApplication::applicationDirPath ());

	QDeclarativeView* view = new QDeclarativeView;
	view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
	view->setSource(QUrl::fromLocalFile("../1.qml"));
	view->show();
	return a.exec();
}
