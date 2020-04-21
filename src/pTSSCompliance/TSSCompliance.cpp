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
#include "XYFormatUtilsSegl.h"

using namespace std;

//---------------------------------------------------------
// Constructor

TSSCompliance::TSSCompliance()
{
  m_got_sepzs = false;
  m_got_lines = false;
  m_got_scheme = false;

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

    if(key == "TSS_LANES"){
      if(!m_got_lines){
      string sval  = msg.GetString();
      m_lane_seglists = parseString(sval, ';');
      m_got_lines = true;
    }
  }

    if(key == "TSS_SEP_ZONES"){
      if(!m_got_sepzs){
      string sval  = msg.GetString();
      m_sep_zones = parseString(sval, ';'); 
      m_got_sepzs = true;   
    }
  }

  if(!m_got_scheme){
    if((m_got_lines)&&(m_got_sepzs)){
      m_tss.ForceConcatenate(m_sep_zones, m_lane_seglists);
      createTSSPolys();
      m_got_scheme = true;
    }
  }

    if (key == "NAV_X"){
      m_nav_x = msg.GetDouble();
    }

    if (key == "NAV_Y"){
      m_nav_y = msg.GetDouble();
    }

    if (key == "NAV_HEADING"){
      m_nav_hdg = msg.GetDouble(); 
    }

  for(int i=0; i<m_tss_gen_lanes.size(); i++){
  if((m_tss_gen_lanes[i].contains(m_nav_x,m_nav_y))&&(m_nav_hdg >= ((m_tss_lane_headings[i])-10))&&(m_nav_hdg <= ((m_tss_lane_headings[i])+10)))
    Notify("TSS_STATUS", "Compliant");
  }


  if(key == m_list_name){
    string str_os_seglist = msg.GetString();
    m_os_seglist = string2SegList(str_os_seglist);
    m_os_seglist.set_label(m_vname);  
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



// for(int i=0; i<m_tss_gen_lanes.size(); i++){
//   if(!m_tss_gen_lanes[i].contains(m_nav_x,m_nav_y))
//   m_in_the_lane = false;
//   m_out_of_lane = true;
// } 

// if(m_in_the_lane)
//     Notify("TSS_STATUS", "Compliant");
  
// if(m_out_of_lane)
//     Notify("TSS_STATUS", "Non-Participant"); 


// for(int i=1; i<m_os_seglist.size(); i++){
//   double x1 = m_os_seglist.get_vx(i-1);
//   double y1 = m_os_seglist.get_vy(i-1);
//   double x2 = m_os_seglist.get_vx(i);
//   double y2 = m_os_seglist.get_vy(i);
//   for(int j=0; j<m_tss_sepz.size(); j++){
//     if(!m_in_the_lane){
//       if(m_tss_sepz[j].seg_intercepts(x1,y1,x2,y2))
//       Notify("TSS_STATUS", "Compliant");
//     }
//   }
// }


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

    if(param == "vname"){
      m_veh_name = toupper(value);
      m_vname = tolower(value);
      m_list_name = "SEGLIST_" + m_veh_name;
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
  Register("TSS_LANES", 0);
  Register("TSS_SEP_ZONES", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("NAV_HEADING", 0);
  Register(m_list_name, 0); //ownship seglist  
}


//------------------------------------------------------------
// Procedure: buildReport()

bool TSSCompliance::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "TSSCompliance Traffic Scheme Populator       " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(2);
  actab << "My TSS" << "";
  actab.addHeaderLines();
  for(int i=0;i<m_sep_zones.size(); i++){
    actab << "Separation Zone" << m_sep_zones[i];
  }
  actab.addHeaderLines();
  for(int i=0;i<m_tss_inbound.size(); i++){
    actab << "Inbound Lane" << m_tss_inbound[i];
  }
  actab.addHeaderLines();
  for(int i=0;i<m_tss_outbound.size(); i++){
    actab << "Outbound Lane" << m_tss_outbound[i];
  }
  actab.addHeaderLines();
  actab << "Generated Headings" << "Generated Lanes";
  for(int i=0; i<m_tss_genpolys.size(); i++){
    actab << m_tss_gen_hdgs[i] << m_tss_genpolys[i];
  }

  m_msgs << actab.getFormattedString();


  return(true);
}


//------------------------------------------------------------
// Procedure: createTSSPolys()

void TSSCompliance::createTSSPolys()
{
  for(int i=0; i<m_sep_zones.size(); i++){
    XYPolygon poly = string2Poly(m_sep_zones[i]);
    m_tss_sepz.push_back(poly);
  }

  m_tss_inbound = m_tss.getInboundPolys();
  for(int i=0; i<m_tss_inbound.size(); i++){
    m_inbound_lanes.push_back(string2Poly(m_tss_inbound[i]));
  }
  
  m_tss_outbound = m_tss.getOutboundPolys();
  for(int i=0; i<m_tss_outbound.size(); i++){
    m_outbound_lanes.push_back(string2Poly(m_tss_outbound[i]));
  }

  m_tss_genpolys = m_tss.getGeneratedPolyPts();
  for(int i=0; i<m_tss_genpolys.size(); i++){
   m_tss_gen_lanes.push_back(string2Poly(m_tss_genpolys[i])); 
  }

  m_tss_gen_hdgs = m_tss.getGeneratedPolyHdgs();
  for(int i=0; i<m_tss_gen_hdgs.size(); i++){
    m_tss_lane_headings.push_back(stod(m_tss_gen_hdgs[i]));
  }

}


