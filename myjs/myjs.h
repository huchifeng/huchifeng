#ifndef MYJS_H
#define MYJS_H

#undef qApp
#define qApp (static_cast<MyApp *>(QCoreApplication::instance()))

#define QtInfoMsg ((QtMsgType)100) 
#define qInfo() QDebug(QtInfoMsg)
#define qFatal() QDebug(QtFatalMsg)

typedef void (*void_func)();
void register_init_func(void_func f);

class MyVariantObject: public QObject
{
	// use Object for deleteLater, not depends on garbage collection
	Q_OBJECT();
public:
	QVariant variant;
	QByteArray bytes;
	MyVariantObject(QObject* parent): QObject(parent)
	{

	}
	~MyVariantObject();
	public slots:
		QString fromUtf8(){ return QString::fromUtf8(bytes); }
		int byteSize(){ return bytes.size(); }
		QString toHex(){ return bytes.toHex(); }
		bool saveBinaryFile(QString path);
		void appendUtf8(QString str);
		bool appendFromBinaryFile(QString path);
        int get(int i){ return i<0 || i>=bytes.size() ? 0 : (quint8)bytes[i]; }
        QString substr(int i, int n){ return QString::fromUtf8(bytes.mid(i, n)); }
};

class MyFile : public QFile
{
	// maybe very big binary file, not fit for memory
	Q_OBJECT;
public:
	MyFile(QObject* parent): QFile(parent)
	{

	}
public slots:
	qint64 size(){ return this->QFile::size(); }
	qint64 get_i64(qint64 p);
	int get_u8(qint64 p);
	QString get_utf8(qint64 p, int n);
	QString get_hex(qint64 p, int n);
};

class MyProcess : public QProcess
{
	// maybe very big binary file, not fit for memory
	Q_OBJECT;
public:
	MyProcess(QObject* parent): QProcess(parent)
	{

	}
	public slots:
		bool waitForReadyRead(int msecs);
		QString readLine(qint64 max, bool utf8);
		void write(QString s, bool utf8);
		int state();
		void closeWriteChannel();
		bool atEnd();
		void close();
};

class MyPen : public QObject
{
    Q_OBJECT;
public:
    QPen p;
    MyPen(QColor c) : p(c)
    {
    }
};
class MyBrush : public QObject
{
    Q_OBJECT;
public:
    QBrush p;
    MyBrush(QColor c) : p(c)
    {
    }
};
class MyImage: public QObject
{
    Q_OBJECT;
    Q_PROPERTY(int width READ width);
    Q_PROPERTY(int height READ height)
public:
    QImage img;
    int width(){ return img.width(); }
    int height(){ return img.height(); }
public slots:
    int getPixel(int x, int y){
        return img.pixel(x, y);
    }
    void rotate();
    void flipX();
    void flipY();
    void negative();
    void inverse();
    void laplace();
    bool save(QString filename);
	QObject* createPainter();
};

class MyPainter: public QObject
{
    Q_OBJECT;
public:
    QPainter *p;
	MyPainter(QPaintDevice* pd)
	{
		p = new QPainter(pd);
	}
	~MyPainter()
	{
		delete p;
		p = NULL;
	}
public slots:
    void drawEllipse(int x, int y, int w, int h){ p->drawEllipse(QRect(x, y, w, h)); }
    void drawText(int x, int y, QString s){ p->drawText(x, y, s); } // left bottom
    void drawText_Flag(int x, int y, int w, int h, QString s, int flags)
    {
        QRect rc;
        p->drawText(x, y, w, h, flags, s, &rc);
    }
    int drawText(int x, int y, int w, int h, QString s)
    {
        QRect rc;
        p->drawText(x, y, w, h, 0, s, &rc);
        return rc.width();
    }
    void setPen(QObject* o){ p->setPen(qobject_cast<MyPen*>(o)->p);  }
    void fillRect(int x, int y, int w, int h, QObject* br)
    {
        p->fillRect(QRectF(x, y, w, h), qobject_cast<MyBrush*>(br)->p);
    }
    void drawRect(int x, int y, int w, int h)
    {
        p->drawRect(QRectF(x, y, w, h));
    }
    void drawLine(int x1, int y1, int x2, int y2)
    {
        p->drawLine(x1, y1, x2, y2);
    }
    void drawImage(int x, int y, int w, int h, QObject* img)
    {
        p->drawImage(QRectF(x,y,w,h), qobject_cast<MyImage*>(img)->img);
    }
    void setRenderHint_SmoothPixmapTransform(bool b)
    {
        p->setRenderHint(QPainter::SmoothPixmapTransform, b);
    }
};

