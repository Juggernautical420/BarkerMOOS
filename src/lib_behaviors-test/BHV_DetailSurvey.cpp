/************************************************************/
/*    NAME: Jason Barker                                              */
/*    ORGN: MIT                                             */
/*    FILE: BHV_FrontSurvey.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <math.h>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "AngleUtils.h"
#include "BHV_DetailSurvey.h"
#include "ZAIC_PEAK.h"
#include "OF_Coupler.h"
#include "OF_Reflector.h"
#include "XYFormatUtilsPoly.h"

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_DetailSurvey::BHV_DetailSurvey(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  straight_done = false;
  getting_cold = false;
  getting_hot = false;

  addInfoVars("NAV_X, NAV_Y");
  addInfoVars("NAV_HEADING", "no_warning");
  addInfoVars("NORTH_TEMP", "no_warning");
  addInfoVars("CURRENT_TEMP", "no_warning");
  addInfoVars("SOUTH_TEMP", "no_warning");
  addInfoVars("N_S_DELTA", "no_warning");
  addInfoVars("STRAIGHTPATH", "no_warning");     
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_DetailSurvey::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  
  if((param == "foo") && isNumber(val)) {
    // Set local member variables here
    return(true);
  }
  else if (param == "bar") {
    // return(setBooleanOnString(m_my_bool, val));
  }

  // If not handled above, then just return false;
  return(false);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_DetailSurvey::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_DetailSurvey::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_DetailSurvey::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_DetailSurvey::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_DetailSurvey::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_DetailSurvey::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_DetailSurvey::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_DetailSurvey::onRunState()
{
  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;
  bool ok1, ok2;
  m_current_x = getBufferDoubleVal("NAV_X", ok1);
  m_current_y = getBufferDoubleVal("NAV_Y", ok2);
  if(!ok1 || !ok2){
    postWMessage("No ownship X/Y info in info_buffer.");
    return(0);
  }

  bool ok4;
  m_current_heading = getBufferDoubleVal("NAV_HEADING", ok4);

  bool oknortht;
  m_low_temp = getBufferDoubleVal("NORTH_TEMP", oknortht);

  bool oksoutht;
  m_high_temp = getBufferDoubleVal("SOUTH_TEMP", oksoutht);
  m_avg_temp = (m_high_temp + m_low_temp)/2;

  bool okdelta;
  m_delta_temp = getBufferDoubleVal("N_S_DELTA", okdelta);
  m_low_temp_turn = (0.1 * m_delta_temp) + m_low_temp;
  if(m_current_temp <= m_low_temp_turn)
    getting_cold = true;

  m_high_temp_turn = (0.5 * m_delta_temp) + m_low_temp;
  if(m_current_temp >= m_high_temp_turn)
    getting_hot = true;

  bool okstr8;
  m_straightpath_done = getBufferStringVal("STRAIGHTPATH", okstr8);
  if(m_straightpath_done == "true")
    straight_done = true;

  bool okcurrtemp;
  m_current_temp = getBufferDoubleVal("CURRENT_TEMP", okcurrtemp);
 


 //Zigline Process
  if(straight_done){
    
  //Zigline North if Temp is Hot
    if((getting_hot) && (m_current_temp >= m_low_temp)){
      zig_direction = 0;
      ipf = buildFunctionWithZAIC(); 
    }

    // //Zigline North if Temp is Hot (local maximum)
    // if((m_current_temp == m_high_temp) && (m_current_heading >= m_low_temp)){
    //   zig_direction = 0;     
    //   ipf = buildFunctionWithZAIC(); 
    // }


    //Zigline South if Temp is Cold
    if((getting_cold) && (m_current_temp <= m_high_temp_turn)){
        zig_direction = 180;
      ipf = buildFunctionWithZAIC(); 
    }

    // //Zigline South if Temp is Cold (local minimum)
    // if((m_current_temp < m_low_temp_turn) && (m_current_heading <=m_avg_temp)){
    //  if(m_current_heading < 180){
    //     zig_direction = 90;
    //   }
    //   else{
    //     zig_direction = 270;
    //   }
    //   ipf = buildFunctionWithZAIC(); 
    // } 

  }  

  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}
//-----------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction *BHV_DetailSurvey::buildFunctionWithZAIC() 
{
  // ZAIC_PEAK spd_zaic(m_domain, "speed");
  // spd_zaic.setSummit(2);
  // spd_zaic.setPeakWidth(0.5);
  // spd_zaic.setBaseWidth(1.0);
  // spd_zaic.setSummitDelta(0.8);  
  // if(spd_zaic.stateOK() == false) {
  //   string warnings = "Speed ZAIC problems " + spd_zaic.getWarnings();
  //   postWMessage(warnings);
  //   return(0);
  // }
  

  ZAIC_PEAK crs_zaic(m_domain, "course");
  crs_zaic.setSummit(zig_direction);
  crs_zaic.setPeakWidth(0);
  crs_zaic.setBaseWidth(180);
  crs_zaic.setSummitDelta(0);  
  crs_zaic.setValueWrap(true);
  if(crs_zaic.stateOK() == false) {
    string warnings = "Course ZAIC problems " + crs_zaic.getWarnings();
    postWMessage(warnings);
    return(0);
  }

  //IvPFunction *spd_ipf = spd_zaic.extractIvPFunction();
  IvPFunction *crs_ipf = crs_zaic.extractIvPFunction();

  //OF_Coupler coupler;
  //IvPFunction *ivp_function = coupler.couple(crs_ipf, spd_ipf, 50, 50);

  return(crs_ipf);
}
