//
// Bit field 
// 
// 

#include <cstdio>
#include <bitset> 

// 交通繁忙时间  
enum CallTariff { tOffPeak, tMediumRate, tPeakTime } ; 

// 账单 
struct Bill 
{
	unsigned int	iNameId;    // 客户名 
	CallTariff		eTariff; 
}; 

struct BillVIP 
{
	unsigned int	iNameId;    // 客户名 
	bool			isVIP; 
	CallTariff		eTariff; 
}; 
 
// optimization 1. 
namespace Op1 {
	
	struct Bill 
	{
		unsigned int iNameId;
		unsigned int iTraffic : 2; // range: [0, 3]; 
	}; 
	
	struct Bill2 
	{
		unsigned int iNameId;
		unsigned int iTraffic : 2; // range: [0, 3]; 
		unsigned int iTraffic2 : 3; // range: [0, 8]; 
	}; 

	struct BillVIP 
	{
		unsigned int	iNameId;    // 客户名 
		bool			isVIP;
		unsigned int	iTariff : 2; 
	};
	struct BillVIP2 
	{
		unsigned int	iNameId;    // 客户名 
		unsigned short	isVIP    : 1; 
		unsigned short	iTariff : 2; 
	};
	struct BillVIP3 
	{
		unsigned int	iNameId;    // 客户名 
		unsigned int	isVIP    : 1; 
		unsigned short	iTariff : 2; 
	};
	struct BillVIP4 
	{
		unsigned int	iNameId : 3;    // 客户名 
		unsigned int	isVIP   : 1; 
		unsigned int	iTariff : 2; 
	};
	struct BillVIP5
	{
		unsigned short	iNameId : 13;    // 客户名 
		unsigned short	isVIP   : 1; 
		unsigned short	iTariff : 2; 
	};
	struct BillVIP6
	{
		unsigned short	iNameId : 8;    // 客户名 
		unsigned short	isVIP   : 7; 
		unsigned short	iTariff : 7; 
	};
	class BillVIP7
	{
	public:
		unsigned short	iNameId : 8;    // 客户名 
		unsigned short	isVIP   : 7; 
		unsigned short	iTariff : 7; 
	};

}; 

int main()
{
	// to check the sizeof some basic types. 
	CallTariff t;	
	printf("sizeof CallTariff: %d, %d\n", sizeof(CallTariff), sizeof(t)); 

	printf("sizeof bool: %d\n", sizeof(bool)); 

	{
		Bill b; 
		printf("sizeof Bill: %d, %d\n", sizeof(Bill), sizeof(b));  

		BillVIP bVIP; 
		printf("sizeof BillVIP: %d, %d\n", sizeof(BillVIP), sizeof(bVIP));  
	}

	{
		printf("\n----op1----\n");  
		
		Op1::Bill b; 
		b.iTraffic = CallTariff::tOffPeak; 
		printf("sizeof Bill: %d, %d\n", sizeof(Op1::Bill), sizeof(b));  

		Op1::Bill2 b2; 
		b2.iTraffic = CallTariff::tOffPeak; 
		printf("sizeof Bill2: %d, %d\n", sizeof(Op1::Bill2), sizeof(b2));

		
		Op1::BillVIP bVIP; 
		bVIP.isVIP = true;
		printf("sizeof BillVIP: %d, %d\n", sizeof(Op1::BillVIP), sizeof(bVIP)); 
		
		Op1::BillVIP2 bVIP2; 
		bVIP2.isVIP = true;
		printf("sizeof BillVIP2: %d, %d\n", sizeof(Op1::BillVIP2), sizeof(bVIP2)); 
		
		Op1::BillVIP3 bVIP3; 
		printf("sizeof BillVIP3: %d\n", sizeof(Op1::BillVIP3));
		Op1::BillVIP4 bVIP4; 
		printf("sizeof BillVIP4: %d\n", sizeof(Op1::BillVIP4)); 
		Op1::BillVIP5 bVIP5; 
		printf("sizeof BillVIP5: %d\n", sizeof(Op1::BillVIP5)); 
		Op1::BillVIP6 bVIP6; 
		printf("sizeof BillVIP6: %d\n", sizeof(Op1::BillVIP6)); 
		Op1::BillVIP7 bVIP7; 
		printf("sizeof BillVIP7: %d\n", sizeof(Op1::BillVIP7)); 
	}

	{
		printf("---- std::bitset ---- \n");
		const unsigned int iFCount = 100000;
		std::vector<bool>
	} 

	return 1; 
}