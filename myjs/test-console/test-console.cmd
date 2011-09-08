/* 2> nul
..\myjs %0
pause
exit /B




*/

var con = app.console;

//app.log("hello, world");
con.moveto(5, 1);
con.put("--clear--"); con.clearLine();
con.attr(4+1, 0);
con.put("-ok-");

con.attr(8+7, 0);
con.moveto(1, 2);
con.put("--hello--");

con.attr(7, 0); // default
con.put(con.getch());
var s = con.gets(); con.put("["+s+"-"+s+"]");