#pragma once

#include <QtGui/QApplication>
#include <QtDeclarative/QtDeclarative>
#include <QtWebkit/QtWebkit>

class MyObject: public QObject //QDeclarativeItem // 
{
    Q_OBJECT;
    Q_PROPERTY(int x READ x WRITE setX NOTIFY xChanged);
    Q_PROPERTY(int y READ y WRITE setY NOTIFY yChanged);
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY widthChanged);
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY heightChanged);
    Q_PROPERTY(QObject *obj READ getObject NOTIFY objChanged);
    Q_PROPERTY(QWidget *widget READ getWidget);



public:
    MyObject(QObject* parent=NULL) : QObject(parent)
    {   //(QDeclarativeItem* parent):QDeclarativeItem(parent) //
        obj = NULL;
    }

    int x()
    {
        return qobject_cast<QWidget*>(obj)->x();
    }
    int y()
    {
        return qobject_cast<QWidget*>(obj)->y();
    }
    int width()
    {
        return qobject_cast<QWidget*>(obj)->width();
    }
    int height()
    {
        return qobject_cast<QWidget*>(obj)->height();
    }
    void setX(int x)
    {
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if(widget) widget->move(x, widget->y());
        emit xChanged();
    }
    void setY(int y)
    {
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if(widget) widget->move(widget->x(), y);
        emit yChanged();
    }
    void setWidth(int w)
    {
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if(widget) widget->resize(w, widget->height());
        emit widthChanged();
    }
    void setHeight(int h)
    {
        QWidget* widget = qobject_cast<QWidget*>(obj);
        if(widget) widget->resize(widget->width(), h);
        emit heightChanged();
    }
    QObject* getObject()
    {
        return obj;
    }
    QWidget* getWidget()
    {
        return qobject_cast<QWidget*>(obj);
    }

public slots:

signals:
    void xChanged();
    void yChanged();
    void widthChanged();
    void heightChanged();
    void objChanged();
	void state_changed();

protected:
    QObject* obj;
};
class MyFactory: public QWebPluginFactory
{
public:
    MyFactory(QObject * parent = 0)
        : QWebPluginFactory(parent)
    {

    }
    virtual QObject * create ( const QString & mimeType, const QUrl & url, const QStringList & argumentNames, const QStringList & argumentValues ) const;
    virtual QList<Plugin>	plugins () const
    {
        return  QList<Plugin>();
    }
};
class MyWebView : public QWebView
{
    virtual void contextMenuEvent ( QContextMenuEvent *)
    {
    }
    virtual void dropEvent ( QDropEvent * event );
public:
    MyWebView(QWidget*parent=NULL):QWebView(parent)
    {
		MyFactory* f = new MyFactory(this);
		page()->settings()->setAttribute(QWebSettings::PluginsEnabled, true);
		page()->setPluginFactory(f);

    }
};
class ObjQWebView: public MyObject
{
    Q_OBJECT;
    Q_PROPERTY(QWebView* webView READ webview);
public:
    ObjQWebView(QObject* parent=NULL);
    QWebView* webview()
    {
        return static_cast<QWebView*>(obj);
    }
public slots:
    void on_javaScriptWindowObjectCleared();
    void printPreview();
    void printPDF(QString filename);
    bool print();
    bool choosePrinter();
    void printToRecentUsedPrinter();
    void printFrameRequest(QWebFrame*);
private:
    QSharedPointer<QPrintDialog> printDialog;
};

QWidget* getTopWidget();
