/* 2> nul
myjs %0
ls -l tmp.db
exit /B

// must use transaction to insert data
// sqlite2 same too

// SQLite2 internally stores every value as a string, regardless of its type. 
// SQLite3 will certainly shrink the database size since numbers and BLOBS get stored in their native formats,
//    which could make things run faster. 
// sqlite3 64-bit ROWIDs.
// http://www.sqlite.org/version3.html
*/
main();


function main(){
	//var sq = app.createSqlite2();
	var sq = app.createSqlite();
	app.log("open:"+sq.open("tmp.db", "test"));
	show_count(sq);
	//var q = sq.exec("drop table obj");
	var q = sq.exec("delete from obj"); // will reset row-id
	if(!q.isActive()){
		app.log(q.lastErrorText());
	}
	var q = sq.exec("create table obj (id integer, hash text)");
	if(!q.isActive()){
		app.log(q.lastErrorText());
		//return null;
	}
	app.log(["transaction", sq.transaction()]);
	test1(sq);
	test2(sq);
	sq.commit();
	show_count(sq);
	type_all(sq);
	//sq.rollback();
	sq.close();
}
function type_all(sq){
	var q = sq.exec("select rowid, id, hash from obj");
	q.first();
	do{
		app.log(["type", q.value(0), q.value(1), q.value(2)]);
	}
	while(q.next());	
	q.finish();
}
function show_count(sq){
	var q = sq.exec("select count(*) from obj");
	if(!q || !q.isActive()){
		app.log(!q);
		app.log(q.lastErrorText());
		return null;
	}
	q.first();
	app.log("count(*):"+q.value(0));
	q.finish();
}
function test1(sq){
	var d = new Date();
	// insert 10/1 sec
	// transaction insert 1000/0.32 sec
	for(var i=0; i<10; i++)
		sq.exec("insert into obj (id, hash) values("+i+", '"+(i*i)+"')");
	app.log((new Date()) - d);
}
function test2(sq){
	var q = sq.prepare("insert into obj (id, hash) values(?, ?)");
	var d = new Date();
	// insert 10/1 sec
	// transaction insert 1000/0.23 sec
	for(var i=0; i<10; i++){
		q.bindValue(0, i);
		q.bindValue(1, i*i);
		q.exec();
		app.log("ins:" + q.lastInsertId());
	}
	app.log((new Date()) - d);
}
