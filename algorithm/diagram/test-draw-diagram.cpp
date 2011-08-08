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

*/

#include <time.h>
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <assert.h>
using namespace std;

class diagram_t{
public:
	diagram_t(int n): m_n(n)
	{
		k.resize(n * n);
		label.resize(n);
	}
	diagram_t& operator()(int i, int j){
		has_link(i, j) = true;
		return *this;
	}
	diagram_t& operator()(int i, string s){
		label[i] = s;
		return *this;
	}
	int& has_link(int i, int j){
		return k[i * m_n + j];
	}
	void print();
	void print_diagram();
private:
	int m_n;
	vector<int> k; // not to use vector<bool>
	vector<string> label;
};

void diagram_t::print(){
	for(int i=0; i<m_n; i++){
		for(int j=0; j<m_n; j++){
			cerr << (has_link(i, j) ? "+" : " ");
		}
		cerr << endl;
	}
}
void diagram_t::print_diagram(){
	vector<int> max_link(m_n);
	vector<int> min_link(m_n);
	for(int i=0; i<m_n; i++){
		min_link[i] = i;
		for(int j=i-1; j>=0; j--){
			if(has_link(j, i)){
				min_link[i] = j;
			}
		}
		for(int j=i+1; j<m_n; j++){
			if(has_link(i, j)){
				max_link[i] = j;
			}
		}
	}
	for(int i=0; i<m_n; i++){
		for(int j=0; j<i; j++){
			if(!has_link(j, i)){
				cerr << "  ";
				continue;
			}
			if(j==min_link[i]){
				if(max_link[j]==i){
					cerr << "©¸";
					continue;
				}
				cerr << "©À";
				continue;
			}
			if(max_link[j]==i){
				cerr << "©Ø";
				continue;
			}
			cerr << "©à";
		}
		if(label[i]=="")
			cerr << i;
		else
			cerr << label[i];
		cerr << endl;
	}
}
//void diagram_t::print_diagram(){
//	vector<string> s((2*m_n-1) * m_n);
//	vector<int> max_link(m_n);
//	for(int i=0; i<m_n; i++){
//		for(int j=0; j<=i; j++){
//			s[i*(2*m_n)+j] = "©¤";
//			if(i<m_n-1)
//				s[i*(2*m_n)+m_n+j] = "  ";
//		}
//	}
//	for(int i=0; i<m_n; i++){
//		for(int j=i+1; j<m_n; j++){
//			if(has_link(i, j))
//				max_link[i] = j;
//		}
//		s[i*(2*m_n) + i] = (max_link[i] > i ? "©Ð" : "©¤");
//		for(int j=i+1; j<=max_link[i]; j++){
//			s[j*(2*m_n)+i] = (has_link(i, j) ? (j==max_link[i] ? "©Ø" : "©à"): "©¤");
//			s[j*(2*m_n)-m_n+i] = "©¦";
//		}
//	}
//	for(int i=0; i<m_n; i++){
//		for(int j=0; j<m_n; j++){
//			cerr << s[i*(2*m_n) + j];
//		}
//		cerr << i << endl;
//		if(i == m_n-1)
//			break;
//		for(int j=0; j<m_n; j++){
//			//cerr << s[i*(2*m_n)+m_n + j];
//		}
//		//cerr << endl;
//	}
//}

void main()
{
	srand(time(NULL));
	diagram_t d(5);
	d(0, "Zero")(1, "First")(2, "Second")(3, "Third")(4, "Forth");
	d(0, 1)(0, 3)(1, 2)(1,3)(1,4)(2, 4);
	d.print();
	d.print_diagram();
}


