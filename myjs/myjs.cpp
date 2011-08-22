/*

myjs: use javascript to do anything instead of C++.
Copyright (C) 2011 huchifeng

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

CONTACT: huchifeng@gmail.com
HOMEPAGE: https://github.com/huchifeng/mygit

*/

#include "stdafx.h"
#include "myjs.h"

#include <conio.h> // _getch

#ifdef _DEBUG
#pragma comment(lib, "qtmaind.lib")
#pragma comment(lib, "QtCored4.lib")
#pragma comment(lib, "QtGuid4.lib")
#pragma comment(lib, "QtNetworkd4.lib")
#pragma comment(lib, "QtScriptd4.lib")
#pragma comment(lib, "QtWebKitd4.lib")
#pragma comment(lib, "QtDeclaratived4.lib")
#else
#pragma comment(lib, "qtmain.lib")
#pragma comment(lib, "QtCore4.lib")
#pragma comment(lib, "QtGui4.lib")
#pragma comment(lib, "QtNetwork4.lib")
#pragma comment(lib, "QtScript4.lib")
#pragma comment(lib, "QtWebKit4.lib")
#pragma comment(lib, "QtDeclarative4.lib")
#endif

#pragma comment(lib, "Opengl32.lib")

bool MyVariantObject::saveBinaryFile(QString path)
{
	QFile f(path);
	if(!f.open(QIODevice::WriteOnly))
	{
		return false;
	}
	if(f.write(bytes) != bytes.length())
		return false;
	f.close();
	return true;
}

void MyVariantObject::appendUtf8( QString str )
{
	bytes.append(str);
}

bool MyVariantObject::appendFromBinaryFile(QString path)
{
	QFile f(path);
	if(!f.open(QIODevice::ReadOnly))
	{
		return false;
	}
	bytes.append(f.readAll());
	f.close();
	return true;
}

MyVariantObject::~MyVariantObject()
{
	//qDebug() << "~MyVariantObject";
}

qint64 MyFile::get_i64(qint64 p)
{
	if(!this->seek(p))
		return 0;
	qint64 q = 0;
	this->read((char*)&q, sizeof(q));
	return q;
}

int MyFile::get_u8( qint64 p )
{
	if(!this->seek(p))
		return 0;
	quint8 q = 0;
	this->read((char*)&q, sizeof(q));
	return q;
}

QString MyFile::get_utf8( qint64 p, int n )
{
	QByteArray b;
	b.resize(n);
	if(!this->seek(p))
		return "";
	this->read((char*)b.data(), b.size());
	return QString::fromUtf8(b);
}

QString MyFile::get_hex( qint64 p, int n )
{
	QByteArray b;
	b.resize(n);
	if(!this->seek(p))
		return "";
	this->read((char*)b.data(), b.size());
	return b.toHex();
}

bool MyProcess::waitForReadyRead(int msecs)
{
	return QProcess::waitForReadyRead(msecs); 
}

QString MyProcess::readLine(qint64 max, bool utf8)
{
	QByteArray a = QProcess::readLine(max);
	QString r = utf8 ? QString::fromUtf8(a) : QString::fromLocal8Bit(a);
	return r;
}

int MyProcess::state()
{
	return QProcess::state();
}

bool MyProcess::atEnd()
{
	return QProcess::atEnd();
}

void MyProcess::close()
{
	QProcess::close();
}

void MyProcess::write( QString s, bool utf8 )
{
	QByteArray b = utf8 ? s.toUtf8().constData() : s.toLocal8Bit().constData();
	int written = QProcess::write(b);
	assert(written == b.size());
}

void MyProcess::closeWriteChannel()
{
	QProcess::closeWriteChannel();
}
void MyMainWindow::setCentralWidget(QObject* w )
{
	QMainWindow::setCentralWidget(qobject_cast<QWidget*>(w));
}

void MyMainWindow::focusOutEvent( QFocusEvent * event )
{
	emit focusOutEvent();
}
MyApp::MyApp(int argc, char* argv[])
: QApplication(argc, argv)
{
	console = NULL;
}

void MyApp::alert(QString s)
{
	QMessageBox::warning(NULL, " ", s);
}

