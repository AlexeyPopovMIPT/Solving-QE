﻿#include <stdio.h>
#include <math.h>
#include <assert.h>

#define GREEN_COLOR "\x1b[37;42m"
#define   RED_COLOR "\x1b[37;41m"
#define BLACK_COLOR "\x1b[0m"

const double ACCURACY = 1e-9;
                  
enum {
    INF   =  4,
    CLOSE = -2,
    END = 0xABBCCC, /*11254988, do not use this number as a parameter in unittests*/
};
/*!
    This function checks if there is the end of file, user's exit command,
      correct or incorrect enter, and assigns *a,*b,*c entered values in case
        the enter is correct
    @perem [IN]      a - address of coef a
    @perem [IN]      b - address of coef b
    @perem [IN]      c - address of coef c
    @perem [IN] stream - address of input stream

    @return EOF if there is the end of file
    @return  -2 if there is user's exit command
    @return   1 if the enter is correct
    @return  -1 if the enter is incorrect
!*/
int                  fGetData(double* a, double* b, double* c, FILE* stream);
/*!
    This function solves an equation ax^2+bx+c=0 for given real a,b,c and writes the roots
        at the pointers x1,x2 if number of roots is 1 or 2

    @perem [IN]   a - coef
    @perem [IN]   b - coef
    @perem [IN]   c - coef
    @perem [IN] *x1 - pointer for the 1st root
    @perem [IN] *x2 - pointer for the 2nd root
    @BUT if pointers are equal to NULL, the function doesn't interact with pointers

    @perem [OUT] x1 - 1st root
    @perem [OUT] x2 - 2nd root

    @return number of roots (in case of infinite number of roots returns a value of
        constant INF)

!*/
int       SolveSquareEquation(double  a, double  b, double  c, double*   x1, double* x2);
/*!
    This functions checks if double-type argument is close to zero enough

    @perem [IN] x - real number

    @return 1 if absolute value of x is less than constant ACCURACY and 0 if it isn't
!*/
int                    IsZero(double  x);
/*!
    This function solves an equation ax+b=0 for given real a,b and writes the root
        to the pointer x if number of roots is 1

    @perem [IN]  a - coef
    @perem [IN]  b - coef
    @perem [IN] *x - pointer for root
    @BUT if pointer is equal to NULL, the function doesn't interact with it

    @return number of roots (in case of infinite roots returns a value of constant INF)

*/
int       SolveLinearEquation(double  a, double  b, double* x);
/*!
    This funсtion checks if the char-type argument is a delimiter character

    @perem [IN] ch - character

    @return 1 if ch equals to ' ', '\t' or '\n' and 0 if it isn't

*/
int                   IsSpace(char    ch);


void SolveSquareEquation_test(void);
void SolveLinearEquation_test(void);

int SolveSquareEquation(double a, double b, double c, double* x1, double* x2) {
    assert(x1 != x2 || x1==NULL);
    assert(a != INFINITY);
    assert(b != INFINITY);
    assert(c != INFINITY);
    assert(a == a);
    assert(b == b);
    assert(c == c);

    if (IsZero(a))
        return SolveLinearEquation(b, c, x1);
    if (IsZero(c) && !IsZero(b))
    {
        if (x1) *x1 = 0;
        return 1 + SolveLinearEquation(a, b, x2);
    }

    double d = b * b - 4 * a * c;
    if (IsZero(d)) {
        if (x1) *x1 = -b / 2 / a;
        return 1;
    }
    if (d < 0)
        return 0;

    double q = (b >= 0) ? (-b - sqrt(d)) / 2 : 
                          (-b + sqrt(d)) / 2;
    if (x1) *x1 = q / a;
    if (x2) *x2 = c / q;
    return 2;
}

int main()
{
    SolveLinearEquation_test();
    SolveSquareEquation_test();
    printf("Solving an equation ax^2+bx+c=0\nVersion 1.0, by razdolby\n");
    printf("To close the program, enter the letter \'x\'\n\n");
    printf("Enter a b c>>");

    double a = 0, b = 0, c = 0;

    int CoefsScannedCorrectly = fGetData (&a, &b, &c, stdin);

    switch (CoefsScannedCorrectly){

    case 0:
        printf(RED_COLOR "\nError: incorrect enter\n" BLACK_COLOR);
        return 0;

    case EOF:
        printf(RED_COLOR "\nError: The enter is void\n" BLACK_COLOR);

    case CLOSE:
        return 0;

    }

    double x1 = 0, x2 = 0;
    int nRoots = SolveSquareEquation (a, b, c, &x1, &x2);

    switch (nRoots) {

    case 1:
        printf( "x = %lf\n", IsZero(x1) ? 0 : x1);
        break;

    case 2:
        printf("x1 = %lf\n", IsZero(x1) ? 0 : x1);
        printf("x2 = %lf\n", IsZero(x2) ? 0 : x2);
        break;

    case 0:
        printf("No roots\n");
        break;
        
    case INF:
        printf("Infinite roots\n");
        break;

    default:
        printf(RED_COLOR "Error code %d\n" BLACK_COLOR, nRoots);

    }

    return 0;
}

