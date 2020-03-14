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

using namespace std;

//---------------------------------------------------------
// Constructor

SegPassing::SegPassing()
{
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

  if(key == m_list_name){
    str_seglist = msg.GetString();
    m_str_name = biteStringX(str_seglist, '=');
    seglist_report = str_seglist;
    
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

  // NodeMessage seglist_message;
  // seglist_message.setSourceNode(m_vname);
  // seglist_message.setDestNode("all");
  // seglist_message.setVarName(m_list_name);
  // seglist_message.setStringVal(seglist_report);
  // m_node_message = seglist_message.getSpec();

  m_source = "src_node=" + m_vname;
  m_dest = "dest_node=all";
  m_var_name = "var_name=SEGLIST";
  //m_str_val = "string_val=vname=" + m_vname + "," + str_seglist;
  m_str_val = "string_val=vname=" + m_vname + ";pts=" + str_seglist;
  m_node_message = m_source + "," + m_dest + "," + m_var_name + "," + m_str_val;
  Notify("NODE_MESSAGE_LOCAL", m_node_message);
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
}


//------------------------------------------------------------
// Procedure: buildReport()

bool SegPassing::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "Ownship SegList                                      " << endl;
  m_msgs << "============================================" << endl;

  // ACTable actab(4);
  // actab << "Alpha | Bravo | Charlie | Delta";
  // actab.addHeaderLines();
  // actab << "one" << "two" << "three" << "four";
  // m_msgs << actab.getFormattedString();

  m_msgs << m_veh_name << endl;
  m_msgs << seglist_report << endl;
  m_msgs << "============================================" << endl;
  m_msgs << "Message Passed: " << endl;
  m_msgs << m_source << endl;
  m_msgs << m_dest << endl;
  m_msgs << m_var_name << endl;
  m_msgs << m_str_val << endl;

  return(true);
}