QString MyApp::loadTextFile(QString local)
{
	QFile f(local);
	if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		return NULL;
	}
	return QTextStream(&f).readAll();
}
QObject* MyApp::loadBinaryFile(QString path)
{
	QFile f(path);
	MyVariantObject* ret = new MyVariantObject(this);
	if(!f.open(QIODevice::ReadOnly))
	{
		return ret; // no return NULL
	}
	ret->bytes = f.readAll();
	f.close();
	return ret;
}
QObject* MyApp::openBinaryFile(QString path)
{
	MyFile* f = new MyFile(this);
	f->setFileName(path);
	if(!f->open(QIODevice::ReadOnly))
	{
		delete f;
		return NULL;
	}
	return f;
}
QObject* MyApp::utf8(QString s)
{
	MyVariantObject* ret=new MyVariantObject(this);
	ret->bytes=s.toUtf8();
	return ret;
}

void MyApp::eval( QString s )
{
	engine.evaluate(s);
}



QWidget* MyApp::createWebView()
{
	MyWebView* ret = new MyWebView;
	return ret;
}



QStringList MyApp::GetLogicalDrives()
{
	QStringList ret;
	quint32 x = ::GetLogicalDrives();
	for(int i=0; i<26; i++)
	{
		bool hasDrive = (x & (1<<i))!=0;
		if(!hasDrive)
		{
			continue;
		}
		ret << QString('A'+i)+":";
	}
	return ret;
}

QVariantMap MyApp::GetDiskFreeSpaceEx( QString drive )
{
	QVariantMap ret;
	qint64 i64FreeBytesToCaller = 0;
	qint64 i64TotalBytes = 0;
	qint64 i64FreeBytes = 0;
	::GetDiskFreeSpaceEx (drive.utf16(),
		(PULARGE_INTEGER)&i64FreeBytesToCaller,
		(PULARGE_INTEGER)&i64TotalBytes,
		(PULARGE_INTEGER)&i64FreeBytes);
	ret["FreeBytesToCaller"] = QString::number(i64FreeBytesToCaller);
	ret["TotalBytes"] = QString::number(i64TotalBytes);
	ret["FreeBytes"] = QString::number(i64FreeBytes);
	return ret;
}

QStringList MyApp::dir( QString path )
{
	QStringList ret;
	//QDirIterator it(path);
	//while(1)
	//{
	//    qDebug() << it.hasNext();
	//    qDebug() << it.next();
	//    //ret.push_back(it.next());
	//}
	//QDir dir(path);
	////ret = dir.entryList();
	//for(int i=0; i<dir.count(); i++)
	//{  // Qdir bug
	//    qDebug()<< dir[i];
	//    ret.push_back(dir[i]);
	//}

	WIN32_FIND_DATA FileData = {0}; 
	HANDLE hSearch = FindFirstFile((path+"/*.*").utf16(), &FileData); 
	if(hSearch)
	{
		while(1)
		{
			if(wcslen(FileData.cFileName)>0
				&& wcscmp(FileData.cFileName, L".")!=0
				&& wcscmp(FileData.cFileName, L"..")!=0)
				ret.push_back(QString::fromUtf16(FileData.cFileName));
			if (!FindNextFile(hSearch, &FileData)) 
			{
				break;
			}
		}
	}
	FindClose(hSearch);
	return ret;
}

QVariantMap MyApp::fileInfo( QString path )
{
	// 到 FTP 的符号连接导致qt不能处理，windows资源管理器删除时也导致问题，dos可以。
	QVariantMap ret;
	if(path.endsWith(".lnk")){
		return ret;
	}
	QFileInfo info(path);
	ret["size"] = info.size();
	ret["isDir"] = info.isDir();
	ret["lastModified"] = info.lastModified();
	ret["suffix"] = info.suffix();
	return ret;
}

bool MyApp::saveTextFile( QString local, QString txt )
{
	QFile f(local);
	if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return false;
	}
	QTextStream(&f) << txt;
	return true;
}

