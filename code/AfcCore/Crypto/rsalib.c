#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
//#include "GLOBAL.H"
//#include "RSAREF.H"
//#include "R_RANDOM.H"
//#include "RSA.H"
//#include "NN.H"
//#include "DIGIT.H"

//#include "RSALib.h"

typedef unsigned long int UINT4;
typedef unsigned short int UINT2;
typedef UINT4 NN_DIGIT;
typedef UINT2 NN_HALF_DIGIT;

#define uint unsigned int
#define uchar unsigned char

#define NN_DIGIT_BITS 32 
#define NN_HALF_DIGIT_BITS 16
#define NN_DIGIT_LEN (NN_DIGIT_BITS/8) 
#define MAX_NN_DIGITS ((MAX_RSA_MODULUS_LEN+NN_DIGIT_LEN-1)/NN_DIGIT_LEN+1)
#define MAX_NN_DIGIT 0xffffffff
#define MAX_NN_HALF_DIGIT 0xffff
#define MAX_RSA_MODULUS_BITS 2048
#define MAX_RSA_MODULUS_LEN ((MAX_RSA_MODULUS_BITS+7)/8)
#define MAX_RSA_PUBLICE_LEN 3

#define RE_CONTENT_ENCODING 0x0400
#define RE_DATA 0x0401
#define RE_DIGEST_ALGORITHM 0x0402
#define RE_ENCODING 0x0403
#define RE_KEY 0x0404
#define RE_KEY_ENCODING 0x0405
#define RE_LEN 0x0406
#define RE_MODULUS_LEN 0x0407
#define RE_NEED_RANDOM 0x0408
#define RE_PRIVATE_KEY 0x0409
#define RE_PUBLIC_KEY 0x040a
#define RE_SIGNATURE 0x040b
#define RE_SIGNATURE_ENCODING 0x040c
#define RE_ENCRYPTION_ALGORITHM 0x040d


#define HIGH_HALF(a) (a>>16)
#define LOW_HALF(a) (a&MAX_NN_HALF_DIGIT)
#define TO_HIGH_HALF(a) (a<<16)
#define DIGIT_2MSB(a) ((a>>30)&3)

/* Random structure.
 */
typedef struct R_RANDOM_STRUCT{
  unsigned int bytesNeeded;
  unsigned char state[16];
  unsigned int outputAvailable;
  unsigned char output[16];
} R_RANDOM_STRUCT;

/* RSA public and private key.
 */
typedef struct R_RSA_PUBLIC_KEY{
  unsigned int bits;                           /* length in bits of modulus */
  unsigned char modulus[MAX_RSA_MODULUS_LEN];                    /* modulus */
  unsigned char exponent[MAX_RSA_PUBLICE_LEN];           /* public exponent */
} R_RSA_PUBLIC_KEY;



#define POINTER unsigned char *

static NN_DIGIT NN_AddDigitMult (NN_DIGIT * a, NN_DIGIT * b, NN_DIGIT  c, NN_DIGIT * d, unsigned int digits);
static NN_DIGIT NN_SubDigitMult (NN_DIGIT *, NN_DIGIT *, NN_DIGIT, NN_DIGIT *, unsigned int);
static unsigned int NN_DigitBits (NN_DIGIT);
unsigned int NN_Digits (NN_DIGIT *a,uint digits);
int NN_Cmp (NN_DIGIT *a,NN_DIGIT *b,uint digits);
int NN_Zero (NN_DIGIT *a,uint digits);

static int RSAPublicBlock (unsigned char *, unsigned int *, unsigned char *, unsigned int, struct R_RSA_PUBLIC_KEY *);
    

void R_memset (POINTER output,int value,uint len)
//POINTER output;                                             /* output block */
//int value;                                                         /* value */
//unsigned int len;                                        /* length of block */
{
  if (len)
    memset (output, value, len);
}

void R_memcpy (POINTER output,POINTER input,uint len)
//POINTER output;                                             /* output block */
//POINTER input;                                               /* input block */
//unsigned int len;                                       /* length of blocks */
{
  if (len)
    memcpy (output, input, len);
}

int R_memcmp (POINTER firstBlock,POINTER secondBlock,uint len)
//POINTER firstBlock;                                          /* first block */
//POINTER secondBlock;                                        /* second block */
//unsigned int len;                                       /* length of blocks */
{
  if (len)
    return (memcmp (firstBlock, secondBlock, len));
  else
    return (0);
}

//////////////////////////////////////////////////////////////////////////

/* Computes a = b * c, where b and c are digits.

   Lengths: a[2].
void NN_DigitMult (a, b, c)
NN_DIGIT a[2], b, c;

 */
void NN_DigitMult (NN_DIGIT a[2],NN_DIGIT b,NN_DIGIT c)
{
  NN_DIGIT t, u;
  NN_HALF_DIGIT bHigh, bLow, cHigh, cLow;

  bHigh = (NN_HALF_DIGIT)HIGH_HALF (b);
  bLow = (NN_HALF_DIGIT)LOW_HALF (b);
  cHigh = (NN_HALF_DIGIT)HIGH_HALF (c);
  cLow = (NN_HALF_DIGIT)LOW_HALF (c);

  a[0] = (NN_DIGIT)bLow * (NN_DIGIT)cLow;
  t = (NN_DIGIT)bLow * (NN_DIGIT)cHigh;
  u = (NN_DIGIT)bHigh * (NN_DIGIT)cLow;
  a[1] = (NN_DIGIT)bHigh * (NN_DIGIT)cHigh;
  
  if ((t += u) < u)
    a[1] += TO_HIGH_HALF (1);
  u = TO_HIGH_HALF (t);
  
  if ((a[0] += u) < u)
    a[1]++;
  a[1] += HIGH_HALF (t);
}
/* Sets a = b / c, where a and c are digits.
   Lengths: b[2].
   Assumes b[1] < c and HIGH_HALF (c) > 0. For efficiency, c should be
   normalized.
 */
