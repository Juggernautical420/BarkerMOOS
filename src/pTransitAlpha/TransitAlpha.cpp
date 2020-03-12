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
#include <numeric>
#include <iostream>
#include <algorithm> 


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
    if(param == "inbound"){
      string InB_poly = value;
      InB_pts.push_back(InB_poly);
      InB_size = uintToString(InB_pts.size());
      for (int pts=0; pts<InB_pts.size(); pts++){
        string InB_PolyMsg;
        InB_PolyMsg = accumulate(begin(InB_pts), end(InB_pts), string(), [](string lhs, const string &rhs) { return lhs.empty() ? rhs : lhs + "; " + rhs; });
        Notify("INBOUND_POLY", InB_PolyMsg);
      }
  
      XYPolygon poly_Inb = string2Poly(value);
      //poly_Inb.set_label("Inbound");
      poly_Inb.set_color("label", "red");
      poly_Inb.set_color("vertex", "red");
      poly_Inb.set_color("edge", "red");
      poly_Inb.set_color("fill", "red");
      poly_Inb.set_transparency(0.05);
      poly_Inb.set_edge_size(1);
      poly_Inb.set_vertex_size(2);

      string poly_Inb_spec = poly_Inb.get_spec();
      //Notify("VIEW_POLYGON", poly_Inb_spec);
      m_polygons.push_back(poly_Inb_spec);
      for (int i=0; i<m_polygons.size(); i++)
      Notify ("VIEW_POLYGON", m_polygons[i]); 
                
      handled = true;
    }
    if(param == "outbound"){
      string OutB_poly = value;
      OutB_pts.push_back(OutB_poly);
      OutB_size = uintToString(OutB_pts.size());
      for (int pts=0; pts<OutB_pts.size(); pts++){
        string OutB_PolyMsg;
        OutB_PolyMsg = accumulate(begin(OutB_pts), end(OutB_pts), string(), [](string lhs, const string &rhs) { return lhs.empty() ? rhs : lhs + "; " + rhs; });
      Notify("OUTBOUND_POLY", OutB_PolyMsg);
    }

      XYPolygon poly_Outb = string2Poly(value);
      //poly_Outb.set_label("Outbound");     
      poly_Outb.set_color("label", "green");
      poly_Outb.set_color("vertex", "green");
      poly_Outb.set_color("edge", "green");
      poly_Outb.set_color("fill", "green");
      poly_Outb.set_transparency(0.025);
      poly_Outb.set_edge_size(1);
      poly_Outb.set_vertex_size(2);

      string poly_Outb_spec = poly_Outb.get_spec();
     // Notify("VIEW_POLYGON", poly_Outb_spec);
      m_polygons.push_back(poly_Outb_spec);
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
  m_msgs << " Traffic Separation Lanes                   " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(3);
  actab << "Name" << "Inbound Lane" << "Outbound Lane";
  actab.addHeaderLines();
  actab << "Number of Polygons" << InB_size << OutB_size;
  actab.addHeaderLines();
  int m_polys = max(InB_pts.size(),OutB_pts.size());
  for (int pts=0; pts<m_polys; pts++)
    actab << "Poly Points" << InB_pts[pts] << OutB_pts[pts];

  m_msgs << actab.getFormattedString();
  return(true);
}




