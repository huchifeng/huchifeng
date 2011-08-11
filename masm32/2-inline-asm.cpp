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

using namespace std;

#include <Windows.h>
#include "QtCore/QCoreApplication"
#include "QtCore/QDebug"
#pragma comment(lib, "QtCore4.lib")
// __imp_??0QDebug@@QAE@W4QtMsgType@@@Z
// __imp_??6QDebug@@QAEAAV0@H@Z
// __imp_??1QDebug@@QAE@XZ

void main(){
	__asm{
		// jmp @F // not supported
		// @@:
		jmp xxx;
	xxx:
	}

	qDebug() << "hello";
	HMODULE hLib = LoadLibrary("QtCore4.dll");
	qDebug() << "hLib:" << hLib;
	const char* (*zlibVersion)() = NULL;
	*(void**)&zlibVersion = GetProcAddress(hLib, "zlibVersion");
	qDebug() << "zLibVersion:" << zlibVersion << "():" << zlibVersion();

	qDebug() << sizeof(qDebug());
test_qDebug:
	{
		QDebug q1(QtDebugMsg);
test_qDebug_1:
		q1 << 99;
test_qDebug_2: ;
	}
test_qDebug_end:


	cerr << rand() << endl;
}

