#include "myapp.h"
#include <QMessageBox>
#include <QFile>
#include <QDebug>

void MyApp::alert(QString s){
	QMessageBox::warning(NULL, " ", s);
}

MyByteArray::~MyByteArray()
{
	qDebug() << "~MyByteArray";
}
bool MyByteArray::saveBinaryFile(QString path)
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
QString MyByteArray::fromPercentEncodingUtf8()
{
    return QString::fromUtf8(QByteArray::fromPercentEncoding(bytes));
    //return QUrl::fromEncoded(bytes, QUrl::TolerantMode).toString();
    // »¹ÊÇ²»Ìæ»» "+"
}



void MyByteArray::appendUtf8( QString str )
{
    bytes.append(str);
}

bool MyByteArray::appendFromBinaryFile(QString path)
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

QObject* MyByteArray::clone()
{
    MyByteArray* obj = new MyByteArray(NULL);
    obj->bytes = bytes;
    return obj;
}
QObject* MyApp::utf8( QString s )
{
    MyByteArray* ret=new MyByteArray(this); // make sure obj has parent, so qscript not to del it
    ret->bytes=s.toUtf8();
    return ret;
}