void NN_DigitDiv (NN_DIGIT *a,NN_DIGIT b[2],NN_DIGIT c)
{
  NN_DIGIT t[2], u, v;
  NN_HALF_DIGIT aHigh, aLow, cHigh, cLow;

  cHigh = (NN_HALF_DIGIT)HIGH_HALF (c);
  cLow = (NN_HALF_DIGIT)LOW_HALF (c);

  t[0] = b[0];
  t[1] = b[1];

  /* Underestimate high half of quotient and subtract.
   */
  if (cHigh == MAX_NN_HALF_DIGIT)
    aHigh = (NN_HALF_DIGIT)HIGH_HALF (t[1]);
  else
    aHigh = (NN_HALF_DIGIT)(t[1] / (cHigh + 1));
  u = (NN_DIGIT)aHigh * (NN_DIGIT)cLow;
  v = (NN_DIGIT)aHigh * (NN_DIGIT)cHigh;
  if ((t[0] -= TO_HIGH_HALF (u)) > (MAX_NN_DIGIT - TO_HIGH_HALF (u)))
    t[1]--;
  t[1] -= HIGH_HALF (u);
  t[1] -= v;

  /* Correct estimate.
   */
  while ((t[1] > cHigh) ||
         ((t[1] == cHigh) && (t[0] >= TO_HIGH_HALF (cLow)))) {
    if ((t[0] -= TO_HIGH_HALF (cLow)) > MAX_NN_DIGIT - TO_HIGH_HALF (cLow))
      t[1]--;
    t[1] -= cHigh;
    aHigh++;
  }

  /* Underestimate low half of quotient and subtract.
   */
  if (cHigh == MAX_NN_HALF_DIGIT)
    aLow = (NN_HALF_DIGIT)LOW_HALF (t[1]);
  else
    aLow = 
      (NN_HALF_DIGIT)((TO_HIGH_HALF (t[1]) + HIGH_HALF (t[0])) / (cHigh + 1));
  u = (NN_DIGIT)aLow * (NN_DIGIT)cLow;
  v = (NN_DIGIT)aLow * (NN_DIGIT)cHigh;
  if ((t[0] -= u) > (MAX_NN_DIGIT - u))
    t[1]--;
  if ((t[0] -= TO_HIGH_HALF (v)) > (MAX_NN_DIGIT - TO_HIGH_HALF (v)))
    t[1]--;
  t[1] -= HIGH_HALF (v);

  /* Correct estimate.
   */
  while ((t[1] > 0) || ((t[1] == 0) && t[0] >= c)) {
    if ((t[0] -= c) > (MAX_NN_DIGIT - c))
      t[1]--;
    aLow++;
  }
  
  *a = TO_HIGH_HALF (aHigh) + aLow;
}



////////////////////////////////////////////////////////////////////////////////////


/* Decodes character string b into a, where character string is ordered
   from most to least significant.

   Lengths: a[digits], b[len].
   Assumes b[i] = 0 for i < len - digits * NN_DIGIT_LEN. (Otherwise most
   significant bytes are truncated.)
 */
void NN_Decode (NN_DIGIT * a,uint digits,uchar * b,uint len)
{
  NN_DIGIT t;
  int j;
  unsigned int i, u;
  
  for (i = 0, j = len - 1; i < digits && j >= 0; i++) {
    t = 0;
    for (u = 0; j >= 0 && u < NN_DIGIT_BITS; j--, u += 8)
      t |= ((NN_DIGIT)b[j]) << u;
    a[i] = t;
  }
  
  for (; i < digits; i++)
    a[i] = 0;
}

/* Encodes b into character string a, where character string is ordered
   from most to least significant.

   Lengths: a[len], b[digits].
   Assumes NN_Bits (b, digits) <= 8 * len. (Otherwise most significant
   digits are truncated.)
 */
void NN_Encode (uchar *a,uint len,NN_DIGIT* b,uint digits)
{
  NN_DIGIT t;
  int j;
  unsigned int i, u;

  for (i = 0, j = len - 1; i < digits && j >= 0; i++) {
    t = b[i];
    for (u = 0; j >= 0 && u < NN_DIGIT_BITS; j--, u += 8)
      a[j] = (unsigned char)(t >> u);
  }

  for (; j >= 0; j--)
    a[j] = 0;
}

/* Assigns a = b.

   Lengths: a[digits], b[digits].
 */
void NN_Assign (NN_DIGIT *a,NN_DIGIT *b,uint digits)
{
  unsigned int i;

  for (i = 0; i < digits; i++)
    a[i] = b[i];
}
/*
	使a[0] = 1;其余digits-1位=0
*/
void NN_ASSIGN_DIGIT(NN_DIGIT *a, NN_DIGIT t, uint digits)
{
	unsigned int i;
	
	a[0] = 1;
	for (i = 1; i < digits; i++)
		a[i] = 0;
}

/* Assigns a = 0.

   Lengths: a[digits]. 
 */
void NN_AssignZero (NN_DIGIT *a,uint digits)
{
  unsigned int i;

  for (i = 0; i < digits; i++)
    a[i] = 0;
}

