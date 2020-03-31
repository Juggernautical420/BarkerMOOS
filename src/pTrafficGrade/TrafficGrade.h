/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TrafficGrade.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef TrafficGrade_HEADER
#define TrafficGrade_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include <vector>
#include <string>

class TrafficGrade : public AppCastingMOOSApp
{
 public:
   TrafficGrade();
   ~TrafficGrade();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();
   void processParameters(std::string str);
   void processNearMiss(std::string str);
   bool processCollision(std::string str);

 protected:
   void registerVariables();
   double m_coll_range;
   double m_nm_range;
   double m_nm_count;
   double m_coll_count;
   double m_traffic_grade;

   std::vector<double> m_nm_cpa;
   std::vector<std::string> m_contact_name;

 private: // Configuration variables

 private: // State variables
};

#endif 
