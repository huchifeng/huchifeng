/*
http://www.utf8-chartable.de/unicode-utf8-table.pl
U+10FFFF
UTF-16 add 0x3A-0x00 for charCode>=65536

*/

var a = [];
for(var i=0; i<65536; i++)
   a[i] = i+":"+String.fromCharCode(i)+"\r\n";
var s = a.join("");
WScript.Echo(s.length);
write_file(i+".txt", s);

function read_file(filename)
{
    var fso = new ActiveXObject("Scripting.FileSystemObject");
    var s = "";
    try{
        var f = fso.OpenTextFile(filename, 1, false, -2); // TristateTrue==-1 means UNICODE
        s = f.ReadAll();
        f.close();
    }
    catch(e){
    }
    return s;
}
function write_file(filename, s)
{
    var fso = new ActiveXObject("Scripting.FileSystemObject");
    var f = fso.OpenTextFile(filename, 2, true, -1); // TristateTrue==-1 means UNICODE
    f.Write(s);
    f.Close();
}