/* Assigns a = 2^b.

   Lengths: a[digits].
   Requires b < digits * NN_DIGIT_BITS.
 */
void NN_Assign2Exp (NN_DIGIT *a,uint b,uint digits)
{
  NN_AssignZero (a, digits);

  if (b >= digits * NN_DIGIT_BITS)
    return;

  a[b / NN_DIGIT_BITS] = (NN_DIGIT)1 << (b % NN_DIGIT_BITS);
}

/* Computes a = b + c. Returns carry.

   Lengths: a[digits], b[digits], c[digits].
 */
NN_DIGIT NN_Add (NN_DIGIT *a,NN_DIGIT *b,NN_DIGIT *c,uint digits)
{
  NN_DIGIT ai, carry;
  unsigned int i;

  carry = 0;

  for (i = 0; i < digits; i++) {
    if ((ai = b[i] + carry) < carry)
      ai = c[i];
    else if ((ai += c[i]) < c[i])
      carry = 1;
    else
      carry = 0;
    a[i] = ai;
  }

  return (carry);
}

/* Computes a = b - c. Returns borrow.

   Lengths: a[digits], b[digits], c[digits].
 */
NN_DIGIT NN_Sub (NN_DIGIT *a,NN_DIGIT * b,NN_DIGIT * c,uint digits)
{
  NN_DIGIT ai, borrow;
  unsigned int i;

  borrow = 0;

  for (i = 0; i < digits; i++) {
    if ((ai = b[i] - borrow) > (MAX_NN_DIGIT - borrow))
      ai = MAX_NN_DIGIT - c[i];
    else if ((ai -= c[i]) > (MAX_NN_DIGIT - c[i]))
      borrow = 1;
    else
      borrow = 0;
    a[i] = ai;
  }

  return (borrow);
}

/* Computes a = b * c.

   Lengths: a[2*digits], b[digits], c[digits].
   Assumes digits < MAX_NN_DIGITS.
 */
void NN_Mult (NN_DIGIT *a,NN_DIGIT * b,NN_DIGIT * c,uint digits)
{
  NN_DIGIT t[2*MAX_NN_DIGITS];
  unsigned int bDigits, cDigits, i;

  NN_AssignZero (t, 2 * digits);
  
  bDigits = NN_Digits (b, digits);
  cDigits = NN_Digits (c, digits);

  for (i = 0; i < bDigits; i++)
    t[i+cDigits] += NN_AddDigitMult (&t[i], &t[i], b[i], c, cDigits);
  
  NN_Assign (a, t, 2 * digits);
  
  /* Zeroize potentially sensitive information.
   */
  R_memset ((POINTER)t, 0, sizeof (t));
}

/* Computes a = b * 2^c (i.e., shifts left c bits), returning carry.

   Lengths: a[digits], b[digits].
   Requires c < NN_DIGIT_BITS.
 */
NN_DIGIT NN_LShift (NN_DIGIT *a,NN_DIGIT *b,uint c,uint digits)
{
  NN_DIGIT bi, carry;
  unsigned int i, t;
  
  if (c >= NN_DIGIT_BITS)
    return (0);
  
  t = NN_DIGIT_BITS - c;

  carry = 0;

  for (i = 0; i < digits; i++) {
    bi = b[i];
    a[i] = (bi << c) | carry;
    carry = c ? (bi >> t) : 0;
  }
  
  return (carry);
}

/* Computes a = c div 2^c (i.e., shifts right c bits), returning carry.

   Lengths: a[digits], b[digits].
   Requires: c < NN_DIGIT_BITS.
 */
NN_DIGIT NN_RShift (NN_DIGIT *a,NN_DIGIT *b,uint c,uint digits)
{
  NN_DIGIT bi, carry;
  int i;
  unsigned int t;
  
  if (c >= NN_DIGIT_BITS)
    return (0);
  
  t = NN_DIGIT_BITS - c;

  carry = 0;

  for (i = digits - 1; i >= 0; i--) {
    bi = b[i];
    a[i] = (bi >> c) | carry;
    carry = c ? (bi << t) : 0;
  }
  
  return (carry);
}

/* Computes a = c div d and b = c mod d.

   Lengths: a[cDigits], b[dDigits], c[cDigits], d[dDigits].
   Assumes d > 0, cDigits < 2 * MAX_NN_DIGITS,
           dDigits < MAX_NN_DIGITS.
 */
void NN_Div (NN_DIGIT *a,NN_DIGIT * b, NN_DIGIT * c,uint cDigits,NN_DIGIT * d,uint dDigits)
{
  NN_DIGIT ai, cc[2*MAX_NN_DIGITS+1], dd[MAX_NN_DIGITS], t;
  int i;
  unsigned int ddDigits, shift;
  
  ddDigits = NN_Digits (d, dDigits);
  if (ddDigits == 0)
    return;
  
  /* Normalize operands.
   */
  shift = NN_DIGIT_BITS - NN_DigitBits (d[ddDigits-1]);
  NN_AssignZero (cc, ddDigits);
  cc[cDigits] = NN_LShift (cc, c, shift, cDigits);
  NN_LShift (dd, d, shift, ddDigits);
  t = dd[ddDigits-1];
  
  NN_AssignZero (a, cDigits);

  for (i = cDigits-ddDigits; i >= 0; i--) {
    /* Underestimate quotient digit and subtract.
     */
    if (t == MAX_NN_DIGIT)
      ai = cc[i+ddDigits];
    else
      NN_DigitDiv (&ai, &cc[i+ddDigits-1], t + 1);
    cc[i+ddDigits] -= NN_SubDigitMult (&cc[i], &cc[i], ai, dd, ddDigits);

    /* Correct estimate.
     */
    while (cc[i+ddDigits] || (NN_Cmp (&cc[i], dd, ddDigits) >= 0)) {
      ai++;
      cc[i+ddDigits] -= NN_Sub (&cc[i], &cc[i], dd, ddDigits);
    }
    
    a[i] = ai;
  }
  
  /* Restore result.
   */
  NN_AssignZero (b, dDigits);
  NN_RShift (b, cc, shift, ddDigits);

  /* Zeroize potentially sensitive information.
   */
  R_memset ((POINTER)cc, 0, sizeof (cc));
  R_memset ((POINTER)dd, 0, sizeof (dd));
}

