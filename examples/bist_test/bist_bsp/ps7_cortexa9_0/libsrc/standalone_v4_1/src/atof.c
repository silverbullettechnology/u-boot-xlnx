/*
Copyright © 2011,2014, Grzegorz Kraszewski
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this list
of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#define DIGITS 18

#define DOUBLE_PLUS_ZERO          0x0000000000000000ULL
#define DOUBLE_MINUS_ZERO         0x8000000000000000ULL
#define DOUBLE_PLUS_INFINITY      0x7FF0000000000000ULL
#define DOUBLE_MINUS_INFINITY     0xFFF0000000000000ULL

union HexDouble
{
  double d;
  unsigned long long u;
};


#define lsr96(s2, s1, s0, d2, d1, d0) \
d0 = ((s0) >> 1) | (((s1) & 1) << 31); \
d1 = ((s1) >> 1) | (((s2) & 1) << 31); \
d2 = (s2) >> 1;

#define lsl96(s2, s1, s0, d2, d1, d0) \
d2 = ((s2) << 1) | (((s1) & (1 << 31)) >> 31); \
d1 = ((s1) << 1) | (((s0) & (1 << 31)) >> 31); \
d0 = (s0) << 1;

#define add96(s2, s1, s0, d2, d1, d0) { \
unsigned long _x, _c; \
_x = (s0); (s0) += (d0); \
if ((s0) < _x) _c = 1; else _c = 0; \
_x = (s1); (s1) += (d1) + _c; \
if (((s1) < _x) || (((s1) == _x) && _c)) _c = 1; else _c = 0; \
(s2) += (d2) + _c; }

#define sub96(s2, s1, s0, d2, d1, d0) { \
unsigned long _x, _c; \
_x = (s0); (s0) -= (d0); \
if ((s0) > _x) _c = 1; else _c = 0; \
_x = (s1); (s1) -= (d1) + _c; \
if (((s1) > _x) || (((s1) == _x) && _c)) _c = 1; else _c = 0; \
(s2) -= (d2) + _c; }


#define FSM_A     0
#define FSM_B     1
#define FSM_C     2
#define FSM_D     3
#define FSM_E     4
#define FSM_F     5
#define FSM_G     6
#define FSM_H     7
#define FSM_I     8
#define FSM_STOP  9


#define DPOINT '.'
#define ISDIGIT(x) (((x) >= '0') && ((x) <= '9'))
#define ISSPACE(x) ((((x) >= 0x09) && ((x) <= 0x13)) || ((x) == 0x20))


struct PrepNumber
{
  long negative;                      /* 0 if positive number, 1 if negative */
  long exponent;                      /* power of 10 exponent */
  unsigned long long mantissa;
};


#define PARSER_OK     0  // parser finished OK
#define PARSER_PZERO  1  // no digits or number is smaller than +-2^-1022
#define PARSER_MZERO  2  // number is negative, module smaller
#define PARSER_PINF   3  // number is higher than +HUGE_VAL
#define PARSER_MINF   4  // number is lower than -HUGE_VAL

#define GETC(s) *s++


static long parser(char *s, struct PrepNumber *pn)
{
  long state = FSM_A;
  long digx = 0, c = ' ';            /* initial value for kicking off the state machine */
  long result = PARSER_OK;
  long expexp = 0;
  long expneg = 0;

  while (state != FSM_STOP)
  {
    switch (state)
    {
      case FSM_A:
        if (ISSPACE(c)) c = GETC(s);
        else state = FSM_B;
      break;


      case FSM_B:
        state = FSM_C;

        if (c == '+') c = GETC(s);
        else if (c == '-')
        {
          pn->negative = 1;
          c = GETC(s);
        }
        else if (ISDIGIT(c));
        else if (c == DPOINT);
        else state = FSM_STOP;
      break;


      case FSM_C:
        if (c == '0') c = GETC(s);
        else if (c == DPOINT)
        {
          c = GETC(s);
          state = FSM_D;
        }
        else state = FSM_E;
      break;


      case FSM_D:
        if (c == '0')
        {
          c = GETC(s);
          if (pn->exponent > -2147483647) pn->exponent--;
        }
        else state = FSM_F;
      break;


      case FSM_E:
        if (ISDIGIT(c))
        {
          if (digx < DIGITS)
          {
            pn->mantissa *= 10;
            pn->mantissa += c - '0';
            digx++;
          }
          else if (pn->exponent < 2147483647) pn->exponent++;

          c = GETC(s);
        }
        else if (c == DPOINT)
        {
          c = GETC(s);
          state = FSM_F;
        }
        else state = FSM_F;
      break;


      case FSM_F:
        if (ISDIGIT(c))
        {
          if (digx < DIGITS)
          {
            pn->mantissa *= 10;
            pn->mantissa += c - '0';
            pn->exponent--;
            digx++;
          }

          c = GETC(s);
        }
        else if ((c == 'E') || (c == 'e'))
        {
          c = GETC(s);
          state = FSM_G;
        }
        else state = FSM_G;
      break;


      case FSM_G:
        if (c == '+') c = GETC(s);
        else if (c == '-')
        {
          expneg = 1;
          c = GETC(s);
        }

        state = FSM_H;
      break;


      case FSM_H:
        if (c == '0') c = GETC(s);
        else state = FSM_I;
      break;


      case FSM_I:
        if (ISDIGIT(c))
        {
          if (expexp < 214748364)
          {
            expexp *= 10;
            expexp += c - '0';
          }

          c = GETC(s);
        }
        else state = FSM_STOP;
      break;
    }
  }

  if (expneg) expexp = -expexp;
  pn->exponent += expexp;

  if (pn->mantissa == 0)
  {
    if (pn->negative) result = PARSER_MZERO;
    else result = PARSER_PZERO;
  }
  else if (pn->exponent > 309)
  {
    if (pn->negative) result = PARSER_MINF;
    else result = PARSER_PINF;
  }
  else if (pn->exponent < -328)
  {
    if (pn->negative) result = PARSER_MZERO;
    else result = PARSER_PZERO;
  }

  return result;
}


