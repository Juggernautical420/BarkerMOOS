/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TrafficScheme.h                                          */
/*    DATE: Mar 26, 2020                             */
/************************************************************/

#ifndef TrafficScheme_HEADER
#define TrafficScheme_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <string>
#include <vector>
#include "XYTrafficScheme.h"
#include "XYTrafficObject.h"
#include "XYPolygon.h"


class TrafficScheme : public AppCastingMOOSApp
{
 public:
   TrafficScheme();
   ~TrafficScheme();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();
   bool processTSSFile(std::string filename);

 private: // Configuration variables
 XYTrafficScheme m_scenario;
 std::string  m_TSS_file;

 std::string type;
 std::string specs;

 private: // State variables

};

#endif 
