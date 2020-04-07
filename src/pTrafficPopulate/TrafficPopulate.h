/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TrafficPopulate.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef TrafficPopulate_HEADER
#define TrafficPopulate_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <vector>
#include <string>
#include "FileBuffer.h"
#include "TrafficScheme.h"
#include "TrafficObject.h"


class TrafficPopulate : public AppCastingMOOSApp
{
 public:
   TrafficPopulate();
   ~TrafficPopulate();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void handleTrafficFile(std::vector<std::string> svector);

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables
   TrafficScheme m_tss;
   std::vector<std::string> m_polygons;
   std::vector<std::string> m_files;
   std::vector<std::string> m_lane_boundaries;


 private: // State variables
};

#endif 
