/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TSSCompliance.h                                          */
/*    DATE: March 12, 2020                            */
/************************************************************/

#ifndef TSSCompliance_HEADER
#define TSSCompliance_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYPoint.h"
#include "XYPolygon.h"
#include "XYSegList.h"
#include <string>
#include <vector>

class TSSCompliance : public AppCastingMOOSApp
{
 public:
   TSSCompliance();
   ~TSSCompliance();

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
 double m_nav_x;
 double m_nav_y;
 double m_nav_hdg;
 bool m_inbound_lane;
 bool m_outbound_lane;

 std::vector<std::string> m_InB_polygons;
 std::vector<std::string> m_OutB_polygons;

 
 private: // State variables
};

#endif 
