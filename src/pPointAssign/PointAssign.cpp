/************************************************************/
/*    NAME: Jason Barker                                    */
/*    ORGN: MIT                                             */
/*    FILE: PointAssign.cpp                                 */
/*    DATE: Mar 15, 2019                                    */
/************************************************************/

#include <iterator>
#include "MBUtils.h"
#include "PointAssign.h"
#include "ACTable.h"

using namespace std;

//---------------------------------------------------------
// Constructor

PointAssign::PointAssign()
{
  m_veh1_named = false;
  m_veh2_named = false;
 
}

//---------------------------------------------------------
// Destructor

PointAssign::~PointAssign()
{
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool PointAssign::OnNewMail(MOOSMSG_LIST &NewMail)
{
  AppCastingMOOSApp::OnNewMail(NewMail); 
  MOOSMSG_LIST::iterator p;
   
  for(p=NewMail.begin(); p!=NewMail.end(); p++) {
    CMOOSMsg &msg = *p;
    string key   = msg.GetKey();
    //  if (key == "NODE_REPORT"){

    //    string sval  = msg.GetString();
    //   VehicalAssign1(sval);
    //   if(VehicalAssign1(sval)){
    //   VehicalAssign2(sval);
    //   Notify("POINTS_PAUSE", "false");  
     


    // }
  



    if (key == "VISIT_POINT"){
      string visit_points  = msg.GetString();
      //Notify("REFER", visit_points);//Troubleshooting and building notification
      if(visit_points == "firstpoint"){
        Notify(m_veh_1, visit_points);
        Notify(m_veh_2, visit_points);
        continue;
      }
      if(visit_points == "lastpoint"){
        Notify(m_veh_1, visit_points);
        Notify(m_veh_2, visit_points);
        continue;
      }

      VectorPoints(visit_points);

      if(!assign_by_region){
        AssignAlternating(visit_points);
        postViewPoint(m_string_x, m_string_y, to_string(m_string_id), m_color);
      } 
      else{
        AssignRegionally(visit_points);
        postViewPoint(m_string_x, m_string_y, to_string(m_string_id), m_color);
      }
      
    }



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

     if(key == "FOO") 
       cout << "great!";

     else if(key != "APPCAST_REQ") // handled by AppCastingMOOSApp
       reportRunWarning("Unhandled Mail: " + key);



   }

   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool PointAssign::OnConnectToServer()
{
   // register for variables here
   // possibly look at the mission file?
   // m_MissionReader.GetConfigurationParam("Name", <string>);
   // m_Comms.Register("VARNAME", 0);
   //whenever pPointAssin connects to server

   RegisterVariables();
   
   return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool PointAssign::Iterate()
{
  AppCastingMOOSApp::Iterate(); 

  AppCastingMOOSApp::PostReport();
  return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool PointAssign::OnStartUp()
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

      if(param == "vname1") {
      m_veh_1 = "VISIT_POINT_" + toupper(value);
      m_veh1_named = true;
      Notify("VEH1", m_veh_1);
      Register(m_veh_1, 0);
      }

      if(param == "vname2") {
      m_veh_2 = "VISIT_POINT_" + toupper(value);
      m_veh2_named = true;
      Notify("VEH2", m_veh_2);
      Register(m_veh_2, 0); 
      }

      if(m_veh1_named && m_veh2_named){
        Notify("POINTS_PAUSE", "false");
      }

  
     

      if (param == "assign_by_region"){//This looks at the Process Config parameter
        if (tolower(value) == "false"){//Toggles the assign_be_region boolean based off config
          assign_by_region = false;
        }
        else{
          assign_by_region = true;
        }
        Notify("REGION", boolToString(assign_by_region));
      }
    }
  }
  
  RegisterVariables();	
  return(true);
}

//---------------------------------------------------------
// Procedure: RegisterVariables

void PointAssign::RegisterVariables()
{
  AppCastingMOOSApp::RegisterVariables();
  // Register("FOOBAR", 0);
 
  Register("VISIT_POINT", 0);
  Register("NODE_REPORT", 0);
  Register("VIEW_POINT", 0);


}
//------------------------------------------------------

void PointAssign::VectorPoints(std::string sval)
{
  vector<string> myvector = parseString(sval, ',');
  for(unsigned int i=0; i<myvector.size(); i++){
    string param = biteStringX(myvector[i], '=');
    string value = myvector[i];
    if(tolower(param) == "id"){
      m_string_id = stoi(value);
    }
    else if(tolower(param) == "x"){
      m_string_x = stoi(value);
    }
    else if(tolower(param) == "y"){ 
      m_string_y = stoi(value);
    }
  }
  return;
}
void PointAssign::AssignAlternating(std::string sval)
{
  if(m_string_id%2 == 0){
    Notify(m_veh_2, sval);
    m_color = "red";
  }
  else{ 
    Notify(m_veh_1, sval);
    m_color = "yellow";
  }
  return;
}
void PointAssign::AssignRegionally(std::string sval)
{
  if(m_string_x >88){
    Notify(m_veh_2, sval); 
    m_color = "red";
  }
  else{
    Notify(m_veh_1, sval);
    m_color = "yellow";
  }
  return;
}
 void PointAssign::postViewPoint(double x, double y, std::string label, std::string color)
 {
   XYPoint point(x, y);
   point.set_label(label);
   point.set_color("vertex", color);
   point.set_param("vertex_size", "2");

   string spec = point.get_spec();
   Notify("VIEW_POINT", spec);
 }

 bool PointAssign::VehicalAssign1(std::string sval)
 {
    if(!m_veh1_named){
        m_veh_1 = "VISIT_POINT_" + toupper((tokStringParse(sval, "NAME", ',', '=')));
        m_veh1_named = true;
        Notify("VEH1", m_veh_1);
        Register(m_veh_1, 0);
      }
    return(true);
 }

  bool PointAssign::VehicalAssign2(std::string sval)
{
      m_veh_2 =  "VISIT_POINT_" + toupper((tokStringParse(sval, "NAME", ',', '=')));
      if(m_veh_2 == m_veh_1){
          return(false);
      }
    else{ 
      m_veh_2 =  "VISIT_POINT_" + toupper((tokStringParse(sval, "NAME", ',', '=')));
      Notify("VEH2", m_veh_2);
      Register(m_veh_2, 0);
      return(true);
        }
}
//------------------------------------------------------------
// Procedure: buildReport()

// bool PointAssign::buildReport() 
// {
//   m_msgs << "============================================ \n";
//   m_msgs << "File:                                        \n";
//   m_msgs << "============================================ \n";

//   ACTable actab(4);
//   actab << "Alpha | Bravo | Charlie | Delta";
//   actab.addHeaderLines();
//   actab << "one" << "two" << "three" << "four";
//   m_msgs << actab.getFormattedString();

//   return(true);
// }