QObject* MyApp::singleShot( int msec )
{
	QTimer* ret = new QTimer;
	ret->setSingleShot(true);
	ret->connect(ret, SIGNAL(timeout()), ret, SLOT(deleteLater()));
	ret->start(msec);
	return ret;
}
QObject* MyApp::createTimer( int msec )
{
	QTimer* ret = new QTimer;
	ret->setSingleShot(false);
	ret->start(msec);
	return ret;
}

QString MyApp::createUuid()
{
	return QUuid::createUuid().toString();
}

QObject* MyApp::createProcess( QString commandline )
{
	MyProcess* process = new MyProcess(this);
	// execute 是阻塞执行 start 不会。
	// 程序退出太快可能进程没有启动， 也可能程序退出时子进程也被退出。
	process->start(commandline);
	// , QIODevice::ReadWrite| QIODevice::Unbuffered
	process->waitForStarted(-1);
	return process;
}
int MyApp::executeProcess( QString commandline )
{
	return QProcess::execute(commandline); 
}

QObject* MyApp::loadImage( QString s )
{
	MyImage* ret = new MyImage;
	ret->img.load(s);
	return ret;
}

void MyApp::setClipboardText( QString s )
{
	QApplication::clipboard()->setText(s);
}

QString MyApp::getClipboardText()
{
	return QApplication::clipboard()->text();
}


QString MyApp::hex( int n, int width )
{
	QString ret = QString::number(n, 16);
	while(ret.size()<width) ret="0"+ret;
	return ret;
}

QWidget* MyApp::createMainWindow_FramelessTransparent()
{
	MyMainWindow* ret = new MyMainWindow(Qt::FramelessWindowHint);
	ret->setAttribute(Qt::WA_TranslucentBackground, true);
	return ret;
}

QString MyApp::formatDateTime( QDateTime dt, QString format )
{
	return dt.toString(format);
}

int MyApp::getPID( QObject* p)
{
	return ((_PROCESS_INFORMATION*)qobject_cast<QProcess*>(p)->pid())->dwProcessId;
}

void MyApp::print( QString s )
{
	std::cout << s.toLocal8Bit().constData();
}
void MyApp::print_err( QString s )
{
	std::cerr << s.toLocal8Bit().constData();
}

void MyApp::log( QString s )
{
	//qDebug() << s;
	std::cerr << s.toLocal8Bit().constData() << std::endl;
}
void MyApp::echo( QString s )
{
	std::cout << s.toLocal8Bit().constData() << std::endl;
}

QObject* MyApp::createMetaTypeObject( QString name )
{
	return static_cast<QObject*>(
		QMetaType::construct(
		QMetaType::type(
		name.toLatin1())));
}

QStringList MyApp::argv()
{
	return QCoreApplication::arguments();
}
void MyApp::sleep( int ms )
{
	::Sleep(ms);
}


int MyApp::GetAsyncKeyState(int vkey)
{
	int s = ::GetAsyncKeyState(vkey);
	return s;
}
void MyApp::beep(int freq, int duration)
{
	::Beep(freq, duration); // freq range 37 through 32,767 (0x25 through 0x7FFF). 
}

QString MyApp::gets(int max, bool utf8)
{
	QByteArray b;
	b.resize(max);
	if(!fgets(b.data(), b.size(), stdin))
		return "";
	return utf8 ? QString::fromUtf8(b.constData())
		: QString::fromLocal8Bit(b.constData());
}

bool MyApp::stdinAtEnd()
{
	return feof(stdin);
}

bool MyApp::_debug()
{
#ifdef _DEBUG
	return true;
#endif
	return false;
}

void MyApp::move( QObject* widget, int x, int y )
{
	qobject_cast<QWidget*>(widget)->move(x, y);
}

void MyApp::resize( QObject* widget, int w, int h )
{
	qobject_cast<QWidget*>(widget)->resize(w, h);
}

QWidget* MyApp::createDeclarativeView()
{
	return new MyDeclarativeView();
}

QObject* MyApp::loadLibrary( QString path )
{
	MyLibrary* lib = new MyLibrary(path, this);
	lib->load();
	return lib;
}

QWidget* MyApp::createMainWindow()
{
	return new MyMainWindow;
}
QObject* MyApp::getConsole()
{
	if(!console){
		console = new MyConsole;
	}
	return console;
}

