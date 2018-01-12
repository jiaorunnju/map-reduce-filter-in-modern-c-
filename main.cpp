#include <iostream>
#include "option.h"
#include <string>
#include <vector>
#include <functional>
#include "composeFunc.h"
#include "simpleLinq.h"
#include "function_traits.h"
using namespace std;
using namespace mylib;

class A {
public:
	int operator() (int x) {
		return x;
	}

	char get(int x) const {
		return 'x';
	}

	int pp(int& x, char y) const {
		return 1;
	}
};

template <typename U>
auto mm(const U& u) {
	auto f = to_stdFunc(u);
	return f;
}

int zz(int x) {
	return x;
}

int main(){
	vector<string> vec = {"1","2","3","4","5","6"};
	vector<int> ppp = { 1,2,3,4,5,6,7,8 };
	auto i = vec.begin();
	auto j = vec.end();
	auto p = ppp.begin();
	auto q = ppp.end();
	auto lamda = [](string& s) {return atoi(s.c_str()); };
	auto f =mm(lamda);
	function<bool(int&)> g = [](int& s) {return s%3==0; };
	function<int(int)> h = [](int a) {return a * 2; };
	linqTemp<string,string,decltype(i)> l(i, j);
	linqTemp<int, int, decltype(p)> m(p, q);
	A a;
	from(i,j).map(lamda).where(g).map(h).map(a).map(zz).print();
	system("pause");
}