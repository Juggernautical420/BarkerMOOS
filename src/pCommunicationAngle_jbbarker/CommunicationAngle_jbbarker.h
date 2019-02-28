/***********************************************************
    NAME: Jason Barker                                              
    ORGN: MIT                                             
    FILE: CommunicationAngle_jbbarker.h                                          
    DATE: 26 FEB 2019                                                
************************************************************/

#ifndef CommunicationAngle_jbbarker_HEADER
#define CommunicationAngle_jbbarker_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include <sstream>
#include <math.h> 
#include <string> 
#include <cstdint>
#include <list>
#include <iostream>
#include <cmath>


using namespace std;

class CommunicationAngle_jbbarker : public CMOOSApp
{
 public:
   CommunicationAngle_jbbarker();
   ~CommunicationAngle_jbbarker();

   //void setNAVX(double v){m_nav_x = v;};
   //void setNAVY(double v){m_nav_y = v;};
   //void setNAVZ(double v){m_nav_depth = v;};



 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();

   double m_surface_sound_speed;
   double m_sound_speed_gradient;
   double m_water_depth;
   
   double m_nav_x;
   double m_nav_y;
   double m_nav_depth;
   double m_nav_heading;
   double m_nav_speed; 

   double m_c_nav_x;
   double m_c_nav_y;
   double m_c_nav_depth;
   double m_c_nav_heading;
   double m_c_nav_speed;
  

   std::string m_coll_name;
   std::string m_ownship_name;
   std::string m_coll_nav_x;
   std::string m_coll_nav_y;
   std::string m_coll_nav_depth;
   std::string m_coll_nav_heading;
   std::string m_coll_nav_speed;

   
   double m_elev_angle;
   double m_calc_Radius;
   double m_calc_arclength;
   double m_calc_r;
   double m_calc_z;


 private: // Configuration variables
 	

 private: // State variables
 	
};

#endif 
