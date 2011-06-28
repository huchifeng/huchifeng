/*

mygit: the simplest storage layer for bakup and version control
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

#ifdef _DEBUG
#pragma comment(lib, "qtmaind.lib")
#pragma comment(lib, "QtCored4.lib")
#pragma comment(lib, "QtGuid4.lib")
#pragma comment(lib, "QtNetworkd4.lib")
#pragma comment(lib, "QtScriptd4.lib")
#pragma comment(lib, "QtWebKitd4.lib")
#else
#pragma comment(lib, "qtmain.lib")
#pragma comment(lib, "QtCore4.lib")
#pragma comment(lib, "QtGui4.lib")
#pragma comment(lib, "QtNetwork4.lib")
#pragma comment(lib, "QtScript4.lib")
#pragma comment(lib, "QtWebKit4.lib")
#endif

#define BEGIN_WITH(a, b) (strncmp(a,b,strlen(b))==0)

struct t_entry{
	qint64 id;
	QByteArray hash;
	QByteArray type;
	qint64 bytes_size;
	qint64 bytes_offset;

	t_entry(){
		id = 0;
		bytes_size = 0;
		bytes_offset = 0;
	}
};
struct t_db{
	QFile file;
	QMap<qint64, t_entry> map_id_to_entry;
	QMap<QByteArray, QVector<qint64> > map_hash_to_id;
	qint64 max_id;
	t_db(){
		max_id = 0;
	}
};
int mygit_get_entry(t_db& db, t_entry& e){
	{
		int n = db.file.read((char*)&e.id, sizeof(e.id));
		if(n == 0){
			qDebug() << "open db fail";
			return 1;
		}
		if(n != sizeof(e.id) || e.id==0){
			//  || e.id <= 0
			qDebug() << "wrong db";
			return 2;
		}
	}
	{
		unsigned char len = 0;
		int n = db.file.read((char*)&len, sizeof(len));
		if(n != sizeof(len) || len<=0){
			qDebug() << "wrong db";
			return 2;
		}
		e.type.resize(len);
		if(db.file.read(e.type.data(), len)!=len){
			qDebug() << "wrong db";
			return 2;
		}
	}
	{
		unsigned char len=0;
		int n = db.file.read((char*)&len, sizeof(len));
		if(n != sizeof(len) || len<=0 ){
			qDebug() << "wrong db";
			return 2;
		}
		e.hash.resize(len);
		if(db.file.read(e.hash.data(), len)!=len){
			qDebug() << "wrong db";
			return 2;
		}
	}
	{
		//qDebug() << e.hash.toHex();
		int n = db.file.read((char*)&e.bytes_size, sizeof(e.bytes_size));
		if(n != sizeof(e.bytes_size) || e.bytes_size<=0 ){
			qDebug() << "wrong db";
			return 2;
		}
		e.bytes_offset = db.file.pos(); // after the size field
		if(! db.file.seek(e.bytes_offset + e.bytes_size)){
			qDebug() << "wrong db";
			return 2;
		}
		db.map_id_to_entry[e.id] = e;
		db.map_hash_to_id[e.hash].push_back(e.id);
		db.max_id = qMax(db.max_id, e.id);
		return 0;
	}
}
int get_hash(QFile& s, QByteArray& hash_result){
	qint64 old_pos = s.pos();
	QCryptographicHash hash(QCryptographicHash::Sha1);
	qint64 s_size = s.size();
	qint64 check_size = 0;
	char buf[100];
	while(1){
		int n = s.read(buf, sizeof(buf));
		check_size += n;
		//std::cerr << "got:" << n << std::endl;
		if(n > 0)
			hash.addData(buf, n);
		if(n < 0)
			return 11;
		if(n == 0)
			break;
	}
	if(check_size != s_size){
		return 12;
	}
	if(!s.seek(old_pos))
		return 1;
	hash_result = hash.result();
	return 0;
}
int file_append(QFile& d, QFile& s){
	s.seek(0);
	qint64 check_size = 0;
	char buf[100];
	while(1){
		int n = s.read(buf, sizeof(buf));
		check_size += n;
		if(n > 0)
			d.write(buf, n);
		if(n <= 0)
			break;
	}
	if(check_size != s.size()){
		return 11;
	}
	return 0;
}
int file_from(QFile& d, qint64 size, QFile& s){
	s.seek(0);
	char buf[100];
	while(size > 0){
		qint64 to_write = qMin(size, (qint64)sizeof(buf));
		int n = d.read(buf, to_write);
		if(n <= 0)
			return 30;
		if(n > 0)
			s.write(buf, n);
		size -= n;
	}
	return size==0 ? 0 : 2;
}
int file_cmp(QFile& d, QFile& s, qint64 size, int& cmp_res){
	cmp_res = 0;
	char buf[100];
	char buf2[100];
	while(size > 0){
		qint64 to_read = qMin(size, (qint64)sizeof(buf));
		int n = d.read(buf, to_read);
		int n2 = s.read(buf2, to_read);
		if(n <= 0 || n2 <= 0 || n != n2 || n>size){
			return 10;
		}
		size -= n;
		int res = memcmp(buf, buf2, n);
		if(res!=0)
		{
			cmp_res = res;
			return 0;
		}
	}
	return 0;
}

int mygit_add_file(QString db_filename, QByteArray type, QString file){
	if(type.size()>255)
		return 100;
	t_db db;
	db.file.setFileName(db_filename);
	if(!db.file.open(QIODevice::ReadWrite)){
		// will create file if not exists
		return 1;
	}
	while(db.file.pos() != db.file.size()){
		t_entry e;
		if(mygit_get_entry(db, e)!=0)
			return 2;
	}

	QFile input(file);
	if(!input.open(QIODevice::ReadOnly)){
		return 10;
	}
	t_entry e1;
	db.max_id ++;
	e1.id = db.max_id;
	e1.type = type;
	if(get_hash(input, e1.hash)!=0){
		return 3;
	}
	std::cerr << e1.hash.toHex().constData() << std::endl;
	QVector<qint64> same_hash = db.map_hash_to_id[e1.hash];
	if(same_hash.size()>0){
		for(int i=0; i<same_hash.size(); i++){
			qint64 id = same_hash[i];
			t_entry e2 = db.map_id_to_entry[id];
			qDebug() << "same hash, checking" << e2.id;
			if(e2.bytes_size != input.size()){
				qDebug() << "same hash, different size";
				continue;
			}
			db.file.seek(e2.bytes_offset);
			input.seek(0);
			int cmp_res = 0;
			if(file_cmp(db.file, input, e2.bytes_size, cmp_res)!=0){
				return 11;
			}
			if(cmp_res == 0){
				qDebug() << "same hash, same content";
				std::cout << e2.id;
				return 0;
			}
		}
	}

	qint64 old_pos = db.file.size();
	db.file.seek(old_pos);
	db.file.write((char*)&e1.id, sizeof(e1.id));
	{
		unsigned char len = e1.type.size();
		db.file.write((char*)&len, sizeof(len));
		db.file.write(e1.type.constData(), e1.type.size());
	}
	{
		unsigned char len = e1.hash.size();
		db.file.write((char*)&len, sizeof(len));
		db.file.write(e1.hash.constData(), e1.hash.size());
	}
	qint64 input_size = input.size();
	db.file.write((char*)&input_size, sizeof(input_size));
	if(file_append(db.file, input) != 0){
		db.file.resize(old_pos);
		return 4;
	}
	std::cout << e1.id;
	return 0;
}

int mygit_add(QString db_filename, QByteArray type, QByteArray content){
	if(type.size()>255)
		return 100;
	t_db db;
	db.file.setFileName(db_filename);
	if(!db.file.open(QIODevice::ReadWrite)){
		// will create file if not exists
		return 1;
	}
	while(db.file.pos() != db.file.size()){
		t_entry e;
		if(mygit_get_entry(db, e)!=0)
			return 2;
	}

	t_entry e1;
	db.max_id ++;
	e1.id = db.max_id;
	e1.type = type;
	{
		QCryptographicHash hash(QCryptographicHash::Sha1);
		hash.addData(content.constData(), content.size());
		e1.hash = hash.result();
	}
	std::cerr << e1.hash.toHex().constData() << std::endl;
	QVector<qint64> same_hash = db.map_hash_to_id[e1.hash];
	if(same_hash.size()>0){
		for(int i=0; i<same_hash.size(); i++){
			qint64 id = same_hash[i];
			t_entry e2 = db.map_id_to_entry[id];
			qDebug() << "same hash, checking" << e2.id;
			if(e2.bytes_size != content.size()){
				qDebug() << "same hash, different size";
				continue;
			}
			db.file.seek(e2.bytes_offset);
			QByteArray tmp;
			tmp.resize(e2.bytes_size);
			db.file.read(tmp.data(), tmp.size());
			int cmp_res = 0;
			if(memcmp(tmp.constData(), content.constData(), tmp.size())==0){
				qDebug() << "same hash, same content";
				std::cout << e2.id;
				return 0;
			}
		}
	}

	qint64 old_pos = db.file.size();
	db.file.seek(old_pos);
	db.file.write((char*)&e1.id, sizeof(e1.id));
	{
		unsigned char len = e1.type.size();
		db.file.write((char*)&len, sizeof(len));
		db.file.write(e1.type.constData(), e1.type.size());
	}
	{
		unsigned char len = e1.hash.size();
		db.file.write((char*)&len, sizeof(len));
		db.file.write(e1.hash.constData(), e1.hash.size());
	}
	qint64 input_size = content.size();
	db.file.write((char*)&input_size, sizeof(input_size));
	if(db.file.write(content.constData(), content.size()) != input_size){
		db.file.resize(old_pos);
		return 4;
	}
	std::cout << e1.id;
	return 0;
}
std::ostream& operator<<(std::ostream& o, QByteArray b)
{
	o << b.constData();
	return o;
}
std::ostream& operator<<(std::ostream& o, QString b)
{
	o << b.toLocal8Bit();
	return o;
}
int mygit_dir(QString db_filename){
	t_db db;
	db.file.setFileName(db_filename);
	if(!db.file.open(QIODevice::ReadWrite)){
		// will create file if not exists
		return 1;
	}
	while(db.file.pos() != db.file.size()){
		t_entry e;
		if(mygit_get_entry(db, e)!=0)
			return 2;
		std::cout << e.id << " " << e.type 
			<< " " << e.hash.toHex()
			<<  " " << e.bytes_size << std::endl;
	}
	return 0;
}
int mygit_get_type(QString db_filename, QByteArray type){
	t_db db;
	db.file.setFileName(db_filename);
	if(!db.file.open(QIODevice::ReadWrite)){
		// will create file if not exists
		return 1;
	}
	while(db.file.pos() != db.file.size()){
		t_entry e;
		if(mygit_get_entry(db, e)!=0)
			return 2;
		if(e.type == type){
			db.file.seek(e.bytes_offset);
			QByteArray tmp;
			tmp.resize(e.bytes_size);
			db.file.read(tmp.data(), tmp.size());
			std::cout << tmp.constData();
			std::cout << std::endl; // for multiple records
			db.file.seek(e.bytes_offset + e.bytes_size);
		}
	}
	return 0;
}
int mygit_get(QString db_filename, QByteArray s_id){
	bool ok;
	qint64 id = s_id.toLongLong(&ok);
	if(!ok)
		return 1;
	t_db db;
	db.file.setFileName(db_filename);
	if(!db.file.open(QIODevice::ReadWrite)){
		// will create file if not exists
		return 1;
	}
	while(db.file.pos() != db.file.size()){
		t_entry e;
		if(mygit_get_entry(db, e)!=0)
			return 2;
		if(e.id == id){
			db.file.seek(e.bytes_offset);
			QByteArray tmp;
			tmp.resize(e.bytes_size);
			db.file.read(tmp.data(), tmp.size());
			std::cout << tmp.constData();
			return 0;
		}
	}
	return 10;
}

int mygit_get_file(QString db_filename, QString s_id, QString file){
	bool ok;
	qint64 id = s_id.toLongLong(&ok);
	if(!ok)
		return 1;
	t_db db;
	db.file.setFileName(db_filename);
	if(!db.file.open(QIODevice::ReadWrite)){
		// will create file if not exists
		return 1;
	}
	while(db.file.pos() != db.file.size()){
		t_entry e;
		if(mygit_get_entry(db, e)!=0)
			return 2;
	}
	if(! db.map_id_to_entry.contains(id)){
		return 2;
	}
	t_entry e = db.map_id_to_entry[id];

	QFile s(file);
	if(!s.open(QIODevice::WriteOnly)){
		return 10;
	}

	db.file.seek(e.bytes_offset);
	return file_from(db.file, e.bytes_size, s);

}

int mygit_del(QString db_filename, QByteArray s_id){
	bool ok;
	qint64 id = s_id.toLongLong(&ok);
	if(!ok){
		qDebug() << "need integer:" << s_id;
		return 1;
	}
	t_db db;
	db.file.setFileName(db_filename);
	if(!db.file.open(QIODevice::ReadWrite)){
		// will create file if not exists
		return 1;
	}
	while(db.file.pos() != db.file.size()){
		t_entry e;
		qint64 pos = db.file.pos();
		if(mygit_get_entry(db, e)!=0)
			return 2;
		if(e.id == id){
			db.file.seek(pos);
			id = -id;
			db.file.write((char*)&id, sizeof(id));
			return 0;
		}
	}
	qDebug() << id << "not found";
	return 100;
}

int mygit_pack(QString db_filename){
	t_db db;
	db.file.setFileName(db_filename);
	if(!db.file.open(QIODevice::ReadWrite)){
		// will create file if not exists
		return 1;
	}
	qint64 write_pos = 0;
	while(db.file.pos() != db.file.size()){
		t_entry e;
		qint64 pos = db.file.pos();
		if(mygit_get_entry(db, e)!=0)
			return 2;
		qint64 pos2 = e.bytes_offset + e.bytes_size;
		if(e.id < 0){
			continue;
		}
		if(write_pos < pos){
			char buf[100];
			for(qint64 n = pos2 - pos; n>0;){
				qint64 to_write = qMin(n, (qint64)sizeof(buf));
				db.file.seek(pos + (pos2-pos - n));
				int done = db.file.read(buf, to_write);
				if(done != to_write){
					qDebug() << "pack fail";
					return 30;
				}
				db.file.seek(write_pos + (pos2-pos - n));
				db.file.write(buf, to_write);
				n -= to_write;
			}
		}
		write_pos += pos2 - pos;
		db.file.seek(pos2);
	}
	db.file.resize(write_pos);
	return 0;
}
int mygit_hash(QByteArray filename){
	QFile input(filename);
	if(!input.open(QIODevice::ReadOnly)){
		return 10;
	}
	QByteArray hash;
	if(get_hash(input, hash)!=0){
		return 3;
	}
	std::cout << hash.toHex().constData() << std::endl;
	return 0;
}
int mygit_hash_value(QByteArray value){
	QCryptographicHash hash(QCryptographicHash::Sha1);
	hash.addData(value.constData(), value.size());
	std::cout <<  hash.result().toHex().constData() << std::endl;
	return 0;
}
int main(int argc, char *argv[]){
    QApplication app(argc, argv); // cannot wchar

	// not allow free order of parameters
	if(argc==8 && strcmp(argv[1], "add")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--type")==0 && strcmp(argv[6], "--value")==0){
		return mygit_add(argv[3], argv[5], argv[7]);
	}
	if(argc==8 && strcmp(argv[1], "add")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--type")==0 && strcmp(argv[6], "--file")==0){
		return mygit_add_file(argv[3], argv[5], QString::fromLocal8Bit(argv[7]));
	}
	if(argc==4 && strcmp(argv[1], "dir")==0 && strcmp(argv[2], "--db")==0){
		return mygit_dir(argv[3]);
	}
	if(argc==6 && strcmp(argv[1], "get")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--id")==0){
		return mygit_get(argv[3], argv[5]);
	}
	if(argc==6 && strcmp(argv[1], "get")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--type")==0){
		return mygit_get_type(argv[3], argv[5]);
	}
	if(argc==8 && strcmp(argv[1], "get")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--id")==0
		&& strcmp(argv[6], "--file-out")==0
		){
		return mygit_get_file(argv[3], argv[5], argv[7]);
	}
	if(argc==6 && strcmp(argv[1], "del")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--id")==0){
		return mygit_del(argv[3], argv[5]);
		// set the id to negative
	}
	if(argc==6 && strcmp(argv[1], "undel")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--id")==0){
		return mygit_del(argv[3], "-" + QByteArray(argv[5]));
	}
	if(argc==4 && strcmp(argv[1], "pack")==0 && strcmp(argv[2], "--db")==0){
		return mygit_pack(argv[3]); // decrease the database file size
	}
	if(argc==4 && strcmp(argv[1], "hash")==0 && strcmp(argv[2], "--file")==0){
		return mygit_hash(argv[3]);
	}
	if(argc==4 && strcmp(argv[1], "hash")==0 && strcmp(argv[2], "--value")==0){
		return mygit_hash_value(argv[3]);
	}

	std::cerr << "usage:" << std::endl
		<< "mygit add --db <filename> --type <type> --value <input-content>" << std::endl
		<< "mygit add --db <filename> --type <type> --file <input-filename>" << std::endl
		<< "mygit dir --db <filename>" << std::endl
		<< "mygit get --db <filename> --id <id>" << std::endl
		<< "mygit get --db <filename> --type <type>" << std::endl
		<< "mygit get --db <filename> --id <int64> --file-out <filename>" << std::endl
		<< "mygit del --db <filename> --id <int64>" << std::endl
		<< "mygit undel --db <filename> --id <int64>" << std::endl
		<< "mygit pack --db <filename>" << std::endl
		<< "mygit hash --file <input-filename>" << std::endl
		;

	return argc==1 ? 0 : 1; //app.exec();
}




