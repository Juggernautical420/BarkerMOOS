/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TrafficGrade.h                                          */
/*    DATE: Mar 31, 2020                            */
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
   int m_nm_count;
   int m_coll_count;
   double m_traffic_grade;
   bool collision;
 

   std::vector<double> m_nm_cpa;
   double m_traffic_slope;
   std::vector<std::string> m_contact_name;
   std::vector<std::string> m_interaction;

   std::string traffic_score;
   std::string coll_cnt;
   std::string nm_cnt;

 private: // Configuration variables

 private: // State variables
};

#endif 
