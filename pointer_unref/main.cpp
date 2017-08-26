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
	              
private:           
	int b;                 
};                   
                       
void func3(B *p)              
{                 
	std::cout << "in func3: " << &p << ", " << p << ", " << &(*p) << std::endl;             
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
	     
	B Binstance;        
	std::cout << "init: " << &Binstance << std::endl;           
	func3( &Binstance );           
	func4( Binstance );             

	int input; 
	std::cin >> input;
	return 1;          
}            