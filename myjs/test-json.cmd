/* 2> nul
myjs %0
exit /B

*/
var args = app.argv();
//app.log(args);
args.splice(0, 2);
app.eval("x = 9;"); // cannot "var x = 9;"
app.eval("app.log(x);");
app.log(x);
app.eval(app.loadTextFile("json2.js"));
app.log(JSON.stringify([1,2,3,6, {x:88}, JSON.parse("[5,6,7]"),
  JSON.parse("{\"x\":[88,9, {\"y\":777}]}")]));
