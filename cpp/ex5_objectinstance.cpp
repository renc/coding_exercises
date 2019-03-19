// default constructor, copy constructor, copy assignement 
// move constructor, move assignemnt ?
// temp object ?


#include <vector>
#include <iostream>
#include <string>

class Foo {
public:
	static int gId; // the next available id

	Foo(int id = Foo::gId++) :m_id(id) {
		std::cout << "Foo(id" << m_id << ")." << std::endl;
	}
	~Foo() {
		std::cout << "~Foo()id" << m_id << "." << std::endl;
	}
	Foo(const Foo& o) {
		m_id = gId++;
		std::cout << "Foo(Foo)id" << m_id << "." << std::endl;
	}
	Foo operator=(const Foo &o) {
		// be careful, i did it wrong before: void operator=(const Foo &o){}
		m_id = gId++;
		std::cout << "Foo.op= id" << m_id << "." << std::endl;
	}
private:
	int m_id = 0; 
};

int Foo::gId = 0; 

Foo func1()
{
	Foo oo;
	return oo;
}
Foo func2()
{ 
	return Foo();
}

int main()
{
	std::cout << "-a-" << std::endl;
	Foo o0;
	std::cout << "-b-" << std::endl;
	Foo o1();
	std::cout << "-c-" << std::endl;
	Foo o2 = Foo();
	std::cout << "-d-" << std::endl;
	Foo o3{};
	std::cout << "-e-" << std::endl;
	Foo o4 = {};
	std::cout << "-f-" << std::endl;

	std::cout << "func1: \n";
	Foo fo1 = func1();
	std::cout << "-g-" << std::endl;
	func1();
	std::cout << "-h-" << std::endl;
	std::cout << "func2: \n";
	Foo fo2 = func2();
	std::cout << "-i-" << std::endl;
	func2();
	std::cout << "-j-" << std::endl;

	getchar();
	return 0; 
}
