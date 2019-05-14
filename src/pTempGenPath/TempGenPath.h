/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TempGenPath.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef TempGenPath_HEADER
#define TempGenPath_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYSegList.h"
#include <string>
#include <vector> 
#include <cstdlib>

class TempGenPath : public AppCastingMOOSApp
{
 public:
   TempGenPath();
   ~TempGenPath();
   bool cycle_done;
   bool initial_temp;
   bool left_points;
   bool right_points;
   bool period_attempt;

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();
   

 protected:
   void registerVariables();
   void TempPoints(std::string sval);

   XYSegList m_waypoints;
   std::vector<std::string> myvector;
   std::string m_string_vname;
   std::string m_cycle_done;
   std::string m_temp_report;
   std::string m_period_try;
   double m_string_utc;
   double m_string_x;
   double m_string_y;
   double m_string_temp;

   double m_next_wpt;
   
   double m_initial_temp;
   double m_current_temp;
   double m_t_cold;
   double m_t_hot;
   double m_tave;
   double m_current_hot;
   double m_temp_delta;
   double m_delta_h;
   double m_delta_l;
 



 private: // Configuration variables

 private: // State variables
};

#endif 
