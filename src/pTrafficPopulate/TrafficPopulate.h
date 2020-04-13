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
#include "XYPolygon.h"
#include "XYFormatUtilsPoly.h"


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
   void publishTrafficInfo(std::vector<std::string> svector, std::string varname);


 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables
   TrafficScheme m_tss;
   std::vector<std::string> m_polygons;
   std::vector<std::string> m_files;
   std::vector<std::string> m_lane_boundaries;
   std::vector<std::string> m_sep_zones;
   std::vector<std::string> m_gen_poly_specs;
   std::vector<std::string> m_gen_poly_hdgs;



   std::string m_sepz_output_var;
   std::string m_genpoly_output_var;
   std::string m_genpolyhdg_output_var;




 private: // State variables
};

#endif 
