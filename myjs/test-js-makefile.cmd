

/* 2> nul
cd %~dp0
myjs %~f0 %* || goto :err
exit /b

:err
echo %errorlevel%
exit /B %errorlevel%

a sample to demonstrate the capability of javascript MAKEFILE.

*/

var targets = {
  "debug":[
	'test.cpp',
	{cmd: 'cmd /c echo debug running'}
  ],
  "release":[
	'test.cpp',
	{cmd: 'cmd /c echo release running'}
  ],
  "test":[
	"debug",
	"release",
	//"test",
	//{cmd: 'cmd /c dir not-exists-file'},
	{cmd: 'cmd /c echo test running'}
  ],
  "clean":[
	{cmd: 'cmd /c echo cleaning'}
  ],
  "all":["debug", "release", "test", "clean",
	{cmd: 'cmd /c echo all done'}
  ]
};
var status = {}; // running, done
try{
	main();
}catch(e){
	app.log(e);
	throw(e);
}

function main(){
	var argv = app.argv();
	//app.log(argv);
	if(argv.length <= 2){
		app.log("available targets:");
		for(var i in targets)
			app.log("\t"+i);
		return;
	}
	var target = argv[2];
	make(target);
}
function make(target){
	if(status[target]=='done'){
		app.log('> target done, skipping:' + target);
		return;
	}
	if(status[target]=='running'){
		throw("! recursive detected, wrong dependency");
	}
	app.log('> making target:' + target);
	status[target]='running';
	if(!(target in targets)){
		if(target.match(/\.(c|cpp)/i)){
			make_cmd("cmd /c echo compiling:" + target);
			status[target]='done';
			return;
		}
		throw('! target not found:' + target);
	}
	var a = targets[target];
	for(var i=0; i<a.length; i++){
		if(typeof(a[i])=="string"){
			make(a[i]);
			continue;
		}
		if(typeof(a[i])=="object" && ("cmd" in a[i])){
			make_cmd(a[i].cmd);
			continue;
		}
		throw("! syntax error");
	}
	status[target]='done';
}
function make_cmd(cmd){
	app.log("> "+cmd);
	if(0!=app.executeProcess(cmd)){
		throw("! fail when:" + cmd);
	}
}
