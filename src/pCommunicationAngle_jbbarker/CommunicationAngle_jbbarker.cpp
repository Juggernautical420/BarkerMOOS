/***********************************************************
    NAME: Jason Barker                                              
    ORGN: MIT                                             
    FILE: CommunicationAngle_jbbarker.cpp                                        
    DATE: 26 FEB 2019                                                
***********************************************************/

#include <iterator>
#include "MBUtils.h"
#include "CommunicationAngle_jbbarker.h"
#include <cmath>

using namespace std;

//---------------------------------------------------------
// Constructor

CommunicationAngle_jbbarker::CommunicationAngle_jbbarker()
{
  m_surface_sound_speed = 1480;
  m_sound_speed_gradient = 0.016;
  m_water_depth =0;
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
  double co_z = m_surface_sound_speed/m_sound_speed_gradient;
  double distance = sqrt(pow((m_c_nav_x - m_nav_x), 2) + pow((m_c_nav_y - m_nav_y), 2));
  Notify("DISTANCE", doubleToString(distance));
  //Troubleshooting Notification

  double center = ((pow((co_z + m_c_nav_depth), 2)-pow((co_z + m_nav_depth), 2))/(2*distance))+(distance/2);
  Notify("CENTER", doubleToString(center));
  //Troubleshooting Notification

  double radius = sqrt(pow(center, 2) + pow((co_z + m_nav_depth),2));
  Notify("RADIUS", doubleToString(radius));
  //Troubleshooting Notification

  double arclength = 2*radius*asin(distance/(2*radius));
  Notify("ARCLENGTH", doubleToString(arclength));
  //Troubleshooting Notification

  
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

