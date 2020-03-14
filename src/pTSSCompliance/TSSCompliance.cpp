/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TSSCompliance.cpp                                        */
/*    DATE: March 12, 2020                                                */
/*    PURPOSE: This is a vehicle application that registers for shoreside */
/*              broadcast of traffic separation lanes and determines ownship  */
/*              compliance with broadcasted traffic lanes IAW COLREGs Rule 10 */
/************************************************************/

#include <iterator>
#include <math.h>
#include <cstdlib>
#include "MBUtils.h"
#include "ACTable.h"
#include "TSSCompliance.h"
#include "XYFormatUtilsPoly.h"

using namespace std;

//---------------------------------------------------------
// Constructor

TSSCompliance::TSSCompliance()
{
  m_inbound_lane = false;
  m_outbound_lane = false;
}

//---------------------------------------------------------
// Destructor

TSSCompliance::~TSSCompliance()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool TSSCompliance::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

    if(key == "INBOUND_POLY"){
      string in_sval  = msg.GetString();
      m_InB_polygons = parseString(in_sval, ';');
    }

    if(key == "OUTBOUND_POLY"){
      string out_sval  = msg.GetString();
      m_OutB_polygons = parseString(out_sval, ';');    
    }

    if (key == "NAV_X")
      m_nav_x = msg.GetDouble();
    if (key == "NAV_Y")
      m_nav_y = msg.GetDouble();
    if (key == "NAV_HEADING")
      m_nav_hdg = msg.GetDouble();


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

bool TSSCompliance::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool TSSCompliance::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!


  for(int i=0; i<m_InB_polygons.size(); i++){
    XYPolygon inbound = string2Poly(m_InB_polygons[i]);
    if(inbound.contains(m_nav_x,m_nav_y)){
      m_inbound_lane = true;
      Notify("COMPLIANCE", "inbound");
    }
  }

  for(int i=0; i<m_OutB_polygons.size(); i++){
    XYPolygon outbound = string2Poly(m_OutB_polygons[i]);
    if(outbound.contains(m_nav_x,m_nav_y)){
      m_outbound_lane = true;
      Notify("COMPLIANCE", "outbound");
    }  
  }



  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool TSSCompliance::OnStartUp()
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
  

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void TSSCompliance::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("INBOUND_POLY", 0);
  Register("OUTBOUND_POLY", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("NAV_HEADING", 0);  
}


//------------------------------------------------------------
// Procedure: buildReport()

bool TSSCompliance::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "File:                                       " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}




