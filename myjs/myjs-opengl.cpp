/*

myjs: use javascript to do anything instead of C++.
Copyright (C) 2011 huchifeng

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

CONTACT: huchifeng@gmail.com
HOMEPAGE: https://github.com/huchifeng/mygit

*/

#include "stdafx.h"
#include "myjs-opengl.h"

#pragma comment(lib, "Glu32.lib")

#ifdef _DEBUG
#pragma comment(lib, "QtOpenGLd4.lib")
#else
#pragma comment(lib, "QtOpenGL4.lib")
#endif

QWidget* MyApp::createGLWidget()
{
	return new MyGLWidget;
}

void MyGLWidget::glClearColor(double r, double g, double b, double a)
{
	::glClearColor(r, g, b, a); // qglClearColor(QColor(i>>16, (i>>8) & 255, i & 255));
}

void MyGLWidget::paintGL()
{
	emit paintGL(this);
}

void MyGLWidget::glClear( int i )
{
//#define GL_DEPTH_BUFFER_BIT               0x00000100
//#define GL_COLOR_BUFFER_BIT               0x00004000
//#define GL_ACCUM_BUFFER_BIT               0x00000200
//#define GL_STENCIL_BUFFER_BIT             0x00000400
	::glClear(i);
}

void MyGLWidget::glBegin( int t )
{
//#define GL_POINTS                         0x0000
//#define GL_LINES                          0x0001
//#define GL_LINE_LOOP                      0x0002
//#define GL_LINE_STRIP                     0x0003
//#define GL_TRIANGLES                      0x0004
//#define GL_TRIANGLE_STRIP                 0x0005
//#define GL_TRIANGLE_FAN                   0x0006
//#define GL_QUADS                          0x0007
//#define GL_QUAD_STRIP                     0x0008
//#define GL_POLYGON                        0x0009
	::glBegin(t);
}

void MyGLWidget::glColor3f( double red, double green, double blue )
{
	::glColor3f(red, green, blue);
}

void MyGLWidget::glVertex3f( double x, double y, double z )
{
	::glVertex3f(x, y, z);
}

void MyGLWidget::glEnd( void )
{
	::glEnd();
}

void MyGLWidget::glEnable( int i )
{
//#define GL_DEPTH_TEST                     0x0B71
	::glEnable(i);
}

void MyGLWidget::glMatrixMode( int i )
{
//#define GL_MODELVIEW                      0x1700
//#define GL_PROJECTION                     0x1701
//#define GL_TEXTURE                        0x1702
	::glMatrixMode(i);
}

void MyGLWidget::glLoadIdentity()
{
	::glLoadIdentity();
}

void MyGLWidget::gluPerspective( double fovy, double aspect, double zNear, double zFar )
{
	::gluPerspective(fovy, aspect, zNear, zFar);
}

void MyGLWidget::glPushMatrix()
{
	::glPushMatrix();
}

void MyGLWidget::glPopMatrix()
{
	::glPopMatrix();
}

void MyGLWidget::glTranslatef( double x, double y, double z )
{
	::glTranslatef(x, y, z);
}

void MyGLWidget::glRotatef( double angle, double x, double y, double z )
{
	::glRotatef(angle, x, y, z);
}

