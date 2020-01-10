/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: SegPassing.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef SegPassing_HEADER
#define SegPassing_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include <cstring>
#include "NodeMessage.h"

class SegPassing : public AppCastingMOOSApp
{
 public:
   SegPassing();
   ~SegPassing();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();
   std::string m_veh_name;
   std::string m_vname;
   std::string m_list_name;
   std::string str_seglist;
   std::string seglist_report;
   std::string m_node_message;
   std::string m_str_name;

   std::string m_source;
   std::string m_dest;
   std::string m_var_name;
   std::string m_str_val;

   // NodeMessage seglist_message;


 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables
};

#endif 
