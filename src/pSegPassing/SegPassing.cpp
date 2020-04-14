/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: SegPassing.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "SegPassing.h"
#include "SegListEdit.h"

using namespace std;

//---------------------------------------------------------
// Constructor

SegPassing::SegPassing()
{
  m_got_x = false;
  m_got_y = false;
}

//---------------------------------------------------------
// Destructor

SegPassing::~SegPassing()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool SegPassing::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

  if(key == "NAV_X"){
    if(!m_got_x){
    m_nav_x = msg.GetDouble();
    m_got_x = true;
    }
  }

  if(key == "NAV_Y"){
    if(!m_got_y){
    m_nav_y = msg.GetDouble();
    m_got_y = true;
    }
  }    

  if(key == m_list_name){
    string str_seglist = msg.GetString();
    XYSegList org_seglist = string2SegList(str_seglist);
    m_old_seglist = org_seglist.get_spec();
    m_seglist = add_origin(org_seglist, m_nav_x, m_nav_y);
    m_new_seglist = m_seglist.get_spec(); 
  } 

  m_source = "src_node=" + m_vname;
  m_dest = "dest_node=all";
  m_var_name = "var_name=SEGLIST";
  m_str_val = "string_val=vname=" + m_vname + "," + m_new_seglist;
  m_node_message = m_source + "," + m_dest + "," + m_var_name + "," + m_str_val;
  Notify("NODE_MESSAGE_LOCAL", m_node_message); 

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

bool SegPassing::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool SegPassing::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool SegPassing::OnStartUp()
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

void SegPassing::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register(m_list_name,0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool SegPassing::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << " SegList                                      " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(2);
  actab.addHeaderLines();
  actab << "Vehicle Name" << m_veh_name;
  actab << "Original Seglist" << m_old_seglist;
  actab.addHeaderLines();
  actab << "X Start Pos" << doubleToString(m_nav_x);
  actab << "Y Start Pos" << doubleToString(m_nav_y);
  actab << "Updated Seglist" << m_new_seglist;
  actab << "" << "";
  actab << "" << "";
  m_msgs << actab.getFormattedString();

  m_msgs << "" << endl;
  m_msgs << " Node Message Passed                               " << endl;
  m_msgs << "============================================" << endl;

  ACTable actabl(2);
  actabl << "Source" << m_source;
  actabl << "Destination" << m_dest;
  actabl << "Varible Name" << m_var_name;
  actabl << "Value" << m_str_val;
  m_msgs << actabl.getFormattedString();

  // m_msgs << "============================================" << endl;
  // m_msgs << "Message Passed: " << endl;
  // m_msgs << m_source << endl;
  // m_msgs << m_dest << endl;
  // m_msgs << m_var_name << endl;
  // m_msgs << m_str_val << endl;

  return(true);
}