/* Computes a = b mod c.

   Lengths: a[cDigits], b[bDigits], c[cDigits].
   Assumes c > 0, bDigits < 2 * MAX_NN_DIGITS, cDigits < MAX_NN_DIGITS.
 */
void NN_Mod (NN_DIGIT *a,NN_DIGIT *b,uint bDigits,NN_DIGIT *c,uint cDigits)
{
  NN_DIGIT t[2 * MAX_NN_DIGITS];
  
  NN_Div (t, a, b, bDigits, c, cDigits);
  
  /* Zeroize potentially sensitive information.
   */
  R_memset ((POINTER)t, 0, sizeof (t));
}

/* Computes a = b * c mod d.

   Lengths: a[digits], b[digits], c[digits], d[digits].
   Assumes d > 0, digits < MAX_NN_DIGITS.
 */
void NN_ModMult (NN_DIGIT *a,NN_DIGIT *b,NN_DIGIT *c,NN_DIGIT *d,uint digits)
{
  NN_DIGIT t[2*MAX_NN_DIGITS];

  NN_Mult (t, b, c, digits);
  NN_Mod (a, t, 2 * digits, d, digits);
  
  /* Zeroize potentially sensitive information.
   */
  R_memset ((POINTER)t, 0, sizeof (t));
}

/* Computes a = b^c mod d.

   Lengths: a[dDigits], b[dDigits], c[cDigits], d[dDigits].
   Assumes d > 0, cDigits > 0, dDigits < MAX_NN_DIGITS.
 */
void NN_ModExp (NN_DIGIT *a,NN_DIGIT *b,NN_DIGIT *c,uint cDigits,NN_DIGIT *d,uint dDigits)
{
  NN_DIGIT bPower[3][MAX_NN_DIGITS], ci, t[MAX_NN_DIGITS];
  int i;
  unsigned int ciBits, j, s;

  /* Store b, b^2 mod d, and b^3 mod d.
   */
  NN_Assign (bPower[0], b, dDigits);
  NN_ModMult (bPower[1], bPower[0], b, d, dDigits);
  NN_ModMult (bPower[2], bPower[1], b, d, dDigits);
  
  NN_ASSIGN_DIGIT (t, 1, dDigits);

  cDigits = NN_Digits (c, cDigits);
  for (i = cDigits - 1; i >= 0; i--) {
    ci = c[i];
    ciBits = NN_DIGIT_BITS;
    
    /* Scan past leading zero bits of most significant digit.
     */
    if (i == (int)(cDigits - 1)) {
      while (! DIGIT_2MSB (ci)) {
        ci <<= 2;
        ciBits -= 2;
      }
    }

    for (j = 0; j < ciBits; j += 2, ci <<= 2) {
      /* Compute t = t^4 * b^s mod d, where s = two MSB's of ci.
       */
      NN_ModMult (t, t, t, d, dDigits);
      NN_ModMult (t, t, t, d, dDigits);
      if ((s = DIGIT_2MSB (ci)) != 0)
        NN_ModMult (t, t, bPower[s-1], d, dDigits);
    }
  }
  
  NN_Assign (a, t, dDigits);
  
  /* Zeroize potentially sensitive information.
   */
  R_memset ((POINTER)bPower, 0, sizeof (bPower));
  R_memset ((POINTER)t, 0, sizeof (t));
}

/* Compute a = 1/b mod c, assuming inverse exists.
   
   Lengths: a[digits], b[digits], c[digits].
   Assumes gcd (b, c) = 1, digits < MAX_NN_DIGITS.
 */
void NN_ModInv (NN_DIGIT *a,NN_DIGIT *b,NN_DIGIT *c,uint digits)
//NN_DIGIT *a, *b, *c;
//unsigned int digits;
{
  NN_DIGIT q[MAX_NN_DIGITS], t1[MAX_NN_DIGITS], t3[MAX_NN_DIGITS],
    u1[MAX_NN_DIGITS], u3[MAX_NN_DIGITS], v1[MAX_NN_DIGITS],
    v3[MAX_NN_DIGITS], w[2*MAX_NN_DIGITS];
  int u1Sign;

  /* Apply extended Euclidean algorithm, modified to avoid negative
     numbers.
   */
  NN_ASSIGN_DIGIT (u1, 1, digits);
  NN_AssignZero (v1, digits);
  NN_Assign (u3, b, digits);
  NN_Assign (v3, c, digits);
  u1Sign = 1;

  while (! NN_Zero (v3, digits)) {
    NN_Div (q, t3, u3, digits, v3, digits);
    NN_Mult (w, q, v1, digits);
    NN_Add (t1, u1, w, digits);
    NN_Assign (u1, v1, digits);
    NN_Assign (v1, t1, digits);
    NN_Assign (u3, v3, digits);
    NN_Assign (v3, t3, digits);
    u1Sign = -u1Sign;
  }
  
  /* Negate result if sign is negative.
    */
  if (u1Sign < 0)
    NN_Sub (a, c, u1, digits);
  else
    NN_Assign (a, u1, digits);

  /* Zeroize potentially sensitive information.
   */
  R_memset ((POINTER)q, 0, sizeof (q));
  R_memset ((POINTER)t1, 0, sizeof (t1));
  R_memset ((POINTER)t3, 0, sizeof (t3));
  R_memset ((POINTER)u1, 0, sizeof (u1));
  R_memset ((POINTER)u3, 0, sizeof (u3));
  R_memset ((POINTER)v1, 0, sizeof (v1));
  R_memset ((POINTER)v3, 0, sizeof (v3));
  R_memset ((POINTER)w, 0, sizeof (w));
}

