#include <iostream>
#include <array>
#include <type_traits>
#include <cstdint> // for uint32_t
#include <cstddef> // for std::byte 
#include <vector>
#include <string>
#include <string_view>
#include <bitset>

uint32_t str2int(std::string_view s)
{
    uint32_t v = {0}; 
    v = v | (uint8_t)(unsigned char)s[0] << 24; 
    v = v | (uint8_t)(unsigned char)s[1] << 16; 
    v = v | (uint32_t)(unsigned char)s[2] << 8; 
    v = v | (uint32_t)(unsigned char)s[3] << 0;
    std::bitset<32> b{v};
    std::cout << "s=" << s << ", v=" << v << ", b=" << b << "\n";
    return v;
}
std::string int2str(uint32_t v)
{
    char c0 = char((v >> 24) & 0xFF);   // i was wrong c0 = v & 0xFF000000;
    char c1 = char(v >> 16 & 0xFF);     // i was wrong c1 = v & 0x00FF0000;
    char c2 = (char)((v & 0x0000FF00) >> 8); // i was wrong c2 = v & 0x0000FF00;
    char c3 = (char)(v & 0xFF);         // i was wrong c3 = v & 0x000000FF;
    std::cout << "c0=" << c0 << ",c1=" << c1 << ",c2=" << c2 << ",c3=" << c3 << ".\n";
    std::string s = "0000";
    s[0] = c0; s[1] = c1; s[2] = c2; s[3] = c3;
    return s;
}
int main() {
    std::string_view s1 = "AABB", s2{"0123"}, s3 = {"AAPL"};
    std::cout << "[" << int2str( str2int(s1) ) << "]\n";
    std::cout << "[" << int2str( str2int(s2) ) << "]\n";
    std::cout << "[" << int2str( str2int(s3) ) << "]\n"; 

    return 0;
}
