#include <iostream>
#include <type_traits>

class Base
{
public:
	int m_iVal = 1;
};

void func1(Base& b) {
	std::cout << "func1.\n";
}
void func2(Base&& b) {
	std::cout << "func2.\n";
} // no type deduction, so para is rvalue reference. 
 
int main()
{
	Base b;
	{
		// 
		//func2(b);  // Error: an ravalue reference cannot be bound to an lvalue. 
	}
	{
		Base& b1 = b; 
		//func2(b1);   // Error: an rvalue reference cannot be bound to an lvalue.
	}
	{
		//Base&& b2 = b; // Error: an rvalue reference cannot be bound to an lvalue.
		Base&& b3 = std::move(b);
		//func2(b3); // error: You cannot bind an lvalue to an rvalue reference
	}
	{ 
		func2(std::move(b)); // V
	}
	{
		func1(std::move(b)); // V. but why lvalue reference accept rvalue. 
		// from [Effective Modern C++],  std::move() result an rvalue. 
		// This also makes sense for object copyable but not movable. 
	}
	return 1;
}