static double converter(struct PrepNumber *pn)
{
  long binexp = 92;
  union HexDouble hd;
  unsigned long s2, s1, s0;      /* 96-bit precision integer */
  unsigned long q2, q1, q0;      /* 96-bit precision integer */
  unsigned long r2, r1, r0;      /* 96-bit precision integer */
  unsigned long mask28 = 0xF << 28;

  hd.u = 0;

  s0 = (unsigned long)(pn->mantissa & 0xFFFFFFFF);
  s1 = (unsigned long)(pn->mantissa >> 32);
  s2 = 0;

  while (pn->exponent > 0)
  {
    lsl96(s2, s1, s0, q2, q1, q0);   // q = p << 1
    lsl96(q2, q1, q0, r2, r1, r0);   // r = p << 2
    lsl96(r2, r1, r0, s2, s1, s0);   // p = p << 3
    add96(s2, s1, s0, q2, q1, q0);   // p = (p << 3) + (p << 1)

    pn->exponent--;

    while (s2 & mask28)
    {
      lsr96(s2, s1, s0, q2, q1, q0);
      binexp++;
      s2 = q2;
      s1 = q1;
      s0 = q0;
    }
  }

  while (pn->exponent < 0)
  {
    while (!(s2 & (1 << 31)))
    {
      lsl96(s2, s1, s0, q2, q1, q0);
      binexp--; 
      s2 = q2;
      s1 = q1;
      s0 = q0;
    }

    q2 = s2 / 10;
    r1 = s2 % 10;
    r2 = (s1 >> 8) | (r1 << 24);
    q1 = r2 / 10;
    r1 = r2 % 10;
    r2 = ((s1 & 0xFF) << 16) | (s0 >> 16) | (r1 << 24);
    r0 = r2 / 10;
    r1 = r2 % 10;
    q1 = (q1 << 8) | ((r0 & 0x00FF0000) >> 16);
    q0 = r0 << 16;
    r2 = (s0 & 0xFFFF) | (r1 << 16);
    q0 |= r2 / 10;
    s2 = q2;
    s1 = q1;
    s0 = q0;

    pn->exponent++;
  }

  if (s2 || s1 || s0)
  {
    while (!(s2 & mask28))
    {
      lsl96(s2, s1, s0, q2, q1, q0);
      binexp--;
      s2 = q2;
      s1 = q1;
      s0 = q0;
    }
  }

  binexp += 1023;

  if (binexp > 2046)
  {
    if (pn->negative) hd.u = DOUBLE_MINUS_INFINITY;
    else hd.u = DOUBLE_PLUS_INFINITY;
  }
  else if (binexp < 1)
  {
    if (pn->negative) hd.u = DOUBLE_MINUS_ZERO;
  }
  else if (s2)
  {
    unsigned long long q;
    unsigned long long binexs2 = (unsigned long long)binexp;

    binexs2 <<= 52;
    q = (((unsigned long long)s2 & ~mask28) << 24) | (((unsigned long long)s1 + 128) >> 8) | binexs2;
    if (pn->negative) q |= (1ULL << 63);
    hd.u = q;
  }

  return hd.d;
}


double atof(char *s)
{
  struct PrepNumber pn;
  union HexDouble hd;
  long i;
  double result;

  pn.mantissa = 0;
  pn.negative = 0;
  pn.exponent = 0;
  hd.u = DOUBLE_PLUS_ZERO;

  i = parser(s, &pn);

  switch (i)
  {
    case PARSER_OK:
      result = converter(&pn);
    break;

    case PARSER_PZERO:
      result = hd.d;
    break;

    case PARSER_MZERO:
      hd.u = DOUBLE_MINUS_ZERO;
      result = hd.d;
    break;

    case PARSER_PINF:
      hd.u = DOUBLE_PLUS_INFINITY;
      result = hd.d;
    break;

    case PARSER_MINF:
      hd.u = DOUBLE_MINUS_INFINITY;
      result = hd.d;
    break;
  }

  return result;
}
