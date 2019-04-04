/************************************************************/
/*    NAME: Jason Barker                                    */
/*    ORGN: MIT                                             */
/*    FILE: GenPath.cpp                                     */
/*    DATE: 19 Mar 2019                                     */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "GenPath.h"

using namespace std;

//---------------------------------------------------------
// Constructor

GenPath::GenPath()
{
 
}

//---------------------------------------------------------
// Destructor

GenPath::~GenPath()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool GenPath::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

      if(key == "VISIT_POINT") {
        m_visit_point = msg.GetString();
        if(m_visit_point != "firstpoint" || m_visit_point != "lastpoint"){
          VectorPoints(m_visit_point);
          m_waypoints.add_vertex(m_dbl_x, m_dbl_y);
        }
    
    XYSegList sorted_waypoints;
    XYSegList working_waypoints = m_waypoints;
    int closest_index = working_waypoints.closest_vertex(m_origin_x, m_origin_y); 
    double next_x = working_waypoints.get_vx(closest_index);
    double next_y = working_waypoints.get_vy(closest_index);
    working_waypoints.delete_vertex(closest_index);
    sorted_waypoints.add_vertex(next_x, next_y); 
    for(int i=1; i<m_waypoints.size(); i++){
      int closest_index = working_waypoints.closest_vertex(next_x, next_y);
       next_x = working_waypoints.get_vx(closest_index);
      next_y = working_waypoints.get_vy(closest_index);
      sorted_waypoints.add_vertex(next_x, next_y);
      working_waypoints.delete_vertex(closest_index);
    }
    string color;
    // if(next_x < 88){
    // color = "orange";
    // }
    if(next_x > 88){
    color = "orange";
    }

    string update_str = "points = ";
    update_str += sorted_waypoints.get_spec();
    update_str += " # visual_hints = edge_color = " + color + ", vertex_color = " + color;
  

     Notify("WPT_UPDATE_" + m_veh_name, update_str);
    }



    else if(key == "NAV_X"){
        m_current_x = msg.GetDouble();
      }
    else if(key == "NAV_Y"){
        m_current_y = msg.GetDouble();
      }
      





      else if(key == "GENPATH_REGENERATE"){
          //Notify("REGEN", "true");
      m_waypoints.add_vertex(m_origin_x, m_origin_y);
      XYSegList sorted_waypoints;
      XYSegList working_waypoints = m_waypoints;
      int closest_index = working_waypoints.closest_vertex(m_origin_x, m_origin_y); 
      double next_x = working_waypoints.get_vx(closest_index);
      double next_y = working_waypoints.get_vy(closest_index);
      working_waypoints.delete_vertex(closest_index);
      sorted_waypoints.add_vertex(next_x, next_y); 
      for(int i=1; i<m_waypoints.size(); i++){
      int closest_index = working_waypoints.closest_vertex(next_x, next_y);
       next_x = working_waypoints.get_vx(closest_index);
      next_y = working_waypoints.get_vy(closest_index);
      sorted_waypoints.add_vertex(next_x, next_y);
      working_waypoints.delete_vertex(closest_index);
      }
      string color;
      // if(next_x < 88){
      // color = "orange";
      // }
      if(next_x > 88){
      color = "yellow";
      }

      string update_str = "points = ";
      update_str += sorted_waypoints.get_spec();
      update_str += " # visual_hints = edge_color = " + color + ", vertex_color = " + color;
    

      Notify("WPT_UPDATE_" + m_veh_name, update_str); 
          
        //}
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

     else if(key == "FOO") 
       cout << "great!";

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);
   }
  
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool GenPath::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool GenPath::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!

  
  int closest_waypoint = m_waypoints.closest_vertex(m_current_x, m_current_y); //waypoint closest to the current position
  double closest_x = m_waypoints.get_vx(closest_waypoint); //grabs the x/y value for waypoint
  double closest_y = m_waypoints.get_vy(closest_waypoint);
  //Notify("CLOSE_X", doubleToString(closest_x));
  //Notify("CLOSE_Y", doubleToString(closest_y));
  double distance = sqrt(pow((closest_x - m_current_x),2) + pow((closest_y - m_current_y),2));
    if(distance < 5){
      m_waypoints.delete_vertex(closest_waypoint);//removes waypoint from list if within visit radius
    }     

 

  
  AppCastingMOOSApp::PostReport();
  return(true);


}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool GenPath::OnStartUp()
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
    else if(param == "vname") { 
      m_veh_name = toupper(value);
      handled=true;
    }    
    else if(param == "start_pos") {
      string m_start_pos = value;
      Notify("START", m_start_pos);
      m_origin_x = stod(MOOSChomp(m_start_pos, ","));
      m_origin_y = stod(m_start_pos);
      handled=true;
    }    

    else if(param == "visit_radius") {
      visit_radius = stod(value);
      handled=true;
    }    
    

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();  
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void GenPath::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("VISIT_POINT",0);
  Register("NODE_REPORT", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("GENPATH_REGENERATE", 0);
  // Register("FOOBAR", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool GenPath::buildReport() 
{
  m_msgs << "============================================ \n";
  m_msgs << "File: GenPath.cpp                            \n";
  m_msgs << "Author: Jason Barker                         \n";
  m_msgs << "============================================ \n";

  m_msgs << m_veh_name << " Waypoints Remaining: " << m_waypoints.size() << endl;
  

  return(true);
}

//------------------------------------------------------------
// Procedure: VectorPoints()  Parses data for X, Y, and ID

void GenPath::VectorPoints(string point)
{
  string str = point;
  vector<string> myvector = parseString(str, ',');
  for(unsigned int i=0; i<myvector.size(); i++){
    string param = biteStringX(myvector[i], '=');
    string value = myvector[i];
    if(tolower(param) == "id"){
      m_dbl_id = stod(value);}
    else if(tolower(param) == "x"){
      m_dbl_x = stod(value);}
    else if(tolower(param) == "y"){ 
      m_dbl_y = stod(value);}
  }
  return;
}

//---------------------------------------------------------------


  