void MyWebView::printPreview()
{
	QPrintPreviewDialog dlg(this);
	connect(&dlg, SIGNAL(paintRequested(QPrinter*)), this, SLOT(print(QPrinter*)));
	dlg.exec();
}
void MyWebView::printPDF(QString filename)
{
	QPrinter printer(QPrinter::HighResolution);
	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(filename);
	QWebView::print(&printer);
}

void MyWebView::print()
{
	QPrintDialog printDialog(this);
	if (printDialog.exec() == QDialog::Accepted) {
		QWebView::print(printDialog.printer());
	}
}

MyWebView::MyWebView(QObject* parent): QWebView(qobject_cast<QWidget*>(parent))
{
	connect(this->page()->currentFrame(), SIGNAL(javaScriptWindowObjectCleared()),
		this, SLOT(on_javaScriptWindowObjectCleared()));
}

void MyWebView::on_javaScriptWindowObjectCleared()
{
	// 如果使用了 frame，要用 window.top.webview 才能访问到
	this->page()->currentFrame()->addToJavaScriptWindowObject("webview", this);
	this->page()->currentFrame()->addToJavaScriptWindowObject("app", qApp);
	//this->page()->history()->clear();
}
void MyWebView::load(QString url )
{
	QWebView::load(url);
}

void MyWebView::setHtml( QString html, QString baseUrl )
{
	QWebView::setHtml(html, baseUrl);
}
QString GetKeyNameText(QKeyEvent* event)
{
	wchar_t buf[100];
	UINT key = event->nativeScanCode(); 
	//UINT key = event->nativeVirtualKey();
	//key = MapVirtualKey(key, MAPVK_VSC_TO_VK);
	//key = MapVirtualKey(key, MAPVK_VK_TO_VSC);
	key = ((key<<16) | 0x2000000);
	GetKeyNameTextW(key, buf, _countof(buf));
	return QString::fromWCharArray(buf);
}
QString KeyEventName_ScanCode(QKeyEvent* event)
{
	QString keySequence = "";
	keySequence += (event->modifiers() & Qt::ControlModifier) ? "Ctrl+" :"";
	keySequence += (event->modifiers() & Qt::ShiftModifier) ? "Shift+" :"";
	keySequence += (event->modifiers() & Qt::AltModifier) ? "Alt+" :"";
	// QKeySequence(32) 可能得到 "空格" 而不是 "Space"; Enter 没有问题
	// 试验笔记本方向键，PC数字小键盘，放大缩小。
	keySequence += GetKeyNameText(event);
	//keySequence += event->key()==' ' ? "Space" : (QString)QKeySequence(event->key());
	//qDebug() << event->nativeVirtualKey() << event->nativeScanCode();
	return keySequence;
}
QString KeyEventName(QKeyEvent* event)
{
	QString keySequence = "";
	// 去掉一些 text 字符串非空但是又不宜写入配置文件的，
	// 对于 A-Z 为了一律大写,也使用 GetKeyNameText 而不用 text()
	// 对于 + - 方向键，数字键盘等，用text() 较适宜，而不宜用 GetKeyNameText，不宜加 shift
	QString txt = event->key()==Qt::Key_Return
		|| event->key()==Qt::Key_Space
		|| event->key()==Qt::Key_Escape
		|| event->key()==Qt::Key_Backspace
		|| event->key()>=Qt::Key_A && event->key()<=Qt::Key_Z
		? "" : event->text();
	txt = event->key()==Qt::Key_Enter ? "Enter" : txt; // 小键盘是 enter, GetKeyNameText="Num Enter"
	keySequence += (event->modifiers() & Qt::ControlModifier) ? "Ctrl+" :"";
	// 非文字还允许 shift，例如 shift+方向键。A-Z 也允许
	keySequence += txt.length()==0 && (event->modifiers() & Qt::ShiftModifier) ? "Shift+" :"";
	keySequence += (event->modifiers() & Qt::AltModifier) ? "Alt+" :"";
	keySequence += txt.length()==0 ? GetKeyNameText(event) : txt;
	return keySequence;
}

