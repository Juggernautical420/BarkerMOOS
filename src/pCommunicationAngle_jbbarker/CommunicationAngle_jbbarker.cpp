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
  m_water_depth =6000;
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
  m_co_g = calcConstant(m_surface_sound_speed, m_sound_speed_gradient);

  m_distance = calc2Distance(m_nav_x, m_nav_y, m_c_nav_x, m_c_nav_y); //Calculating 2D Distance
  Notify("DISTANCE", doubleToString(m_distance));
  //Troubleshooting Notification

  m_sound_speed = calcSoundSpeed(m_surface_sound_speed, m_nav_depth, m_sound_speed_gradient); //Sound speed at ownshp depth
  m_rev_sound_speed = calcSoundSpeed(m_surface_sound_speed, m_c_nav_depth, m_sound_speed_gradient); //Calulate sound speed of collaborator
  
  m_calc_center = calcCircleCenter(m_co_g, m_nav_depth, m_c_nav_depth, m_distance);//Using geometry to solve for center of the circle
  Notify("CENTER", doubleToString(m_calc_center));
  //Troubleshooting Notification

  m_calc_radius = calcRadius(m_nav_depth, m_co_g, m_calc_center); //Determining R 
  Notify("RADIUS", doubleToString(m_calc_radius));
  //Troubleshooting Notification

  m_calc_arclength = calcArcLength(m_calc_radius, m_distance); //Geometrically solving for arclength
  Notify("ARCLENGTH", doubleToString(m_calc_arclength));
  //Troubleshooting Notification
 
  m_elev_angle = calcElevAngle(m_sound_speed, m_calc_radius, m_sound_speed_gradient);//Calculating incident angle
  Notify("ELEV_ANGLE", doubleToString(m_elev_angle));
  //Troubleshooting Notification

  double m_elev_rad = m_elev_angle * M_PI/180;

  double m_graz_rad = m_elev_rad - (m_calc_arclength/m_calc_radius);//Solving for grazing angle
  double m_graz_angle = m_graz_rad * 180/M_PI;
  Notify("GRAZ_ANGLE", doubleToString(m_graz_angle));
  //Troubleshooting Notification



  m_calc_rs = calcRs(m_calc_radius, m_calc_arclength, m_elev_angle);//Calculation of r(s)
  Notify("R_S", doubleToString(m_calc_rs));
  //Troubleshooting Notification
  
  m_calc_zs = calcZs(m_calc_radius, m_calc_arclength, m_elev_angle, m_co_g);//Calculation of z(s)
  Notify("Z_S", doubleToString(m_calc_zs));
  //Troubleshooting Notification


  double m_phi = atan((m_c_nav_y - m_nav_y)/(m_c_nav_x - m_nav_x));//Angle between us and target in radians in the r plane

  double m_delta_theta = m_elev_rad + 0.000001;
  double m_delta_degrees = m_delta_theta * 180/M_PI;
  

  double m_new_radius = m_sound_speed/(m_sound_speed_gradient * cos(m_delta_theta));
  Notify("NEW_RADIUS", doubleToString(m_new_radius));
  //Troubleshooting Notification
  double m_new_rs = calcRs(m_new_radius, m_calc_arclength, m_delta_degrees);
  Notify("R_S1", doubleToString(m_new_rs));
  //Troubleshooting Notification
  double m_dr_dtheta = (m_new_rs - m_calc_rs)/(m_delta_theta - m_elev_rad);
  Notify("DR_DT", doubleToString(m_dr_dtheta));
  //Troubleshooting Notification
  double m_Js = (m_calc_rs/(sin(m_graz_rad))) * m_dr_dtheta;
  Notify("J_S", doubleToString(m_Js));
  //Troubleshooting Notification
  double m_new_speed = calcSoundSpeed(m_surface_sound_speed, m_calc_zs, m_sound_speed_gradient);

  double m_ps = m_normalized_press * sqrt(abs(((m_new_speed * cos(m_elev_rad))/(m_sound_speed * m_Js))));

  double m_trans_loss = -20 * log10(m_ps/m_normalized_press);
  Notify("TRANS_LOSS", doubleToString(m_trans_loss));
  //Troubleshooting Notification

  string elev = "elev_angle=" + doubleToString(m_elev_angle, 4);
  string tl = ", transmission_loss=" + doubleToString(m_trans_loss, 4);
  string id = ", id=jbbarker@mit.edu";
  string path = elev + tl + id;
  Notify("ACOUSTIC_PATH", path);

  double m_z_max = (m_sound_speed/((m_sound_speed_gradient*cos(m_elev_rad)))) - m_co_g;
  

  if(m_z_max <= m_water_depth){
    string ax = "x=" + doubleToString(m_nav_x, 3);
    string ay = ", y=" + doubleToString(m_nav_y, 3);
    string adepth = ", depth=" + doubleToString(m_nav_depth, 1);
    string cona = ax + ay + adepth + id;
    Notify("CONNECTIVITY_LOCATION", cona);
  }
  else{
    double m_sound_max = calcSoundSpeed(m_surface_sound_speed, m_z_max, m_sound_speed_gradient);//Solve for sound speed at zmax
    double m_theta_max = acos(m_rev_sound_speed/m_sound_max); //Using collaborator as origin and calculate max angle for the collaborator
    double m_max_radius = m_rev_sound_speed/(m_sound_speed_gradient * cos(m_theta_max)); // Determine new radius using collaborator as origin
    double m_new_center = sqrt((pow(m_max_radius,2)) - (pow((m_c_nav_depth + m_co_g),2)));//Determine new center with new radius
    double m_rho = atan((m_co_g+m_nav_depth)/(m_distance - m_new_center)); //Path to new point on new radius 
    double m_max_rs = m_max_radius * sin(m_rho); 
    double m_max_zs = m_max_radius * cos(m_rho);
    double m_new_nav_x = m_max_rs * cos(m_phi);
    double m_new_nav_y = m_max_rs * sin(m_phi);

    string newx = "x=" + doubleToString(m_new_nav_x,3);
    string newy = ", y=" + doubleToString(m_new_nav_y, 3);
    string newz = ", depth=" + doubleToString(m_max_zs, 1);
    string newcon = newx + newy + newz + id;
    Notify("CONNECTIVITY_LOCATION", newcon);
  }



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

