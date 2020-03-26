/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TrafficScheme.cpp                                        */
/*    DATE: Mar 26, 2020                                                */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "TrafficScheme.h"
#include "FileBuffer.h"

using namespace std;

//---------------------------------------------------------
// Constructor

TrafficScheme::TrafficScheme()
{
}

//---------------------------------------------------------
// Destructor

TrafficScheme::~TrafficScheme()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool TrafficScheme::OnNewMail(MOOSMSG_LIST &NewMail)
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

     if(key == "FOO") 
       cout << "great!";

     // else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
     //   reportRunWarning("Unhandled Mail: " + key);
   }
	
   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool TrafficScheme::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool TrafficScheme::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool TrafficScheme::OnStartUp()
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
    if(param == "TSS_FILE")
      handled = processTSSFile(value);

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void TrafficScheme::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  // Register("PMV_CONNECT",0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool TrafficScheme::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "File:                                       " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}

//------------------------------------------------------------
// Procedure: processHazardFile

bool TrafficScheme::processTSSFile(string filename)
{
  string msg = "Reading " + filename + ": ";

  vector<string> lines = fileBuffer(filename);
  unsigned int i, vsize = lines.size();
  if(vsize == 0) {
    reportRunWarning(msg + "Empty or File Not found.");
    return(false);
  }
  else
    m_TSS_file = filename;

  XYTrafficScheme traffic_polys;

  for(i=0; i<vsize; i++) {
    string orig  = lines[i];
    lines[i] = stripBlankEnds(stripComment(lines[i], "//"));
    string left  = biteStringX(lines[i], '=');
    string right = lines[i];
    if(left == "traffic object") {
      type = biteStringX(right, ';');
      Notify("POLY_TYPE", type);
      specs = biteStringX(right, ';');
      Notify("POLY_SPEC", specs);
      string entrance = biteStringX(right, ';');
      string exit = right;
      }
    }





      // XYTrafficObject new_object.createTrafficPoly(right);
      // if(new_hazard.valid()) 
  
  
  return(true);
}



