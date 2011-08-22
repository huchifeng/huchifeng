

/* 2> nul
cd %~dp0
myjs --eval "app.log(app._debug())"
myjs --eval "app.eval(app.loadTextFile('%~nx0').split('--js--')[2])" %~nx0
exit /B

// put cmd,js,qml into one file //
--js--
//app.log(app.argv()[3]);
var qml = app.createDeclarativeView();
qml.setContextProperty("app", app);
qml.setSourceUrl(app.argv()[3]);
qml.setResizeMode(1);
qml.showMaximized();
app.init();
app.exec();
--js--

*/

import Qt 4.7
import QtWebKit 1.0
//import "."
import "json2.js" as Js

Rectangle {
    width:800; height:600;
    id: root
    color: "lightblue";//"#e5e9f2";
    Text {
		x : (root.width - width)/2;
		y : 10;
		text : "hello, world\n中文支持, 需要将本文件保存为 UTF-8";
		horizontalAlignment : "AlignHCenter";
		font.pointSize : 20;
    }
    Connections{
        target: app;
        onInit: {
			app.alert([root.width, root.height]);
		}
	}
}

