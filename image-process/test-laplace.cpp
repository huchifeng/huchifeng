/*

Copyright (C) 2011 huchifeng@gmail.com

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
HOMEPAGE: https://github.com/huchifeng/huchifeng

*/
/*
Laplace:
  D[f[x,y], x, x] + D[f[x,y], y, y]
  f is the image;
  D is "Partial differential"

Discrete expression:
  f[x+1, y]+f[x-1, y]+f[x, y+1]+f[x,y-1]-4*f[x,y]


download test images from goole search: 
http://decsai.ugr.es/~javier/denoise/test_images/index.htm
*/


#include <time.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator> // front-inserter
#include <vector>
#include <deque>
#include <map>
#include <functional> // tr1::function
#include <assert.h>

#pragma warning(disable:4127)
#include <QtGui/QImage>
using namespace std;

void copy(uchar* d, int w, int w2, int h, uchar* s, int w3){
	for(int i=0; i<h; i++){
		memcpy(d+w2*i, s+w3*i, w);
	}
}
void copy(uchar* d, int c, int w, int w2, int h, uchar* s, int c3, int w3){
	for(int i=0; i<h; i++){
		for(int j=0; j<w/c; j++){
			d[i*w2+j*c] = s[i*w3+j*c3];
		}
	}
}
void set(uchar*d, int c, int w, int w2, int h, int x){
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j+=c){
			d[i*w2+j] = x;
		}
	}
}
void laplace(uchar *d, int c, int w, int w2, int h, uchar* s, int c3, int w3){
#define F(i, j)  ((i)>=0 && (i)<h && (j)>=0 && (j)<w ? s[(i)*w3+(j)*c3] : 0)
//#define R(x, a, b) ((x)<(a)?(a):(x)>(b)?(b):(x))
	for(int i=0; i<h; i++){
		for(int j=0; j<w/c; j++){
			int t = F(i,j-1)+F(i,j+1)+F(i+1,j)+F(i-1,j)-4*F(i,j);
			t = abs(t)/4; //(t+4*255)/8;
			//t = t*5; // too dark
			t = t <0 ? 0 : t>255 ? 255 : t;
			d[i*w2+j*c] = t;
		}
	}
//#undef R
#undef F
}
void u32_from_u8(uchar* d, int w, int w2, int h, uchar* s, int c3, int w3){
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j+=4){
			int t = s[i*w3+j/4*c3];
			d[i*w2+j] = t;
			d[i*w2+j+1] = t;
			d[i*w2+j+2] = t;
			d[i*w2+j+3] = 255;
		}
	}
}


void main(){
	// boat
	QImage img = QImage("data/fingerprint.png").convertToFormat(QImage::Format_Indexed8);
	cerr << img.width() << "*" << img.height() << "*" << img.depth() << "," << img.format() << endl;
	assert(img.depth()==8);
	QImage tmp(img.width()*2, img.height(), QImage::Format_Indexed8);
	QVector<QRgb> tab;
	tab.resize(256);
	for(int i=0; i<tab.size(); i++)
		tab[i] = qRgb(i, i, i);
	tmp.setColorTable(tab);
	copy(tmp.bits(), img.width(), img.width()*2, img.height(), img.bits(), img.width());
	laplace(tmp.bits()+img.width(), 1, img.width(), img.width()*2, img.height(), img.bits(), 1, img.width());
	tmp.save("tmp.png");
	system("start tmp.png");
}

