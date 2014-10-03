/*
    Xavier Banks
    Assignment 2: Huge Fibonacci
    Date: 3/2/2014
*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "Fibonacci.h"

// Prototypes
HugeInteger *hugeAdd(HugeInteger *p, HugeInteger *q);
HugeInteger *hugeDestroyer(HugeInteger *p);
HugeInteger *parseString(char *str);
HugeInteger *parseInt(unsigned int n);
unsigned int *toUnsignedInt(HugeInteger *p);
HugeInteger *fib(int n);


// Adds HugeIntegers p and q and returns the sum.
HugeInteger *hugeAdd(HugeInteger *p, HugeInteger *q)
{
    int i, a, b;
    HugeInteger *sum;

    if(p == NULL || q == NULL || p->digits == NULL || q->digits == NULL)
        return NULL;

    sum = calloc(1, sizeof(HugeInteger));

    if(sum == NULL)
        return NULL;

    // Finds the larger length of the two numbers
    sum->length = (p->length > q->length) ? p->length : q->length;
    sum->digits = calloc(sum->length+1, sizeof(int));
    
    if(sum->digits == NULL)
    {
        free(sum);
        return NULL;
    }

    // Adds the two HugeIntegers
    for(i = 0; i < sum->length; i++)
    {
        if(p->length <= i && q->length <= i)
            break;
        // gives the correct value without going out of bounds
        a = (i < p->length) ? p->digits[i] : 0;
        b = (i < q->length) ? q->digits[i] : 0;
        sum->digits[i] += (a+b);

        // Makes sure to carry the 1
        if(sum->digits[i] > 9)
        {
            sum->digits[i+1] += 1;
            if(i == sum->length-1)
                sum->length += 1;
        }
        
        sum->digits[i] %= 10; 
    }

    return sum;
}

// Destroyes and frees memory from the HugeInteger
HugeInteger *hugeDestroyer(HugeInteger *p)
{
    if(p == NULL)
        return NULL;

    if(p->digits == NULL)
    {
        free(p->digits);
        return NULL;
    }

    free(p->digits);
    free(p);

    return NULL;
}

// Parses a string and converts it into the HugeInteger
// value that it returns
HugeInteger *parseString(char *str)
{
    int i, a, j = 0;
    HugeInteger *hugeInt;

    if(str == NULL)
        return NULL;

    hugeInt = malloc(sizeof(HugeInteger));
   
    if (hugeInt == NULL)
            return NULL;

    if(str != "")
    {
    	hugeInt->digits = calloc(strlen(str), sizeof(int));
    	if(hugeInt->digits == NULL)
	    {
	        free(hugeInt);
	        return NULL;
	    }
        hugeInt->length = strlen(str);
        
        for(i = hugeInt->length-1; i >= 0; i--)
        {
            // Converts a character to an integer value
            a = str[i] - '0';
            hugeInt->digits[j] = a;
            j++;
        }
    }
    else
    {   // Treats the "" empty string as a "0"
		
        hugeInt->length = 1;
        hugeInt->digits = calloc(hugeInt->length, sizeof(int));
        if(hugeInt->digits == NULL)
	    {
	        free(hugeInt);
	        return NULL;
	    }
        hugeInt->digits[0] = 0;
    }

    return hugeInt;
}

// Converts the input value to a HugeInteger which it returns
HugeInteger *parseInt(unsigned int n)
{
    HugeInteger *hugeInt;
    unsigned int temp = n;
    int i, j = 0;

    hugeInt = malloc(sizeof(HugeInteger));
    
    if(hugeInt == NULL)
        return NULL;
    
    hugeInt->length = 1;
    
    // Calculates the length of the number
    while(temp >= 10)
    {
        hugeInt->length += 1;
        temp = temp/10;
    }

    hugeInt->digits = calloc(hugeInt->length, sizeof(int));

    if(hugeInt->digits == NULL){
        free(hugeInt);
        return NULL;
    }

    for (i = hugeInt->length-1; i >= 0; i--)
    {
        hugeInt->digits[j] = (n == 0) ? 0 : n % 10;
        n = n / 10;
        j++;
    }

    return hugeInt;
}

// Converts a HugeInteger to an unsigned int
unsigned int *toUnsignedInt(HugeInteger *p)
{
    int i;
    unsigned int *num;
    HugeInteger *max_unsigned;

    if(p == NULL || p->digits == NULL || p->length > 10)
        return NULL;

    max_unsigned = parseInt(UINT_MAX);
    if(max_unsigned == NULL)
        return NULL;

    // Checking if given value can be represented as an unsigned int
    if(p->length == 10)
        for(i = 9; i >= 0; i--)
        {
            if(p->digits[i] <= max_unsigned->digits[i])
                break;
            else
                return hugeDestroyer(max_unsigned);
        }

    num = calloc(1, sizeof(unsigned int));

    // Converts the number to an unsigned int value
    for(i = p->length-1; i >= 0 ; i--)
        *num = *num*10 + p->digits[i];

    hugeDestroyer(max_unsigned);
    return num;
}

// Calculates and returns the nth fibonacci term
HugeInteger *fib(int n)
{
    int i;
    HugeInteger *Fn, **ints;

    // Base cases for Fibonacci
    if(n == 0 || n == 1)
        return parseInt(n);
    if(n == 2)
        return parseInt(1);

    ints = malloc(sizeof(HugeInteger*) * n);
    ints[0] = parseInt(0);
    ints[1] = parseInt(1);
    
    // Makes sure to only keep the previous two Fn values in memory
    for(i = 2; i < n; i++)
    {
        ints[i] = hugeAdd(ints[i-1], ints[i-2]);
        hugeDestroyer(ints[i-2]);
    }

    // Calculates the nth term
    Fn = hugeAdd(ints[i-1], ints[i-2]);
    hugeDestroyer(ints[i-1]);
    hugeDestroyer(ints[i-2]);
    free(ints);

    return Fn;
}
