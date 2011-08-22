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
#include "myjs-sqlite.h"


#ifdef _DEBUG
#pragma comment(lib, "QtSqld4.lib")
#else
#pragma comment(lib, "QtSql4.lib")
#endif

QObject* MyApp::createSqlite()
{
	return new MySqlite();
}
QObject* MyApp::createSqlite2()
{
	MySqlite* ret = new MySqlite();
	ret->type = "QSQLITE2";
	return ret;
}

bool MySqlite::open(QString filename, QString connectionName){
	close();
	db = new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE", "test"));
	db->setDatabaseName(filename);
	return db->open();
}

MySqlite::MySqlite()
{
	db = NULL;
	type = "QSQLITE";
}

MySqlite::~MySqlite()
{
	close();
}

void MySqlite::close()
{
	if(!db)
		return;
	//qDebug() << "close" << db->connectionName();
	delete db;
	db = NULL;
	//QSqlDatabase::removeDatabase(db.connectionName());
}

QObject* MySqlite::exec( QString query )
{
	if(!db)
		return NULL;
	return new MyQuery(db->exec(query));
}

QObject* MySqlite::prepare( QString query )
{
	if(!db)
		return NULL;
	QSqlQuery q("", *db);
	q.prepare(query);
	return new MyQuery(q);
}

bool MySqlite::transaction()
{
	if(!db)
		return false;
	return db->transaction();
}

bool MySqlite::commit()
{
	if(!db)
		return false;
	return db->commit();
}

bool MySqlite::rollback()
{
	if(!db)
		return false;
	return db->rollback();
}