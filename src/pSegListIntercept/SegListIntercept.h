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
#include "GeomUtils.h"
#include "AngleUtils.h"
#include "XYSegList.h"
#include "XYPoint.h"
#include "XYFormatUtilsSegl.h"
#include "XYFormatUtilsPoint.h"
#include "SegIntercept.h"
#include "SegParts.h"
#include "SegListExtrapolator.h"


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

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();


 protected:
   void registerVariables();
   std::string m_veh_name;
   std::string m_vname;
   std::string m_veh_handle;

   std::string str_os_seglist;
   std::string str_contact_seglist;

   XYSegList os_seglist;
   XYSegList contact_seglist;

   std::string os_remaining_seglist;


   std::vector<double> m_px;
   std::vector<double> m_py;
   std::vector<std::string> m_con_name;

 
   XYPoint point;

   SegIntercept m_charlie_dana;

 ///////////////////////////
   SegParts m_os_details;
   double m_nav_spd;
////////////////////////////
   SegListExtrapolator os_extra_pts;
   double ppx;
   double ppy;

 private: // Configuration variables


 private: // State variables
};

#endif 
