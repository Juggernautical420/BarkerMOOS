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
  m_extra_ready = false;
  m_extra_done = false;
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
    m_con_segnames.push_back(contact_name);
    if(contact_name!=m_vname){
      XYSegList contact_seglist;
      contact_seglist = string2SegList(str_contact_seglist);
      m_con_seglists.push_back(contact_seglist);
      contact_seglist.set_label(contact_name);
      m_os_intercept.getIntercept(m_os_seglist, contact_seglist);
    }
      
  }

  if(key == "NAV_SPEED"){
    double dval = msg.GetDouble();
      if(dval != 0){
        m_nav_spd =  dval;
      }
  }

  if(key == "NODE_REPORT"){
    string sval  = msg.GetString(); 
    handleNodeMsg(sval); 
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

//The following plots a visual point for each intersection
for(int l=0; l<m_os_intercept.size(); l++){
 m_point.set_vertex(m_os_intercept.get_px(l), m_os_intercept.get_py(l)); 
 m_point.set_color("vertex", "red");
 m_point.set_param("vertex_size", "8");  
 string point_spec = m_point.get_spec();
 Notify("VIEW_POINT", point_spec);
}

calcTime(m_nav_spd);
populateContacts();


 
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
  Register("NAV_SPEED",0);
  Register("NODE_REPORT", 0); //to get contact speeds

}


//------------------------------------------------------------
// Procedure: buildReport()

bool SegListIntercept::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "Predicted Intercept Point(s)                " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(5);
  actab << "Name | X POS | Y POS | Int Length | Int Time" ;
  actab.addHeaderLines();
  for(unsigned int k=0; k<m_os_intercept.size(); k++){
    actab << m_os_intercept.get_pname(k) << doubleToString(m_os_intercept.get_px(k)) << doubleToString(m_os_intercept.get_py(k)) << doubleToString(m_length[k]) << doubleToString(m_time[k]);
  }
  m_msgs << actab.getFormattedString();

  ACTable actabl(2);
  actabl << "" << "";
  actabl << "" << "";
  actabl << "Time" << "Contact Location" ;
  actab.addHeaderLines();
  for(int i=0; i<m_os_intercept.size(); i++){
    vector <XYPoint> extra_predicts = m_tss_contacts.extrapolate_all(m_time[i]);
      actabl << m_time[i] << "" ;
    for(int j=0; j<extra_predicts.size(); j++){
      actabl << "" << extra_predicts[j].get_spec();
    }
  }
  m_msgs << actabl.getFormattedString();


  return(true);
}

//------------------------------------------------------------
// Procedure: calcTime

void SegListIntercept::calcTime(double speed)
{
for(int l=0; l<m_os_intercept.size(); l++){
 XYSegList remaining = biteSegList(m_os_seglist, m_os_intercept.get_px(l), m_os_intercept.get_py(l));
 double length = remaining.length();
 m_length.push_back(length);
 double time = length/speed;
 m_time.push_back(time);
}
}

//------------------------------------------------------------
// Procedure: handleNodeMsg

void SegListIntercept::handleNodeMsg(string report)
{
  vector<string> str_vector = parseString(report, ',');
    for(unsigned int i=0; i<str_vector.size(); i++) {
      string param = biteStringX(str_vector[i], '=');
      string value = str_vector[i];

      if(param == "NAME")
        m_current_name = value;

      if(param == "SPD")
        m_current_spd = stod(value);

      if(isUnique(m_current_name)){
        m_con_nodenames.push_back(m_current_name);
        m_con_nodespds.push_back(m_current_spd);
      }

    }
     
}

//------------------------------------------------------------
// Procedure: isUnique

bool SegListIntercept::isUnique(string name)
{
  if(name == m_vname)
    return(false);

  for(int i=0; i<m_con_nodenames.size(); i++){
    if(name == m_con_nodenames[i])
      return(false);
  }

  return(true);
}

//------------------------------------------------------------
// Procedure: populateContacts()

void SegListIntercept::populateContacts()
{
  for(int i=0; i<m_con_segnames.size(); i++){
    for(int j=0; j<m_con_nodenames.size(); j++){
      if(m_con_segnames[i] == m_con_nodenames[j]){
        SegListContact current_contact;
        current_contact.set_contact(m_con_segnames[i], m_con_seglists[i], m_con_nodespds[j]);
        m_tss_contacts.addSegListContact(current_contact);
      }
    }
  }
  m_extra_ready = true;
}