/* Computes a = gcd(b, c).

   Lengths: a[digits], b[digits], c[digits].
   Assumes b > c, digits < MAX_NN_DIGITS.
 */
void NN_Gcd (NN_DIGIT *a,NN_DIGIT *b,NN_DIGIT *c,uint digits)
//NN_DIGIT *a, *b, *c;
//unsigned int digits;
{
  NN_DIGIT t[MAX_NN_DIGITS], u[MAX_NN_DIGITS], v[MAX_NN_DIGITS];

  NN_Assign (u, b, digits);
  NN_Assign (v, c, digits);

  while (! NN_Zero (v, digits)) {
    NN_Mod (t, u, digits, v, digits);
    NN_Assign (u, v, digits);
    NN_Assign (v, t, digits);
  }

  NN_Assign (a, u, digits);

  /* Zeroize potentially sensitive information.
   */
  R_memset ((POINTER)t, 0, sizeof (t));
  R_memset ((POINTER)u, 0, sizeof (u));
  R_memset ((POINTER)v, 0, sizeof (v));
}

/* Returns sign of a - b.

   Lengths: a[digits], b[digits].
 */
int NN_Cmp (NN_DIGIT *a,NN_DIGIT *b,uint digits)
//NN_DIGIT *a, *b;
//unsigned int digits;
{
  int i;
  
  for (i = digits - 1; i >= 0; i--) {
    if (a[i] > b[i])
      return (1);
    if (a[i] < b[i])
      return (-1);
  }
  return (0);
}
/* Returns nonzero iff a is zero.

   Lengths: a[digits].
 */
int NN_Zero (NN_DIGIT *a,uint digits)
{
  unsigned int i;
  
  for (i = 0; i < digits; i++)
    if (a[i])
      return (0);
    
  return (1);
}

/* Returns the significant length of a in bits.

   Lengths: a[digits].
 */
unsigned int NN_Bits (NN_DIGIT *a,uint digits)
{
  if ((digits = NN_Digits (a, digits)) == 0)
    return (0);

  return ((digits - 1) * NN_DIGIT_BITS + NN_DigitBits (a[digits-1]));
}

/* Returns the significant length of a in digits.

   Lengths: a[digits].
 */
unsigned int NN_Digits (NN_DIGIT *a,uint digits)
//NN_DIGIT *a;
//unsigned int digits;
{
  int i;
  
  for (i = digits - 1; i >= 0; i--)
    if (a[i])
      break;

  return (i + 1);
}



/* Computes a = b + c*d, where c is a digit. Returns carry.

   Lengths: a[digits], b[digits], d[digits].
 */

static NN_DIGIT NN_AddDigitMult (NN_DIGIT *a,NN_DIGIT *b,NN_DIGIT c,NN_DIGIT *d,uint digits)
{
  NN_DIGIT carry, t[2];
  unsigned int i;

  if (c == 0)
    return (0);

  carry = 0;
  for (i = 0; i < digits; i++) {
    NN_DigitMult (t, c, d[i]);
    if ((a[i] = b[i] + carry) < carry)
      carry = 1;
    else
      carry = 0;
    if ((a[i] += t[0]) < t[0])
      carry++;
    carry += t[1];
  }
  
  return (carry);
}

/* Computes a = b - c*d, where c is a digit. Returns borrow.

   Lengths: a[digits], b[digits], d[digits].
 */
static NN_DIGIT NN_SubDigitMult (NN_DIGIT *a,NN_DIGIT *b,NN_DIGIT c,NN_DIGIT *d,uint digits)
{
  NN_DIGIT borrow, t[2];
  unsigned int i;

  if (c == 0)
    return (0);

  borrow = 0;
  for (i = 0; i < digits; i++) {
    NN_DigitMult (t, c, d[i]);
    if ((a[i] = b[i] - borrow) > (MAX_NN_DIGIT - borrow))
      borrow = 1;
    else
      borrow = 0;
    if ((a[i] -= t[0]) > (MAX_NN_DIGIT - t[0]))
      borrow++;
    borrow += t[1];
  }
  
  return (borrow);
}

/* Returns the significant length of a in bits, where a is a digit.
 */
static unsigned int NN_DigitBits (NN_DIGIT a)
//NN_DIGIT a;
{
  unsigned int i;
  
  for (i = 0; i < NN_DIGIT_BITS; i++, a >>= 1)
    if (a == 0)
      break;
    
  return (i);
}



/////////////////////////////////////////////////////////////



/* RSA public-key encryption, according to PKCS #1.
 */
int RSAPublicEncrypt
  (uchar *output,uint *outputLen,uchar *input,uint inputLen,struct R_RSA_PUBLIC_KEY *publicKey, struct R_RANDOM_STRUCT *randomStruct)
