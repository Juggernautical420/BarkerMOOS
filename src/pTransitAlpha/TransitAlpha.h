/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TransitAlpha.h                                          */
/*    DATE: October 18, 2019                             */
/*	  PURPOSE: This is a shoreside application to take strings */
/*			   of points from the configuration parameters and generates */
/*			   inbound and outbound visual polygons.    		*/
/************************************************************/

#ifndef TransitAlpha_HEADER
#define TransitAlpha_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "XYPoint.h"
#include "XYPolygon.h"
#include "XYSegList.h"
#include <string>
#include <vector>


class TransitAlpha : public AppCastingMOOSApp
{
 public:
   TransitAlpha();
   ~TransitAlpha();

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

 std::string InB_PolyMsg;
 std::string OutB_PolyMsg;
 private: // State variables
};

#endif 
