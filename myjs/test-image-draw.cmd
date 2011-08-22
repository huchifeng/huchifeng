/* 2> nul
myjs %0
tmp.jpg
rem ping localhost >nul
rem del tmp.jpg
exit /B

*/

app.pic = app.loadImage("../pic/em05.gif"); // add any property
app.log("size:"+[app.pic.width, app.pic.height]);
var p = app.pic.createPainter();
p.drawText(10,20, "HELLO");
app.pic.save("tmp.jpg");
