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
#include "TrafficScheme.h"
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
   void createTSSPolys();


 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables
 double m_nav_x;
 double m_nav_y;
 double m_nav_hdg;
 bool m_got_sepzs;
 bool m_got_lines;
 bool m_got_scheme;

 bool tss_status;
 


 std::vector<std::string> m_sep_zones;
 std::vector<std::string> m_lane_seglists;

 TrafficScheme m_tss;
 std::vector<std::string> m_tss_genpolys;
 std::vector<XYPolygon> m_tss_gen_lanes;
 std::vector<std::string> m_tss_gen_hdgs;
 std::vector<double> m_tss_lane_headings;

/////////////////////////////
 std::vector<XYPolygon> m_tss_sepz;
 std::vector<std::string> m_tss_inbound;
 std::vector<XYPolygon> m_inbound_lanes;
 std::vector<std::string> m_tss_outbound;
 std::vector<XYPolygon> m_outbound_lanes;

//////////////////////////////
 std::string m_veh_name;
 std::string m_vname;
 std::string m_veh_handle;
 std::string m_list_name; 
 XYSegList m_os_seglist;
 
 private: // State variables
};

#endif 
