/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: TrafficGrade.cpp                                        */
/*    DATE: Mar 31, 2020                                                */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "ACTable.h"
#include "TrafficGrade.h"
#include <iostream>
#include <fstream>

using namespace std;

//---------------------------------------------------------
// Constructor

TrafficGrade::TrafficGrade()
{
  m_coll_count = 0;
  m_nm_count = 0;
  collision = false;
  m_traffic_slope = 0;

}

//---------------------------------------------------------
// Destructor

TrafficGrade::~TrafficGrade()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool TrafficGrade::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail);

  MOOSMSG_LIST::iterator p;
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key    = msg.GetKey();

    if(key == "COLLISION_DETECT_PARAMS"){
      string sval  = msg.GetString();
      processParameters(sval);
    }

    if(key == "NEAR_MISS"){
      string sval  = msg.GetString();
      processNearMiss(sval);     
    }

    if(key == "COLLISION"){
      string sval  = msg.GetString();
      collision = processCollision(sval);
      if(collision){
      } 

    }

  for(int i=0; i<m_nm_cpa.size(); i++){
    double slope = 0;
    slope = (m_nm_range - m_nm_cpa[i])/(m_nm_range - m_coll_range);
    string m_slope_interaction = doubleToString(slope);
    m_interaction.push_back(m_slope_interaction);
    m_traffic_slope += slope;
  }

  if((m_nm_count == 0) && (m_coll_count == 0))
    m_traffic_grade = 1.00;
  else if((m_nm_count != 0) && (m_coll_count == 0)){
  m_traffic_grade = 1-(m_traffic_slope/m_nm_count);
    if(m_traffic_grade < 0)
      m_traffic_grade = 0;
  }
  else if (m_coll_count != 0)
    m_traffic_grade = 0;

  // if(collision)
  //   m_traffic_grade = 0;


  traffic_score = doubleToString(m_traffic_grade);



  if(key == "SCORE"){
    bool mstr  = msg.IsString();
    if(mstr)
      outputScore(m_filename);
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

bool TrafficGrade::OnConnectToServer()
{
   registerVariables();
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool TrafficGrade::Iterate()
{
  AppCastingMOOSApp::Iterate();
  // Do your thing here!
  nm_cnt = intToString(m_nm_count); 
  coll_cnt = intToString(m_coll_count);  



  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool TrafficGrade::OnStartUp()
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

    if(param == "filename"){
      m_filename = tolower(value);
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

void TrafficGrade::registerVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
  Register("COLLISION", 0);
  Register("NEAR_MISS", 0);
  Register("COLLISION_DETECT_PARAMS", 0);
  Register("SCORE", 0);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool TrafficGrade::buildReport() 
{
  m_msgs << "============================================" << endl;
  m_msgs << "Traffic Scheme Grader                        " << endl;
  m_msgs << "============================================" << endl;

  ACTable actab(4);
  actab << "" << "Traffic Scheme Conditions" << "" << "";
  actab.addHeaderLines();
  actab << "" << "Overall Traffic Score" << traffic_score << "";
  actab.addHeaderLines();
  actab.addHeaderLines();
  actab << "Number of Collisions" << coll_cnt << "" << "";
  actab << "Collision Range" << doubleToString(m_coll_range) << "" << "";

  if(m_coll_count != 0){
    actab << "" << "COLLISION RESULTS IN FAILURE" << "" << "";
  }
  actab.addHeaderLines();
  actab.addHeaderLines();
  actab << "Number of Near Misses" << nm_cnt << "" << "";
  actab << "Near Miss Range" << doubleToString(m_nm_range) << "" << "" ;
  actab.addHeaderLines();
  actab.addHeaderLines();
  actab << "Interactions" << "" << "" << "";
  actab.addHeaderLines();
  actab  <<"Contact Name" << "CPA" << "Penalty" << "";
  actab.addHeaderLines();
  for(int i=0; i<m_nm_cpa.size(); i++){
    actab << m_contact_name[i] << doubleToString(m_nm_cpa[i]) << m_interaction[i] << "";
  }

  m_msgs << actab.getFormattedString();

  return(true);
}

//------------------------------------------------------------
// Procedure: processParameters

void TrafficGrade::processParameters(string s)
{
  string collrng = tokStringParse(s, "collision_range", ',', '=');
  m_coll_range = stod(collrng);
  string nmrng = tokStringParse(s, "near_miss_range", ',', '=');
  m_nm_range = stod(nmrng);

}

//------------------------------------------------------------
// Procedure: processNearMiss

void TrafficGrade::processNearMiss(string s)
{
  string vname1 = tokStringParse(s, "vname1", ',', '=');
  string vname2 = tokStringParse(s, "vname2", ',', '=');
  string en_cpa = tokStringParse(s, "cpa", ',', '=');
  double enc_cpa = stod(en_cpa); 

  if(vname1 == "usv"){
    m_nm_cpa.push_back(enc_cpa);
    m_contact_name.push_back(vname2);
    ++m_nm_count; 
  } 

  if(vname2 == "usv"){
    m_nm_cpa.push_back(enc_cpa);
    m_contact_name.push_back(vname1);
    ++m_nm_count;
  }


}

//------------------------------------------------------------
// Procedure: processCollision

bool TrafficGrade::processCollision(string s)
{
  string vname1 = tokStringParse(s, "vname1", ',', '=');
  string vname2 = tokStringParse(s, "vname2", ',', '=');
  if((vname1 == "usv")||(vname2 == "usv")){
    ++m_coll_count;  
    return(true);
  }   
  else
    return(false);
}

//------------------------------------------------------------
// Procedure: outputScore
void TrafficGrade::outputScore(string filename)
{
  ofstream myfile;
  myfile.open (filename, ios::out | ios::app | ios::binary);
  myfile << traffic_score << endl;
  myfile.close();
}
