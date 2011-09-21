import Qt 4.7
import QtWebKit 1.0
import MyComponents 1.0
import "."


Rectangle {
	width:800; height:600;
	id: root
	color: "#e5e9f2";
	ObjQWebView{
		id:web1;
		x:10; y:100; width:root.width-x*2; height:root.height-y-10;
		property variant v : [1,2,3];
		function print_v(){
			app.alert("print-v");
			for(var i in v){
				app.alert(i+":"+v[i]);
			}
		}
		
		property variant f;
		function alert(s){
			app.alert("from webview in qml:" + s);
		}
		function call_f(){
			app.alert(f);
			(f)(); // fail
		}
	}
	Component.onCompleted: {
		console.log("Completed Running!")
		web1.obj.url = "1.html";
		//web1.alert(99);
	}
}