//unsigned char *output;                                      /* output block */
//unsigned int *outputLen;                          /* length of output block */
//unsigned char *input;                                        /* input block */
//unsigned int inputLen;                             /* length of input block */
//R_RSA_PUBLIC_KEY *publicKey;                              /* RSA public key */
//R_RANDOM_STRUCT *randomStruct;                          /* random structure */
{
  int status;
  unsigned char  pkcsBlock[MAX_RSA_MODULUS_LEN];
  unsigned int modulusLen;
  modulusLen = (publicKey->bits + 7) / 8;
//  if (inputLen + 11 > modulusLen)
//    return (RE_LEN);


  pkcsBlock[0] = 0;
  
  /* block type 2 */

  pkcsBlock[1] = 2;

/*
  for (i = 2; i < modulusLen - inputLen - 1; i++) 
  {
    //Find nonzero random byte.

    do 
	{
      R_GenerateBytes (&byte, 1, randomStruct);
    } while (byte == 0);
    pkcsBlock[i] = byte;
  }
  // separator 
  pkcsBlock[i++] = 0;
  R_memcpy ((POINTER)&pkcsBlock[i], (POINTER)input, inputLen);

*/


  
  R_memcpy ((POINTER)&pkcsBlock[0], (POINTER)input, inputLen);

  status = RSAPublicBlock
    (output, outputLen, pkcsBlock, modulusLen, publicKey);

  /* Zeroize sensitive information.
   */
 // byte = 0;
  R_memset ((POINTER)pkcsBlock, 0, sizeof (pkcsBlock));
  return (status);
}




/* Raw RSA public-key operation. Output has same length as modulus.

   Assumes inputLen < length of modulus.
   Requires input < modulus.
 */
static int RSAPublicBlock (uchar *output,uint *outputLen,uchar *input,uint inputLen,struct R_RSA_PUBLIC_KEY *publicKey)
//unsigned char *output;                                      /* output block */
//unsigned int *outputLen;                          /* length of output block */
//unsigned char *input;                                        /* input block */
//unsigned int inputLen;                             /* length of input block */
//R_RSA_PUBLIC_KEY *publicKey;                              /* RSA public key */
{
  NN_DIGIT c[MAX_NN_DIGITS], e[MAX_NN_DIGITS], m[MAX_NN_DIGITS],
    n[MAX_NN_DIGITS];
  unsigned int eDigits, nDigits;
  unsigned int elen=0;

  int modulus=(publicKey->bits+7)/8;
  
  if(publicKey->exponent[0]==0x03)
	  elen=0x01;
  else
	  elen=0x03;


  NN_Decode (m, MAX_NN_DIGITS, input, inputLen);
  NN_Decode (n, MAX_NN_DIGITS, publicKey->modulus, modulus);//MAX_RSA_MODULUS_LEN
  NN_Decode (e, MAX_NN_DIGITS, publicKey->exponent, elen);//MAX_RSA_MODULUS_LEN
  nDigits = NN_Digits (n, MAX_NN_DIGITS);
  eDigits = NN_Digits (e, MAX_NN_DIGITS);
  
  if (NN_Cmp (m, n, nDigits) >= 0)
    return (RE_DATA);
  
  /* Compute c = m^e mod n.
   */
  NN_ModExp (c, m, e, eDigits, n, nDigits);

  *outputLen = (publicKey->bits + 7) / 8;
  NN_Encode (output, *outputLen, c, nDigits);
  
  /* Zeroize sensitive information.
   */
  R_memset ((POINTER)c, 0, sizeof (c));
  R_memset ((POINTER)m, 0, sizeof (m));

  return (0);
}

///////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////

static int RSAPublicBlock000 (uchar *output,uint *outputLen,uchar *input,uint inputLen,uchar *modulebuf,uint modLen,
							  uchar *expbuf,uint expLen)

{
  NN_DIGIT c[MAX_NN_DIGITS], e[MAX_NN_DIGITS], m[MAX_NN_DIGITS],
    n[MAX_NN_DIGITS];
  unsigned int eDigits, nDigits;
  unsigned int elen=0;

  int modulus=modLen;
  elen=expLen;

  NN_Decode (m, MAX_NN_DIGITS, input, inputLen);
  NN_Decode (n, MAX_NN_DIGITS, modulebuf, modulus);
  NN_Decode (e, MAX_NN_DIGITS, expbuf, elen);
  nDigits = NN_Digits (n, MAX_NN_DIGITS);
  eDigits = NN_Digits (e, MAX_NN_DIGITS);
  
  if (NN_Cmp (m, n, nDigits) >= 0)
    return (RE_DATA);
  
  /* Compute c = m^e mod n.
   */
  NN_ModExp (c, m, e, eDigits, n, nDigits);

  *outputLen = modulus;
  NN_Encode (output, *outputLen, c, nDigits);
  
  /* Zeroize sensitive information.
   */
  R_memset ((POINTER)c, 0, sizeof (c));
  R_memset ((POINTER)m, 0, sizeof (m));

  return (0);
}


int RSAPublicEncryptRecovery
  (uchar *output,uint *outputLen,uchar *input,uint inputLen,uchar *modulebuf,uint modLen,uchar *exp,uint explen)
{
  int status;
  unsigned char  pkcsBlock[MAX_RSA_MODULUS_LEN];
  unsigned int modulusLen;
  modulusLen=modLen;

  R_memcpy ((POINTER)&pkcsBlock[0], (POINTER)input, inputLen);

  status = RSAPublicBlock000
    (output, outputLen, pkcsBlock, modulusLen, modulebuf,modulusLen,exp,explen);

  R_memset ((POINTER)pkcsBlock, 0, sizeof (pkcsBlock));
  return (status);
}

