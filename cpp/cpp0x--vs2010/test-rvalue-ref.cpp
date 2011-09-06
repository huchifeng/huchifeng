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
 lvalues and rvalues in C++98/03.
 "Every expression is either an lvalue or an rvalue."  
  lvalueness versus rvalueness is a property of expressions, not of objects.
  obj , *ptr , ptr[index] , and ++x are all lvalues.
  1729 , x + y , std::string("meow") , and x++ are all rvalues
  The address-of operator requires that its "operand shall be an lvalue" (C++03 5.3.1/2).
  A function call is an lvalue if and only if the result type is a reference.
  Both lvalues and rvalues can be either modifiable (non-const) or non-modifiable (const).

  unnamed lvalue references are also lvalues
    v[0] returns int& , an unnamed lvalue reference whose address you can take

  Type& binds to modifiable lvalues (and can be used to observe and mutate them). 
  It can't bind to const lvalues, as that would violate const correctness. 
  It can't bind to modifiable rvalues , as that would be extremely dangerous.
  //  VC has an evil extension that allows this, but if you compile with /W4, ...

  const Type& binds to everything

  a reference bound to an rvalue is itself an lvalue 

  void observe(const string& str)
  observe("purr")  // within observe(), str is a name, so it's an lvalue. 

  non-const member functions can be called on modifiable rvalues

  returning by value should return Type instead of const Type


  some copy constructor calls are elided by the RVO and NRVO, but the compiler isn't always able to apply them. 
  Move constructors optimize the remaining cases.

  // the forwarding problem
template <typename T> void outer(const T& t) {    inner(t);} // for inner(const T&) // e.g. inner(5)
template <typename T> void outer(T& t) {    inner(t); } // for inner(T&)
// (VC9 SP1's tr1::bind() is desperate enough to do this for the first 5 arguments, involving 2^6-1=63 overloads. 
std::identity and std::forward()  solve this
template <typename T> struct Identity {
    typedef T type;
}; 
template <typename T> T&& Forward(typename Identity<T>::type&& t) {
    return t;
}
template <typename T1, typename T2> void outer(T1&& t1, T2&& t2) {
    inner(Forward<T1>(t1), Forward<T2>(t2));
}
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

class A{
public:
	A(){ i=0; }
	void f(){ i++; cerr << "A::f " << i << endl; }
	int i;
};
A g(){
	A a;
	return a;
}
void main1()
{
	g().f(); // A::f() not need to be const, even under /W4 it's OK
	A& a = g(); // when /W1, no warning
	const A& a2 = g(); // it's OK even /W4
}
// --------------------
string modifiable_rvalue() {
    return "modifiable_rvalue";
}
 
const string const_rvalue() {
    return "const_rvalue";
}
void meow(string& s) {
    cout << "meow(string&): " << s << endl;
}
 
void meow(const string& s) {
    cout << "meow(const string&): " << s << endl;
}
 
void meow(string&& s) {
    cout << "meow(string&&): " << s << endl;
}
 
void meow(const string&& s) {
    cout << "meow(const string&&): " << s << endl;
}
 
void main2(){
    string modifiable_lvalue("modifiable_lvalue");
    const string const_lvalue("const_lvalue");
	meow(modifiable_lvalue); // if not exists, prefer to convert to (const string&), cannot use (string&&) 
	meow(const_lvalue);
	meow(modifiable_rvalue());
	meow(const_rvalue()); // const T&& can -> const T&
 }

class B{
public:
	B() {
        cout << "Default constructor." << endl; 
        m_p = NULL;
    } 
    explicit B(const int n) {
        cout << "Unary constructor." << endl; 
        m_p = new int(n);
    } 
    B(const B& other) {
        cout << "Copy constructor." << endl; 
        if (other.m_p) {
            m_p = new int(*other.m_p);
        } else {
            m_p = NULL;
        }
    }
    B(B&& other) 
		// move all members; e.g.  member1(std::move(other.member1));
	{
        cout << "MOVE CONSTRUCTOR." << endl; 
        m_p = other.m_p;
        other.m_p = NULL;
    }
    B& operator=(const B& other) {
        cout << "Copy assignment operator." << endl;
         if (this != &other) {
            delete m_p; 
            if (other.m_p) {
                m_p = new int(*other.m_p);
            } else {
                m_p = NULL;
            }
        } 
        return *this;
    }
    B& operator=(B&& other) {
        cout << "MOVE ASSIGNMENT OPERATOR." << endl; 
        if (this != &other) {
            delete m_p;
 
            m_p = other.m_p;
            other.m_p = NULL;
        } 
        return *this;
    }
    ~B() {
        cout << "Destructor." << endl;
 
        delete m_p;
    }
 
    int get() const {
        return m_p ? *m_p : 0;
    }
private:
	int *m_p;
};
B square(const B& r) {
    const int i = r.get();
    return B(i * i);
}
B square2(const B& r) {
    const int i = r.get();
	B b(i*i);
    return b;
}




void main(){
	cout << "-------------\n";
	{
		B b(8);
	}
	cout << "-------------\n";
	{
		B b(9);
		b = square(B(9)); // MOVE ASSIGNMENT !
	}
	cout << "-------------\n";
	{
		B b = (square(B(9))); // just 2 construct, save 1
	}
	cout << "-------------\n";
	{
		B b = (square2(B(9))); // MOVE CONSTRUCTOR !
		B b2 = std::move(b); // MOVE CONSTRUCTOR !
		cout << b.get() << "," << b2.get() << endl; // b lose it's pointer
	}
}