class ObjQWheelEvent: public QObject
{
    Q_OBJECT();
    Q_PROPERTY(int delta READ delta);
    Q_PROPERTY(int x READ x);
    Q_PROPERTY(int y READ y);
public:
    QWheelEvent* p;
    int delta(){ return p->delta(); }
    int x(){ return p->x(); }
    int y(){ return p->y(); }
};
class ObjQMouseEvent: public QObject
{
    Q_OBJECT();
    Q_PROPERTY(int x READ x);
    Q_PROPERTY(int y READ y);
    Q_PROPERTY(QPoint globalPos READ globalPos);
    Q_PROPERTY(int modifiers READ modifiers);
    Q_PROPERTY(int buttons READ buttons);
public:
    QMouseEvent* p;
    int x(){ return p->x(); }
    int y(){ return p->y(); }
    QPoint globalPos(){ return p->globalPos(); }
    int modifiers(){ return p->modifiers(); }
    int buttons(){ return p->buttons(); }
};
class ObjQKeyEvent: public QObject
{
    Q_OBJECT();
    Q_PROPERTY(QString desc READ desc);
public:
    QKeyEvent* p;
    QString desc();
};


class MyMainWindow: public QMainWindow
{
    Q_OBJECT;
public:
    MyMainWindow( Qt::WindowFlags flags = 0): QMainWindow(NULL, flags)
    {

    }
    virtual void paintEvent ( QPaintEvent * event )
    {
        MyPainter ev(this);
        paint(&ev);
        //QMainWindow::paintEvent(event);
    }
    virtual void keyPressEvent( QKeyEvent * event )
    {
        // TAB ÊÕ²»µ½
        ObjQKeyEvent evt;
        evt.p = event;
        emit keyPressEvent(&evt);
    }
    void mousePressEvent ( QMouseEvent * e )
    {
        ObjQMouseEvent evt;
        evt.p = e;
        emit mousePressEvent(&evt);
    }
    void mouseMoveEvent ( QMouseEvent * e )
    {
        ObjQMouseEvent evt;
        evt.p = e;
        emit mouseMoveEvent(&evt);
    }
    void mouseReleaseEvent( QMouseEvent * e )
    {
        ObjQMouseEvent evt;
        evt.p = e;
        emit mouseReleaseEvent(&evt);
    }
    void wheelEvent( QWheelEvent * e )
    {
        ObjQWheelEvent evt;
        evt.p = e;
        emit wheelEvent(&evt);
    }
    void focusOutEvent ( QFocusEvent * event );
public slots:
    void setCentralWidget(QObject* w);
    void updateRect(int x, int y, int w, int h){
        update(x, y, w, h);
    }
    void move(int x, int y){ QWidget::move(x, y); }
    void resize(int w, int h){ QWidget::resize(w, h); }
signals:
    void paint(QObject*);
    void keyPressEvent(QObject*);
    void wheelEvent(QObject* event);
    void mousePressEvent(QObject* event);
    void mouseMoveEvent(QObject* event);
    void mouseReleaseEvent(QObject* event);
    void focusOutEvent();
};

class MyConsole: public QObject
{
	Q_OBJECT();

	HANDLE hOut;
public:
	MyConsole();
	public slots:
		void put(QString s);
		void moveto(int x, int y);
		void clearLine();
		void attr(int fg, int bg);
		int getch();
		QString gets();
};

