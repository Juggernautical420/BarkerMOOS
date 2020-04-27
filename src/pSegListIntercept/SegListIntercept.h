/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT - 2N                                             */
/*    FILE: SegListIntercept.h                                          */
/*    DATE: October 29, 2019                            */
/************************************************************/

#ifndef SegListIntercept_HEADER
#define SegListIntercept_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include "XYSegList.h"
#include "XYPoint.h"
#include "XYFormatUtilsSegl.h"
#include "XYFormatUtilsPoint.h"
#include "SegIntercept.h"
#include "SegListContact.h"
#include "SegListContactSet.h"
#include "GeomUtils.h"


class SegListIntercept : public AppCastingMOOSApp
{
 public:
   SegListIntercept();
   ~SegListIntercept();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   void calcTime(double speed);
   void handleNodeMsg(std::string report);
   bool isUnique(std::string name);
   void populateContacts();
   void predictContacts();
   void processParameters(std::string str);
   void manageContacts();
   



 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();


 protected:
   void registerVariables();
   std::string m_veh_name;
   std::string m_vname;
   std::string m_veh_handle;
   std::string m_list_name;

   XYSegList m_os_seglist;
   // std::vector<double> m_length;
   // std::vector<double> m_time;
   double m_nav_spd;
   double m_coll_range;
   double m_nm_range;
   double m_range_concern;
   double m_buffer;

   double m_desired_speed;

   XYPoint m_point;
   SegIntercept m_os_intercept;
   std::string m_current_name;
   double m_current_spd;

   std::vector<std::string> m_con_segnames;
   std::vector<XYSegList>  m_con_seglists;

   std::vector<std::string> m_con_nodenames;
   std::vector<double> m_con_nodespds;

   SegListContactSet m_tss_contacts;
   bool m_got_speed;
   bool m_extra_ready;
   bool m_extra_done;
   bool m_got_calc;
   bool m_got_predict;
   bool m_got_limiting;
   bool m_spd_update;

   double m_init_dist;
   std::vector<std::string> m_extrapolated_contacts;
   // std::vector<double> m_extrapo_dists;

   std::vector<std::string> m_init_limit_contacts;
   // std::vector<double> m_limit_dist;
   std::string m_input_speed;
   std::string m_final_speed;
   std::string m_limiting_contacts;
   std::string m_extra_points;
   std::string m_speed_update;
  

 private: // Configuration variables


 private: // State variables
};

#endif 