//////////////////////////////////////////////////////////////////////











/*
RSAPublicKey::=SEQUENCE{
  modulus INTEGER, -- n
  publicExponent INTEGER -- e }
*/

int EncodeDerPubKeyQpboc(unsigned char *derkey, R_RSA_PUBLIC_KEY pkey,int exponent,int modulus)
{
	int i,elen,nlen;
	int offset=0;
	
	i=pkey.bits;
        i=i;

    	elen=exponent;
		nlen=modulus;
		
		if((elen+nlen)>=128 && (elen+nlen)<256)
		{
			offset=0;
			derkey[offset++]=0x30;
			derkey[offset++]=0x81;
			derkey[offset++]=elen+nlen+4;
			derkey[offset++]=0x02;
			if(nlen>=0x80)
			{
				derkey[2]++;
				derkey[offset++]=0x81;
				if(pkey.modulus[0]>=0x80){
					derkey[offset++]=nlen+1;
					derkey[offset++]=0x0;
					derkey[2]++;
				}
				else{
					derkey[offset++]=nlen;
				}
			}
			else
			{
				return -1;
			}
			memcpy(&derkey[offset],pkey.modulus,nlen);
			offset+=nlen;
			derkey[offset++]=0x02;
			derkey[offset++]=elen;
			memcpy(&derkey[offset],pkey.exponent,elen);
			offset+=elen;
			return offset;
		}


     
          if(elen==3)
		  {

			     if(nlen>=0x78 && nlen<=0x7e)
				 {
					offset=0;
					derkey[offset++]=0x30;
					derkey[offset++]=0x81;
					derkey[offset++]=elen+nlen+5;
					derkey[offset++]=0x02;
					derkey[offset++]=nlen+1;
					derkey[offset++]=0x00;
					memcpy(&derkey[offset],pkey.modulus,nlen);
					offset+=nlen;
					derkey[offset++]=0x02;
					derkey[offset++]=elen;
					memcpy(&derkey[offset],pkey.exponent,elen);
					offset+=elen;
					return offset;
				}
				else
				{
					offset=0;
					derkey[offset++]=0x30;
					derkey[offset++]=elen+nlen+5;
					derkey[offset++]=0x02;
					derkey[offset++]=nlen+1;
					derkey[offset++]=0x00;
					memcpy(&derkey[offset],pkey.modulus,nlen);
					offset+=nlen;
					derkey[offset++]=0x02;
					derkey[offset++]=elen;
					memcpy(&derkey[offset],pkey.exponent,elen);
					offset+=elen;
					return offset;
					
				}
		  }
		  else
		  {

            if(nlen>=0x7A && nlen<=0x7E)
			{
					offset=0;
					derkey[offset++]=0x30;
					derkey[offset++]=0x81;
					derkey[offset++]=elen+nlen+5;
					derkey[offset++]=0x02;
					derkey[offset++]=nlen+1;
					derkey[offset++]=0x00;
					memcpy(&derkey[offset],pkey.modulus,nlen);
					offset+=nlen;
					derkey[offset++]=0x02;
					derkey[offset++]=elen;
					memcpy(&derkey[offset],pkey.exponent,elen);
					offset+=elen;
					return offset;

			}
			else
			{
					offset=0;
					derkey[offset++]=0x30;
					derkey[offset++]=elen+nlen+5;
					derkey[offset++]=0x02;
					derkey[offset++]=nlen+1;
					derkey[offset++]=0x00;
					memcpy(&derkey[offset],pkey.modulus,nlen);
					offset+=nlen;
					derkey[offset++]=0x02;
					derkey[offset++]=elen;
					memcpy(&derkey[offset],pkey.exponent,elen);
					offset+=elen;
					return offset;

			}
		  }

	

		

}



int EncodeDerPubKey(unsigned char *derkey,R_RSA_PUBLIC_KEY pkey)
{
	int i,elen,nlen;
	int offset=0;
	i=pkey.bits;
//	if(i=0x400){ //1024 bits
	if(i==0x400){ //1024 bits
		elen=sizeof(pkey.exponent);
		nlen=sizeof(pkey.modulus);
		if((elen+nlen)>=128 && (elen+nlen)<256){
			offset=0;
			derkey[offset++]=0x30;
			derkey[offset++]=0x81;
			derkey[offset++]=elen+nlen+4;
			derkey[offset++]=0x02;
			if(nlen>=0x80){
				derkey[2]++;
				derkey[offset++]=0x81;
				if(pkey.modulus[0]>=0x80){
					derkey[offset++]=nlen+1;
					derkey[offset++]=0x0;
					derkey[2]++;
				}
				else{
					derkey[offset++]=nlen;
				}
			}else{
				return -1;
			}
			memcpy(&derkey[offset],pkey.modulus,nlen);
			offset+=nlen;
			derkey[offset++]=0x02;
			derkey[offset++]=elen;
			memcpy(&derkey[offset],pkey.exponent,elen);
			offset+=elen;
			return offset;
		}else{
			return -2;
		}
//	}else if(i=0x200){
	}else if(i==0x200){
		return -3;
	}else
		return 0;
}


