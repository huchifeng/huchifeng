#pragma once

#include "myjs.h"
#include <QtSql\QtSql>


class MySqlite: public QObject{
	Q_OBJECT();
public:
	MySqlite();
	~MySqlite();
	QString type;
private:
	QSqlDatabase* db;

	public slots:
		bool open(QString filename, QString connectionName);
		QObject* exec(QString query);
		QObject* prepare(QString query);
		void close();
		bool transaction();
		bool commit();
		bool rollback();
signals:
};

class MyQuery: public QObject{
	Q_OBJECT();
public:
	MyQuery(QSqlQuery q) : q(q)
	{
	}
	~MyQuery()
	{
	}
private:
	QSqlQuery q;

	public slots:
		bool isActive(){ return q.isActive(); }
		QString lastErrorText(){ return q.lastError().text(); }
		int size(){ return q.size(); }
		bool isValid(){ return q.isValid(); }
		bool isSelect(){ return q.isSelect(); }
		bool first(){ return q.first(); }
		QVariant value(int i){ return q.value(i); }
		bool next(){ return q.next(); }
		int numRowsAffected(){ return q.numRowsAffected(); }
		QVariantMap boundValues(){ return q.boundValues(); }
		int fieldCount(){ return q.record().count(); }
		QString fieldName(int i){ return q.record().fieldName(i); }
		void bindValue(int i, QVariant v){ q.bindValue(i, v); }
		void bindValue_byName(QString n, QVariant v){ q.bindValue(n, v); }
		bool exec(){ return q.exec(); }
		void addBindValue(QVariant v){ q.addBindValue(v); }
		bool execBatch(){ return q.execBatch(QSqlQuery::ValuesAsRows); }
		// ValuesAsColumns is a special case for the Oracle driver
		void finish(){ q.finish(); }
		int fieldType(int i){ return q.record().field(i).type(); }
		QVariant lastInsertId(){ return q.lastInsertId(); }
signals:
};
