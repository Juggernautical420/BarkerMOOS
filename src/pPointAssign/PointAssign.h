/************************************************************/
/*    NAME: Jason Barker                                    */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.h                                   */
/*    DATE: MAR 15, 2019                                    */
/************************************************************/

#ifndef PointAssign_HEADER
#define PointAssign_HEADER

#include "MOOS/libMOOS/MOOSLib.h"
#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h" // Allows AppCasting
#include <string>
#include <vector> 
#include "XYPoint.h"

using namespace std;

class PointAssign : public AppCastingMOOSApp
{
 public:
   PointAssign();
   ~PointAssign();
  

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();
   std::string m_veh_1;
   std::string m_veh_2;
  
   
 protected:
   void RegisterVariables();
   void VectorPoints(std::string sval);
   void AssignAlternating(std::string sval);
   void AssignRegionally(std::string sval);
   void postViewPoint(double x, double y, std::string label, std::string color);
   bool VehicalAssign1(std::string sval);
   bool VehicalAssign2(std::string sval);
   bool buildReport(); 


   std::string m_color;
   std::string point;
   std::string vname;
   std::string vname1;
   std::string vname2;
 

   int m_string_x;
   int m_string_y;
   int m_string_id;

   bool assign_by_region;
   bool m_veh1_named;
   bool m_veh2_named;
   //bool m_veh2_named;

 private: // Configuration variables

 private: // State variables
};

#endif 