//
// Decode Der Public Key.
//
int DecodeDerPubKey(unsigned char *derkey,R_RSA_PUBLIC_KEY *pkey)
{
	int offset;
	int ilen,llen;
	offset=0;
	if(derkey[offset++]!=0x30) return -1;
	if(derkey[offset]>0x80)
	{
		offset+=(derkey[offset]-0x80);
		offset++;
	}
	else
	{
        ilen=derkey[offset];
		offset++;
		if(derkey[offset++]!=0x02)
		{
			return -1;
		}
		llen=derkey[offset++];
        if(derkey[offset]!=0x00)
		{
			return -1;
		}
		//n
        memcpy(pkey->modulus,&derkey[offset+1],llen-1);
        offset+=llen;

		//e
        if(derkey[offset++]!=0x02) return -1;
          
        memcpy(pkey->exponent,&derkey[offset+1],derkey[offset]);
		if(ilen!=llen+2+2+derkey[offset])
			return -1;
		return offset+1+derkey[offset];
	}
	//-------------------------------------------------------------
	if(derkey[offset++]!=0x02) return -1;
	if(derkey[offset]>0x80){
		//长度字节的长度
		llen=(derkey[offset]-0x80);
		offset++;
		//内容长度
		if(llen==1)ilen=derkey[offset];
		else if(llen==2)ilen=derkey[offset]*256+derkey[offset+1];
		else return -1;
	}
	else{
		llen=1;
		//内容长度
		ilen=derkey[offset];
	}
	//
	offset+=llen;
	//n
	if(derkey[offset]==0)
		memcpy(pkey->modulus,&derkey[offset+1],ilen-1);
	else
		memcpy(pkey->modulus,&derkey[offset],ilen);
	//偏移量增加
	offset+=ilen;
	//-----------------------------------------------------------
	//-------------------------------------------------------------
	if(derkey[offset++]!=0x02) return -1;
	if(derkey[offset]>0x80){
		//长度字节的长度
		llen=(derkey[offset]-0x80);
		offset++;
		//内容长度
		if(llen==1)ilen=derkey[offset];
		else if(llen==2)ilen=derkey[offset]*256+derkey[offset+1];
		else return -1;
	}
	else{
		llen=1;
		//内容长度
		ilen=derkey[offset];
	}
	//
	offset+=llen;
	//e
	if(derkey[offset]==0)
		memcpy(pkey->exponent,&derkey[offset+1],ilen-1);
	else
		memcpy(pkey->exponent,&derkey[offset],ilen);
	//偏移量增加
	offset+=ilen;
	return offset;
}





int	   Pubkey_Enc(
							 unsigned char *indata,
							 int			indataLen,
							 unsigned char *outdata,
							 int		   *outdataLen,
							 unsigned char *PubKeyInfo,
							 int			PubKeyInfoSize)
{
	int ilen;
	R_RANDOM_STRUCT randomStruct;
	R_RSA_PUBLIC_KEY PUBLIC_KEY3;
	PUBLIC_KEY3.bits=indataLen*8;
  
  ilen=DecodeDerPubKey(PubKeyInfo,&PUBLIC_KEY3);
	ilen=RSAPublicEncrypt(outdata,(uint *)outdataLen,indata,indataLen,&PUBLIC_KEY3,&randomStruct);
	return ilen;
}

int	   Pubkey_Recovery(
							 unsigned char *indata,
							 int			indataLen,
							 unsigned char *outdata,
							 int		   *outdataLen,
							 unsigned char *modulubuf,
							 int			modLen,
							 unsigned char *exp,
							 int explen)
{
	int ilen;
  
    ilen=RSAPublicEncryptRecovery(outdata,(uint *)outdataLen,indata,indataLen,modulubuf,modLen,exp,explen);
	return ilen;
}




int PubkeyEncode(unsigned char *derkey,unsigned char *exp,int exponent,unsigned char *modubuf,int modulus)
{
  int st;
	R_RSA_PUBLIC_KEY pkey;
	pkey.bits=modulus*8;
	memcpy(pkey.exponent,exp,exponent);
	memcpy(pkey.modulus,modubuf,modulus);
	
  st=EncodeDerPubKeyQpboc(derkey,pkey,exponent,modulus);
  
	return st;
}




/////////////////////////////////////////////////////////////////////////////

static int RSAPrivateBlock000 (uchar *output,uint *outputLen,uchar *input,uint inputLen,
								uchar *pbuf,uchar *qbuf,uchar *dpbuf,uchar *dqbuf,uchar *qinvbuf,uint Len)

{
  NN_DIGIT c[MAX_NN_DIGITS], e[MAX_NN_DIGITS], m[MAX_NN_DIGITS],
    n[MAX_NN_DIGITS],p[MAX_NN_DIGITS],q[MAX_NN_DIGITS];
  unsigned int eDigits, nDigits;
 // unsigned int elen=0;

  int modulus=Len;

  NN_Decode (m, MAX_NN_DIGITS, input, inputLen);
  NN_Decode (p, MAX_NN_DIGITS, pbuf, modulus*2);
  NN_Decode (q, MAX_NN_DIGITS, qbuf, modulus);
  nDigits = NN_Digits (n, MAX_NN_DIGITS);
  eDigits = NN_Digits (e, MAX_NN_DIGITS);
  
  if (NN_Cmp (m, n, nDigits) >= 0)
    return (RE_DATA);
  
  /* Compute c = m^e mod n.
   */
  NN_ModExp (c, m, e, eDigits, n, nDigits);

  *outputLen = modulus;
  NN_Encode (output, *outputLen, c, nDigits);
  
  /* Zeroize sensitive information.
   */
  R_memset ((POINTER)c, 0, sizeof (c));
  R_memset ((POINTER)m, 0, sizeof (m));

  return (0);
}


// ModInv

