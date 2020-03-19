/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TransitLanes.cpp                                        */
/*    DATE: March 18, 2020                                                */
/************************************************************/

#include <iterator>
#include <math.h>
#include <cstdlib>
#include "MBUtils.h"
#include "ACTable.h"
#include "TransitLanes.h"
#include "XYFormatUtilsPoly.h"
#include "XYFormatUtilsSegl.h"
#include "AngleUtils.h"
#include "GeomUtils.h"
#include <numeric>
#include <iostream>
#include <algorithm> 

using namespace std;

//---------------------------------------------------------
// Constructor

TransitLanes::TransitLanes()
{
}

//---------------------------------------------------------
// Destructor

TransitLanes::~TransitLanes()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool TransitLanes::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

    if(key == "PMV_CONNECT"){
      for (int i=0; i<m_polygons.size(); i++)
      Notify ("VIEW_POLYGON", m_polygons[i]);

      for(int j=0; j<InB_midpoints.size(); j++)
      Notify ("VIEW_SEGLIST", InB_midpoints[j]);

      for(int k=0; k<OutB_midpoints.size(); k++)
      Notify ("VIEW_SEGLIST", OutB_midpoints[k]);         
    }    

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

     // else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
     //   reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool TransitLanes::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool TransitLanes::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool TransitLanes::OnStartUp()
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


    if(param == "inbound"){ //Reads in the number of polygons 
      string InB_poly = value;
      InB_pts.push_back(InB_poly); // creates a vector of string polygon information
      InB_size = uintToString(InB_pts.size()); //used for appcasting in buildreport
      for (int pts=0; pts<InB_pts.size(); pts++){
        InB_PolyMsg = accumulate(begin(InB_pts), end(InB_pts), string(), [](string lhs, const string &rhs) { return lhs.empty() ? rhs : lhs + ";" + rhs; });
        Notify("INBOUND_POLY", InB_PolyMsg); //creates a long sting information with all the polygon specs combined
      }
      
      // for generating lines through the midsection of polygons and determining heading  
      for (int r=0; r<InB_pts.size(); r++){
        XYPolygon Inbound = string2Poly(InB_pts[r]); //turns each string into a polygon
        double x1 = Inbound.get_vx(0);//assumption: polygons only have four points (weakness)
        double y1 = Inbound.get_vy(0);//not rubust to more polygons

        double x2 = Inbound.get_vx(1);
        double y2 = Inbound.get_vy(1);

        double x3 = Inbound.get_vx(2);
        double y3 = Inbound.get_vy(2);        

        double x4 = Inbound.get_vx(3);
        double y4 = Inbound.get_vy(3);

        double length1 = distPointToPoint(x1, y1, x2, y2);//determine which side is the longest
        double length2 = distPointToPoint(x1, y1, x4, y4); //assumption that path through is along the longest leg (weakness)

        if(length1>length2){
          XYSegList Bisect;//determine midpoints of opposite ends to create a viewable seglist
          double x_mid1 = (x1 + x4)/2;
          double x_mid2 = (x2 + x3)/2;

          double y_mid1 = (y1 + y4)/2;
          double y_mid2 = (y2 + y3)/2;
          Bisect.add_vertex(x_mid1, y_mid1);
          Bisect.add_vertex(x_mid2, y_mid2);

          string bisectspec = Bisect.get_spec_pts();
          InB_midpoints.push_back(bisectspec);//creates a vector of seglists to make viewable
          //////////////// HEADING ///////////////////////
          double polybrg = relAng(x_mid1, y_mid1, x_mid2, y_mid2);
          string polyhdg = doubleToString(polybrg);
          InB_hdgs.push_back(polyhdg);           
        }
        else{
          XYSegList Bisect;
          double x_mid1 = (x1 + x2)/2;
          double x_mid2 = (x3 + x4)/2;

          double y_mid1 = (y1 + y2)/2;
          double y_mid2 = (y3 + y4)/2;
          Bisect.add_vertex(x_mid1, y_mid1);
          Bisect.add_vertex(x_mid2, y_mid2);

          string bisectspec = Bisect.get_spec_pts();
          InB_midpoints.push_back(bisectspec); 
          ////////////////////////////////////////
          // double polybrg = relAng(x_mid1, y_mid1, x_mid2, y_mid2);
          // string polyhdg = doubleToString(polybrg);
          // InB_hdgs.push_back(polyhdg);         
        }

      }

      for (int m=0; m<InB_hdgs.size(); m++){
        InB_HdgMsg = accumulate(begin(InB_hdgs), end(InB_hdgs), string(), [](string lhs, const string &rhs) { return lhs.empty() ? rhs : lhs + "," + rhs; });
        //InB_HdgMsg = uintToString(InB_midpoints.size());
        Notify("INBOUND_POLY_HDG", InB_HdgMsg);

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

                
      handled = true;
    }


    if(param == "outbound"){
      string OutB_poly = value;
      OutB_pts.push_back(OutB_poly);
      OutB_size = uintToString(OutB_pts.size());
      for (int pts=0; pts<OutB_pts.size(); pts++){
        OutB_PolyMsg = accumulate(begin(OutB_pts), end(OutB_pts), string(), [](string lhs, const string &rhs) { return lhs.empty() ? rhs : lhs + ";" + rhs; });
      Notify("OUTBOUND_POLY", OutB_PolyMsg);
      }

      for (int r=0; r<OutB_pts.size(); r++){
        XYPolygon Outbound = string2Poly(OutB_poly);
        double x1 = Outbound.get_vx(0);
        double y1 = Outbound.get_vy(0);

        double x2 = Outbound.get_vx(1);
        double y2 = Outbound.get_vy(1);

        double x3 = Outbound.get_vx(2);
        double y3 = Outbound.get_vy(2);        

        double x4 = Outbound.get_vx(3);
        double y4 = Outbound.get_vy(3);

        double length1 = distPointToPoint(x1, y1, x2, y2);
        double length2 = distPointToPoint(x1, y1, x4, y4);

        if(length1>length2){
          XYSegList Bisect;
          double x_mid1 = (x1 + x4)/2;
          double x_mid2 = (x2 + x3)/2;

          double y_mid1 = (y1 + y4)/2;
          double y_mid2 = (y2 + y3)/2;
          Bisect.add_vertex(x_mid1, y_mid1);
          Bisect.add_vertex(x_mid2, y_mid2);

          string bisectspec = Bisect.get_spec_pts();
          OutB_midpoints.push_back(bisectspec);
          /////////////////////////////////////////////
          // double polybrg = relAng(x_mid1, y_mid1, x_mid2, y_mid2);
          // string polyhdg = doubleToString(polybrg);
          // OutB_hdgs.push_back(polyhdg);            
        }
        else{
          XYSegList Bisect;
          double x_mid1 = (x1 + x2)/2;
          double x_mid2 = (x3 + x4)/2;

          double y_mid1 = (y1 + y2)/2;
          double y_mid2 = (y3 + y4)/2;
          Bisect.add_vertex(x_mid1, y_mid1);
          Bisect.add_vertex(x_mid2, y_mid2);

          string bisectspec = Bisect.get_spec_pts();
          OutB_midpoints.push_back(bisectspec); 
          //////////////////////////////////////////////
          // double polybrg = relAng(x_mid1, y_mid1, x_mid2, y_mid2);
          // string polyhdg = doubleToString(polybrg);
          // OutB_hdgs.push_back(polyhdg);                     
        }

      }
  

      // for (int m=0; m<OutB_hdgs.size(); m++){
      //   OutB_HdgMsg = accumulate(begin(OutB_hdgs), end(OutB_hdgs), string(), [](string lhs, const string &rhs) { return lhs.empty() ? rhs : lhs + "," + rhs; });
      //   Notify("OUTBOUND_POLY_HDG", OutB_HdgMsg);            
      // }

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
      // for (int i=0; i<m_polygons.size(); i++)
      // Notify ("VIEW_POLYGON", m_polygons[i]); 
       
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

void TransitLanes::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("PMV_CONNECT",0);
  // Register("NODE_MESSAGE_LOCAL", 0); //Register Node_Report to determine if vehicle nav x,y
  //                             // are contained within the polygon  
}


//------------------------------------------------------------
// Procedure: buildReport()

bool TransitLanes::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << " Traffic Separation Lanes                   " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(3);
  actab << "" << "Inbound Lane" << "";
  actab.addHeaderLines();
  actab << "Number of Polygons" << InB_size << "Heading";
  actab.addHeaderLines();
  int m_polys = max(InB_pts.size(),OutB_pts.size());
  for (int pts=0; pts<m_polys; pts++)
    actab << "Poly Points" << InB_pts[pts] << "";
  actab.addHeaderLines();
  actab.addHeaderLines();
  actab << "" << "Outbound Lane" << "";
    actab.addHeaderLines();
  actab << "Number of Polygons" << OutB_size << "Heading";
  actab.addHeaderLines();
   for (int pts=0; pts<m_polys; pts++)
    actab << "Poly Points" << OutB_pts[pts] << "";
  
  m_msgs << actab.getFormattedString();
  return(true);
}




