/************************************************************/
/*    NAME: Jason Barker                                     */
/*    ORGN: MIT                                             */
/*    FILE: BHV_Tripwire.cpp                                    */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <math.h>
#include <cstdlib>
#include "MBUtils.h"
#include "BuildUtils.h"
#include "AngleUtils.h"
#include "BHV_Tripwire.h"
#include "ZAIC_PEAK.h"
#include "OF_Coupler.h"
#include "OF_Reflector.h"
#include "XYFormatUtilsPoly.h"


using namespace std;

//---------------------------------------------------------------
// Constructor

BHV_Tripwire::BHV_Tripwire(IvPDomain domain) :
  IvPBehavior(domain)
{
  // Provide a default behavior name
  IvPBehavior::setParam("name", "defaultname");

  // Declare the behavior decision space
  m_domain = subDomain(m_domain, "course,speed");

  // Add any variables this behavior needs to subscribe for
  m_descriptor     = "tripwire_region";
  m_hint_vertex_size  = 3;
  m_hint_vertex_color = "orange"; 
  m_hint_edge_size    = 1;
  m_hint_edge_color   = "orange"; 

  m_current_x = 0;
  m_current_y = 0;
  m_current_wpt = 5;
  m_zig_angle = 180;
  

  got_heading = false;
  tripwire_active = false;
  
  cycle_done = false;
  getting_cold = false;
  getting_hot = false;
  


  addInfoVars("NAV_X, NAV_Y");
  addInfoVars("WPT_INDEX", "no_warning");
  addInfoVars("NAV_HEADING", "no_warning");
  addInfoVars("OPREG_TRAJECTORY_PERIM_DIST", "no_warning");
  addInfoVars("NORTH_TEMP", "no_warning");
  addInfoVars("CURRENT_TEMP", "no_warning");
  addInfoVars("SOUTH_TEMP", "no_warning");
  addInfoVars("N_S_DELTA", "no_warning");
  addInfoVars("GENPATH", "no_warning");     
 
}

//---------------------------------------------------------------
// Procedure: setParam()

bool BHV_Tripwire::setParam(string param, string val)
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
    return(true);
  }
  else if(param == "polygon") {
    XYPolygon new_poly = string2Poly(val);
    if(!new_poly.is_convex())  // Should be convex - false otherwise
      return(false);
    m_polygon = new_poly;
    m_polygons.push_back(new_poly);
    return(true);
  }
  else if(param == "visual_hints")  {
    vector<string> svector = parseStringQ(val, ',');
    unsigned int i, vsize = svector.size();
    for(i=0; i<vsize; i++) 
      handleVisualHint(svector[i]);
    return(true);
  }      

  // If not handled above, then just return false;
  return(false);
}

//---------------------------------------------------------------
// Procedure: onSetParamComplete()
//   Purpose: Invoked once after all parameters have been handled.
//            Good place to ensure all required params have are set.
//            Or any inter-param relationships like a<b.

void BHV_Tripwire::onSetParamComplete()
{
}

//---------------------------------------------------------------
// Procedure: onHelmStart()
//   Purpose: Invoked once upon helm start, even if this behavior
//            is a template and not spawned at startup

void BHV_Tripwire::onHelmStart()
{
}

//---------------------------------------------------------------
// Procedure: onIdleState()
//   Purpose: Invoked on each helm iteration if conditions not met.

void BHV_Tripwire::onIdleState()
{
    postErasablePolygon(); 
}

//---------------------------------------------------------------
// Procedure: onCompleteState()

void BHV_Tripwire::onCompleteState()
{
}

//---------------------------------------------------------------
// Procedure: postConfigStatus()
//   Purpose: Invoked each time a param is dynamically changed

void BHV_Tripwire::postConfigStatus()
{
}

//---------------------------------------------------------------
// Procedure: onIdleToRunState()
//   Purpose: Invoked once upon each transition from idle to run state

void BHV_Tripwire::onIdleToRunState()
{
}

//---------------------------------------------------------------
// Procedure: onRunToIdleState()
//   Purpose: Invoked once upon each transition from run to idle state

void BHV_Tripwire::onRunToIdleState()
{
}

//---------------------------------------------------------------
// Procedure: onRunState()
//   Purpose: Invoked each iteration when run conditions have been met.

