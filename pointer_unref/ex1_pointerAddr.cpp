// try to check if a pointer will be copied during passing-by-pointer. 
//
#include <iostream>             
                   
void func1(int *p)           
{               
	std::cout << "in func1: " << *p << ", " << &p << ", " << p << ", " << &(*p) << std::endl;                 
	*p = 1;                
}                      
void func2(int &p)                    
{            
	std::cout << "in func2: " << p << ", " << &p << std::endl;        
	p = 2;            
}          
            
class B             
{          
public:               
	B() : b(0) {};                  
	B(const B &_inB) { b = _inB.b; }             
	void operator=(const B &_inB) { b = _inB.b; }           
	int getB() const { return b; };
	void setB(int _b) { b = _b; }
private:           
	int b;                 
};                   
                       
void func3(B *p)              
{
	std::cout << "in func3: " << &p << ", " << p << ", " << &(*p) << std::endl;
	p = new B; // 指向新的一个实例了, 对调用方没影响. 
	std::cout << "in func3: " << &p << ", " << p << ", " << &(*p) << std::endl;
	p->setB(1);
}                          
void func4(const B &p)                
{                                
	std::cout << "in func4: " << &p << std::endl;             
}              
              
int main()            
{                
	int a = 0;                  
	std::cout << "init: " << a << ", " << &a << std::endl;        
	func1( &a );      
	std::cout << "a = " << a << std::endl;
	func2( a );
	std::cout << "a = " << a << std::endl;
	     
	B bInstance;        
	std::cout << "init: " << &bInstance << std::endl;
	func3( &bInstance);
	func4(bInstance);
	std::cout << "b = " << bInstance.getB() << std::endl;

	int input; 
	std::cin >> input;
	return 1;          
}            