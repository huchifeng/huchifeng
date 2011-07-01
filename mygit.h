struct t_entry{
	qint64 id;
	QByteArray hash;
	QByteArray type;
	qint64 bytes_size;
	qint64 bytes_offset;
	// of course no data here, data is on disk only, too big

	t_entry(){
		id = 0;
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
	public slots:
		bool open(QString path);
private:
public:
	QFile file;
	QMap<qint64, t_entry> map_id_to_entry;
	QMap<QByteArray, QVector<qint64> > map_hash_to_id;
	qint64 max_id;
};

