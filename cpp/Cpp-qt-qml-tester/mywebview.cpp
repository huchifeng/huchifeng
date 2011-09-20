#include "mywebview.h"
#include <QtUiTools/QUiLoader>



void MyWebView::dropEvent( QDropEvent * event )
{
    const QMimeData *mimeData = static_cast<QDropEvent*>(event)->mimeData();

    if (mimeData->hasUrls())
    {
        //qDebug() << "drop";
        //static_cast<QDropEvent*>(event)->acceptProposedAction();
#ifndef _DEBUG
        // 仅在debug模式下允许拖拽其他 html 文件到 webview 内
        event->ignore();
        return;
#endif
    }
    __super::dropEvent(event);
}

void ObjQWebView::printPreview()
{
    QPrintPreviewDialog dlg(webview());
    connect(&dlg, SIGNAL(paintRequested(QPrinter*)), webview(), SLOT(print(QPrinter*)));
    dlg.exec();
}
void ObjQWebView::printPDF(QString filename)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);
    webview()->print(&printer);
}


bool ObjQWebView::print()
{
    if(!choosePrinter())
    {
        printDialog = QSharedPointer<QPrintDialog>(NULL);
        return false;
    }
    webview()->print(printDialog->printer());
    return true;
}
void ObjQWebView::printToRecentUsedPrinter()
{
    if (printDialog.isNull()) {
        print();
        return;
    }
    webview()->print(printDialog->printer());
}
bool ObjQWebView::choosePrinter()
{
    if(printDialog.isNull())
    {
        printDialog = QSharedPointer<QPrintDialog>(new QPrintDialog(webview()));
    }
    return printDialog->exec() == QDialog::Accepted;
}

ObjQWebView::ObjQWebView(QObject* parent): MyObject(parent)
{
    obj = new MyWebView(getTopWidget());
    connect(webview()->page()->currentFrame(), SIGNAL(javaScriptWindowObjectCleared()),
        this, SLOT(on_javaScriptWindowObjectCleared()));
    connect(webview()->page(), SIGNAL(printRequested(QWebFrame*)),
        this, SLOT(printFrameRequest(QWebFrame*)));
}

void ObjQWebView::printFrameRequest(QWebFrame* frame)
{
    if(choosePrinter())
    {
        frame->print(printDialog->printer());
    }
}


void ObjQWebView::on_javaScriptWindowObjectCleared()
{
    // 如果使用了 frame，要用 window.top.webview 才能访问到
    webview()->page()->currentFrame()->addToJavaScriptWindowObject("webview", this);
    webview()->page()->currentFrame()->addToJavaScriptWindowObject("app", qApp);
    webview()->page()->history()->clear();
}


QObject * MyFactory::create( const QString & mimeType, const QUrl & url, const QStringList & argumentNames, const QStringList & argumentValues ) const
{
	for(int i=0; i<argumentNames.size(); i++)
	{
		if(argumentNames[i]=="classid")
		{
			//return new QCalendarWidget;
			QString classid = argumentValues[i];
			QUiLoader loader;
			QWidget* parent = qobject_cast<QWidget*>(QObject::parent());
			QObject *result = loader.createWidget(classid, parent);
			if (!result)
			{
				result = static_cast<QObject*>(
					QMetaType::construct(
					QMetaType::type(
					classid.toLatin1())));
			}
			return result;
		}
	}
	return NULL;
}
