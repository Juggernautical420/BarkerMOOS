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
    string str_os_seglist = msg.GetString();
    m_os_seglist = string2SegList(str_os_seglist);
    m_os_seglist.set_label(m_vname);  
  }

  if(key == "SEGLIST"){
    string str_contact_seglist  = msg.GetString();
    string contact_name = biteStringX(str_contact_seglist, ';');
    string contact_nameparse = biteStringX(contact_name, '=');
    if(contact_name!=m_vname){
      XYSegList contact_seglist;
      contact_seglist = string2SegList(str_contact_seglist);
      contact_seglist.set_label(contact_name);
      m_os_intercept.getIntercept(m_os_seglist, contact_seglist);
    }
      
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

int l;//The following plots a visual point for each intersection
for(l=0; l<m_os_intercept.size(); l++){
 m_point.set_vertex(m_os_intercept.get_px(l), m_os_intercept.get_py(l)); 
 m_point.set_color("vertex", "red");
 m_point.set_param("vertex_size", "8");  
 string point_spec = m_point.get_spec();
 Notify("VIEW_POINT", point_spec);
 XYSegList remaining = biteSegList(m_os_seglist, m_os_intercept.get_px(l), m_os_intercept.get_py(l));
 m_remaining_seglist.push_back(remaining);
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

}


//------------------------------------------------------------
// Procedure: buildReport()

bool SegListIntercept::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "Predicted Intercept Point(s)                " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  actab << "Name | X POS | Y POS | Remaining Seglist" ;
  actab.addHeaderLines();
  for(unsigned int k=0; k<m_os_intercept.size(); k++){
    actab << m_os_intercept.get_pname(k) << doubleToString(m_os_intercept.get_px(k)) << doubleToString(m_os_intercept.get_py(k)) << m_remaining_seglist[k].get_spec();
  }
  m_msgs << actab.getFormattedString();


  return(true);
}



