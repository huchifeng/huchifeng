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
histogram eualization
  should be Idempotent
  todo: prove it

  suppose histogram is his[256]
  the point w*h/256, W*h*2/256 .... should locate in his[0]...his[256]
  map m[0~256]=0~256

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

void copy(uchar* d, int w, int h,  int w2,uchar* s, int w3){
	for(int i=0; i<h; i++){
		memcpy(d+w2*i, s+w3*i, w);
	}
}
void copy(uchar* d, int c, int w, int h, int w2, uchar* s, int c3, int w3){
	for(int i=0; i<h; i++){
		for(int j=0; j<w/c; j++){
			d[i*w2+j*c] = s[i*w3+j*c3];
		}
	}
}
void set(uchar*d, int c, int w, int h, int w2, int x){
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j+=c){
			d[i*w2+j] = x;
		}
	}
}
void calc_histogram(int counter[256], const uchar* s, int w, int h, int bytesPerLine)
{
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			counter[*(const uchar*)((uchar*)s+i* bytesPerLine + j)] ++;
		}
	}
}
void from_counter_to_map(int counter[256], int m[256]){
	int sum = 0;
	for(int i=0; i<256; i++)
		sum += counter[i];
	for(int i=0, j=0, cum=0; i<256; i++){
		cum += counter[i];
		j = cum*256/sum;
		j = j < 256 ? j : 255;
		m[i] = j;
		//cerr << i << "->" << j << endl;
	}
}
void hist_eq(uchar *d, int w, int h, int bytesPerLine, const uchar* s, int bytesPerLine2){
	int counter[256] = {0};
	calc_histogram(counter, s, w, h, bytesPerLine2);
	int m[256];
	from_counter_to_map(counter, m);
	{
		// check Idempotent
		int counter2[256] = {0};
		for(int i=0; i<256; i++)
			counter2[m[i]] += counter[i];
		int m2[256];
		from_counter_to_map(counter2, m2);
		for(int i=0; i<256; i++){
			assert(m2[i]==i || counter2[i]==0);
		}
	}
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			int from = *(const uchar*)((const uchar*)s+i* bytesPerLine2 + j);
			*(uchar*)((uchar*)d+i* bytesPerLine + j) = m[from];
		}
	}
}
template<typename T>
T get_max(T* t, int w, int h, int bytesPerLine){
	T r = t[0];
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			T x = *(T*)((const uchar*)t + i*bytesPerLine + j*sizeof(T));
			r = r < x ? x : r;
		}
	}
	return r;
}
template<typename T>
double get_sum(T* t, int w, int h, int bytesPerLine){
	double r = 0;
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			T x = *(T*)((const uchar*)t + i*bytesPerLine + j*sizeof(T));
			r += x;
		}
	}
	return r;
}
template<typename T>
void cum_sum(T* t, int w, int h, int bytesPerLine){
	T x = 0;
	for(int i=0; i<h; i++){
		for(int j=0; j<w; j++){
			T* p = (T*)((const uchar*)t + i*bytesPerLine + j*sizeof(T));
			x += *p;
			*p = x;
		}
	}

}
template<typename T>
void print_star(T* t, int N)
{
	T m = get_max(t, 1, N, sizeof(T));
	cerr << "max:"<< m << endl;
	for(int i=0; i<N; i++){
		int x = t[i]*(80-1)/(m+1);
		char buf[80] = {0};
		for(int j=0; j<x; j++)
			buf[j] = ' ';
		buf[x] = '*';
		buf[x+1] = 0;
		cerr << buf << endl;
	}
}
void print_histogram(uchar *s, int w, int h, int bytesPerLine){
	int counter[256] = {0};
	calc_histogram(counter, s, w, h, bytesPerLine);
	cerr << "sum:"<< get_sum(counter, 1, 256, sizeof(counter[0])) << endl;
	print_star(counter, 256);
	cum_sum(counter, 1, 256, sizeof(counter[0]));
	print_star(counter, 256);
}


void main(){
	// boat fingerprint
	QImage img = QImage("data/fingerprint.png").convertToFormat(QImage::Format_Indexed8);
	cerr << img.width() << "*" << img.height() << "*" << img.depth() << "," << img.format() << endl;
	assert(img.depth()==8);
	QImage tmp(img.width()*2, img.height(), QImage::Format_Indexed8);
	QVector<QRgb> tab;
	tab.resize(256);
	for(int i=0; i<tab.size(); i++)
		tab[i] = qRgb(i, i, i);
	tmp.setColorTable(tab);
	copy(tmp.bits(), img.width(), img.height(), tmp.bytesPerLine(), img.bits(), img.bytesPerLine());
	hist_eq(tmp.bits()+img.width(), img.width(), img.height(), tmp.bytesPerLine(), img.bits(), img.bytesPerLine());
	//print_histogram(tmp.bits(), img.width(), img.height(), tmp.bytesPerLine());
	//print_histogram(tmp.bits()+img.width(), img.width(), img.height(), tmp.bytesPerLine());
	tmp.save("tmp.bmp");
	system("start tmp.bmp");
}

