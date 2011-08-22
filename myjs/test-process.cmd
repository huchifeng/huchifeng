/* 2> nul
myjs %0 1
ls -l tmp.1
od -t x1z -N 10 tmp.1
type tmp.1

myjs %0 2

myjs %0 3

dir | myjs %0 4

myjs %0 5

del tmp.*
exit /B

*/
var args = app.argv();
//app.log(args);
args.splice(0, 2);
if(args[0]=="1"){
	var p = app.executeProcess("cmd /c dir > tmp.1");
	app.log(p);
}
else if(args[0]=="2"){
	var p = app.createProcess("cmd /c dir");
	p.waitForReadyRead(-1); // otherwise no data
	while(!p.atEnd()){
		app.print(p.readLine(1000, false));
		//app.log(p.state()+","+p.atEnd());
	}
	p.close();
}
else if(args[0]=="3"){
	var p = app.createProcess("wc");
	for(var i=0; i<10000; i++) p.write("hello, World\n 1,2,3 中文 中文2", false);
	app.log(p.state()+","+p.atEnd());
	p.closeWriteChannel();
	p.waitForReadyRead(-1);
	while(!p.atEnd()){
		app.print(p.readLine(1000, false));
		//app.log(p.state()+","+p.atEnd());
	}
	p.close();
}
else if(args[0]=="4"){
	while(!app.stdinAtEnd()){
		var s = app.gets(1000, false);
		app.print("uppercase["+s.length+"]"+s.toUpperCase());
	}
	app.echo("echo--stdin--end");
	app.log("stdin--end");
}
else if(args[0]=="5"){
	var p = app.createProcess("myjs test-process.cmd 4");
	p.readyReadStandardOutput.connect(function(){
		while(1){
			var s = p.readLine(1000, false);
			if(s.length==0) return;
			app.print("answer["+s.length+"]"+s);
			if(s.match(/end-of-push/i)) app.quit();
		}
	});
	for(var i=0; i<10; i++){
		p.write("hello, World:" + i + ","+ "\n", false);
		app.log(i);
	}
	p.write("end-of-push", false);
	p.closeWriteChannel();
	app.exec();
	p.close();
}

