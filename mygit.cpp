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
#include "mygit.h"

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


bool MyGit::get_entry(t_entry& e){
	e.pos = file.pos();
	{
		int n = this->file.read((char*)&e.id, sizeof(e.id));
		if(n == 0){
			qDebug() << "open db fail";
			return false;
		}
		if(n != sizeof(e.id) || e.id==0){
			//  || e.id <= 0
			qDebug() << "wrong db";
			return false;
		}
	}
	{
		unsigned char len = 0;
		int n = this->file.read((char*)&len, sizeof(len));
		if(n != sizeof(len) || len<=0){
			qDebug() << "wrong db";
			return false;
		}
		e.type.resize(len);
		if(this->file.read(e.type.data(), len)!=len){
			qDebug() << "wrong db";
			return false;
		}
	}
	{
		unsigned char len=0;
		int n = this->file.read((char*)&len, sizeof(len));
		if(n != sizeof(len) || len<=0 ){
			qDebug() << "wrong db";
			return false;
		}
		e.hash.resize(len);
		if(this->file.read(e.hash.data(), len)!=len){
			qDebug() << "wrong db";
			return false;
		}
	}
	{
		//qDebug() << e.hash.toHex();
		int n = this->file.read((char*)&e.bytes_size, sizeof(e.bytes_size));
		if(n != sizeof(e.bytes_size) || e.bytes_size<=0 ){
			qDebug() << "wrong db";
			return false;
		}
		e.bytes_offset = this->file.pos(); // after the size field
		if(! this->file.seek(e.bytes_offset + e.bytes_size)){
			qDebug() << "wrong db";
			return false;
		}
		this->map_id_to_entry[e.id] = e;
		this->map_hash_to_id[e.hash].push_back(e.id);
		this->max_id = qMax(this->max_id, e.id);
		return true;
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

qint64 MyGit::add_file(QString type, QString file)
{
	QFile input(file);
	if(!input.open(QIODevice::ReadOnly)){
		return 0;
	}
	t_entry e1;
	if(get_hash(input, e1.hash)!=0){
		return 0;
	}
	std::cerr << e1.hash.toHex().constData() << std::endl;
	QVector<qint64> same_hash = this->map_hash_to_id[e1.hash];
	if(same_hash.size()>0){
		for(int i=0; i<same_hash.size(); i++){
			qint64 id = same_hash[i];
			t_entry e2 = this->map_id_to_entry[id];
			qDebug() << "same hash, checking" << e2.id;
			if(e2.bytes_size != input.size()){
				qDebug() << "same hash, different size";
				continue;
			}
			this->file.seek(e2.bytes_offset);
			input.seek(0);
			int cmp_res = 0;
			if(file_cmp(this->file, input, e2.bytes_size, cmp_res)!=0){
				return 0;
			}
			if(cmp_res == 0){
				qDebug() << "same hash, same content";
				std::cout << e2.id << std::endl;
				return e2.id;
			}
		}
	}

	this->max_id ++;
	e1.id = this->max_id;
	e1.type = type.toUtf8();
	e1.pos = this->file.size();
	this->file.seek(e1.pos);
	this->file.write((char*)&e1.id, sizeof(e1.id));
	{
		unsigned char len = e1.type.size();
		this->file.write((char*)&len, sizeof(len));
		this->file.write(e1.type.constData(), e1.type.size());
	}
	{
		unsigned char len = e1.hash.size();
		this->file.write((char*)&len, sizeof(len));
		this->file.write(e1.hash.constData(), e1.hash.size());
	}
	e1.bytes_size = input.size();
	this->file.write((char*)&e1.bytes_size, sizeof(e1.bytes_size));
	e1.bytes_offset = this->file.pos();
	if(file_append(this->file, input) != 0){
		this->file.resize(e1.pos);
		return 0;
	}
	std::cout << e1.id << std::endl;
	this->map_id_to_entry[e1.id] = e1;
	this->map_hash_to_id[e1.hash].push_back(e1.id);
	this->max_id = qMax(this->max_id, e1.id);
	return e1.id;
}

qint64 MyGit::add_data(QString type1, QString content1 )
{
	QByteArray type = type1.toUtf8();
	QByteArray content = content1.toUtf8();
	t_entry e1;
	{
		QCryptographicHash hash(QCryptographicHash::Sha1);
		hash.addData(content.constData(), content.size());
		e1.hash = hash.result();
	}
	std::cerr << e1.hash.toHex().constData() << std::endl;
	QVector<qint64> same_hash = this->map_hash_to_id[e1.hash];
	if(same_hash.size()>0){
		for(int i=0; i<same_hash.size(); i++){
			qint64 id = same_hash[i];
			t_entry e2 = this->map_id_to_entry[id];
			qDebug() << "same hash, checking" << e2.id;
			if(e2.bytes_size != content.size()){
				qDebug() << "same hash, different size";
				continue;
			}
			this->file.seek(e2.bytes_offset);
			QByteArray tmp;
			tmp.resize(e2.bytes_size);
			this->file.read(tmp.data(), tmp.size());
			int cmp_res = 0;
			if(memcmp(tmp.constData(), content.constData(), tmp.size())==0){
				qDebug() << "same hash, same content";
				std::cout << e2.id << std::endl;
				return e2.id;
			}
		}
	}

	this->max_id ++;
	e1.id = this->max_id;
	e1.type = type;
	e1.pos = this->file.size();
	this->file.seek(e1.pos);
	this->file.write((char*)&e1.id, sizeof(e1.id));
	{
		unsigned char len = e1.type.size();
		this->file.write((char*)&len, sizeof(len));
		this->file.write(e1.type.constData(), e1.type.size());
	}
	{
		unsigned char len = e1.hash.size();
		this->file.write((char*)&len, sizeof(len));
		this->file.write(e1.hash.constData(), e1.hash.size());
	}
	e1.bytes_size = content.size();
	this->file.write((char*)&e1.bytes_size, sizeof(e1.bytes_size));
	e1.bytes_offset = this->file.pos();
	if(this->file.write(content.constData(), content.size()) != e1.bytes_size){
		this->file.resize(e1.pos);
		return 0;
	}
	std::cout << e1.id << std::endl;
	this->map_id_to_entry[e1.id] = e1;
	this->map_hash_to_id[e1.hash].push_back(e1.id);
	this->max_id = qMax(this->max_id, e1.id);
	return e1.id;
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
bool MyGit::cmd_dir(){
	for(QMap<qint64, t_entry>::const_iterator it=map_id_to_entry.constBegin();
		it!=map_id_to_entry.constEnd(); it++)
	{
		const t_entry& e = it.value();
		std::cout << e.id << " " << e.type 
			<< " " << e.hash.toHex()
			<<  " " << e.bytes_size << std::endl;
	}
	return true;
}
bool MyGit::cmd_get_by_type(QByteArray type){
	for(QMap<qint64, t_entry>::const_iterator it=map_id_to_entry.constBegin();
		it!=map_id_to_entry.constEnd(); it++)
	{
		const t_entry& e = it.value();
		if(e.type == type){
			file.seek(e.bytes_offset);
			QByteArray tmp;
			tmp.resize(e.bytes_size);
			file.read(tmp.data(), tmp.size());
			std::cout << tmp.constData();
			std::cout << std::endl; // for multiple records
		}
	}
	return true;
}
bool MyGit::cmd_get_by_id(qint64 id){
	if(!map_id_to_entry.contains(id))
		return false;
	const t_entry& e = map_id_to_entry[id];
	file.seek(e.bytes_offset);
	QByteArray tmp;
	tmp.resize(e.bytes_size);
	file.read(tmp.data(), tmp.size());
	std::cout << tmp.constData();
	return true;
}

bool MyGit::save_by_id_to_file(qint64 id, QString file){
	if(!map_id_to_entry.contains(id))
		return false;
	const t_entry& e = map_id_to_entry[id];

	QFile s(file);
	if(!s.open(QIODevice::WriteOnly)){
		return false;
	}

	this->file.seek(e.bytes_offset);
	return file_from(this->file, e.bytes_size, s)==0 ? true : false;
}

bool MyGit::del_by_id(qint64 id){
	if(!map_id_to_entry.contains(id))
		return false;
	const t_entry& e = map_id_to_entry[id];
	file.seek(e.pos);
	qint64 new_id = -id;
	file.write((char*)&new_id, sizeof(new_id));
	map_id_to_entry[new_id] = e;
	map_id_to_entry.remove(id);
	return true;
}

bool MyGit::pack_db(){
	file.seek(0);
	qint64 write_pos = 0;
	QList<qint64> keys = map_id_to_entry.keys();
	for(int i=keys.size()-1; i>=0; i--){
		assert(keys[i]!=0);
		if(keys[i]<0){
			keys.removeAt(i);
		}
	}
	qSort(keys);
	for(int i=0; i<keys.size(); i++)
	{
		assert(map_id_to_entry.contains(keys[i]));
		const t_entry& e = map_id_to_entry[keys[i]];
		qint64 pos = e.pos;
		qint64 pos2 = e.bytes_offset + e.bytes_size;
		if(write_pos < pos){
			char buf[100];
			for(qint64 n = pos2 - pos; n>0;){
				qint64 to_write = qMin(n, (qint64)sizeof(buf));
				file.seek(pos + (pos2-pos - n));
				int done = file.read(buf, to_write);
				if(done != to_write){
					qDebug() << "pack fail";
					return false;
				}
				file.seek(write_pos + (pos2-pos - n));
				file.write(buf, to_write);
				n -= to_write;
			}
		}
		write_pos += pos2 - pos;
		file.seek(pos2);
	}
	file.resize(write_pos);
	QString filename = file.fileName();
	file.close();
	return open(filename);
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

int main(int argc, char *argv[]){
    QApplication app(argc, argv); // cannot wchar

	QByteArray usage;

	// not allow free order of parameters
	usage += "mygit add --db <filename> --type <type> --value <input-content>\n";
	if(argc==8 && strcmp(argv[1], "add")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--type")==0 && strcmp(argv[6], "--value")==0){
			QByteArray type = argv[5];
			if(type.size()>255)
				return 100;
			QByteArray content = argv[7];
			MyGit db;
			return db.open(argv[3]) && db.add_data(type, content) ? 0 : 100;
	}

	usage += "mygit add --db <filename> --type <type> --file <input-filename>\n";
	if(argc==8 && strcmp(argv[1], "add")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--type")==0 && strcmp(argv[6], "--file")==0){
			QByteArray type = argv[5];
			if(type.size()>255)
				return 100;
			QString file = QString::fromLocal8Bit(argv[7]);
			MyGit db;
			return db.open(argv[3]) && db.add_file(type, file) ? 0 : 100;
	}

	usage += "mygit dir --db <filename>\n";
	if(argc==4 && strcmp(argv[1], "dir")==0 && strcmp(argv[2], "--db")==0){
		MyGit db;
		return db.open(argv[3]) && db.cmd_dir() ? 0 : 100;
	}
	
	usage += "mygit get --db <filename> --id <id>\n";
	if(argc==6 && strcmp(argv[1], "get")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--id")==0){
			bool ok;
			qint64 id = QByteArray(argv[5]).toLongLong(&ok);
			if(!ok)
				return 10;
			MyGit db;
			return db.open(argv[3]) && db.cmd_get_by_id(id) ? 0 : 100;
	}
	
	usage += "mygit get --db <filename> --type <type>\n";
	if(argc==6 && strcmp(argv[1], "get")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--type")==0){
			MyGit db;
			return db.open(argv[3]) && db.cmd_get_by_type(argv[5]) ? 0 : 100;
	}
	
	usage += "mygit get --db <filename> --id <int64> --file-out <filename>\n";
	if(argc==8 && strcmp(argv[1], "get")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--id")==0
		&& strcmp(argv[6], "--file-out")==0
		){
			bool ok;
			qint64 id = QByteArray(argv[5]).toLongLong(&ok);
			if(!ok)
				return 10;
			MyGit db;
			return db.open(argv[3]) && db.save_by_id_to_file(id, argv[7]) ? 0 : 100;
	}
	
	usage += "mygit del --db <filename> --id <int64>\n";
	if(argc==6 && strcmp(argv[1], "del")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--id")==0){
			bool ok;
			qint64 id = QByteArray(argv[5]).toLongLong(&ok);
			if(id<=0 || !ok)
				return 10;
			MyGit db;
			return db.open(argv[3]) && db.del_by_id(id) ? 0 : 100;		
	}
	
	usage += "mygit undel --db <filename> --id <int64>\n";
	if(argc==6 && strcmp(argv[1], "undel")==0 && strcmp(argv[2], "--db")==0
		&& strcmp(argv[4], "--id")==0){
			bool ok;
			qint64 id = QByteArray(argv[5]).toLongLong(&ok);
			if(id<=0 || !ok)
				return 10;
			MyGit db;
			return db.open(argv[3]) && db.del_by_id(-id) ? 0 : 100;		
	}
	
	usage += "mygit pack --db <filename>\n";
	if(argc==4 && strcmp(argv[1], "pack")==0 && strcmp(argv[2], "--db")==0){
		MyGit db;
		return db.open(argv[3]) && db.pack_db() ? 0 : 100;
	}
	
	usage += "mygit hash --file <input-filename>\n";
	if(argc==4 && strcmp(argv[1], "hash")==0 && strcmp(argv[2], "--file")==0){
		return mygit_hash(argv[3]);
	}
	
	usage += "mygit hash --value <input-filename>\n";
	if(argc==4 && strcmp(argv[1], "hash")==0 && strcmp(argv[2], "--value")==0){
		QByteArray content = argv[3];
		QCryptographicHash hash(QCryptographicHash::Sha1);
		hash.addData(content.constData(), content.size());
		std::cout <<  hash.result().toHex().constData() << std::endl;
		return 0;
	}

	std::cerr << "usage:" << std::endl
		<< usage.constData();

	return argc==1 ? 0 : 1; //app.exec();
}


extern "C" _declspec(dllexport) QObject* createMyGit(QObject* parent){
	//qDebug() << "param" << param;
	//*result = "result";
	//qRegisterMetaType<FramelessDialog>("FramelessDialog");
	MyGit* ret = new MyGit(parent);
	return ret;
}





bool MyGit::open(QString db_filename){
	//qDebug() << p;
	map_id_to_entry.clear();
	map_hash_to_id.clear();
	max_id = 0;
	file.setFileName(db_filename);
	if(!file.open(QIODevice::ReadWrite)){
		// will create file if not exists
		return false;
	}
	while(file.pos() != file.size()){
		t_entry e;
		if(!get_entry(e))
			return false;
	}
	return true;
}

qint64 MyGit::fileSize()
{
	return file.size();
}

bool MyGit::contains( qint64 id )
{
	return map_id_to_entry.contains(id);
}

bool MyGit::undel_by_id( qint64 id )
{
	return del_by_id(-id);
}