/************************************************************/
/*    NAME: Jason Barker                                              
/*    ORGN: MIT                                             
/*    FILE: Odometry.cpp                                        
/*    DATE: Feb 14, 2019  
/*    This app will start a running odometer during MOOS Mission   
/*    This app registers for NAV_X and NAV_Y from uSimMarine   
/*    Returns distance traveled                         
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "Odometry.h" 

using namespace std;

//---------------------------------------------------------
// Constructor

Odometry::Odometry()
{ //initializing variables
  m_first_reading == true;
  m_current_x=0;
  m_current_y=0;
  m_previous_x=0;
  m_previous_y=0;
  m_total_distance=0;
}

//---------------------------------------------------------
// Destructor

Odometry::~Odometry()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool Odometry::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail); 
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) { //This iterates through MOOS messages 
    CMOOSMsg &msg = *p;

    string key   = msg.GetKey();
    double dval  = msg.GetDouble();
    if (key == "NAV_X") // When you get new NAV_X mail,
      m_current_x = dval; // change the variable current_x to the new value
    if (key == "NAV_Y") // When you get new NAV_Y mail,
      m_current_y = dval; // change the variable current_y to the new value
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool Odometry::OnConnectToServer()
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

bool Odometry::Iterate() // This is the odometery function
{
  AppCastingMOOSApp::Iterate(); 
  if (m_first_reading == true){ // This accounts for the first reading and sets the total distance to 0
    m_total_distance=0; // this if loop accounts for starting at a location that is not the origin and exits the iterate loop
    m_first_reading == false;
    return 0;}
  
  double distance = sqrt(pow((m_current_x - m_previous_x), 2)+pow((m_current_y - m_previous_y), 2)); //calculates 2D distance
  m_total_distance = m_total_distance + distance; //Adds calculated distance to previous running total
  
  m_previous_x = m_current_x; // Updates the variables x and y
  m_previous_y = m_current_y;
  Notify("ODOMETRY_DIST", m_total_distance); //Notify publishes to MOOS 
  AppCastingMOOSApp::PostReport();
  return(true); //exits the iterate loop
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool Odometry::OnStartUp()
{
  AppCastingMOOSApp::OnStartUp();
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

void Odometry::RegisterVariables()
{
  AppCastingMOOSApp::RegisterVariables(); 
  Register("NAV_X", 0); //This is the function Register variables
  Register("NAV_Y", 0); //This function will register for NAV_X and Y mail
}
bool Odometry::buildReport()
{
  m_msgs << "Mission Total Distance Traveled: " << m_total_distance << endl;
  return (true);
}

