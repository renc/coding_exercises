// Test case of char array vs char pointer. 
// 

#include <iostream>   

int main()     
{       
	char strA[] = "char array!";          
	char *strP = "char array?";        
	        
	std::cout << "sizeof(strA): " << sizeof(strA) << ", of: " << strA << ", addr: " << &strA << std::endl;          
	std::cout << "sizeof(strA): " << sizeof(strA) << ", of: " << &strA[0] << ", addr: " << &strA << std::endl;           
	std::cout << "sizeof(strP): " << sizeof(strP) << ", of: " << strP << ", addr: " << &strP << std::endl;            
	std::cout << "sizeof(strP): " << sizeof(strP) << ", of: " << *strP << ", addr: " << &strP << std::endl;          
	          
	strA[0] = 'C';              
	strP[0] = 'C';           
	           
	return 1;            
}      
