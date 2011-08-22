/* 2> nul
myjs %0
rem pause
exit /B

*/
test("1+1");
test("1.0/77.7*77.7==1");
test("1.0/.0");
test("-1.0/.0");
test("1.0/.0-1.0/0");
test("null==undefined"); // true
test("typeof(x)"); // undefined
var x;
test("typeof(x)"); // undefined
x = 100;
test("typeof(x)"); // undefined
x = "100";
test("typeof(x)"); // undefined;
app.log("begin:"+app.formatDateTime(new Date(), "yyyy-MM-dd HH-mm-ss.zzz"));
var a = new Date();
var N=100000; for(var i=0; i<N; i++) ;
app.log("avg:"+N*1000/((new Date()) - a));
app.log("end:"+app.formatDateTime(new Date(), "yyyy-MM-dd HH-mm-ss.zzz"));

function test(e){
  app.log(e+":"+ eval(e));
}
