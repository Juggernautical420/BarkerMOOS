/************************************************************/
/*    NAME: Jason Barker                                    */
/*    ORGN: MIT                                             */
/*    FILE: BHV_Pulse.cpp                                   */
/*    DATE: 18 Apr 2019                                     */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "BHV_Pulse.h"

using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_Pulse::BHV_Pulse(IvPDomain domain) :
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
  addInfoVars("CYCLE_COMPLETE", "no_warning");
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_Pulse::setParam(string param, string val)
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  // Get the numerical value of the param argument for convenience once
  double double_val = atof(val.c_str());
  if((param == "pulse_range") && isNumber(val)) {
    m_range = double_val;
    // Set local member variables here
    return(true);
  }
  else if ((param == "pulse_duration") && isNumber(val)){
    m_pulse_duration = double_val;
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

void BHV_Pulse::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_Pulse::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_Pulse::onIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_Pulse::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_Pulse::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_Pulse::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_Pulse::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_Pulse::onRunState()
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

 

if(m_next_wpt != m_current_wpt){
 m_wpt_time = getBufferCurrTime();
 m_moos_time = m_wpt_time + 5;

 XYRangePulse pulse;
//The following lines of code change the pulse color for each of the waypoints in the alpha mission
if(m_current_wpt == 0){
  pulse.set_color("edge", "red");
  pulse.set_color("fill", "red");
  }  

if(m_current_wpt == 1){
  pulse.set_color("edge", "orange");
  pulse.set_color("fill", "orange");
  }

if(m_current_wpt == 2){
  pulse.set_color("edge", "yellow");
  pulse.set_color("fill", "yellow");
  }

if(m_current_wpt == 3){
  pulse.set_color("edge", "green");
  pulse.set_color("fill", "green");
  }

if(m_current_wpt == 4){
  pulse.set_color("edge", "blue");
  pulse.set_color("fill", "blue");
  }


  pulse.set_x(m_current_x);
  pulse.set_y(m_current_y);
  pulse.set_label("bhv_pulse");
  pulse.set_rad(m_range);
  pulse.set_duration(m_pulse_duration);
  pulse.set_time(m_moos_time);
 

  string spec = pulse.get_spec();
  postMessage("VIEW_RANGE_PULSE", spec); 
  m_current_wpt = m_next_wpt;
}

  // Part N: Prior to returning the IvP function, apply the priority wt
  // Actual weight applied may be some value different than the configured
  // m_priority_wt, depending on the behavior author's insite.
  if(ipf)
    ipf->setPWT(m_priority_wt);

  return(ipf);
}

