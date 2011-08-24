/*
set path=C:\_download\3-06-nodeJs-javascript-server;%path%
set path=C:\_install;%path%
start node.exe %0
telnet localhost 1337
exit /b

*/

var net = require('net');

var server = net.createServer(function (socket) {
  socket.write("Echo server\r\n");
  socket.pipe(socket);
});

server.listen(1337, "127.0.0.1");

