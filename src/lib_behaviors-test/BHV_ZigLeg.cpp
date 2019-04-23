/************************************************************/
/*    NAME: Jason Barker                                    */
/*    ORGN: MIT                                             */
/*    FILE: BHV_ZigLeg.cpp                                   */
/*    DATE: 18 Apr 2019                                     */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_ZigLeg.h"
#include "ZAIC_PEAK.h"
#include "OF_Coupler.h"
#include <math.h>

using namespace std;

//---------------------------------------------------------------
// Constructor

bool got_heading = false;

BHV_ZigLeg::BHV_ZigLeg(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "default_pulse");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for

  m_current_x = 0;
  m_current_y = 0;
  m_current_wpt = 0;

  addInfoVars("NAV_X, NAV_Y");
  addInfoVars("WPT_INDEX", "no_warning");
  addInfoVars("NAV_HEADING", "no_warning");
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_ZigLeg::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  if((param == "zig_angle") && isNumber(val)) {
    m_zig_angle = double_val;
    // Set local member variables here
    return(true);
  }
  else if ((param == "zig_duration") && isNumber(val)){
    m_zig_duration = double_val;
    return(true);
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

void BHV_ZigLeg::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_ZigLeg::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_ZigLeg::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_ZigLeg::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_ZigLeg::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_ZigLeg::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_ZigLeg::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_ZigLeg::onRunState()
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
  bool ok3;
  m_next_wpt = getBufferDoubleVal("WPT_INDEX", ok3);
 
 m_current_time = getBufferCurrTime();

if(m_next_wpt != m_current_wpt){
 m_wpt_time = getBufferCurrTime();
 m_moos_time = m_wpt_time + 5;
 m_current_wpt = m_next_wpt;
}


if((m_current_time >= m_moos_time) && !got_heading){
 bool ok4;
 m_current_heading = getBufferDoubleVal("NAV_HEADING", ok4);
 got_heading = true;
}
 
 m_end_time = m_moos_time + m_zig_duration;
 if((m_current_time >= m_moos_time) && (m_current_time <= m_end_time)){
  //IvPFunction *ipf=0;
  ipf = buildFunctionWithZAIC(); 
 }

 if(m_current_time >= m_end_time){
  got_heading = false;

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

IvPFunction *BHV_ZigLeg::buildFunctionWithZAIC() 
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
  
  double zig_direction = (m_current_heading + m_zig_angle);
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
