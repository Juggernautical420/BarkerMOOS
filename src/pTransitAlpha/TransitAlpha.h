/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TransitAlpha.h                                          */
/*    DATE: October 18, 2019                             */
/*	  PURPOSE: This is a shoreside application to take two strings */
/*			   of points from the configuration parameters and generate */
/*			   northbound and southbound visual polygons		*/
/************************************************************/

#ifndef TransitAlpha_HEADER
#define TransitAlpha_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYPoint.h"
#include "XYPolygon.h"
#include "XYSegList.h"
#include <string>
#include <vector>


class TransitAlpha : public AppCastingMOOSApp
{
 public:
   TransitAlpha();
   ~TransitAlpha();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();


 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();


 private: // Configuration variables
 XYPolygon m_poly_nb;
 XYPolygon m_poly_sb;
 
 std::string veh_name;
 std::string veh_nav_x;
 std::string veh_nav_y;
 std::string veh_spd;

 std::vector<std::string> m_polygons;
 // std::vector<std::string> m_vessels_name;
 // std::vector<std::string> m_vessels_x;
 // std::vector<std::string> m_vessels_y;
 // std::vector<std::string> m_vessels_spd;
 private: // State variables
};

#endif 
