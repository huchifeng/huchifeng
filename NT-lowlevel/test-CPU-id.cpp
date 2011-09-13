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

mov eax,0 或者xor eax,eax
输出
EAX=xxxx_xxxxh
得到CPUID指令所支持的最大值 EBX-EDX-ECX 厂家的名称字符串处理器

mov eax ,1
输出
EAX=xxxx_xxxxh
处理器type/family/model/stepping

mov eax ,3
输出
EBX=xxxx_xxxxh 处理器序列号(只只是Transmeta Crusoe)
ECX=xxxx_xxxxh 处理器序列号
EDX=xxxx_xxxxh 处理器序列号
		// 仅当PSN有效时. 才可以返回处理器序号，否则不支持该命令
		psn(processor serial numbers，处理器序列号)

*/

#include <time.h>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <windows.h>


using namespace std;


void main(){
	std::string str;

	int mycpuid[20]={0};         //用于存放CPU厂商ASCII码值
	static int time[2]={0};      //用于存放记数器临时值
	int cpuclock=0;

	__asm
	{
		mov eax,0
			cpuid

			mov mycpuid[0],ebx
			mov mycpuid[4],edx
			mov mycpuid[8],ecx
	}

	str+=(char *)mycpuid;

	__asm
	{
		mov eax,3
			cpuid
		mov eax,0
	}

	//计算主频，方法一
	/*__asm
	{     //存于变量time中.
	rdtsc
	mov ecx,offset time
	mov [ecx+0],edx
	mov [ecx+4],eax
	}
	Sleep(1000);

	__asm
	{
	rdtsc
	mov ebx,offset time
	sub eax,[ebx+4]
	sbb edx,[ebx+0]
	mov ecx,1000000
	div ecx
	mov cpuclock,eax
	}*/

	//计算主频，方法二
	int   tick1,tick2;   
	_asm   rdtsc   
	_asm   mov   tick1,eax   
	Sleep(50);   
	_asm   rdtsc   
	_asm   mov   tick2,eax   
	cpuclock=(tick2-tick1)/50000;

	char buf[MAX_PATH]={0};
	sprintf_s(buf," %d MHZ",cpuclock);
	
	//str+=buf;

	cerr << str << endl << buf;
	return;	
}



