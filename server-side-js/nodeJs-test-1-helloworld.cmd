/*
set path=C:\_download\3-06-nodeJs-javascript-server;%path%
start http://127.0.0.1:1337/
node.exe %0
exit /b

*/

var http = require('http');
http.createServer(function (req, res) {
  res.writeHead(200, {'Content-Type': 'text/plain'});
  res.end('Hello World:'+Math.random()+'\n');
}).listen(1337, "127.0.0.1");
console.log('Server running at http://127.0.0.1:1337/');

