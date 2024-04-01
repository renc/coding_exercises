#include <vector>
#include <assert.h>

int min_refills(int start_pos, int finish_pos, int range, const std::vector<int>& tap_positions) {
    // please implement this function...
    int next_pos = start_pos + range;
    if (next_pos >= finish_pos) return 0;
    int count = 0;
    int ii = 0;
    for (ii = 0; ii < tap_positions.size(); ++ii) // fix 3, bug that ii <= tap_positions.size()-1 when sz==0,
    {
        if (tap_positions[ii] > next_pos)
        {
            // first one > 
            if (ii > 0) {
                // tap_positions[ii-1] is the new start point
                next_pos = tap_positions[ii-1] + range;
                ++count;
                if (next_pos >= finish_pos) 
                {
                    return count;
                } 
                else if (next_pos < tap_positions[ii]) { // fixed 1. 
                    return -1;
                }
            }
            else
                return -1; 
        }
        else if (tap_positions[ii] == next_pos)
        {
            ++count;
            next_pos += range;
            if (next_pos >= finish_pos)
                return count;
        }
    }
    if (ii == tap_positions.size() && next_pos >= tap_positions.back())// fixed 2
    {
        ++count;
        next_pos = tap_positions.back() + range;
        if (next_pos >= finish_pos) 
        {
            return count;
        } 
        else if (next_pos < tap_positions[ii]) {
            return -1;
        } 
    }
    return -1;
}

int main(int argc, char **argv)
{ 
    assert(min_refills(10, 25, 5, std::vector<int>{ 13, 17, 18, 21, 22, 30 }) == 3);
    assert(min_refills(10, 21, 10, std::vector<int>{0, 9, 10, 21, 22}) == -1); /// fix 1
    assert(min_refills(10, 25, 5, std::vector<int> {13, 16, 19, 22}) == 4); // fix 2
    return 0;
}