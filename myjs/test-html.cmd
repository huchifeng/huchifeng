myjs --eval "app.eval(app.loadTextFile('%~nx0').split('--js--')[2])" %~nx0
exit /b

--js--
var s = app.loadTextFile(app.argv()[3]).split('--html--')[2];
//app.log(s);
var web = app.createWebView();
//app.resize(web, 1440, 800);
web.show();
web.setHtml(s, "");
app.exec();
--js--

--html--
<html>
<head>
<title>
my title
</title>
</head>
<body>
hello, world
<br/>
<input type=button value="app.alert('hello')" onclick="app.eval(this.value)" />
<br/>
<input type=button value="app.quit()" onclick="app.eval(this.value)" />
<br/>
<input type=button value="app.echo('console output')" onclick="app.eval(this.value)" />

</body>
</html>


