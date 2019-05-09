/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TempGenPath.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "TempGenPath.h"

using namespace std;

//---------------------------------------------------------
// Constructor

TempGenPath::TempGenPath()
{
  cycle_done = false;
  initial_temp = false;

  left_points = false;
  right_points = false;
}

//---------------------------------------------------------
// Destructor

TempGenPath::~TempGenPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool TempGenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

  if(key == "UCTD_MSMNT_REPORT"){
    m_temp_report = msg.GetString();
    TempPoints(m_temp_report);
  } 
  else if(key == "WPT_INDEX"){
    m_next_wpt = msg.GetDouble();
  }
  else if(key == "CYCLE_COMPLETE"){
    m_cycle_done = msg.GetString();
    if(m_cycle_done == "true"){
      cycle_done = true;
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

bool TempGenPath::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool TempGenPath::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
//Grab initial temp on each leg
m_current_temp = m_string_temp;
Notify("CURRENT_TEMP", m_current_temp);

if(m_next_wpt == 0){
if(!initial_temp){
m_initial_temp = m_current_temp;
Notify("INITIAL_TEMP", m_initial_temp);
initial_temp = true;  
}

   
if(m_current_temp > m_initial_temp){
    m_t_hot = max(m_initial_temp, m_current_temp);
    m_current_hot = max(m_t_hot, m_current_hot);
  }

Notify("NORTH_TEMP", m_initial_temp);
Notify("SOUTH_TEMP", m_current_hot);
m_tave = (m_current_hot + m_initial_temp)/2;
m_temp_delta = m_current_hot - m_initial_temp;
Notify("N_S_DELTA", m_temp_delta);
}

// m_delta_h = (0.75 * m_temp_delta) + m_initial_temp;
// m_delta_l = (0.25 * m_temp_delta) + m_initial_temp;

// //Now grab Avg Temp Points on left
// if((m_next_wpt==0) && (cycle_done)){
//   if((m_current_temp >= m_delta_l) && (m_current_temp <= m_delta_h)){
//   m_waypoints.add_vertex(m_string_x, m_string_y);
//   left_points = true;  
//   }
// }

// //Grab Avg Temp Points on right
// if((m_next_wpt==2) && (cycle_done)){
//   if((m_current_temp >= m_delta_l) && (m_current_temp <= m_delta_h)){
//   m_waypoints.add_vertex(m_string_x, m_string_y);
//   right_points = true;  
//   }
// }

//if((left_points) && (right_points)){
if(m_next_wpt == 4){
Notify("GENPATH", "true");  
  string update_str = "points = -30,-120: 130,-40";
  //update_str += m_waypoints.get_spec();
  //update_str +=" # repeat = 2";
  //update_str +=" # visual_hints = edge_color = yellow, vertex_color = yellow ";
Notify("SURVEY_UPDATES", update_str);
}





  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool TempGenPath::OnStartUp()
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

void TempGenPath::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("WPT_INDEX",0);
  Register("UCTD_MSMNT_REPORT",0);
  Register("CYCLE_COMPLETE",0);
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool TempGenPath::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File: TempGenPath.cpp                        \n";
  m_msgs << "Author: Jason Barker & Team Awesome          \n";
  m_msgs << "============================================ \n";
  
  m_msgs << "North Temperature: " << doubleToString(m_current_hot) << endl;
  m_msgs << "South Temperature: " << doubleToString(m_initial_temp) << endl;
  m_msgs << "Delta Temperature: " << doubleToString(m_temp_delta) << endl;

  // ACTable actab(4);
  // actab << "Alpha | Bravo | Charlie | Delta";
  // actab.addHeaderLines();
  // actab << "one" << "two" << "three" << "four";
  // m_msgs << actab.getFormattedString();


  return(true);
}
//-----------------------------------------------------------
// Procedure: TempPoints(string report)

void TempGenPath::TempPoints(string sval){
  vector<string> myvector = parseString(sval, ',');
  for(unsigned int i=0; i<myvector.size(); i++){
    string param = biteStringX(myvector[i], '=');
    string value = myvector[i];
    if(tolower(param) == "vname"){
      m_string_vname = value;
    }
    else if(tolower(param) == "utc"){
      m_string_utc = stod(value);
    }
    else if(tolower(param) == "x"){ 
      m_string_x = stod(value);
    }
    else if(tolower(param) == "y"){ 
      m_string_y = stod(value);
    }
    else if(tolower(param) == "temp"){ 
      m_string_temp = stod(value);
    }  
  }
  return;
}



