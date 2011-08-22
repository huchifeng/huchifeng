/* 2> nul
myjs %0
ls -l myjs.h
wc myjs.h
rem pause
exit /B

*/

app.log(app.loadTextFile("myjs.h").length);
var b = app.loadBinaryFile("myjs.h");
app.log(b.byteSize());
//b = null;
