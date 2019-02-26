/***********************************************************
    NAME: Jason Barker                                              
    ORGN: MIT                                             
    FILE: CommunicationAngle_jbbarker.cpp                                        
    DATE: 26 FEB 2019                                                
***********************************************************/

#include <iterator>
#include "MBUtils.h"
#include "CommunicationAngle_jbbarker.h"

using namespace std;

//---------------------------------------------------------
// Constructor

CommunicationAngle_jbbarker::CommunicationAngle_jbbarker()
{
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
}

