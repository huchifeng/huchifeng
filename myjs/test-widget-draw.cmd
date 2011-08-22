/* 2> nul
myjs %0
exit /B

*/

app.pic = app.loadImage("../pic/em05.gif"); // add any property
app.log("size:"+[app.pic.width, app.pic.height]);

var mw = app.createMainWindow();
mw.move(0, 0);
mw.resize(640, 480);
mw.paint.connect(function(p) {
    p.drawText(50, 20, "hello\nworld:" + app.pic); // left bottom
    p.drawText(50, 25, 70, 100, "hello world\n again\n need \\n");
   p.drawImage(50, 200, app.pic.width*2,  app.pic.height*2, app.pic);
});
mw.show();
app.exec();
