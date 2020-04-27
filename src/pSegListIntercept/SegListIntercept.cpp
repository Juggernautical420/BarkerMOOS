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
  m_got_speed = false;
  m_extra_ready = false;
  m_extra_done = false;
  m_got_calc = false;
  m_got_predict = false;
  m_got_limiting = false;

  m_final_speed = m_input_speed;
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
        if(!m_got_speed){
        m_nav_spd =  dval;
        m_got_speed = true; 
        }
      }
  }

  if(key == "NODE_REPORT"){
    string sval  = msg.GetString(); 
    handleNodeMsg(sval); 
  }

  if(key == "COLLISION_DETECT_PARAMS"){
    string sval  = msg.GetString();
    processParameters(sval);
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

//calcTime(m_nav_spd);
populateContacts();
// predictContacts();
// findLimitContacts();
// recommendSpeed();
manageContacts();

 
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

    if(param == "buffer") {
      m_buffer = stod(value);
      handled = true;
    }

    if(param == "input_speed") {
      m_desired_speed = stod(value);
      m_input_speed = value;
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
  Register("COLLISION_DETECT_PARAMS", 0);

}


//------------------------------------------------------------
// Procedure: buildReport()

bool SegListIntercept::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "Predicted Intercept Point(s)                " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(3);
  actab << "Name | X POS | Y POS " ;
  actab.addHeaderLines();
  for(int k=0; k<m_os_intercept.size(); k++){
    actab << m_os_intercept.get_pname(k) << doubleToString(m_os_intercept.get_px(k)) << doubleToString(m_os_intercept.get_py(k));
  }

  m_msgs << actab.getFormattedString();

  ACTable actabl(3);
  actabl << "" << "" << "";
  actabl << "" << "" << "" ;
  actabl << "Node Report Size" << intToString(m_con_nodenames.size()) << "";
  actabl << "SegList Size" << intToString(m_con_seglists.size()) << "";
  actabl << "TSS Size" << intToString(m_tss_contacts.size()) << "";
  actabl << "TSS Contacts" << "Speed" << "SegList" ;
  actabl.addHeaderLines();
  for(int i=0; i<m_tss_contacts.size(); i++){
    SegListContact current = m_tss_contacts.get_contact(i);
    actabl << current.getContactName() << doubleToString(current.getContactSpd()) << current.getContactSegList();
  }
  actabl.addHeaderLines();
  actabl << "" << "" << "";
  actabl << "" << "" << "" ; 
  actabl << "Collision Range" << doubleToString(m_range_concern) << "";
  actabl << "Intercept Points" << intToString(m_os_intercept.size()) << "" ;
  actabl << "Extrapolated Points" << intToString(m_extrapolated_contacts.size()) << "";
  actabl << "Initial Limiting Contacts" << intToString(m_init_limit_contacts.size()) << "";
  actabl << "Final Limiting Contacts" << m_limiting_contacts << "";
  actabl << "Input Speed" << m_input_speed << "";
  actabl << "Final Speed" << m_final_speed << "";
  m_msgs << actabl.getFormattedString();


  return(true);
}

//------------------------------------------------------------
// Procedure: calcTime

// void SegListIntercept::calcTime(double speed)
// {
//   if((!m_got_calc)&&(m_got_speed)){
//     for(int l=0; l<m_os_intercept.size(); l++){
//       XYSegList remaining = biteSegList(m_os_seglist, m_os_intercept.get_px(l), m_os_intercept.get_py(l));
//       double length = remaining.length();
//       m_length.push_back(length);
//       double time = length/speed;
//       m_time.push_back(time);
//     }
  
//   if(m_time.size() != 0)
//     m_got_calc = true;
//   }
// }

//------------------------------------------------------------
// Procedure: handleNodeMsg

