#include <assert.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <atomic>
#include <tuple> 

namespace om 
{
    // fake omnet, 
    #pragma pack(1)
    struct BroadcaseHeader
    {
        struct BroadcastType
        {
            char type[3]; // bo5 or bd6 
        };
        BroadcastType bcType;
        std::uint16_t items;
        std::uint16_t size;
    };
    struct SubItemHeader
    {
        std::uint16_t nameStructure;
        std::uint16_t size;
    };
    struct TransactionLayout
    {
        // name structure = 33001
        std::uint8_t seqnum;
        char client[5];
        std::uint16_t orderId;
        char status; // n, m, c
    };
    struct ChangeLayout
    {
        // name struct = 33002
        std::uint8_t seqnum;
        std::uint16_t orderId;
        char status; 
    };
    #pragma pack()
    std::ostream& operator<<(std::ostream& io, const TransactionLayout& t)
    {
        io << "TransactionLayout[seqnum=" << std::to_string(t.seqnum) 
           << ", client[5]=" << std::string(t.client, sizeof(t.client))
           << ", orderId=" << std::to_string(t.orderId) << ", status=" << t.status << "]\n";
        return io;
    }
    std::ostream& operator<<(std::ostream& io, const ChangeLayout& c)
    {
        io << "ChangeLayout[seqnum=" << std::to_string(c.seqnum) << ", orderId=" << std::to_string(c.orderId) 
           << ", status=" << c.status << "]\n";
        return io;
    }
// BO5
// BroadcaseHeader; SubItemHeader item;  SubItemHeader item; 
}

int main()
{
    {
        om::SubItemHeader subItem1;
        subItem1.nameStructure = 33001;
        subItem1.size = sizeof(om::SubItemHeader) + sizeof(om::TransactionLayout);
        om::TransactionLayout t;
        t.seqnum = 1; 
        t.client[0]='u';t.client[1]='s';t.client[2]='e';t.client[3]='r';t.client[4]='1';
        t.orderId = 12345;
        t.status = 'n';

        om::SubItemHeader subItem2;
        subItem2.nameStructure = 33002;
        subItem2.size = sizeof(om::SubItemHeader) + sizeof(om::ChangeLayout);
        
        om::ChangeLayout c;
        c.seqnum = 2;
        c.orderId = 12346;
        c.status = 'c';

        om::BroadcaseHeader bHeader;
        std::cout << "sizeof(bHeader.bcType.type)=" << sizeof(bHeader.bcType.type) << std::endl;
        std::memcpy(bHeader.bcType.type, "bo5", sizeof(bHeader.bcType));
        bHeader.items = 2;
        bHeader.size = sizeof(om::BroadcaseHeader) + subItem1.size + subItem2.size;
        std::cout << "total size of msg=" << bHeader.size << std::endl;

        // socket send(fd, void * or char*, len); // i google,most of time use char * or char[].
        char* pSendBufBHeader = reinterpret_cast<char*>(&bHeader);
        char* pSendBufSubItem1Header = reinterpret_cast<char*>(&subItem1);
        char* pSendBufSubItem1 = reinterpret_cast<char*>(&t);
        char* pSendBufSubItem2Header = reinterpret_cast<char*>(&subItem2);
        char* pSendBufSubItem2 = reinterpret_cast<char*>(&c);
        // pretend these buf are sent to socket. 

        char recvBuf[1024] = {'\0'};
        std::size_t pos = 0;
        std::memcpy(recvBuf + pos, pSendBufBHeader, sizeof(om::BroadcaseHeader));
        pos += sizeof(om::BroadcaseHeader);
        std::memcpy(recvBuf + pos, pSendBufSubItem1Header, sizeof(om::SubItemHeader));
        pos += sizeof(om::SubItemHeader);
        std::memcpy(recvBuf + pos, pSendBufSubItem1, sizeof(om::TransactionLayout));
        pos += sizeof(om::TransactionLayout);
        std::memcpy(recvBuf + pos, pSendBufSubItem2Header, sizeof(om::SubItemHeader));
        pos += sizeof(om::SubItemHeader);
        std::memcpy(recvBuf + pos, pSendBufSubItem2, sizeof(om::ChangeLayout));
        pos += sizeof(om::ChangeLayout);
        std::cout << "recv buf length=" << pos << std::endl;

        // now with the recv buf, need to convert them back to structure 
        {
            // test with char* 
            // https://github.com/isocpp/CppCoreGuidelines/issues/1203 
            om::BroadcaseHeader* pRecvBHeader = reinterpret_cast<om::BroadcaseHeader *>(recvBuf);
            om::SubItemHeader* pRecvSubItemHeader1 = reinterpret_cast<om::SubItemHeader *>(pRecvBHeader + 1);
            om::TransactionLayout* pRecvT = reinterpret_cast<om::TransactionLayout *>(pRecvSubItemHeader1 + 1);
            om::SubItemHeader* pRecvSubItemHeader2 = reinterpret_cast<om::SubItemHeader *>(pRecvT + 1);
            om::ChangeLayout* pRecvC = reinterpret_cast<om::ChangeLayout *>(pRecvSubItemHeader2+1);
            std::cout << *pRecvT;
            std::cout << *pRecvC;
        } 
        {
            // test with std::span
        }
    }
    return 0; 
}
