/* 2> nul
myjs %0
ls -l tmp.db
exit /B

// must use transaction to insert data
// Transaction speed is limited by disk drive speed 
// on a 7200RPM disk drive limits you to about 60 transactions per second.
//  That way, if you suddenly lose power or if your OS crashes, your data is still safe. 
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
	var q = sq.exec("create table obj (id integer, hash text)");
	if(!q.isActive()){
		app.log(q.lastErrorText());
		return null;
	}
	sq.transaction();
	test1(sq);
	test2(sq);
	sq.commit();
	test3(sq);
	show_count(sq);
	//sq.rollback();
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
	var d = new Date();
	// insert 10/1 sec
	// transaction insert 1000/0.32 sec
	for(var i=0; i<1000; i++)
		sq.exec("insert into obj (id, hash) values("+i+", '"+(i*i)+"')");
	app.log((new Date()) - d);
}
function test2(sq){
	var q = sq.prepare("insert into obj (id, hash) values(?, ?)");
	var d = new Date();
	// insert 10/1 sec
	// transaction insert 1000/0.23 sec
	for(var i=0; i<1000; i++){
		q.bindValue(0, i);
		q.bindValue(1, i*i);
		q.exec();
	}
	app.log((new Date()) - d);
}
function test3(sq){
	var q = sq.exec("select id,hash from obj");
	var d = new Date();
	q.first();
	var t = 0;
	for(var i=0; q.isValid(); i++){
		t += q.value(1).length;
		q.next();
	}
	app.log([i, t, (new Date()) - d]);
}