void SegListIntercept::handleNodeMsg(string report)
{
  vector<string> str_vector = parseString(report, ',');
    for(unsigned int i=0; i<str_vector.size(); i++) {
      string param = biteStringX(str_vector[i], '=');
      string value = str_vector[i];
      bool valid_speed = false;

      if(param == "NAME")
        m_current_name = value;

      if(param == "SPD"){
        double current_speed = stod(value);
        if(current_speed != 0){
          m_current_spd = current_speed;
          valid_speed = true;
        }
      }

      if(isUnique(m_current_name)&&(valid_speed)){
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
// Procedure: processParameters

void SegListIntercept::processParameters(string s)
{
  string collrng = tokStringParse(s, "collision_range", ',', '=');
  m_coll_range = stod(collrng);
  string nmrng = tokStringParse(s, "near_miss_range", ',', '=');
  m_nm_range = stod(nmrng);
  m_range_concern = m_nm_range * m_buffer;  //Built in buffer that accounts for the fact
  // that speed is set early and prior to full speed for the class instance

}



//------------------------------------------------------------
// Procedure: populateContacts()

void SegListIntercept::populateContacts()
{
  if(!m_extra_ready){
  for(int i=0; i<m_con_segnames.size(); i++){
    for(int j=0; j<m_con_nodenames.size(); j++){
      if(m_con_segnames[i] == m_con_nodenames[j]){
        SegListContact current_contact;
        current_contact.set_contact(m_con_nodenames[i], m_con_seglists[i], m_con_nodespds[j]);
        m_tss_contacts.addSegListContact(current_contact);
      }
    }
  }

  if((m_tss_contacts.size() == m_con_segnames.size())&&(m_tss_contacts.size() != 0))
    m_extra_ready = true;
  }
}


//------------------------------------------------------------
// Procedure: predictContacts()

// void SegListIntercept::predictContacts()
// {

//   if((!m_got_predict)&&(m_extra_ready)&&(m_got_calc)){
//     for(int i=0; i<m_time.size(); i++){
//         XYPoint ownship;
//         ownship.set_vertex(m_os_intercept.get_px(i), m_os_intercept.get_py(i));
//       for(int j=0; j<m_tss_contacts.size(); j++){
//         SegListContact curr_contact = m_tss_contacts.get_contact(j);
//         XYPoint guess_point = curr_contact.extrapolate_point(m_time[i]);
//         string guess_info = guess_point.get_spec();
//         m_extrapo_contacts.push_back(guess_info);
//         double guess_dist = distPointToPoint(ownship, guess_point);
//         m_extrapo_dists.push_back(guess_dist);
//       }
//     }

//       if(m_extrapo_contacts.size() !=0)
//       m_got_predict = true;
//   }
// }  

//------------------------------------------------------------
// Procedure: manageContacts

void SegListIntercept::manageContacts()
{
  vector<double> m_length;
  vector<double> m_time; 

  if((!m_got_calc)&&(m_got_speed)){
    for(int l=0; l<m_os_intercept.size(); l++){
      XYSegList remaining = biteSegList(m_os_seglist, m_os_intercept.get_px(l), m_os_intercept.get_py(l));
      double length = remaining.length();
      m_length.push_back(length);
      double time = length/m_nav_spd;
      m_time.push_back(time);
    }
  
  if(m_time.size() != 0)
    m_got_calc = true;
  }


  vector<string> m_extrapo_contacts;
  vector<double> m_extrapo_dists;
  
  if((!m_got_predict)&&(m_extra_ready)&&(m_got_calc)){
    for(int i=0; i<m_time.size(); i++){
        XYPoint ownship;
        ownship.set_vertex(m_os_intercept.get_px(i), m_os_intercept.get_py(i));
      for(int j=0; j<m_tss_contacts.size(); j++){
        SegListContact curr_contact = m_tss_contacts.get_contact(j);
        XYPoint guess_point = curr_contact.extrapolate_point(m_time[i]);
        string guess_info = guess_point.get_spec();
        m_extrapo_contacts.push_back(guess_info);
        double guess_dist = distPointToPoint(ownship, guess_point);
        m_extrapo_dists.push_back(guess_dist);
        m_extrapolated_contacts.push_back(guess_info);
      }
    }

      if(m_extrapo_contacts.size() !=0){
      m_got_predict = true;
    }
  }
 
  
  if(m_got_predict){
  vector<string> m_limit_contacts;
  vector<double> m_limit_dist;

  for(int i=0; i<m_extrapo_dists.size(); i++){
    if(m_extrapo_dists[i] <= m_range_concern){
      m_limit_dist.push_back(m_extrapo_dists[i]);
      m_limit_contacts.push_back(m_extrapo_contacts[i]);
      m_init_limit_contacts.push_back(m_extrapo_contacts[i]);
      }
  }

  m_limiting_contacts = intToString(m_limit_contacts.size());
  if(m_limit_contacts.size() != 0){
      double new_speed = m_desired_speed - 0.1;
      if(new_speed > 0.1){
      m_speed_update = "speed =" + doubleToString(new_speed);
      Notify("WPT_UPDATE", m_speed_update);
      m_final_speed = doubleToString(new_speed); 
      m_got_speed = false;
      m_got_calc = false;
      m_got_predict = false;
    }
   } 
  }



}

// //------------------------------------------------------------
// // Procedure: recommendSpeed

// void SegListIntercept::recommendSpeed()
// {
//   if(m_got_limiting){
//     if(m_limit_contacts.size() !=0){
//       double new_speed = m_nav_spd - 0.05;
//       if(new_speed > 0.5){
//       string speed_update = "speed =" + doubleToString(new_speed);
//       m_final_speed = doubleToString(new_speed);
//       Notify("WPT_UPDATE", speed_update);
//       m_got_speed = false;
//       m_got_calc = false;
//       m_got_predict = false;
//       m_got_limiting = false;
//       }
//     }
//   }
// }
