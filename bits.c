/* 
 * CS:APP Data Lab 
 * 
 * <Nurzhan Yergozhin p20140926>
 * 
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

 igned float_i2f(int x)Each "Expr" is an expression using ONLY the following:
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
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

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
  } unsigned float_i2f(int x)

FLOATING POINT CODING RULES

For the problems that require you to implent floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants.

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
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
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
/* 
 * bitNor - ~(x|y) using only ~ and & 
 *   Example: bitNor(0x6, 0x5) = 0xFFFFFFF8
 *   Legal ops: ~ &
 *   Max ops: 8
 *   Rating: 1
 */
int bitNor(int x, int y) {
  return ((~x) & (~y));

}

/* 
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
  return  (x << 31) >> 31 ;
}

/* 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {
  return  (!(x ^ y)); 
}

/* 
 * bitMask - Generate a mask consisting of all 1's 
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
 int mask1 = (1 << (highbit)) >> lowbit;
 int mask2 = ~0;
 mask2 = (mask2 << (highbit)) >> lowbit;
 mask2 ^= mask1;
 mask2 = ~mask2;
 mask2 <<= lowbit;
 return mask2;
}

/*
 * bitCount - returns count of number of 1's in word
 *   Examples: bitCount(5) = 2, bitCount(7) = 3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 40
 *   Rating: 4
 */
int bitCount(int x) {
 int mask = 127 + 130 + (1 << 16) + (1 << 24);
 int sum = mask & x;
 int mask2 = ~0;
 int res = 0;
 x >>= 1;
 sum += mask & x;
 x >>= 1;
 sum += mask & x;
 x >>= 1;
 sum += mask & x;
 x >>= 1;
 sum += mask & x;
 x >>= 1; 
 sum += mask & x;
 x >>= 1;
 sum += mask & x;
 x >>= 1;
 sum += mask & x;
 mask2 <<= 8;
 mask2 = ~mask2;								
 res = (sum & mask2) + ((sum >> 8) & mask2)+ ((sum >> 16) & mask2) + ((sum >> 24) & mask2);	
 return res;
}
/* 
 * TMax - return maximum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmax(void) {
  return ~(1 << 31);
}

/* 
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3 unsigned float_i2f(int x)
 */
int isNonNegative(int x) {
  return !(x >> 31);
}

/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  return !((!((x >> 31) ^ (y >> 31))) & ((x >> 31) ^ ((x + y) >> 31))); 
}

/* 
 * rempwr2 - Compute x%(2^n), for 0 <= n <= 30
 *   Negative arguments should yield negative remainders
 *   Examples: rempwr2(15,2) = 3, rempwr2(-35,3) = -3
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int rempwr2(int x, int n) {
 int w = ~0;
 int xx;
 int y;
 int d;
 int sign;
 int t;
 w <<= n;
 w = ~w;
 xx = x & w;
 y = (x & (1 << 31));
 y >>= 31;
 y <<= n;
 y |= xx;
 d = (y >> n) << n;
 sign = !!(y ^ d);
 t = (sign << 31) >> 31;
 return (y & t);  
}

/* 
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {

  int raznica = x + (~y) + 1;
  int minx = x >> 31;
  int miny = y >> 31;

  int odin = (!(minx ^ miny)) & (raznica >> 31);
  int nodin = minx & (!miny);


  return (!!(odin | nodin));

}

/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  int y = x + (~0);
  int w = x & (1 << 31);     
  int z = w  >> 31;
  return (y ^ z) + (!(w));
}

/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
   x |= (!!(x & (1 << 31))) << 16;
   return !((x & (~x + 1)) ^ x) ^ (!(x));
}

/* 
 * float_neg - Return bit-level equivalent of expression -f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   When argument is NaN, return argument.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *  Max ops: 10
 *   Rating: 2
 */
unsigned float_neg(unsigned uf) {
   int x = 1 << 31; 
   int mask = x >> 8;
   mask ^= x;
   
   if ((mask & uf) == mask) {
	 mask |= x;  
	 if (((~mask) & uf)) return uf;
   }
   return uf ^ x;

}

/* 
 * float_half - Return bit-level equivalent of expression 0.5*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_half(unsigned uf) {
 int x = 1 << 31;
 int mask = x >> 8;
 int exp;
 int last;
 int sh;
 mask ^= x;
 
 if ((mask & uf) == mask) {
 	return uf;
 }
 exp = mask & uf;
 exp >>= 23;

 if (exp < 2) {
    mask |= x;
    mask = ~mask;
    
    last = !((3 & uf) ^ 3);
    sh = ((uf & mask) + last) >> 1;
    if (exp == 0) return sh | (x & uf);
    return (x & uf) | (1 << 22) | sh;
 }
/*

 if (exp == 1) {
    mask |= 1 << 31;
    mask = ~mask;
    int last = !((3 & uf) ^ 3);
    int sh = ((uf & mask) >> 1) + last;
    return (x & uf) | (1 << 22) | sh;	      
 }

 */
 exp = exp + (~0); // decrease the exponent power by 1
 exp <<= 23; // move it to the exponent position in the mask
 uf = uf & (~mask);
 uf = uf | exp;

 return uf;
}

/* 
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned float_i2f(int x) {
   int exp = 0;
   int f = 0;
   unsigned ans = x & (1 << 31);
   int bit;
   int bitf;
   int add;
   if (x >> 31) exp = 158;
   if (x << 1) { 
	if (x < 0) x = -x;
	bit = 30;
	while ( !((x >> bit) & 1) ) bit--;
	exp = bit + 127;
	x = x << (31 - bit);
	bitf = (1 << 23) - 1;
	f = bitf & (x >> 8);
	x = x & (255);
	 add = 0;
	if(x > 128 || ((x == 128) && (f & 1)))
		add = 1;
	f += add;
	if(f >> 23) {
	   f &= bitf;
	   exp += 1;										        }
   }
    ans = ans | (exp << 23) | f; 
   return ans;
}




