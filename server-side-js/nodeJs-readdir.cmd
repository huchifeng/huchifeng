/*
set path=D:\huchifeng.2\2-01-tools-no-install;%path%
node.exe %0
pause
exit /b

*/
var fs = require('fs');


filenames = fs.readdirSync(".");
for (i = 0; i < filenames.length; i++) {
    console.log(filenames[i]);
}
console.log('Current uid: ');// + process.getuid());


console.log('--------------Òì²½---------');

//Òì²½
fs.readdir(".", function (err, filenames) {
    var i;
    for (i = 0; i < filenames.length; i++) {
        console.log(filenames[i]);
    }
});
console.log('Current uid: ');// + process.getuid());

