/* 2> nul
myjs %0
ls -l tmp.db
dir tmp.db
exit /B

insert 6M*1000 ok, and insertion time is stable, 0.8sec;
'ls' fail to show 6G file, 'dir' is ok
auto rollback when ctrl+C

insert widthout transaction; 6M * 1 record/1.5sec
can commit ok when ctrl+C

*/
main();


function main(){
	var sq = app.createSqlite();
	app.log("open:"+sq.open("tmp.db", "test"));
	show_count(sq);
	//var q = sq.exec("drop table obj");
	//if(!q.isActive()){
	//	app.log(q.lastErrorText());
	//}
	var q = sq.exec("create table obj (id integer, hash text)");
	if(!q.isActive()){
		app.log(q.lastErrorText());
		//return null;
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
	// 6M*10 data / 8sec
	var s = ["hello"];
	for(var i=0; i<20; i++)
		s = s.concat(s);
	app.log(s.length);
	s = s.join(",");
	var q = sq.prepare("insert into obj (id, hash) values(?, ?)");
	var d = new Date();
	for(var i=0; i<1000; i++){
		q.bindValue(0, i);
		q.bindValue(1, s);
		q.exec();
		app.log(i+":"+((new Date()) - d));
	}
}
