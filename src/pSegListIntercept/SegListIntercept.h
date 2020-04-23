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


 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();


 protected:
   void registerVariables();
   std::string m_veh_name;
   std::string m_vname;
   std::string m_veh_handle;
   std::string m_list_name;

   XYSegList m_os_seglist;
   std::vector<double> m_length;
   std::vector<double> m_time;
   

   XYPoint m_point;
   SegIntercept m_os_intercept;
   std::string m_current_name;
   double m_current_spd;

   std::vector<std::string> m_con_segnames;
   std::vector<XYSegList>  m_con_seglists;

   std::vector<std::string> m_con_nodenames;
   std::vector<double> m_con_nodespds;

   SegListContactSet m_tss_contacts;


 private: // Configuration variables


 private: // State variables
};

#endif 
