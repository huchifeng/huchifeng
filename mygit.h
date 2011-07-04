struct t_entry{
	qint64 id;
	QByteArray hash;
	QByteArray type;
	qint64 pos; // when modify it's useful
	qint64 bytes_size;
	qint64 bytes_offset;
	// of course no data here, data is on disk only, too big

	t_entry(){
		id = 0;
		pos = 0;
		bytes_size = 0;
		bytes_offset = 0;
	}
};

class MyGit : public QObject{
	Q_OBJECT();
public:
	MyGit(QObject* parent=NULL)
		: QObject(parent)
	{
		max_id = 0;
	}
	// cmd_* print outout
	bool cmd_dir();
	bool cmd_get_by_type(QByteArray type);
	bool cmd_get_by_id(qint64 id);

	public slots:
		bool open(QString path);
		qint64 add_file(QString type, QString file);
		qint64 add_data(QString type, QString content );
		bool save_by_id_to_file(qint64 id, QString file);
		bool del_by_id(qint64 id); // set the id to negative
		bool undel_by_id(qint64 id);
		bool pack_db(); // decrease the database file size
		bool fsck(QString db_filename);
		qint64 fileSize();
		bool contains(qint64 id);
private:
	bool get_entry(t_entry& e);
private:
	QFile file;
	QMap<qint64, t_entry> map_id_to_entry;
	QMap<QByteArray, QVector<qint64> > map_hash_to_id;
	qint64 max_id;
};

