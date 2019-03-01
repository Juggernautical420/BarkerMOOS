/***********************************************************
   NAME: Jason Barker                                              
   ORGN: MIT                                             
   FILE: AngleCalcs.h                                          
   DATE: Feb 28, 2019                                              
************************************************************/

#define _USE_MATH_DEFINES

#include <sstream>
#include <math.h> 
#include <string> 
#include <cstdint>
#include <list>
#include <iostream>
#include <cmath>

using namespace std;


double calcConstant (double a, double b);

double calc2Distance (double a, double b, double c, double d);

double calc3Distance (double a, double b, double c, double d, double e, double f);

double calcSoundSpeed (double a, double b, double c);

double calcCircleCenter (double a, double b, double c, double d);

double calcRadius (double a, double b, double c);

double calcArcLength (double a, double b);

double calcElevAngle (double a, double b, double c);

double calcRs (double a, double b, double c);

double calcZs (double a, double b, double c, double d);


