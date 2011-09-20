#include "mywebview.h"
#include "myapp.h"

#ifdef _DEBUG
#pragma comment(lib, "QtDeclaratived4.lib")
#else
#pragma comment(lib, "QtDeclarative4.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "QtWebKitd4.lib")
#else
#pragma comment(lib, "QtWebKit4.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "QtUiToolsd.lib")
#else
#pragma comment(lib, "QtUiTools.lib")
#endif

QDeclarativeView* view = NULL;

QWidget* getTopWidget()
{
	return view;
}

int main(int argc, char *argv[])
{
	MyApp a(argc, argv);
	QDir::setCurrent(QCoreApplication::applicationDirPath ());

    qmlRegisterType<QWidget>();
    qmlRegisterType<QWebView >();
    qmlRegisterType<ObjQWebView>("MyComponents", 1, 0, "ObjQWebView");

	view = new QDeclarativeView;
	view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->rootContext()->setContextProperty("app", &a);
	view->setSource(QUrl::fromLocalFile("../1.qml"));
	view->show();
	return a.exec();
}


