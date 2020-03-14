/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: SegListIntercept.cpp                                        */
/*    DATE: October 30, 2019                                                */
/*    This file reads the node message SEGLIST_$V and compares seglists */
/*    for intercept point using the fast seglist intercept method */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "SegListIntercept.h"
#include "SegListEdit.h"


using namespace std;

//---------------------------------------------------------
// Constructor

SegListIntercept::SegListIntercept()
{

}

//---------------------------------------------------------
// Destructor

SegListIntercept::~SegListIntercept()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool SegListIntercept::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();
 
  if(key == m_list_name){
    str_os_seglist = msg.GetString();
    os_seglist = string2SegList(str_os_seglist);
    os_seglist.set_label(m_vname);
    m_os_intercept.getIntercept(os_seglist, contact_seglist);      
  }

  if(key == "SEGLIST"){
    str_contact_seglist  = msg.GetString();
    m_contact_name = biteStringX(str_contact_seglist, ';');
    string m_contact_nameparse = biteStringX(m_contact_name, '=');
    if(m_contact_name!=m_vname){
      contact_seglist = string2SegList(str_contact_seglist);
      contact_seglist.set_label(m_contact_name);
      m_os_intercept.getIntercept(os_seglist, contact_seglist); 
    } 
  }



  
/// Trials and Errors //////
  // if(key == "NAV_SPEED"){
  //   m_nav_spd = msg.GetDouble();

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

     // else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
     //   reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool SegListIntercept::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool SegListIntercept::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
//////////
 // m_os_details.getParts(os_seglist, m_nav_spd);
 // os_extra_pts.seglistExtrapolate(os_seglist, m_os_details, m_nav_spd);
//////////

// THIS PART WORKS ///////// 



int l;//The following plots a visual point for each intersection
for(l=0; l<m_os_intercept.size(); l++){
 point.set_vertex(m_os_intercept.get_px(l), m_os_intercept.get_py(l)); 
 point.set_color("vertex", "red");
 point.set_param("vertex_size", "5");  
 string point_spec = point.get_spec();
 Notify("VIEW_POINT", point_spec);





//////////////////////////////////////////////////
//// Trials and Errors //////
// ppx = m_charlie_dana.get_px(l);
// ppy = m_charlie_dana.get_py(l);
// // string pcontact = m_charlie_dana.get_pname(l);

//  os_remaining_seglist = biteSegListRight(os_seglist, ppx, ppy);
//  Notify("BITE_SEGLIST", os_remaining_seglist);


/////// Need to Fix get_t ///////

// int int_time = os_extra_pts.get_t(ppx, ppy);
// string time_test = intToString(int_time);

// double m_int_x = os_extra_pts.get_xt(int_time);
// string m_ix = doubleToString(m_int_x);
// double m_int_plusone = os_extra_pts.get_xt(int_time);
// string m_ix1 = doubleToString(m_int_plusone);
// Notify("TEST1", m_ix1);
// double m_int_y = os_extra_pts.get_yt(int_time);
// string m_iy = doubleToString(m_int_y);
// string time_coords = m_ix + "," + m_iy;

// double dist = m_os_details.get_legLength(0);
// double dist_limit = dist/m_nav_spd;
// int limit_t = int(dist_limit);
// string time_test = doubleToString(limit_t);

 // Notify("TEST", m_ix);
 // Notify("TESTING", time_test);
 
}


 
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool SegListIntercept::OnStartUp()
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

void SegListIntercept::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("SEGLIST", 0); //each contact seglist as a node msg betweeen vessels
 

  Register(m_list_name, 0); //ownship seglist

  /// Trials and Errors //////
  // Register("NAV_SPEED", 0);


}


//------------------------------------------------------------
// Procedure: buildReport()

bool SegListIntercept::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "Predicted Intercept Point(s)                " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(3);
  actab << "Name | X POS | Y POS";
  actab.addHeaderLines();
  for(unsigned int k=0; k<m_os_intercept.size(); k++){
    actab << m_os_intercept.get_pname(k) << doubleToString(m_os_intercept.get_px(k)) << doubleToString(m_os_intercept.get_py(k));
  }
  m_msgs << actab.getFormattedString();


  return(true);
}



