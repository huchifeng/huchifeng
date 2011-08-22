/* 2> nul
myjs %0
exit /B

*/
var pt =[
	{v:[0.5, -0.5, 0], c:[1.0, 0, 0]},
	{v:[0, 0.5, 0], c:[0, 1.0, 0]},
	{v:[-0.5, -0.5, 0], c:[0, 0, 1.0]},
	{v:[0, 0, 0.5], c:[1.0, 1.0, 1.0]}
];
var ang = [0, 0, 10];

var glw = app.createGLWidget();
glw.paintGL.connect(gl_paint);
glw.keyPressEvent.connect(gl_key);
//app.createTimer(10).timeout.connect(gl_timer);
glw.show();
app.exec();

function gl_key(ev){
	//app.log(ev.desc);
	var d = ev.desc;
	var k = 10;
	if(d=="Down") ang[0] += k;
	else if(d=="Up") ang[0] -= k;
	else if(d=="Left") ang[1] += k;
	else if(d=="Right") ang[1] -= k;
	else if(d=="Z") ang[2] += k;
	else if(d=="X") ang[2] -= k;
	glw.updateGL();
}
function gl_timer(){
	ang[2] ++;
	glw.updateGL();
}
function gl_paint(gl){
	gl.glEnable(0x0B71); // depth-test
	gl.glMatrixMode(0x1701); // project-view
	gl.glLoadIdentity();
	gl.gluPerspective(30, 1, 3.0, 7.0);
	gl.glMatrixMode(0x1700); // model-view
	gl.glPushMatrix();
	gl.glClearColor(0, 0, 0, 0); // put here, not outside
	gl.glClear(0x00004000 + 0x00000100); // color+depth
	gl.glTranslatef(0, 0.0, -4.5);
	gl.glRotatef(ang[0], 1, 0, 0);
	gl.glRotatef(ang[1], 0, 1, 0);
	gl.glRotatef(ang[2], 0, 0, 1);
	paint_tri(gl, [0, 1, 2]);
	paint_tri(gl, [0, 1, 3]);
	paint_tri(gl, [0, 2, 3]);
	paint_tri(gl, [2, 1, 3]);
	gl.glPopMatrix();
}
function paint_tri(gl, i){
	gl.glBegin(0x0004); // triangles
	for(var j=0; j<3; j++){
		gl.glColor3f(pt[i[j]].c[0], pt[i[j]].c[1], pt[i[j]].c[2]);
		gl.glVertex3f(pt[i[j]].v[0], pt[i[j]].v[1], pt[i[j]].v[2]);
	}
	gl.glEnd();
}