QString ObjQKeyEvent::desc()
{
	return KeyEventName(p);
}
void MyImage::rotate()
{
	QImage dst(img.height(), img.width(), QImage::Format_RGB32);
	quint32* d = (quint32*)dst.bits();
	quint32* s = (quint32*)img.bits();
	for(int i=0, h=img.height(); i<h; i++)
	{
		for(int j=0, w=img.width(); j<w; j++)
		{
			d[(w-1-j)*h+i] = s[i*w+j];
		}
	}
	img = dst;
}

void MyImage::flipX()
{
	quint32* d = (quint32*)img.bits();
	for(int i=0, h=img.height(); i<h; i++)
	{
		for(int j=0, w=img.width(); j<w/2; j++)
		{
			qSwap(d[i*w+(w-1-j)], d[i*w+j]);
		}
	}
}

void MyImage::flipY()
{
	quint32* d = (quint32*)img.bits();
	for(int i=0, h=img.height(); i<h/2; i++)
	{
		for(int j=0, w=img.width(); j<w; j++)
		{
			qSwap(d[i*w+j], d[(h-1-i)*w+j]);
		}
	}
}

bool MyImage::save( QString filename )
{
	return img.save(filename);
}

void MyImage::negative()
{
	quint32* d = (quint32*)img.bits();
	for(int i=0, h=img.height(); i<h; i++)
	{
		for(int j=0, w=img.width(); j<w; j++)
		{
			quint8* x = (quint8*)&d[i*w+j];
			x[0] = 255 - x[0];
			x[1] = 255 - x[1];
			x[2] = 255 - x[2];
		}
	}
}

void MyImage::inverse()
{
	quint32* d = (quint32*)img.bits();
	for(int i=0, h=img.height(); i<h; i++)
	{
		for(int j=0, w=img.width(); j<w; j++)
		{
			quint8* x = (quint8*)&d[i*w+j];
			// 转换一次很光滑 ，转换二次还原效果不佳 
			//int sum = x[0] + x[1] + x[2];
			//if(sum == 0)
			//{
			//    x[0] = 255;
			//    x[1] = 255;
			//    x[2] = 255;
			//    continue;
			//}
			//int sum2 = (765-sum);
			//int b = x[0] * sum2/sum;
			//int g = x[1] * sum2/sum;
			//int r = x[2] * sum2/sum;
			//int maxBgr = qMax(r, qMax(g, b));
			//sum2 = r+g+b;
			//if(maxBgr > 255)
			//{
			//    // sum2*k/3 + maxBgr*(1-k)=255
			//    // k = (maxBgr-255)/(maxBgr - sum2/3);
			//    int over = sum2*(maxBgr-255)/(3*maxBgr - sum2);
			//    r = r * 3 * (maxBgr-255)/(3*maxBgr - sum2) + over;
			//    g = g * 3 * (maxBgr-255)/(3*maxBgr - sum2) + over;
			//    b = b * 3 * (maxBgr-255)/(3*maxBgr - sum2) + over;
			//    x[0] = qMin(255, b);
			//    x[1] = qMin(255, g);
			//    x[2] = qMin(255, r);
			//}
			//else
			//{
			//    x[0] = b;
			//    x[1] = g;
			//    x[2] = r;
			//}
			// 使用 hsv 转换两次，还原效果不佳； 转一次也不光滑。
			int h, s, v;
			QColor(x[2], x[1], x[0]).getHsv(&h, &s, &v);
			int r, g, b;
			QColor::fromHsv(h, s, 255-v).getRgb(&r, &g, &b);
			x[0] = b;
			x[1] = g;
			x[2] = r;
		}
	}
}
#define LAPLACE(x,a,b,c,d) ((4*(x)-(a)-(b)-(c)-(d))/8+128)
#define LINEAR(x, a, b) ((x)<=(a) ? 0 : (x)>=(b) ? 255 : ((x)-(a))*255/((b)-(a)) )
void MyImage::laplace()
{
	QImage src = img;
	quint8* s = (quint8*)src.bits();
	quint8* d = (quint8*)img.bits();
	for(int i=0, h=img.height(); i<h; i++)
	{
		for(int j=0, w=img.width(); j<w; j++)
		{
			for(int k=0; k<3; k++)
				d[(i*w+j)*4+k] = LAPLACE(s[(i*w+j)*4+k], i<=0 ? 0 : s[((i-1)*w+j)*4+k], j<=0 ? 0 : s[((i)*w+(j-1))*4+k], i>=h-1 ? 0 : s[((i+1)*w+j)*4+k], j>=w-1 ? 0 : s[((i)*w+j+1)*4+k]);
			for(int k=0; k<3; k++)
				d[(i*w+j)*4+k] = LINEAR(d[(i*w+j)*4+k], 128-10, 128+10);
			d[(i*w+j)*4+3] = s[(i*w+j)*4+3];
		}
	}
}

