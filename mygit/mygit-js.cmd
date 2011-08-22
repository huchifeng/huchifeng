/* 2>nul
rem del 1.db
..\myjs\myjs %~f0 ..\mygit\1.db || goto :err
ls -l 1.db
mygit dir --db 1.db
exit /b
:err
echo error:%errorlevel%
exit /b

*/
try{
	//test();
	main();
}
catch(e){
	app.log(e);
	throw(1);
}
// TODO: write a html GUI version for mygit interface, use mygitlib.dll instead of mygit.exe

function main(){
	// process redirect output "dir /s /b /a:-d" not reliable
	app.eval(app.loadTextFile("json2.js"));

	var db_file = app.argv()[2];
	cmd_prefix = "..\\mygit\\mygit add --db "+db_file+" --type file --file ";

	var m = [];
	save_recursive(m, "e:/"); // 2011-04-24/

	var lib = app.loadLibrary("../mygit/mygitlib.dll");
	git = lib.create("createMyGit");
	if(!git.open(db_file))
		throw("open fail:"+db_file);
	//app.log(JSON.stringify(m));
	if(!git.add_data("list", JSON.stringify(m)))
		throw("save list fail");
}
function save_recursive(m, p){
	var all = app.dir(p);
	for(var i=0; i<all.length; i++){
		var p1 = p+all[i];
		if(app.fileInfo(p1).isDir){
			save_recursive(m, p1+"/");
			continue;
		}
		//var id = git.add_file("file", p1);
		//if(!id)
		//	throw("add file fail"+p1);
		var cmd = cmd_prefix + p1;
		app.log(cmd);
		var proc = app.createProcess(cmd);
		proc.waitForReadyRead(-1); // otherwise no data
		var id = proc.readLine(1000, false);
		proc.close();
		app.log(id);
		id = Number(id);
		app.log(p1+":"+id);
		m.push({path:p1, id:id});
	}
}

function test(){
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
