/************************************************************/
/*    NAME: Team Awesome                                              */
/*    ORGN: MIT                                             */
/*    FILE: TempPassing.cpp                                        */
/*    DATE: May 9, 2019                                                */
/************************************************************/

#include "TempPassing.h"

using namespace std;

//---------------------------------------------------------
// Constructor

TempPassing::TempPassing()
{
  m_sent_time = 0;
}

//---------------------------------------------------------
// Destructor

TempPassing::~TempPassing()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool TempPassing::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

#if 0 // Keep these around just for template
    string comm  = msg.GetCommunity();
    double dval  = msg.GetDouble();
    string sval  = msg.GetString(); 
    string msrc  = msg.GetSource();
    double mtime = msg.GetTime();
    bool   mdbl  = msg.IsDouble();
    bool   mstr  = msg.IsString();
#endif

     if(key == "UCTD_MSMNT_REPORT")
      {
       TempEntry m_new_entry; //Create new object Temperature Entry
       m_new_entry.parseMessage(msg.GetString()); //Parse MOOS Message and get object member variables
       m_temperature_queue.push_back(m_new_entry); //Add the item to the list of entries
      }
     // else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
     //  {
     //    reportRunWarning("Unhandled Mail: " + key);
     //  }
   }
  
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool TempPassing::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool TempPassing::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!

  bool in_range = true;

  bool in_period;
    if(MOOSTime() - m_sent_time > 15) {
      in_period = true;
    }
    else {
      in_period = false;
    }

  if(in_range && in_period)
    {
      list<TempEntry>::iterator p;
      int size = m_temperature_queue.size();
      for(int i=0; i<size; i++) {
      //for(p = m_temperature_queue.begin(); p !=m_temperature_queue.end(); p++) {
        p = m_temperature_queue.begin();
        TempEntry& current_temp = *p;
        NodeMessage node_message; //generate and send a Node Message containing the current temperature reading in the queue
        node_message.setSourceNode(m_veh_name);//my boat name ===SET ME!!!!
        node_message.setDestNode("all");
        node_message.setVarName("UCTD_MSMNT_REPORT");
        node_message.setStringVal(current_temp.genMessage());
        string msg = node_message.getSpec();
        Notify("NODE_MESSAGE_LOCAL", msg);
        p = m_temperature_queue.erase(p); //erase sent temperatures from the queue
        m_sent_time = MOOSTime();
      }
    }



  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool TempPassing::OnStartUp()
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

  else if(param=="VNAME"){
    m_veh_name = value;
    handled = true;
  }

    // if(!handled)
    //   reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();  
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void TempPassing::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  Register("UCTD_MSMNT_REPORT", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool TempPassing::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "File: pTempPassing                          " << endl;
  m_msgs << "Team Awesome                                " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}