QObject* MyImage::createPainter()
{
	return new MyPainter(&img);
}


FramelessDialog::FramelessDialog( QObject* parent/*=NULL*/ ) 
: QDialog((QWidget*)parent, Qt::FramelessWindowHint)
{
	setAttribute(Qt::WA_TranslucentBackground, true);

}

void MyDeclarativeView::setSourceUrl(QString url)
{
	QDeclarativeView::setSource(QUrl(url));
	// qt will report error to console
}

void MyDeclarativeView::setResizeMode( int mode )
{
	QDeclarativeView::setResizeMode((ResizeMode)mode);
}

void MyDeclarativeView::setContextProperty( QString name, QObject* obj )
{
	QDeclarativeView::rootContext()->setContextProperty(name, obj);
}


int main(int argc, char *argv[]){


    MyApp app(argc, argv); // cannot wchar
	QDir::setCurrent(QCoreApplication::applicationDirPath ()); // safer to load files
	app.engine.globalObject().setProperty("app", app.engine.newQObject(&app));

	QByteArray usage;


	// not allow free order of parameters
	usage += "myjs -e <js-code>\n";
	if(argc>=3 && strcmp(argv[1], "--eval")==0){
		app.engine.evaluate(argv[2]);
		if(app.engine.hasUncaughtException())
		{
			qDebug() << "script error";
			return 100;
		}
		return 0;
	}
	if(argc>=2){
		QString s = app.loadTextFile(argv[1]);
		if(s.size() == 0)
		{
			qDebug() << "cannot load file or empty file:" << argv[1];
			return 99;
		}
		app.engine.evaluate(s);
		if(app.engine.hasUncaughtException())
		{
			qDebug() << "script error\n"
				<< "line:" << QString::number(app.engine.uncaughtExceptionLineNumber());
			return 100;
		}
		return 0;
	}

	std::cerr << "usage:" << std::endl
		<< usage.constData();

	return argc==1 ? 0 : 1; //app.exec();
}

QObject* MyLibrary::create(QString name)
{
	// create("DLL-API-name"), instead of "createMetaTypeObject"
	typedef QObject* (*Func)(QObject* parent);
	Func f = (Func)resolve(name.toLocal8Bit().constData());
	return !f ? NULL : f(this);
}

MyConsole::MyConsole()
{
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);	
}
void MyConsole::put(QString s)
{
	std::cerr << s.toAscii().constData();
}
void MyConsole::moveto(int x, int y)
{
	COORD home = {x, y};
	SetConsoleCursorPosition(hOut, home);
}
void MyConsole::clearLine()
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo( hOut, &bInfo ); 
	WORD att = bInfo.wAttributes;
	unsigned long size = (bInfo.dwSize.X - bInfo.dwCursorPosition.X);
	DWORD set;
	FillConsoleOutputAttribute(hOut, att, size, bInfo.dwCursorPosition, &set);
	FillConsoleOutputCharacter(hOut, ' ', size, bInfo.dwCursorPosition, &set);
}
void MyConsole::attr(int fg, int bg)
{
	SetConsoleTextAttribute(hOut, fg|(bg<<4));
}
int MyConsole::getch()
{
	return _getch();
}
QString MyConsole::gets()
{
	char buf[100];
	gets_s(buf, sizeof(buf)-1);
	return QString::fromAscii(buf);
}
