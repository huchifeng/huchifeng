#pragma once

#include <QtGui/QApplication>

class MyByteArray: public QObject
{
    // 返回 QByteArray,QVariant 内存需要一定时间才回收，而且有可能异常
    // 使用 Object 封装，可以自行 deleteLater
    Q_OBJECT();
public:
    QByteArray bytes;
    MyByteArray(QObject* parent): QObject(parent)
    {

    }
    ~MyByteArray();

public slots:
    QString fromUtf8(){ return QString::fromUtf8(bytes); }
    QString fromPercentEncodingUtf8();
    int byteSize(){ return bytes.size(); }
    QString toHex(){ return bytes.toHex(); }
    bool saveBinaryFile(QString path);
    void appendUtf8(QString str);
    bool appendFromBinaryFile(QString path);
    QObject* clone();
};

class MyApp : public QApplication
{
	Q_OBJECT;
	//Q_PROPERTY(QWidget* qmlView READ qmlView);
public:
	MyApp(int argc, char** argv):QApplication(argc, argv){
	}
	public slots:
		void alert(QString s);
		QObject* utf8( QString s );
};
