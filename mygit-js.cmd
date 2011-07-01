/* 2>nul
..\myjs\myjs %~f0
exit /b

*/

var lib = app.loadLibrary("../mygit/mygitlib.dll");
var git = lib.create("createMyGit");
//app.log(git);
app.log(git.open("1.db"));
