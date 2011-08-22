/* 2> nul

del 1.db
..\mygit\mygit add --db 1.db --type b --file %0
..\mygit\mygit add --db 1.db --type b --file myjs.exe
..\mygit\mygit add --db 1.db --type b --file %0
..\mygit\mygit get --db 1.db --id 1
..\mygit\mygit dir --db 1.db 
ls -l 1.db
myjs %0 dir --db 1.db

exit /B

*/
var args = app.argv();
//args.splice(0, 2);
app.log(args);
try{
	if(args.length==3 && args[0]=="dir" && args[1]=="--db")
		mygit_dir(args[2]);
}
catch(e){ app.log(e); }

function mygit_dir(db){
	var b = app.openBinaryFile(db);
	var size = b.size();
	app.log("size:" + size);
	for(var i=0; i<size;){
		var id = b.get_i64(i);
		if(id == 0) throw("wrong file");
		i += 8;
		var len = b.get_u8(i);
		i++;
		var type = b.get_utf8(i, len);
		i += len;
		var len = b.get_u8(i);
		i++;
		var hash = b.get_hex(i, len);
		i += len;
		var len = b.get_i64(i);
		i += 8;
		i += len;
		
		if(i > size) throw("wrong file");
		app.echo(id+" "+type+" "+hash+" "+len);
	}
}
