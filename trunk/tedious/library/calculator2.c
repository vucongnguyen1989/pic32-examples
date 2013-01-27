//  File:   calculator2.c
//  Author: Yuri Panchul

#include "string.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////

enum
{
    add          = 0xa,
    substract    = 0xb,
    multiply     = 0xc,
    divide       = 0xd,
    parentheses  = 0xe,
    equal        = 0xf
};

////////////////////////////////////////////////////////////////////////////

static char buf [128] = "\n";

#define newline_before_text  buf
#define text_start           (buf + 1)

static char * p = text_start;

////////////////////////////////////////////////////////////////////////////

static void int_to_string (int n, char * s)
{
    uint i;

    if (n < 0)
    {
        * s ++ = '-';
        n = - n;
    }

    for (i = 1000 * 1000 * 1000; i >= 1; i /= 10)
    {
        if (n >= i || i == 1)
            * s ++ = '0' + n / i % 10;
    }

    * s = '\0';
}

////////////////////////////////////////////////////////////////////////////

static bool expr1 (int * pn);
static bool expr2 (int * pn);
static bool expr3 (int * pn);

////////////////////////////////////////////////////////////////////////////

static bool expr1 (int * pn)
{
    int op, n1, n2, n;

    if (! expr2 (& n1))
        return false;

    op = * p;

    if (op != '+' && op != '-')
    {
        * pn = n1;
        return true;
    }

    p ++;

    if (! expr1 (& n2))
        return false;

    if (op == '-')
        n2 = - n2;

    n = n1 + n2;

    // checking overflow

    if (    n1 > 0 && n2 > 0 && n < 0
         || n1 < 0 && n2 < 0 && n > 0)
    {
        return false;
    }

    * pn = n;
    return true;
}

////////////////////////////////////////////////////////////////////////////

static bool expr2 (int * pn)
{
    int op, n1, n2, n;

    if (! expr3 (& n1))
        return false;

    op = * p;

    if (op != '*' && op != '/')
    {
        * pn = n1;
        return true;
    }

    p ++;

    if (! expr2 (& n2))
        return false;

    if (op == '*')
    {
        n = n1 * n2;
 
        // checking overflow

        if (n2 != 0 && n / n2 != n1)
            return false;
    }
    else
    {
        if (n2 == 0)
            return false;

        n = n1 / n2;
    }

    * pn = n;
    return true;
}

////////////////////////////////////////////////////////////////////////////

static bool expr3 (int * pn)
{
    if (* p == '-' && (p == text_start || strchr ("+-*/(", p [-1])))
    {
        p ++;

        if (! expr3 (pn))
            return false;

        * pn = - *pn;
    }
    else if (* p == '(')
    {
        p ++;

        if (! expr1 (pn))
            return false;

        if (* p != ')')
            return false;

        p ++;
    }
    else if (isdigit (* p))
    {
        int n = 0;

        do
        {
            int d  = * p - '0';
            int nn = n * 10 + d;

            // checking overflow

            if ((nn - d) / 10 != n)
                return false;

            n = nn;
        }
        while (isdigit (* ++ p));

        * pn = n;
    }
    else
    {
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////

char * calculator2 (char in)
{
    if (in == equal)
    {
        int n;

        p = text_start;

        if (expr1 (& n) && *p == '\0')
        {
            int_to_string (n, text_start);
        }
        else
        {
            strcpy (text_start, "error @ ");

            int_to_string
            (
                p - text_start + 1,
                text_start + strlen (text_start)
            );
        }

        p = text_start;

        return newline_before_text;
    }

    switch (in)
    {
    case add        : in = '+'; break;
    case substract  : in = '-'; break;
    case multiply   : in = '*'; break;
    case divide     : in = '/'; break;

    case parentheses:

        if (p == text_start || strchr ("+-*/(", p [-1]))
            in = '(';
        else
            in = ')';

        break;

    default:
        
        in = '0' + in;
        break;
    }

    if (p == buf + sizeof (buf) - 2)
    {
        strcpy (text_start, "buffer overflow @ ");

        int_to_string
        (
            p - text_start + 1,
            text_start + strlen (text_start)
        );

        return newline_before_text;
    }

    * p ++ = in;
    * p    = '\0';

    return p == text_start + 1 ? newline_before_text : p - 1;
}

////////////////////////////////////////////////////////////////////////////

#ifndef __PIC32MX

#include "conio.h"
#include "ctype.h"
#include "stdio.h"

int main (void)
{
    char c;

    for (;;)
    {
        c = getch ();

        if (strchr ("zq.", c) != NULL)
            break;

        switch (c)
        {
        case '+'  : c = add         ; break;
        case '-'  : c = substract   ; break;
        case '*'  : c = multiply    ; break;
        case '/'  : c = divide      ; break;
        case '('  : c = parentheses ; break;
        case ')'  : c = parentheses ; break;
        default   : c = c - '0'     ; break;
        }

        printf ("%s", calculator (c));
    }

    return 0;
}

#endif
