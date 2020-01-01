/* 
 * CS:APP Data Lab 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* We do support the IEC 559 math functionality, real and complex.  */
/* wchar_t uses ISO/IEC 10646 (2nd ed., published 2011-03-15) /
   Unicode 6.0.  */
/* We do not support C11 <threads.h>.  */
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
  /* AND x with 55555555 to check all even bits, compare with 55555555
   * if they are equal then the even bits are all one
   * if they aren't equal then not all even bits are 1
   * */
  int mask = 0x55;
  int maskFull = mask | (mask << 8) | (mask << 16) | (mask << 24);
  return !((x & maskFull)^maskFull); 
}
/* 
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int bang(int x) {
  /*set y to the sign bit of x repeated 32 times
   * set z to the sign bit of negation of x + 1 repeated 32 times
   * OR these two numbers and add 1. If x was zero the OR will result in 0
   * if x was non-zero the OR will result in 11...111 which adding 1 will overflow back to 0*/
  int y = x >> 31;
  int z = (~x + 1) >> 31;
  return (y | z) + 1;
}
/* 
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 */
int floatIsEqual(unsigned uf, unsigned ug) {
  /*get the exponent of each by capturing only those 8 bits and shifting to the right
   * get the mantissa of each by capturing the last 23 digits
   * if the exponent of either is maximum and that number's mantissa is not zero that is NaN
   * if the unsigned numbers are equal or both mantissas and exponents are zero, the numbers are equal
   * */
  int expF = (uf & 0x7F800000) >> 23;
  int expG = (ug & 0x7F800000) >> 23;
  int manF = (uf & 0x007FFFFF);
  int manG = (ug & 0x007FFFFF);
  int manGZero = (manG == 0);
  int manFZero = (manF == 0);
  if ((expF == 0xFF && (!manFZero)) || (expG == 0xFF && (!manGZero))){
    return 0;
  }
  return ((uf == ug) || ((manFZero) && (manGZero) && (expF == 0) && (expG == 0)));
} 
/* 
 * floatUnsigned2Float - Return bit-level equivalent of expression (float) u
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatUnsigned2Float(unsigned u) {
  /*printf("%u\n", u);
  int fraction = 0;
  int exp = 0;
  unsigned result = 0;
  int j = 0;
  unsigned x = u;
  for (x = u; x > 0; x /= 2){
    fraction = fraction | ((x % 2) << j);
    j += 1;
  }
  printf("fraction %x\n", fraction);
  exp = j;
  if (exp > 0){
    exp = exp - 1 + 127;
  }
  if (fraction != 0){
    j = 32 - j + 1;
  }
  int removed = fraction & 0x1FF;
  printf("removed: %x\n", removed);
  fraction = fraction << j;
  fraction = fraction >> 9;
  fraction = fraction & 0x7FFFFF;*/
  /*int tieAndOdd = (removed == (1 << 8)) && (fraction & 1);
  int maxAndOdd = (removed == 0x1FF) && (fraction & 1);
  int notMaxNotTie = (removed > (1 << 8)) && (removed != 0x1FF) && (fraction & 1);
  int notGSticky = (removed < (1 << 8)) && ((removed & 0x7F) > 0);
  printf("%d\n", tieAndOdd);
  printf("%d\n", maxAndOdd);
  printf("%d\n", notMaxNotTie);
  printf("%d\n", notGSticky);
  if (tieAndOdd || maxAndOdd || notMaxNotTie){
    printf("append %d\n", 0);
    fraction += 1;
  }*/
  /*int Guard = fraction & 1;
  int Round = removed >= (1 << 8);
  int Sticky = (removed & 0x7F) != 0;
  printf("Guard: %d\n", Guard);
  printf("Round: %d\n", Round);
  printf("Sticky: %d\n", Sticky);
  if ((Guard && Round && Sticky) || (Guard && Round && (!Sticky))){
    printf("Increment %d\n", 0);
    fraction += 1;
  }
  printf("J %d\n", j);
  printf("fraction 2 %x\n", fraction);
  printf("final fraction %x\n\n", fraction);
  result = (exp << 23) | fraction;
  return result;*/
  return 2;
}
/* 
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {
  /* determine if same signs
   * a is true if they are not the same signs andx is negative
   * b is true if they are the same size but x - y is negative
   * if either is true x is smaller than y
   * */ 
  int sameSigns = !((x >> 31)^(y >> 31));
  int a = (!sameSigns) & ((x >> 31)^0);
  int b = sameSigns & !!((x + (~y + 1)) >> 31);
  return a | b;
}
/* 
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int isNonNegative(int x) {
  /*return the opposite of X's sign bit */
  return !((x >> 31) & 1);
}
/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  /* x added to iself and 1 will equal itself if x is 011...111 or 111..111, a is 1 if x is either of these numbers
   * b is 1 if x is not 111...111 so that leaves us with only 011...111, the largest 2's complement number*/ 
  int a = !((x + 1 + x) ^ (~0));
  int b = !!(x^(~0));
  return a & b;
}
/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
  /* a is 32 - n
   * b is FFFFFFFF
   * shift b and get the complement to pull just the last digits
   * combine x shifted left by n and x shifted right by n AND'd with b to get the rotated value
   */
  int a = 32 + (~n + 1);
  int b = 0xFF;
  b = b | (b << 8) | (b << 16) | (b << 24);
  b = b << n;
  b = ~b;
  return (x << n) | ((x >> a) & b);
}
/* 
 * signMag2TwosComp - Convert from sign-magnitude to two's complement
 *   where the MSB is the sign bit
 *   Example: signMag2TwosComp(0x80000005) = -5.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 4
 */
int signMag2TwosComp(int x) {
  /*determine sign
   * determine magnitude
   * determine inverted magnitude(if negative)
   * return magnitude if positive or the inverted magnitude if negative
   * */
  int a = 1 << 31;
  int sign = x & (a);
  int mag = 0;
  int invMag = 0;
  sign = sign >> 31;
  mag = x & (~(a));
  invMag = ~mag + 1;
  return ((~(sign^0)) & mag) | ((~(sign^(~0))) & invMag);
}
/* 
 * thirdBits - return word with every third bit (starting from the LSB) set to 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 8
 *   Rating: 1
 */
int thirdBits(void) {
  /* return 49249249
   * */
  int a = 0x49;
  int b = 0x24;
  int c = 0x92;
  return (a << 24) | (b << 16) | (c << 8) | a;
}