class MyApp : public QApplication
{
    Q_OBJECT;
	Q_PROPERTY(QObject* console READ getConsole);

private:
	MyConsole* console;
public:
    MyApp(int argc, char* argv[]);
    QScriptEngine engine;
	QVariantMap v; // share data between multi html pages or js-engines
	QObject* getConsole();
   
public slots:
	QVariant getData(QString key){
		return v[key];
	}
	void setData(QString key, QVariant v1){
		v[key] = v1;
	}
    void alert(QString s);
    void exec(){ this->QApplication::exec(); }
    QString applicationDirPath(){ return QCoreApplication::applicationDirPath(); }

    void eval(QString s);
    
    QStringList GetLogicalDrives();
    QVariantMap GetDiskFreeSpaceEx(QString drive);

    QWidget* createMainWindow();
    QWidget* createMainWindow_FramelessTransparent();
    QRect rect(int x, int y, int w, int h){ return QRect(x, y, w, h); }

    QObject* createPen(int r, int g, int b, int a){ return new MyPen(QColor(r, g, b, a)); }
    QObject* createBrush(int r, int g, int b, int a){ return new MyBrush(QColor(r, g, b, a)); }

    void log(QString s);
	void print(QString s);
	void print_err(QString s);
	void echo(QString s); // ends with \n
	QString gets(int max, bool utf8);
	bool stdinAtEnd();

    QStringList dir(QString path);
    QVariantMap fileInfo(QString path);

    QString loadTextFile(QString local);
    bool saveTextFile(QString local, QString txt);

	QObject* loadBinaryFile(QString local);
    QObject* utf8(QString s);

	QObject* openBinaryFile(QString local);

	QObject* singleShot(int msec);
    QObject* createTimer(int msec);
    QString createUuid();
    QObject* createProcess(QString commandline);

	int executeProcess(QString commandline);
    int getPID(QObject*);

    QObject* loadImage(QString s);
    void setClipboardText(QString s);
    QString getClipboardText();

    QString hex(int n, int width);

    QObject* createMetaTypeObject(QString name);
    QString formatDateTime(QDateTime dt, QString format);

    QStringList argv();

	void sleep(int ms);
	int GetAsyncKeyState(int vkey);
	void beep(int freq, int duration);

	bool _debug();
	void move(QObject* widget, int x, int y);
	void resize(QObject* widget, int w, int h);

	QWidget* createDeclarativeView();

	QWidget* createWebView();

	QWidget* createGLWidget();

	QObject* loadLibrary(QString path); // cannot name "import"

	QObject* createSqlite();
	QObject* createSqlite2();

signals:
	void init();
};

class MyWebView : public QWebView
{
    Q_OBJECT;
    virtual void	contextMenuEvent ( QContextMenuEvent *)
    {
    }
public:
    MyWebView(QObject* parent=NULL);
public slots:
    void on_javaScriptWindowObjectCleared();
    void printPreview();
    void printPDF(QString filename);
    void print();
    void load(QString url);
	void setHtml(QString html, QString baseUrl);
};

class FramelessDialog : public QDialog
{
    Q_OBJECT;
public:
    FramelessDialog(const FramelessDialog&){}
    FramelessDialog(QObject* parent=NULL);

public slots:

};

class MyDeclarativeView: public QDeclarativeView
{
	Q_OBJECT;
public:
	MyDeclarativeView(QWidget* parent=NULL) : QDeclarativeView(parent)
	{
	}

	public slots:
		void setSourceUrl(QString url);
		void setResizeMode(int mode);
		void setContextProperty(QString name, QObject* obj);
};

class MyLibrary: public QLibrary
{
	Q_OBJECT;
public:
	MyLibrary(QString path, QObject* parent=NULL) : QLibrary(path, parent)
	{
	}

	public slots:
		QObject* create(QString name);
};


#endif // MYJS_H