/*!
    This function checks if there is the end of file, user's exit command,
      correct or incorrect enter, and assigns *a,*b,*c entered values in case
        the enter is correct
    @perem [IN]      a - address of coef a
    @perem [IN]      b - address of coef b
    @perem [IN]      c - address of coef c
    @perem [IN] stream - address of input stream

    @return EOF if there is the end of file
    @return  -2 if there is user's exit command
    @return   1 if the enter is correct
    @return  -1 if the enter is incorrect
!*/
int fGetData(double* a, double* b, double* c, FILE* stream) {
    assert(a != b);
    assert(b != c);
    assert(a != c);
    assert(a != NULL);
    assert(b != NULL);
    assert(c != NULL);

    if (stream == NULL) stream = stdin;

    char ch = 0;
    while (IsSpace (ch = getchar()));
    if (ch == EOF)
        return EOF;
    if (ch == 'x' || ch == 'X')
        return CLOSE;
    ungetc (ch, stream);

    return fscanf_s(stream,"%lf %lf %lf", a,b,c) == 3;
}

int IsZero(double x) {
    return fabs(x) < ACCURACY;
}

int SolveLinearEquation(double a, double b, double *x) {
    assert(a != INFINITY);
    assert(b != INFINITY);
    assert(a == a);
    assert(b == b);
    assert(x != NULL);

    if (IsZero(a)) {
        if (IsZero(b))
            return INF;
        return 0;
    }
    if (x) *x = -b / a;
    return 1;
}

int IsSpace(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\n';
}

void SolveLinearEquation_test(void) {

    double as[] = { 0,           0,   -0.000008,153, 8317.7897, 1024,   99999.999, END },
           bs[] = { 0, 0.000000002,     1564615,  0, 8317.7897,65536,   11111.111 };
    int    ns[] = { INF,         0,           1,  1,         1,    1,           1 };
    double xs[] = { 0,           0,195576875000,  0,        -1,  -64,-0.111111111 };

    int n; double x;

    for (int i = 0; as[i] != END; i++) {
        n = SolveLinearEquation(as[i], bs[i], &x);
        if (n != ns[i])
            printf(RED_COLOR "Test %d failed: expected n=%d, got %d\n",
                                   i,                    ns[i],  n);
        else if (n == 1 && !IsZero(x - xs[i]))
            printf(RED_COLOR "Test %d failed: expected x1=%lf, got %lf\n",
                                   i,                     xs[i],   x);
        else
            printf(GREEN_COLOR "Test %d OK\n" "\x1b[0m",
                                     i);
    }
    printf(BLACK_COLOR);
}

void SolveSquareEquation_test(void) {
    double  as[] = { 1,   0,  1, 1,           0.000000000001,5632131312123.21,        7985651.64,END },
            bs[] = { 0,   0,  2, 2,              0.000000001,             213,       64.79880909 },
            cs[] = { 0,   0,  1, 2,-0.0000000000000000000032,    -0.000000001, -7985716.43880909 };
    int     ns[] = { 1, INF,  1, 0,                        1,               2,                 2 };
    double x1s[] = { 0,   0, -1, 0,                        0,               0,      -1.000008114 },
           x2s[] = { 1,   1,  1, 1,                        1,               0,                 1 };

    int n; double x1 = 0, x2 = 0;

    for (int i = 0; as[i] != END; i++) {
        n = SolveSquareEquation(as[i], bs[i], cs[i], &x1, &x2);
        if (n != ns[i])
            printf(RED_COLOR " Test %d failed: expected n=%d, got %d\n",
                                    i,                    ns[i],  n);
        else if (n == 1) {
            if(!IsZero(x1 - x1s[i]))
                printf(RED_COLOR " Test %d failed: expected x1=%lf, got %lf\n",
                                        i,                     x1s[i],  x1);
            else
                printf(GREEN_COLOR " Test %d OK\n",
                                          i);
        }

        else if (n == 2) {
            if(IsZero(x1 - x1s[i]) && IsZero(x2 - x2s[i]) || IsZero(x2 - x1s[i]) && IsZero(x1 - x2s[i]))
                printf(GREEN_COLOR " Test %d OK\n",
                                           i);
            else
                printf(RED_COLOR " Test %d failed: expected x1,x2=%lf,%lf, got %lf,%lf\n",
                                           i,                     x1s[i],x2s[i],  x1, x2);
        }
        
        else
            printf(GREEN_COLOR " Test %d OK\n",
                                       i);
    }
    printf(BLACK_COLOR);

}
