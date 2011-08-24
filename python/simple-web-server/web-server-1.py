# -*- coding: UTF-8 -*- 

import os
import sys
import time
import datetime
import BaseHTTPServer 
import SimpleHTTPServer
import threading
import random

reply = u"""
{
"ID":"543de894-6f54-4fc7-9c48-e238f9f9a39820100726",
"ScanInfo":{
"DeviceCheckJobID":"0001",
"ImageUrl":"xxxx",
"ThumbnailUrl":"",
"FromDevice":null,
"ImageType":0,
"ScanTime":"\/Date(1280112603360+0800)\/",
"Priority":0,
"ChannelNo":0
},
"Baggage":{
"BID":"abcdefg   ",
"PassengerName":"张三",
"PassengerID":"zhang san",
"Flight":"%(flight)s",
"Destination":"dongjing",
"Counter":"10",
"BoardingTime":"2010-07-29",
"SeatNo":"A215"
},
"CheckType":1,
"CheckTime":{
"From":"\/Date(1280112603000+0800)\/",
"To":"\/Date(1280112613000+0800)\/"
},
"CheckedBy":{
"ID":"user1",
"Name":"user1",
"Password":"P1I80usBwcHU65B/+Ve6y7gQuNM=",
"Role":2,
"Group":{
"ID":"00000000-0000-0000-0000-000000000000",
"Name":"Empty"
}
},
"Result":{
"IsSuspect":true,
"Tags":[{
"X":10,
"Y":20,
"Width":210,
"Height":150,
"ViewID":0
}]
},
"IsScanInfoReady":false,
"HasResult":true,
"HasBaggageInfo":true
}
""";

reply2 = u"""
你好，
<form method=POST>
<input name=name1 value="NAME1">
<input name=name2 value="NAME2，中文测试">
<input type=submit value="SUBMIT1">
</form>
""";

reply3=u"""
你好,hello,<a href='/test.html'>test again</a><br>
<input type="button" value="quit" onclick="(window.app.quit())" />
<br>
---DATA---
<br>
""";

class MyHTTPRequestHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    def do_GET(self):
        """Serve a GET request."""
        print 'GET', self.path
        if(self.path == "/favicon.ico"):
            s = open(r'D:\Program Files\Microsoft Visual Studio 9.0\Setup\setup.ico').read()
        elif(self.path.find('getimageinfo')>=0):
            reply1 = reply % {'flight': "CA100"+str(random.randint(100,900))};
            s = reply1.encode('utf-8');
        else:
            s = reply2.encode('utf-8');
        self.wfile.write('HTTP/1.1 200 OK\r\n');
        #self.wfile.write('Content-Type: text/plain; charset=UTF-8\r\n'); # plain is important
        self.wfile.write('Content-Type: text/html; charset=UTF-8\r\n'); # plain is important
        self.wfile.write('Content-Length: ' + str(len(s)) + '\r\n');
        self.wfile.write('\r\n');
        self.wfile.write(s);
    def do_POST(self):
        print "POST", self.path
        #print dir(self)
        #print type(self.headers), self.headers, dir(self.headers), self.headers["Content-Length"]
        data = self.rfile.read(int(self.headers["Content-Length"]))
        print "---DATA---\n", data
        s = (reply3+data).encode("utf-8");
        self.wfile.write('HTTP/1.1 200 OK\r\n');
        self.wfile.write('Content-Type: text/html; charset=UTF-8\r\n'); # plain is important
        self.wfile.write('Content-Length: ' + str(len(s)) + '\r\n');
        self.wfile.write('\r\n');
        self.wfile.write(s);
def run_webserver():
    BaseHTTPServer.HTTPServer(('', 2008), MyHTTPRequestHandler).serve_forever()
print time.time(); # seconds_since_1970UTC    
threading.Thread(target=run_webserver).start()
print 'press enter to end process';
os.system("start http://localhost:2008")
sys.stdin.read(1);