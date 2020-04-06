/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TrafficPopulate.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "TrafficPopulate.h"

using namespace std;

//---------------------------------------------------------
// Constructor

TrafficPopulate::TrafficPopulate()
{
}

//---------------------------------------------------------
// Destructor

TrafficPopulate::~TrafficPopulate()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool TrafficPopulate::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();


  if(key == "PMV_CONNECT"){
    for (int i=0; i<m_polygons.size(); i++)
    Notify ("VIEW_POLYGON", m_polygons[i]); 
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

bool TrafficPopulate::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool TrafficPopulate::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool TrafficPopulate::OnStartUp()
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

    if(param == "traffic_file"){
      handled = handleTrafficFile(value);
      m_polygons = m_tss.getAllViewableObjectSpecs();
    }

    if(!handled)
      reportUnhandledConfigWarning(orig);

  }
  
  registerVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void TrafficPopulate::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("PMV_CONNECT",0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool TrafficPopulate::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "Traffic Separation Scheme Built             " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(2);
  actab.addHeaderLines();
  actab << "Number of Polygons" << intToString(m_polygons.size());
  actab.addHeaderLines();  
  for(int i=0; i<m_polygons.size(); i++){
    actab << "Polygon" << m_polygons[i];
  }
  m_msgs << actab.getFormattedString();

  return(true);
}

//------------------------------------------------------------
// Procedure: handleTrafficFile()

bool TrafficPopulate::handleTrafficFile(string s)
{
  vector<string> lines = fileBuffer(s);
    if(lines.size() == 0)
      return(false);

    TrafficObject new_object;
      for(unsigned int i=0; i<lines.size(); i++){
      string str = stripBlankEnds(lines[i]);
      if(strBegins(str, "//"))
       continue;
      if(str == "")
        continue;
       string param = biteStringX(str, '=');
      if(param == "type")
       new_object.setTrafficType(str);
     if(param == "poly")
       new_object.setPolyType(str);
     if(param == "points")
       new_object.setPolyPts(str);
     if(param == "label")
       new_object.setPolyName(str);
     if(param == "start")
       new_object.setStartPt(str); 
    }

    m_tss.addTrafficObject(new_object);

    return(true);
}

