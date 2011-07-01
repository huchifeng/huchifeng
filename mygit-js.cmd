/* 2>nul
del 1.db
..\myjs\myjs %~f0 ..\mygit\1.db || goto :err
ls -l 1.db
mygit dir --db 1.db
exit /b
:err
echo error:%errorlevel%
exit /b

*/
try{
	main();
}
catch(e){
	app.log(e);
	throw(1);
}

function main(){
	var lib = app.loadLibrary("../mygit/mygitlib.dll");
	git = lib.create("createMyGit");
	//app.log(git);
	check_true(git.open(app.argv()[2]));
	check_true(0==git.fileSize());
	var id1=git.add_data("test", "abc");
	check_true(id1==1);
	check_true(45==git.fileSize());
	var id2=git.add_data("test", "def");
	check_true(id2==2);
	check_true(90==git.fileSize());
	check_true(90==git.fileSize());
	check_true(id1==git.add_data("test", "abc"));
	check_true(90==git.fileSize());
	check_true(3==git.add_file("test", "stdafx.h"));
	check_true(git.save_by_id_to_file(3, "tmp.1"));
	check_true(0==app.executeProcess("cmd /c ls -l stdafx.h"));
	check_true(0==app.executeProcess("fc /b tmp.1 stdafx.h"));

	check_true(git.contains(2));
	check_true(git.del_by_id(2));
	check_true(!git.contains(2));
	check_true(git.undel_by_id(2));
	check_true(git.contains(2));
	check_true(!git.undel_by_id(2));
	check_true(git.del_by_id(2));
	check_true(git.contains(1) && !git.contains(2));
	check_true(null, "git.contains(-2)");
	check_true(git.pack_db());
	check_true(git.contains(1) && !git.contains(2));
	check_true(null, "!git.contains(-2)");
}

function check_true(e, hint){
	hint = (hint==null ? "" : hint);
	if(e==null)
		e = eval(hint);
	if(!e)
		throw("fail:" + hint);
	app.log("ok:"+hint);	
}
