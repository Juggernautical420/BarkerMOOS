/************************************************************/
/*    NAME: Jason Barker                                    */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.h                                       */
/*    DATE: 19 MR 2019                                      */
/************************************************************/

#ifndef GenPath_HEADER
#define GenPath_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <string>
#include "XYSegList.h"

class GenPath : public AppCastingMOOSApp
{
 public:
   GenPath();
   ~GenPath();
   double m_origin_x;
   double m_origin_y;
   double next_x;
   double next_y;
   

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
   void VectorPoints(std::string point);
 
  
   
   double m_dbl_x;
   double m_dbl_y;
   double m_dbl_id;
   double m_current_x;
   double m_current_y;
   double x_check;
   double y_check;
   std::string point;
   std::string m_visit_point;

   XYSegList sorted_points;
   XYSegList m_waypoints;
   

 private: // Configuration variables
   std::string m_veh_name;
   double visit_radius;
  
   

 
  

   std::string m_veh1_color;
   std::string m_veh2_color;
 private: // State variables
  
};

#endif 
