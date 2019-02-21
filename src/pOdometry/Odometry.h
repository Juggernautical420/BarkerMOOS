/************************************************************/
/*    NAME: Jason Barker                                              
/*    ORGN: MIT                                           
/*    FILE: Odometry.h                                          
/*    DATE: Feb 14, 2019                                             
/************************************************************/

#ifndef Odometry_HEADER
#define Odometry_HEADER

#include "MOOS/libMOOS/MOOSLib.h"  // Standard MOOS App library
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h" // Allows AppCasting

class Odometry : public AppCastingMOOSApp
{
 public:
   Odometry();
   ~Odometry();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected:
   void RegisterVariables();
   bool   m_first_reading;  // Class Variables
   double m_current_x;
   double m_current_y;
   double m_previous_x;
   double m_previous_y;
   double m_total_distance;
 private: // Configuration variables

 private: // State variables
bool buildReport();  // AppCasting Output buildReport()
};

#endif 
