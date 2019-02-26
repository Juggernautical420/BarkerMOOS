/************************************************************/
/*    NAME: Jason Barker                                             
/*    ORGN: MIT                                             
/*    FILE: Priming.cpp                                        
/*    DATE: Feb 21, 2019 
/* 	  This app returns the prime numbers associated with a given input  
/*    This is included only as reference for prime factorization                                             
/************************************************************/

// Including //
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <stdio.h>      
#include <math.h> 

// Using //
using namespace std;

// Functions // From online: https://www.geeksforgeeks.org/print-all-prime-factors-of-a-given-number/
void primeFactors(int n) 
{ 
    // Print the number of 2s that divide n 
    while (n%2 == 0) 
    { 
        printf("%d ", 2); 
        n = n/2; 
    } 
  
    // n must be odd at this point.  So we can skip  
    // one element (Note i = i +2) 
    for (int i = 3; i <= sqrt(n); i = i+2) 
    { 
        // While i divides n, print i and divide n 
        while (n%i == 0) 
        { 
            printf("%d ", i); 
            n = n/i; 
        } 
    } 
  
    // This condition is to handle the case when n  
    // is a prime number greater than 2 
    if (n > 2) 
        printf ("%d ", n); 
}

// Main //
int main(int argc, char **argv){// This takes command line arguments as inputs
    //int argc counts the number of arguments and argc=1 is the function name
    //char **argv stores the arguments as integers
    int a = atoi(argv[1]); //Declares the first argument as variable a
    primeFactors(a); 
return 0;
}