/* 2> nul
myjs %0
ls -l tmp.db
exit /B


// fine for two process access same db file, database level lock

*/


var sq = app.createSqlite();
app.log("open:"+sq.open("tmp.db", "test"));
check(sq, "drop table obj");
//check(sq, "create table if not exists obj (id integer, hash text)");
check(sq, "create table obj (id integer, hash text)");
for(var i=1; i<10; i++)
	check(sq, "insert into obj (id, hash) values("+i+", '"+(i*i)+"')");
check(sq, "select * from obj");

try{
	app.log("--------------");
	var q = sq.prepare("select * from obj where id>=? and hash!=?");
	// cannot use '?'
	q.bindValue(0, 5);
	q.bindValue(1, '64'); // cannot use 64
	q.exec();
	check_q(q);
	
	app.log("--------------");	
	q.bindValue(0, 7);
	q.exec();
	check_q(q);
}catch(e){ app.log(e); }

	app.log("--------------");	
var q = sq.prepare("insert into obj (id, hash) values(:id, :hash)");
q.addBindValue([100, 101, 102]);
q.addBindValue([100100, 101101, 102102]);
q.execBatch();
check_q(q);
q.bindValue_byName(":id", 200);
q.bindValue_byName(":hash", '200200');
q.exec();
check(sq, "select * from obj where id>=100");

sq.close();

function check(sq, s){
	app.log(">"+s);
	var q = sq.exec(s);
	check_q(q);
}
function check_q(q){
	if(!q.isActive()){
		app.log(q.lastErrorText());
		return null;
	}
	if(!q.isSelect()){
		app.log("numRowsAffected:"+q.numRowsAffected());
		return null;
	}
	q.first();
	var d = [];
	for(var i=0; i< q.fieldCount(); i++)
		d[i] = q.fieldName(i);
	app.log(d);
	while(1){
		app.log([q.isActive(), q.isValid(), q.size(), q.fieldCount()]);
		if(! q.isValid())
			break;
		//app.log(q.value(0));
		//var m = q.boundValues();
		//for(i in m) app.log(i);
		var d = [];
		for(var i=0; i<q.fieldCount(); i++) d[i] = (q.value(i));
		app.log(d);
		q.next();
	}
	return null;
}

