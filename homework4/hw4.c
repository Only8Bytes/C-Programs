#include <stdlib.h>
// rotate the values pointed to by three pointers
// so values move from xp to yp, yp to zp and zp to xp
void rotate(int *xp, int *yp, int *zp) {
  //store value of xp temporarily, rotate zp and yp, replace yp with the temporary value
  int temp = *xp;
  *xp = *zp;
  *zp = *yp;
  *yp = temp;
  return;
}

// Write a function that returns 0 if x is 0, returns -1 
// if x < 0, returns 1 if x > 0
// Your code must follow the Bit-Level Integer Coding Rules
// on the textbook (located between hw 2.60 and 2.61).
// You can assume w = 32. 
// The only allowed operations in your code are:
// ! ~ & ^ | + << >>
// This requirement is more restrictive than the coding rules. 

int sign(int x) {
  //!x to determine if x == 0
  //x & (1 << 31) to AND x with 100..000 to see if x is negative
  //if x is not zero and x is positive then return 1
  if (!x){
    return 0;
  } else if (x & (1 << 31)){
    return -1;
  } else {
    return 1;
  }
}

// Given a source string, and a destination string, write the
// reversed source string into destination. Make sure you terminate
// the reversed string properly with an integer value 0.
// You may assume that the destination array is big enough to hold
// the reversed string. 
void reverse(const char source[], char destination[]) {
  int i;
  int len = 0;
  //declare i and len 
  //iterate through source until termination to get length of string 
  for (i = 0; source[i] != 0; i++){
    len++;
  }
  //iterate through source string and place characters in destination in reverse
  for (i = 0; i < len; ++i){
    destination[i] = source[len - i - 1];
  }
  //add termination to end of destination string
  destination[len] = 0;
  return;
}
