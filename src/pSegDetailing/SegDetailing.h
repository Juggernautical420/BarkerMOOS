/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: SegDetailing.h                                          */
/*    DATE: January 8, 2020                             */
/************************************************************/

#ifndef SegDetailing_HEADER
#define SegDetailing_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include "GeomUtils.h"
#include "AngleUtils.h"
#include "XYSegList.h"
#include "XYFormatUtilsSegl.h"
#include "SegParts.h"

class SegDetailing : public AppCastingMOOSApp
{
 public:
   SegDetailing();
   ~SegDetailing();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

   // SegParts os_details;
   // XYSegList os_seglist;

 private: // Configuration variables

 private: // State variables
};

#endif 
