/***********************************************************
   NAME: Jason Barker                                              
   ORGN: MIT                                             
   FILE: AngleCalcs.cpp                                          
   DATE: Feb 28, 2019                                              
************************************************************/

#include "AngleCalcs.h"


double calcConstant (double a, double b){
	//This value is c_o / g
	double constant = a/b;
	return(constant);
}

double calc2Distance (double a, double b, double c, double d){
	//Order of variables O/S X,Y then TGT X,Y
	double straight_distance = sqrt(pow((c - a), 2) + pow((d - b), 2));
	return(straight_distance);
}

double calc3Distance (double a, double b, double c, double d, double e, double f){
	//Order of variables O/S X,Y,Z  then TGT X,Y,Z
	double three_distance = sqrt(pow((d - a), 2) + pow((e - b), 2) + pow((f - c), 2));
	return(three_distance);	
}

double calcSoundSpeed (double a, double b, double c){
	//This calcs sound speed at depth using surface sound speed, ship depth, and speed gradient
	double sound_speed = a + (b * c);
	return(sound_speed);
}

double calcCircleCenter (double a, double b, double c, double d){
	//This is the calculation for the geometric center of the circle with radius R to ownship and target
	//Variables are Constant (c_o/g), Ownship Depth, Target Depth, Straight_Line Distance
	double cirle_center = ((pow((a + c), 2)-pow((a + b), 2))/(2*d))+(d/2);
	return(cirle_center);
}

double calcRadius (double a, double b, double c){
	//This calculates the Radius shared by both vessels provided that the circle center has been calculated
	//Variables Ownship Depth, Constant (c_o/g), and Circle Center
	double circle_radius = sqrt(pow(c, 2) + pow((b + a),2));
	return(circle_radius);
}

double calcArcLength (double a, double b){
	//This calculates the arclength without theta using circle radius and straight line distance between two points
	double arc_length = 2*a*asin(b/(2*a));
	return(arc_length);
}

double calcElevAngle (double a, double b, double c){
	//Calculates Elevation Angle using Incident Sound Speed, Circle Radius, and Speed Gradient
	double calc_elev_ang = (180/M_PI)*acos(a/(b * c));
	return(calc_elev_ang);
}

double calcRs (double a, double b, double c){
	//Calculates r(s) using Radius, Arclength, and Calculated Elevation Angle
	double r_s = a * ((sin(c*(M_PI/180))) + (sin((b/a) - (c * (M_PI/180)))));
	return(r_s);
}

double calcZs (double a, double b, double c, double d){
	//Calculates z(s) using Radius, Arclength, Calculated Elevation Angle, and the Constant (c_o/g)
	double z_s = (a * cos((b/a) - (c * (M_PI/180)))) - d;
	return(z_s);
}