notes

20231216 
lc1, two sum. 
lc2,
lc3, 
20231228 
lc10. regular regex match  char . for any, char * for zero/many prev char;
(aa, a)  is false
(aa, a*)  is true
(ab, .*) is true
solution: recursive, move left pointer and right pointer, 
solution: dp top-down 

lc11. container with most water 
solution: two pointers, left and right pointers, move the pointer if the area become bigger after move.

lc12. integer to roman. 
solution. build a map from integer (1000, 900, 500,...) to roman chars; integer / big num = count, this count is the roman chars; then integer / smaller num = count again, recurisve
lc13. roman to integer. 
solution. reverse of lc12, map the roman to base (1000, 900, 500, ...), then integer += integer from left to right. 

lc14. longest common prefix of several strings.
solution. one char move step by step. easy. 

lc15. 3sum to == given target. 
for 3sum, the simplest is triple loops. but to avoid duplicate, sort the input array first. three pointers a,b,c  
for a from 0 to n-1
    while b from a+1 to n-1  < c from n-1 to b+1    // once a is set, b, c are left right pointers
        check (a, b, c) , move b increase or move c decrease. 

lc16. 3sum closest
simple solution n*(n-1)*(n-2) which is O(n^3), 
another solution O(n^2).  sort array, O(n*lgn); similar to lc15, three pointers, to increase b or decrease c;

lc18. 4sum to == given target.
sort input array, to detect duplicated value. 
...

lc22. generate parentheses given n, generate () *n; 
backtrack 

lc30. substring with contatenation of all words. hard. 
two pointers and sliding windows (input word.len *  input words.count), the words inside the windows should count each input word 1 times.

lc31. next permutation
(1,2,3) -> this is beginning, ascending order 
(1,3,2) ->                                                            Every time, in reversed (3,2), move backward. 
(2,1,3) -> 
(2,3,1) ->                                                            Every time, in reversed (3,1), move backward. 
(3,1,2) -> 
(3,2,1)    this is reversed order means the end of these three values.

one more example 
1,2,3,4,
      i     where a[i-1]<a[i], then a[i, n-1] in reversed order 
1,2,4,3,  here 4,3 this is reversed order,
    i,      where a[i-1]<a[i], then a[i, n-1] in reversed order 
      j     from right to left, find the first one a[j]>a[i], 
            swap(a[i-1],a[j])-> (1,3,4,2);
                                     i   ; reverse(a[i] to a[n-1]) -> (1,3,2,4)
1,3,2,4,   
1,3,4,2,
    i 
    j  
            swap(a[i-1],a[j]) -> (1,4,3,2); check my onenote, there is pic
1,4,2,3,
1,4,3,2,
2,1,3,4,
2,1,4,3, 
2,3,1,4,
2,3,4,1,
2,4,1,3,
2,4,3,1,
3,1,2,4,
... 


how many permutations ? n*(n-1)*(n-2)... = n! 
lc46. permuations 