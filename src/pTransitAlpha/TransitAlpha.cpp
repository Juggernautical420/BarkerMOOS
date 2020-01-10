/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TransitAlpha.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <math.h>
#include <cstdlib>
#include "MBUtils.h"
#include "ACTable.h"
#include "TransitAlpha.h"
#include "XYFormatUtilsPoly.h"

using namespace std;

//---------------------------------------------------------
// Constructor

TransitAlpha::TransitAlpha()
{
}

//---------------------------------------------------------
// Destructor

TransitAlpha::~TransitAlpha()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool TransitAlpha::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
     // The following is for having the transit lane appcast vehicles inside the lane.  
     // This could be useful as a flag for identifying vehicles compliant with the traffic lane
     // 
     // if (key == "NODE_MESSAGE_LOCAL"){
     //    string sval  = msg.GetString(); 
     //    string veh_name = tokStringParse(sval, "NAME", ',', '=');
     //    string veh_nav_x = tokStringParse(sval, "X", ',', '=');
     //    string veh_nav_y = tokStringParse(sval, "Y", ',', '=');
     //    string veh_spd = tokStringParse(sval, "SPD", ',', '=');
     //    double nav_x = stod(veh_nav_x);
     //    double nav_y = stod(veh_nav_y);
     //    // if((poly_sb.contains(nav_x, nav_y)) || (poly_nb.contains(nav_x, nav_y))){
        //  vehname = veh_name;
        //  vehx = veh_nav_x;
        //  vehy = veh_nav_y;
        //  vehspd = veh_spd; 
        // } 
   

     //    string veh_spd = tokStringParse(sval, "SPD", ',', '=');
          //       if((poly_nb.contains(nav_x, nav_y)) || (poly_sb.contains(nav_x, nav_y)))
          // m_vessels_name.push_back(veh_name);
          //   for (int i=0; i<m_vessels_name.size(); i++)
          //     string vehnames = m_vessels_name[i];

          // m_vessels_x.push_back(veh_nav_x);
          //   for (int i=0; i<m_vessels_x.size(); i++)
          //     string vehx = m_vessels_x[i];

          // m_vessels_y.push_back(veh_nav_y);
          //   for (int i=0; i<m_vessels_y.size(); i++)
          //     string vehy = m_vessels_y[i]; 

          // m_vessels_spd.push_back(veh_spd);
          //   for (int i=0; i<m_vessels_spd.size(); i++)
          //     string vehspd = m_vessels_spd[i]; 
  
     // }

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     if(key == "FOO") 
       cout << "great!";

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool TransitAlpha::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool TransitAlpha::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!

  

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool TransitAlpha::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();

  STRING_LIST sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

  STRING_LIST::iterator p;
  for(p=sParams.begin(); p!=sParams.end(); p++) {
    string orig  = *p;
    string line  = *p;
    string param = tolower(biteStringX(line, '='));
    string value = line;

    bool handled = false;
    if(param == "foo") {
      handled = true;
    }
    else if(param == "bar") {
      handled = true;
    }
    if(param == "port"){
      string SB_poly = value;
      Notify("SB_POLY", SB_poly);
  
      XYPolygon poly_sb = string2Poly(value);
      //poly_sb.set_label("Southbound");
      poly_sb.set_color("label", "red");
      poly_sb.set_color("vertex", "red");
      poly_sb.set_color("edge", "red");
      poly_sb.set_color("fill", "red");
      poly_sb.set_transparency(0.05);
      poly_sb.set_edge_size(1);
      poly_sb.set_vertex_size(2);

      string poly_sb_spec = poly_sb.get_spec();
      //Notify("VIEW_POLYGON", poly_sb_spec);
      m_polygons.push_back(poly_sb_spec);
      for (int i=0; i<m_polygons.size(); i++)
      Notify ("VIEW_POLYGON", m_polygons[i]); 
      
      handled = true;
    }
    if(param == "starboard"){
      string NB_poly = value;
      Notify("NB_POLY", NB_poly);

      XYPolygon poly_nb = string2Poly(value);
      //poly_nb.set_label("Northbound");     
      poly_nb.set_color("label", "green");
      poly_nb.set_color("vertex", "green");
      poly_nb.set_color("edge", "green");
      poly_nb.set_color("fill", "green");
      poly_nb.set_transparency(0.025);
      poly_nb.set_edge_size(1);
      poly_nb.set_vertex_size(2);

      string poly_nb_spec = poly_nb.get_spec();
     // Notify("VIEW_POLYGON", poly_nb_spec);
      m_polygons.push_back(poly_nb_spec);
      for (int i=0; i<m_polygons.size(); i++)
      Notify ("VIEW_POLYGON", m_polygons[i]); 
       
      handled = true;
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void TransitAlpha::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  // Register("NODE_MESSAGE_LOCAL", 0); //Register Node_Report to determine if vehicle nav x,y
  //                             // are contained within the polygon
}


//------------------------------------------------------------
// Procedure: buildReport()

bool TransitAlpha::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "Vehicles in Transit Lane                    " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  actab << "Name | Nav X | Nav Y | Speed";
  actab.addHeaderLines();
  actab << veh_name << veh_nav_x << veh_nav_y << veh_spd;
  m_msgs << actab.getFormattedString();

  return(true);
}




