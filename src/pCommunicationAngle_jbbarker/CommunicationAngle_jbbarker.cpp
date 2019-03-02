/***********************************************************
    NAME: Jason Barker                                              
    ORGN: MIT                                             
    FILE: CommunicationAngle_jbbarker.cpp                                        
    DATE: 26 FEB 2019                                                
***********************************************************/

#include <iterator>
#include "MBUtils.h"
#include "CommunicationAngle_jbbarker.h"
#include "AngleCalcs.h"


using namespace std;

//---------------------------------------------------------
// Constructor

CommunicationAngle_jbbarker::CommunicationAngle_jbbarker()
{
  m_surface_sound_speed = 1480;
  m_sound_speed_gradient = 0.016;
  m_water_depth =0;
  m_normalized_press = 1/(4 * M_PI);
}

//---------------------------------------------------------
// Destructor

CommunicationAngle_jbbarker::~CommunicationAngle_jbbarker()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool CommunicationAngle_jbbarker::OnNewMail(MOOSMSG_LIST &NewMail)
{
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;

    //Register for new mail
    string key   = msg.GetKey();
    
    if (key == "VEHICLE_NAME")
      m_ownship_name = msg.GetString(); 
    if (key == "NAV_X")
      m_nav_x = msg.GetDouble();
    if (key == "NAV_Y")
      m_nav_y = msg.GetDouble();
    if (key == "NAV_DEPTH")
      m_nav_depth = msg.GetDouble();
    if (key == "NAV_HEADING")
      m_nav_heading = msg.GetDouble();
    if (key == "NAV_SPEED")
      m_nav_speed = msg.GetDouble(); 
    
    
   
  
    
    //Establish a string for collecting collaborator information
    if (key == "COLLABORATOR_NAME")
      m_coll_name = msg.GetString();
      m_coll_nav_x = m_coll_name + "_NAV_X";
      Register(m_coll_nav_x,0);
      m_coll_nav_y = m_coll_name + "_NAV_Y";
      Register(m_coll_nav_y,0);
      m_coll_nav_depth = m_coll_name + "_NAV_DEPTH";
      Register(m_coll_nav_depth,0);
      m_coll_nav_heading = m_coll_name + "_NAV_HEADING";
      Register(m_coll_nav_heading,0);
      m_coll_nav_speed = m_coll_name + "_NAV_SPEED";
      Register(m_coll_nav_speed,0);
    //Registering for Collaborator information
    if (key == m_coll_nav_x)
      m_c_nav_x = msg.GetDouble();
    if (key == m_coll_nav_y)
      m_c_nav_y = msg.GetDouble();
    if (key == m_coll_nav_depth)
      m_c_nav_depth = msg.GetDouble();
    if (key == m_coll_nav_heading)
      m_c_nav_heading = msg.GetDouble();
    if (key == m_coll_nav_speed)
      m_c_nav_speed = msg.GetDouble();
  

#if 0 // Keep these around just for template
    string key   = msg.GetKey();
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool CommunicationAngle_jbbarker::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
	
   RegisterVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool CommunicationAngle_jbbarker::Iterate()
{
  m_co_z = calcConstant(m_surface_sound_speed, m_sound_speed_gradient);

  m_distance = calc2Distance(m_nav_x, m_nav_y, m_c_nav_x, m_c_nav_y);
  Notify("DISTANCE", doubleToString(m_distance));
  //Troubleshooting Notification

  m_sound_speed = calcSoundSpeed(m_surface_sound_speed, m_nav_depth, m_sound_speed_gradient);
 
  m_calc_center = calcCircleCenter(m_co_z, m_nav_depth, m_c_nav_depth, m_distance);
  Notify("CENTER", doubleToString(m_calc_center));
  //Troubleshooting Notification

  m_calc_radius = calcRadius(m_nav_depth, m_co_z, m_calc_center);
  Notify("RADIUS", doubleToString(m_calc_radius));
  //Troubleshooting Notification

  m_calc_arclength = calcArcLength(m_calc_radius, m_distance);
  Notify("ARCLENGTH", doubleToString(m_calc_arclength));
  //Troubleshooting Notification
 
  m_elev_angle = calcElevAngle(m_sound_speed, m_calc_radius, m_sound_speed_gradient);
  Notify("ELEV_ANGLE", doubleToString(m_elev_angle));
  //Display Elevation Angle Answer

  m_calc_rs = calcRs(m_calc_radius, m_calc_arclength, m_elev_angle);
  Notify("R_S", doubleToString(m_calc_rs));
  //Troubleshooting Notification
  
  m_calc_zs = calcZs(m_calc_radius, m_calc_arclength, m_elev_angle, m_co_z);
  Notify("Z_S", doubleToString(m_calc_zs));
  //Troubleshooting Notification




  double m_elev_rad = m_elev_angle * M_PI/180;

  double m_graz_rad = m_elev_rad - (m_calc_arclength/m_calc_radius);
  double m_graz_angle = m_graz_rad * 180/M_PI;
  Notify("GRAZ_ANGLE", doubleToString(m_graz_angle));

  double m_delta_theta = m_elev_rad + 0.0001;
  double m_delta_degrees = m_delta_theta * 180/M_PI;
  

  double m_new_radius = m_sound_speed/(m_sound_speed_gradient * cos(m_delta_theta));
  Notify("NEW_RADIUS", doubleToString(m_new_radius));

  double m_new_arclength = calcArcLength(m_new_radius, m_distance);

  double m_new_rs = calcRs(m_new_radius, m_new_arclength, m_delta_degrees);
  Notify("R_S1", doubleToString(m_new_rs));

  double m_dr_dtheta = (m_new_rs - m_calc_rs)/(m_delta_theta - m_elev_rad);
  Notify("DR_DT", doubleToString(m_dr_dtheta));

  double m_Js = (m_calc_rs/(sin(m_graz_rad))) * m_dr_dtheta;
  Notify("J_S", doubleToString(m_Js));

  double m_new_speed = calcSoundSpeed(m_surface_sound_speed, m_calc_zs, m_sound_speed_gradient);

  double m_ps = m_normalized_press * sqrt(abs(((m_new_speed * cos(m_elev_rad))/(m_sound_speed * m_Js))));

  double m_trans_loss = -20 * log10(m_ps/m_normalized_press);
  Notify("TRANS_LOSS", doubleToString(m_trans_loss));



  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool CommunicationAngle_jbbarker::OnStartUp()
{
  list<string> sParams;
  m_MissionReader.EnableVerbatimQuoting(false);
  if(m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
    list<string>::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
      string line  = *p;
      string param = tolower(biteStringX(line, '='));
      string value = line;
      
      if(param == "foo") {
        //handled
      }
      else if(param == "bar") {
        //handled
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void CommunicationAngle_jbbarker::RegisterVariables()
{
  // Register("FOOBAR", 0);
  Register("VEHICLE_NAME", 0);
  Register("COLLABORATOR_NAME", 0);
  Register("NAV_X", 0);
  Register("NAV_Y", 0);
  Register("NAV_DEPTH", 0);
  Register("NAV_HEADING", 0);
  Register("NAV_SPEED", 0);
}

