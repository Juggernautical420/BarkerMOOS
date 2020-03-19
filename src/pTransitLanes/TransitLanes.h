/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TransitLanes.h                                          */
/*    DATE: March 18, 2020                             */
/*	  PURPOSE: This is a shoreside application to take strings */
/*			   of points from the configuration parameters and generates */
/*			   inbound and outbound visual polygons.    		*/
/************************************************************/

#ifndef TransitLanes_HEADER
#define TransitLanes_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYPoint.h"
#include "XYPolygon.h"
#include "XYSegList.h"
#include <string>
#include <vector>

class TransitLanes : public AppCastingMOOSApp
{
 public:
   TransitLanes();
   ~TransitLanes();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables
 XYPolygon m_poly_Outb;
 XYPolygon m_poly_Inb;
 
 std::string InB_size;
 std::string OutB_size;

 std::vector<std::string> m_polygons;
 std::vector<std::string> InB_pts;
 std::vector<std::string> OutB_pts;
 std::vector<std::string> InB_midpoints;
 std::vector<std::string> OutB_midpoints;
 std::vector<std::string> InB_hdgs;
 std::vector<std::string> OutB_hdgs;

 std::string InB_PolyMsg;
 std::string OutB_PolyMsg;

 std::string InB_HdgMsg;
 std::string OutB_HdgMsg;

 private: // State variables
};

#endif 