IvPFunction* BHV_Tripwire::onRunState()
{

  postViewablePolygon();

  // Part 1: Build the IvP function
  IvPFunction *ipf = 0;
  bool ok1, ok2;
  m_current_x = getBufferDoubleVal("NAV_X", ok1);
  m_current_y = getBufferDoubleVal("NAV_Y", ok2);
  if(!ok1 || !ok2){
    postWMessage("No ownship X/Y info in info_buffer.");
    return(0);
  }
  
  m_current_time = getBufferCurrTime();

  bool ok3;
  m_next_wpt = getBufferDoubleVal("WPT_INDEX", ok3);

  bool ok4;
  m_current_heading = getBufferDoubleVal("NAV_HEADING", ok4);
  
  bool ok5;
  m_distance = getBufferDoubleVal("OPREG_TRAJECTORY_PERIM_DIST", ok5);

  bool oknortht;
  m_low_temp = getBufferDoubleVal("NORTH_TEMP", oknortht);

  bool oksoutht;
  m_high_temp = getBufferDoubleVal("SOUTH_TEMP", oksoutht);
  m_avg_temp = (m_high_temp + m_low_temp)/2;

  bool okdelta;
  m_delta_temp = getBufferDoubleVal("N_S_DELTA", okdelta);
  m_low_temp_turn = (0.25 * m_delta_temp) + m_low_temp;
  if(m_current_temp <= m_low_temp_turn)
    getting_cold = true;

  m_high_temp_turn = (0.75 * m_delta_temp) + m_low_temp;
  if(m_current_temp >= m_high_temp_turn)
    getting_hot = true;

  bool okzig;
  m_cycle_done = getBufferStringVal("GENPATH", okzig);
  if(m_cycle_done == "true")
    cycle_done = true;

  bool okcurrtemp;
  m_current_temp = getBufferDoubleVal("CURRENT_TEMP", okcurrtemp);


  // //Zigline Process
  // if((cycle_done) && (tripwire_active)){
    
  // //Zigline North if Temp is Hot
  //   if((getting_hot) && (m_current_temp >= m_low_temp_turn)){
  //     if(m_current_heading < 180){
  //       zig_direction = 045;
  //     }
  //     else{
  //       zig_direction = 315;
  //     }
  //     ipf = buildFunctionWithZAIC(); 
  //   }

  //   // //Zigline North if Temp is Hot (local maximum)
  //   // if((m_current_temp == m_high_temp) && (m_current_heading >= m_low_temp_turn)){
  //   //   zig_direction = 0;     
  //   //   ipf = buildFunctionWithZAIC(); 
  //   // }


  //   //Zigline South if Temp is Cold
  //   if((getting_cold) && (m_current_temp <= m_high_temp_turn)){
  //     if(m_current_heading < 180){
  //       zig_direction = 135;
  //     }
  //     else{
  //       zig_direction = 225;
  //     }
  //     ipf = buildFunctionWithZAIC(); 
  //   }

  //   // //Zigline South if Temp is Cold (local minimum)
  //   // if((m_current_temp == m_low_temp) && (m_current_heading <=m_high_temp_turn)){
  //   //   zig_direction = 180;     
  //   //   ipf = buildFunctionWithZAIC(); 
  //   // }  

  // }


  ///Starts Tripwire after deployment.  Without this, the boat just spins on the origin
  if(m_current_y <= -30){
    tripwire_active = true;
  }

  
  // // ///Tripwire Actions
  if (tripwire_active){

  if ((m_distance <= 40) && (!got_heading)){
  m_tripwire_heading = m_current_heading;
  got_heading = true;
  }

  if ((m_current_x <= -30) && (!got_heading)){
  m_tripwire_heading = m_current_heading;
  got_heading = true;
  }

  if ((got_heading) && (m_current_x >= 150)){
  zig_direction = (m_tripwire_heading + m_zig_angle); 
  ipf = buildFunctionWithZAIC(); 
  }

   if ((got_heading) && (m_current_x <= -50)){
  zig_direction = (m_tripwire_heading + m_zig_angle); 
  ipf = buildFunctionWithZAIC(); 
  }

  if ((got_heading) && (m_current_y >= -50)){
  zig_direction = (m_tripwire_heading + m_zig_angle); 
  ipf = buildFunctionWithZAIC(); 
  }


  if ((got_heading) && (m_distance <= 60)){
  zig_direction = (m_tripwire_heading + m_zig_angle); 
  ipf = buildFunctionWithZAIC(); 
  }

  if((got_heading) && (m_distance >= 60)){
    got_heading = false;
  }
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

IvPFunction *BHV_Tripwire::buildFunctionWithZAIC() 
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

//-----------------------------------------------------------
// Procedure: postViewablePolygon()
//      Note: Even if the polygon is posted on each iteration, the
//            helm will filter out unnecessary duplicate posts.

void BHV_Tripwire::postViewablePolygon()
{
  if(m_polygon.size() == 0)
    return;

  if(m_us_name == "")
    return;

  if(m_polygon.get_label() == "") {
    string label = m_descriptor;
    m_polygon.set_label(label);
  }

  XYPolygon poly_duplicate = m_polygon;
  if(m_hint_vertex_color != "")
    poly_duplicate.set_color("vertex", m_hint_vertex_color);
  if(m_hint_edge_color != "")
    poly_duplicate.set_color("edge", m_hint_edge_color);
  if(m_hint_edge_size >= 0)
    poly_duplicate.set_edge_size(m_hint_edge_size);
  if(m_hint_vertex_size >= 0)
    poly_duplicate.set_vertex_size(m_hint_vertex_size);

  string poly_spec = poly_duplicate.get_spec();
  postMessage("VIEW_POLYGON", poly_spec);
}
//-----------------------------------------------------------
// Procedure: postErasablePolygon()
//      Note: Even if the polygon is posted on each iteration, the
//            helm will filter out unnecessary duplicate posts.

void BHV_Tripwire::postErasablePolygon()
{
  if(m_polygon.size() == 0)
    return;
  XYPolygon poly_duplicate = m_polygon;
  poly_duplicate.set_active(false);
  string poly_spec = poly_duplicate.get_spec();
  postMessage("VIEW_POLYGON", poly_spec);
}
//-----------------------------------------------------------
// Procedure: handleVisualHint()

void BHV_Tripwire::handleVisualHint(string hint)
{
  string param = tolower(stripBlankEnds(biteString(hint, '=')));
  string value = stripBlankEnds(hint);
  double dval  = atof(value.c_str());

  if((param == "vertex_color") && isColor(value))
    m_hint_vertex_color = value;
  else if((param == "edge_color") && isColor(value))
    m_hint_edge_color = value;
  else if((param == "edge_size") && isNumber(value) && (dval >= 0))
    m_hint_edge_size = dval;
  else if((param == "vertex_size") && isNumber(value) && (dval >= 0))
    m_hint_vertex_size = dval;
}



