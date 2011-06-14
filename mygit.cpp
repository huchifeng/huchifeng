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

struct t_entry{
	// id type hash bytes
	// id type ref_id name date
	// id type ref_id[] name date // 目录,标签 是等价的
	qint64 id; // no need to be unsigned
	enum type{
		content=1,
		ref=2
	};
	unsigned char t;
	
	QByteArray hash;
	qint64 bytes_size;
	qint64 bytes_offset;

	qint64 ref_id;
	QByteArray name; // UTF8
	qint64 date; // MSecsSinceEpoch  // 浮点可以精确到 1ms 以下，但没有意义， 浮点本身不精确

	t_entry(){
		id = 0;
		t = 0;
		ref_id = 0;
		date = 0;
	}
};
struct t_db{
	QFile d;
	QMap<qint64, t_entry> map_id_to_entry;
	QMap<QByteArray, QVector<qint64> > map_hash_to_id;
	QMap<QByteArray, QVector<qint64> > map_name_to_id; // then get the ref
	QMap<qint64, QVector<qint64> > map_ref_to_id;
	qint64 max_id;
	t_db(){
		max_id = 0;
	}
};
double seconds_since_1970UTC()
{
	// python code: return time.time(); # it's float
	QDateTime t = QDateTime::currentDateTime();
	return t.toTime_t() + t.time().msec()/1000.0;
}
int mygit_get_entry(t_db& db, t_entry& e){
	int n = db.d.read((char*)&e.id, sizeof(e.id));
	//std::cerr << "got:" << n << std::endl;
	if(n == 0)
		return 1;
	//qDebug() << s.errorString(); // unknown error
	if(n != sizeof(e.id))
		return 2;
	if(e.id <= 0)
		return 2;
	n = db.d.read((char*)&e.t, sizeof(e.t));
	if(n != sizeof(e.t) || e.t>t_entry::ref)
		return 3;
	if(e.t == t_entry::content){
		int len=0;
		if(db.d.read((char*)&len, sizeof(len))!=sizeof(len)
			|| len<=0 )
		{
			return 4;
		}
		e.hash.resize(len);
		if(db.d.read(e.hash.data(), len)!=len){
			return 5;
		}
		//qDebug() << e.hash.toHex();
		if(db.d.read((char*)&e.bytes_size, sizeof(e.bytes_size))!=sizeof(e.bytes_size)
			|| e.bytes_size<=0 )
		{
			return 6;
		}
		e.bytes_offset = db.d.pos(); // after the size field
		if(!db.d.seek(e.bytes_offset + e.bytes_size)){
			return 7;
		}
		db.map_id_to_entry[e.id] = e;
		db.map_hash_to_id[e.hash].push_back(e.id);
		db.max_id = qMax(db.max_id, e.id);
		return 0;
	}
	if(e.t == t_entry::ref){
		if(db.d.read((char*)&e.ref_id, sizeof(e.ref_id))!=sizeof(e.ref_id)
			|| e.ref_id<=0 )
		{
			return 4;
		}
		int len=0;
		if(db.d.read((char*)&len, sizeof(len))!=sizeof(len)
			|| len<=0 )
		{
			return 4;
		}
		e.name.resize(len);
		if(db.d.read(e.name.data(), len)!=len){
			return 5;
		}
		if(db.d.read((char*)&e.date, sizeof(e.date))!=sizeof(e.date))
		{
			return 4;
		}
		db.map_id_to_entry[e.id] = e;
		db.map_name_to_id[e.name].push_back(e.id);
		db.map_ref_to_id[e.ref_id].push_back(e.id);
		db.max_id = qMax(db.max_id, e.id);
		return 0;
	}
	return 9;
}
int get_hash(QFile& s, QByteArray& hash_result){
	qint64 old_pos = s.pos();
	QCryptographicHash hash(QCryptographicHash::Md5);
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
int mygit_add_name(t_db& db, QByteArray filename, qint64 ref_id){
	QVector<qint64> id = db.map_ref_to_id[ref_id];
	for(int i=0; i<id.size(); i++){
		t_entry e = db.map_id_to_entry[id[i]];
		if(e.t != t_entry::ref
			|| e.ref_id != ref_id)
			return 20;
		if(e.name == filename){
			qDebug() << "ignore name " << e.id << QString::fromUtf8(e.name);
			return 0;
		}
	}
	t_entry e2;
	qint64 old_pos = db.d.size();
	db.d.seek(old_pos);
	db.max_id ++;
	e2.id = db.max_id;
	e2.t = t_entry::ref;
	e2.ref_id = ref_id;
	db.d.write((char*)&e2.id, sizeof(e2.id));
	db.d.write((char*)&e2.t, sizeof(e2.t));
	db.d.write((char*)&e2.ref_id, sizeof(e2.ref_id));
	{
		int len = filename.size();
		db.d.write((char*)&len, sizeof(len));
		db.d.write(filename.constData(), filename.size());
	}
	e2.date = QDateTime::currentDateTime().toMSecsSinceEpoch();
	db.d.write((char*)&e2.date, sizeof(e2.date));
	return 0;
}
int mygit_add(QString db_filename, QString file){
	t_db db;
	db.d.setFileName(db_filename);
	if(!db.d.open(QIODevice::ReadWrite)){
		// will create file if not exists
		return 1;
	}
	while(db.d.pos() != db.d.size()){
		t_entry e;
		if(mygit_get_entry(db, e)!=0)
			return 2;
	}

	QFile s(file);
	if(!s.open(QIODevice::ReadOnly)){
		return 10;
	}
	t_entry e2;
	db.max_id ++;
	e2.id = db.max_id;
	e2.t = t_entry::content;
	if(get_hash(s, e2.hash)!=0){
		return 3;
	}
	std::cerr << e2.hash.toHex().constData() << std::endl;
	QVector<qint64> same_hash = db.map_hash_to_id[e2.hash];
	if(same_hash.size()>0)
	{
		for(int i=0; i<same_hash.size(); i++){
			qint64 id = same_hash[i];
			t_entry e2 = db.map_id_to_entry[id];
			qDebug() << "same hash, checking" << e2.id;
			if(e2.bytes_size != s.size()){
				qDebug() << "same hash, different size";
				continue;
			}
			db.d.seek(e2.bytes_offset);
			s.seek(0);
			int cmp_res = 0;
			if(file_cmp(db.d, s, e2.bytes_size, cmp_res)!=0){
				return 11;
			}
			if(cmp_res == 0){
				qDebug() << "same hash, same content";
				return mygit_add_name(db, file.toUtf8(), e2.id);
			}
		}
	}

	qint64 old_pos = db.d.size();
	db.d.seek(old_pos);
	db.d.write((char*)&e2.id, sizeof(e2.id));
	db.d.write((char*)&e2.t, sizeof(e2.t));
	{
		int len = e2.hash.size();
		db.d.write((char*)&len, sizeof(len));
		db.d.write(e2.hash.constData(), e2.hash.size());
	}
	qint64 s_size = s.size();
	db.d.write((char*)&s_size, sizeof(s_size));
	if(file_append(db.d, s) != 0){
		db.d.resize(old_pos);
		return 4;
	}
	return mygit_add_name(db, file.toUtf8(), e2.id);
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
	db.d.setFileName(db_filename);
	if(!db.d.open(QIODevice::ReadWrite)){
		// will create file if not exists
		return 1;
	}
	while(db.d.pos() != db.d.size()){
		t_entry e;
		if(mygit_get_entry(db, e)!=0)
			return 2;
		if(e.t == t_entry::content){
			std::cout << "dat " << e.id << " " << e.hash.toHex()  << " " << e.bytes_size<< std::endl;
		}
		else if(e.t == t_entry::ref){
			std::cout << "ref " << e.id << " " << e.ref_id 
				<< " " << QDateTime::fromMSecsSinceEpoch(e.date).toString("yyyy-MM-dd,hh-mm-ss.zzz")
				<< " " << QString::fromUtf8(e.name) << std::endl;
		}		
	}
	return 0;
}

int mygit_copy(QString db_filename, QString s_id, QString file){
	bool ok;
	qint64 id = s_id.toLongLong(&ok);
	if(!ok)
		return 1;
	t_db db;
	db.d.setFileName(db_filename);
	if(!db.d.open(QIODevice::ReadWrite)){
		// will create file if not exists
		return 1;
	}
	while(db.d.pos() != db.d.size()){
		t_entry e;
		if(mygit_get_entry(db, e)!=0)
			return 2;
	}
	if(! db.map_id_to_entry.contains(id)){
		return 2;
	}
	t_entry e = db.map_id_to_entry[id];
	if(e.t != t_entry::content){
		return 3;
	}

	QFile s(file);
	if(!s.open(QIODevice::WriteOnly)){
		return 10;
	}

	db.d.seek(e.bytes_offset);
	return file_from(db.d, e.bytes_size, s);

}

int main(int argc, char *argv[]){
    QApplication app(argc, argv); // cannot wchar

	if(argc==4 && strcmp(argv[1], "add")==0){
		return mygit_add(argv[2], QString::fromLocal8Bit(argv[3]));
	}
	else if(argc==3 && strcmp(argv[1], "dir")==0){
		// 可以选择前缀，只显示符合条件的部分
		return mygit_dir(argv[2]);
	}
	else if(argc==5 && strcmp(argv[1], "copy")==0){  //使用 data id
		return mygit_copy(argv[2], argv[3], argv[4]);
		// 不能使用 ref id，都是全路径，不安全。
	}
	else if(argc==5 && strcmp(argv[1], "ref")==0){
		//return mygit_ref(argv[2], argv[3], argv[3]); // 创建新符号名
	}
	else if(argc==4 && strcmp(argv[1], "del")==0){
		//return mygit_del(argv[2], argv[3]); // id 置为负
	}
	else if(argc==4 && strcmp(argv[1], "undel")==0){
		//return mygit_undel(argv[2], argv[3]);
	}
	else if(argc==3 && strcmp(argv[1], "pack")==0){
		//return mygit_pack(argv[2]); // 回收删除的空间
	}
	else if(argc==2 && strcmp(argv[1], "term")==0){
		// terminal style interaction
	}
	else if(argc==2 && strcmp(argv[1], "web")==0){
		// run as a web server, 
	}

	std::cout << "usage:\n"
		<< "mygit add <db-filename> <input-filename>\n"
		<< "mygit dir <db-filename>\n"
		<< "mygit copy <db-filename> <int64-id> <output-filename>\n"
		;

	return 0; //app.exec();
}




