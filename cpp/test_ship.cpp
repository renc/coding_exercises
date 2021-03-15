#include <cstdlib> 
#include <cstring> 
#include <string> 
#include <iostream>
#include <sstream> // for std::istringstream 
#include <assert.h>
#include <string>
// you can use includes, for example:
// #include <algorithm>

// you can write to stdout for debugging purposes, e.g.
// cout << "this is a debug message" << endl;
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std; 

void printVectorString(const std::vector<std::string> a);
std::vector<std::string> stringSplit(std::string input_string, char delimiter);
void readPosition(std::string cell, int& r, int& c);

class Ship {
public:
    Ship() {}
    Ship(std::string s, int id) : m_shape(s), m_iId(id) {}
    std::string shape() const { return m_shape; };
    int getId() const { return m_iId; }
    void setSize(int s) { m_iSize = s; }
    int getSize() const { return m_iSize; }
    void addHit() { ++m_iHit; }
    int getHit() const { return m_iHit; }
private:
    std::string m_shape;
    int m_iId = -1;
    int m_iSize = -1;
    int m_iHit = 0; // num of be hit
};

class Map {
public:
    Map(int N) : m_iLength(N) {
        m_aCell.resize(N + 1);
        for (int i = 0; i < m_aCell.size(); ++i) {
            m_aCell[i] = std::vector<int>(N + 1, -1); // no ship 
        }
    }
    void addShip(Ship& ship) {
        std::string sShipShape = ship.shape();
        std::vector<std::string> cells
            = stringSplit(sShipShape, ' ');
        printVectorString(cells);

        // ship area
        if (cells.size() == 2) {
            std::string sTopLeft = cells[0];
            int top = -1, left = -1;
            readPosition(sTopLeft, top, left);

            std::string sBottomRight = cells[1];
            int bottom = -1, right = -1;
            readPosition(sBottomRight, bottom, right);

            int iShipSize = 0; 
            if (top != -1 && left != -1 && bottom != -1 && right != -1)
            {
                // put the ship on the map 
                for (int row = top; row <= bottom; ++row) {
                    for (int column = left; column <= right; ++column) {
                        if (row <= m_iLength && column <= m_iLength) {
                            m_aCell[row][column] = ship.getId();
                            ++iShipSize;
                        }
                    }
                }
            }
            ship.setSize(iShipSize);
        }
    }
    void addBomb(std::string sBomb, std::vector<Ship*> aShip) {
        int r = -1, c = -1;
        readPosition(sBomb, r, c);
        if (r != -1 && c != -1 && r <= m_iLength && c <= m_iLength) {
            int iShipId = m_aCell[r][c];
            if (iShipId != -1) {
                if (iShipId < aShip.size() && aShip[iShipId])
                    aShip[iShipId]->addHit();
            }
        }
    }
private:
    const int m_iLength = -1; // ==N+1 
    std::vector<std::vector<int> > m_aCell;
};

string solution(int N, string& S, string& T) {
    // write your code in C++14 (g++ 6.2.0)
    Map map(N);

    // load the ship into the map 
    std::vector<std::string> aShipString = stringSplit(S, ',');
    printVectorString(aShipString);
    std::vector<Ship*> aShip;
    for (int i = 0; i < aShipString.size(); ++i) {
        Ship* pShip = new Ship(aShipString[i], i);
        aShip.push_back(pShip);

        map.addShip(*pShip);
    }

    // load the hip into the map 
    std::vector<std::string> aBombString = stringSplit(T, ' ');
    for (auto e : aBombString) {
        map.addBomb(e, aShip);
    }

    // count
    int countSunk = 0, countHitNotSunk = 0; 
    for (int i = 0; i < aShip.size(); ++i) {
        Ship* pShip = aShip[i];
        if (!pShip) continue;
        if (pShip->getHit() >= pShip->getSize()) {
            ++countSunk;
        }
        else if (pShip->getHit() > 0) {
            ++countHitNotSunk;
        }
    }
    std::string result = std::to_string(countSunk) + "," + std::to_string(countHitNotSunk);
    return result;
}

void main()
{
    {
        int N = 4;
        std::string S = "1B 2C,2D 4D";
        std::string T = "2B 2D 3D 4D 4A";
        string sol = solution(N, S, T);
        std::cout << "sol=" << sol << std::endl;
    }
}

void printVectorString(const std::vector<std::string> a)
{
    std::cout << "size=" << a.size() << ".\n";
    for (int i = 0; i < a.size(); ++i) {
        std::cout << "s" << i << "=" << a[i] << ", ";
    } std::cout << std::endl;
}
std::vector<std::string> stringSplit(std::string input_string, char delimiter) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [](const char& x, const char& y) {
        return x == y && x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }
    std::vector<std::string> splits;
    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}
void readPosition(std::string s, int& r, int& c)
{
    int i = 0;
    for (; i < s.size(); ++i) {
        if ('A' <= s[i] && s[i] <= 'Z') {
            c = s[i] - 'A' + 1; // 'A' = 1, 'Z' = 26
            r = std::stoi(s.substr(0, i));
        }
    }
}

