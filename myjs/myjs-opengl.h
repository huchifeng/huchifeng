#pragma once

#include "myjs.h"
#include <QtOpenGL/QtOpenGL>


class MyGLWidget: public QGLWidget{
	Q_OBJECT();

	virtual void paintGL ();
	virtual void keyPressEvent( QKeyEvent * event )
	{
		// TAB  ’≤ªµΩ
		ObjQKeyEvent evt;
		evt.p = event;
		emit keyPressEvent(&evt);
	}

	public slots:
		void glClearColor(double r, double g, double b, double a);
		void glClear(int i);
		void glBegin(int t);
		void glColor3f(double red, double green, double blue);
		void glVertex3f(double x, double y, double z);
		void glEnd(void);
		void glEnable(int i);
		void glMatrixMode(int i);
		void glLoadIdentity();
		void gluPerspective(double fovy, double aspect, double zNear, double zFar);
		void glPushMatrix();
		void glPopMatrix();
		void glTranslatef(double x, double y, double z);
		void glRotatef(double angle, double x, double y, double z);
signals:
		void paintGL(QObject*);
		void keyPressEvent(QObject*);
};
