/* 2> nul
myjs %0
ls -l tmp.db
dir tmp.db
exit /B


*/
main();


function main(){
	var sq = app.createSqlite();
	app.log("open:"+sq.open("tmp.db", "test"));
	show_count(sq);
	var q = sq.exec("drop table obj");
	if(!q.isActive()){
		app.log(q.lastErrorText());
	}
	var q = sq.exec("create table obj (id integer, txt text, hash blob)");
	if(!q.isActive()){
		app.log(q.lastErrorText());
		return null;
	}
	sq.transaction();
	test1(sq);
	sq.commit();
	show_count(sq);
	sq.close();
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
	var q = sq.prepare("insert into obj (id, txt, hash) values(?, ?, ?)");
	var d = new Date();
	for(var i=0; i<1000; i++){
		q.bindValue(0, i);
		q.bindValue(1, "hello:" + i*i);
		q.bindValue(2, "world:" + i*i*i + "ÖÐÎÄ\x00\xEF\u1111AB");
		// string converted to utf8, then bytearray
		q.exec();
	}
	app.log(i+":"+((new Date()) - d));
	var q = sq.exec("select * from obj where id==9");
	q.first();
	app.log("type:"+q.fieldType(0)); // INTEGER: 2 : int
	app.log("type:"+q.fieldType(1)); // TEXT: 10 : QString
	app.log("type:"+q.fieldType(2)); // BLOB: 12 : QVariant::ByteArray
	app.log(q.value(2));
	app.log(typeof(q.value(2))); // string
	app.log(q.value(2).length); // include data after \0
	app.log([q.value(2).charCodeAt(9),
		q.value(2).charCodeAt(10),
		q.value(2).charCodeAt(11),
		q.value(2).charCodeAt(12)]); // right
}
