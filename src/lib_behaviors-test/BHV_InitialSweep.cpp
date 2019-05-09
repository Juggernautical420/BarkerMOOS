/*****************************************************************/
/*    NAME: M.Benjamin, H.Schmidt, J. Leonard                    */
/*    ORGN: Dept of Mechanical Eng / CSAIL, MIT Cambridge MA     */
/*    FILE: BHV_SimpleWaypoint.cpp                               */
/*    DATE: July 1st 2008  (For purposes of simple illustration) */
/*    MODIFIED: Jason Barker                                     */
/*    FILE: BHV_InitialSweep.cpp                                   */
/*    DATE: May 5, 2019                                          */
/*                                                               */
/* This program is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU General Public License   */
/* as published by the Free Software Foundation; either version  */
/* 2 of the License, or (at your option) any later version.      */
/*                                                               */
/* This program is distributed in the hope that it will be       */
/* useful, but WITHOUT ANY WARRANTY; without even the implied    */
/* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR       */
/* PURPOSE. See the GNU General Public License for more details. */
/*                                                               */
/* You should have received a copy of the GNU General Public     */
/* License along with this program; if not, write to the Free    */
/* Software Foundation, Inc., 59 Temple Place - Suite 330,       */
/* Boston, MA 02111-1307, USA.                                   */
/*****************************************************************/

#include <cstdlib>
#include <math.h>
#include <iostream>
#include "BHV_InitialSweep.h"
#include "MBUtils.h"
#include "AngleUtils.h"
#include "BuildUtils.h"
#include "ZAIC_PEAK.h"
#include "OF_Coupler.h"
#include "OF_Reflector.h"
#include "AOF_SimpleWaypoint.h"
#include "XYFormatUtilsPoly.h"

using namespace std;

//-----------------------------------------------------------
// Procedure: Constructor

BHV_InitialSweep::BHV_InitialSweep(IvPDomain gdomain) : 
  IvPBehavior(gdomain)
{
  IvPBehavior::setParam("name", "simple_waypoint");
  m_domain = subDomain(m_domain, "course,speed");

  // All distances are in meters, all speed in meters per second
  // Default values for configuration parameters 
  m_desired_speed  = 0; 
  m_arrival_radius = 10;
  m_ipf_type       = "zaic";
  m_descriptor     = "tripwire_region";
  m_hint_vertex_size  = 3;
  m_hint_vertex_color = "orange"; 
  m_hint_edge_size    = 1;
  m_hint_edge_color   = "yellow";  

  // Default values for behavior state variables
  m_osx  = 0;
  m_osy  = 0;
  m_current_wpt = 0;
  m_zig_angle = 180;
  m_zig_duration = 5;

  bool got_heading = false;

  addInfoVars("NAV_X, NAV_Y");
  addInfoVars("WPT_INDEX", "no_warning");
  addInfoVars("NAV_HEADING", "no_warning");
  addInfoVars("OPREG_TRAJECTORY_PERIM_DIST", "no_warning");
  addInfoVars("UCTD_MSMNT_REPORT", "no_warning");

}

//---------------------------------------------------------------
// Procedure: setParam - handle behavior configuration parameters

bool BHV_InitialSweep::setParam(string param, string val) 
{
  // Convert the parameter to lower case for more general matching
  param = tolower(param);

  double double_val = atof(val.c_str());
  if((param == "ptx")  && (isNumber(val))) {
    m_nextpt.set_vx(double_val);
    return(true);
  }
  else if((param == "pty") && (isNumber(val))) {
    m_nextpt.set_vy(double_val);
    return(true);
  }
  else if((param == "speed") && (double_val > 0) && (isNumber(val))) {
    m_desired_speed = double_val;
    return(true);
  }
  else if((param == "radius") && (double_val > 0) && (isNumber(val))) {
    m_arrival_radius = double_val;
    return(true);
  }
  else if(param == "ipf_type") {
    val = tolower(val);    
    if((val == "zaic") || (val == "reflector")) {
      m_ipf_type = val;
      return(true);
    }
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
  return(false);
}

//-----------------------------------------------------------
// Procedure: onIdleState

void BHV_InitialSweep::onIdleState() 
{
  postViewPoint(false);
  postErasablePolygon();  
}

//-----------------------------------------------------------
// Procedure: postViewPoint

void BHV_InitialSweep::postViewPoint(bool viewable) 
{
  m_nextpt.set_label(m_us_name + "'s next waypoint");
  
  string point_spec;
  if(viewable)
    point_spec = m_nextpt.get_spec("active=true");
  else
    point_spec = m_nextpt.get_spec("active=false");
  postMessage("VIEW_POINT", point_spec);
}

//-----------------------------------------------------------
// Procedure: onRunState

IvPFunction *BHV_InitialSweep::onRunState() 
{
  // Part 1: Get vehicle position from InfoBuffer and post a 
  // warning if problem is encountered
  bool ok1, ok2;
  m_osx = getBufferDoubleVal("NAV_X", ok1);
  m_osy = getBufferDoubleVal("NAV_Y", ok2);
  if(!ok1 || !ok2) {
    postWMessage("No ownship X/Y info in info_buffer.");
    return(0);
  }
  postViewablePolygon(); 
  // Part 2: Determine if the vehicle has reached the destination 
  // point and if so, declare completion.
#ifdef WIN32
  double dist = _hypot((m_nextpt.x()-m_osx), (m_nextpt.y()-m_osy));
#else
  double dist = hypot((m_nextpt.x()-m_osx), (m_nextpt.y()-m_osy));
#endif
  if(dist <= m_arrival_radius) {
    setComplete();
    postViewPoint(false);
    return(0);
  }
  bool ok3;
  m_current_wpt = getBufferDoubleVal("WPT_INDEX", ok3);
  m_current_time = getBufferCurrTime(); 
  bool ok5;
  m_distance = getBufferDoubleVal("OPREG_TRAJECTORY_PERIM_DIST", ok5);

  if(m_distance < 10){
  m_wpt_time = getBufferCurrTime();  
  bool ok4;
  m_current_heading = getBufferDoubleVal("NAV_HEADING", ok4);
 

  m_end_time = m_wpt_time + m_zig_duration;

  if(m_current_time <= m_end_time){
  m_next_wpt = m_current_wpt + 1;
  IvPFunction *ipf = 0;
  ipf = buildTripwireWithZAIC();  
  }
}




  // Part 3: Post the waypoint as a string for consumption by 
  // a viewer application.
  postViewPoint(true);

  // Part 4: Build the IvP function with either the ZAIC tool 
  // or the Reflector tool.
  IvPFunction *ipf = 0;
  if(m_ipf_type == "zaic")
    ipf = buildFunctionWithZAIC();
  else
    ipf = buildFunctionWithReflector();
  if(ipf == 0) 
    postWMessage("Problem Creating the IvP Function");

  if(ipf)
    ipf->setPWT(m_priority_wt);
  
  return(ipf);
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithZAIC

IvPFunction *BHV_InitialSweep::buildFunctionWithZAIC() 
{
  ZAIC_PEAK spd_zaic(m_domain, "speed");
  spd_zaic.setSummit(m_desired_speed);
  spd_zaic.setPeakWidth(0.5);
  spd_zaic.setBaseWidth(1.0);
  spd_zaic.setSummitDelta(0.8);  
  if(spd_zaic.stateOK() == false) {
    string warnings = "Speed ZAIC problems " + spd_zaic.getWarnings();
    postWMessage(warnings);
    return(0);
  }
  
  double rel_ang_to_wpt = relAng(m_osx, m_osy, m_nextpt.x(), m_nextpt.y());
  ZAIC_PEAK crs_zaic(m_domain, "course");
  crs_zaic.setSummit(rel_ang_to_wpt);
  crs_zaic.setPeakWidth(0);
  crs_zaic.setBaseWidth(180.0);
  crs_zaic.setSummitDelta(0);  
  crs_zaic.setValueWrap(true);
  if(crs_zaic.stateOK() == false) {
    string warnings = "Course ZAIC problems " + crs_zaic.getWarnings();
    postWMessage(warnings);
    return(0);
  }

  IvPFunction *spd_ipf = spd_zaic.extractIvPFunction();
  IvPFunction *crs_ipf = crs_zaic.extractIvPFunction();

  OF_Coupler coupler;
  IvPFunction *ivp_function = coupler.couple(crs_ipf, spd_ipf, 50, 50);

  return(ivp_function);
}

//-----------------------------------------------------------
// Procedure: buildFunctionWithReflector

IvPFunction *BHV_InitialSweep::buildFunctionWithReflector() 
{
  IvPFunction *ivp_function;

  bool ok = true;
  AOF_SimpleWaypoint aof_wpt(m_domain);
  ok = ok && aof_wpt.setParam("desired_speed", m_desired_speed);
  ok = ok && aof_wpt.setParam("osx", m_osx);
  ok = ok && aof_wpt.setParam("osy", m_osy);
  ok = ok && aof_wpt.setParam("ptx", m_nextpt.x());
  ok = ok && aof_wpt.setParam("pty", m_nextpt.y());
  ok = ok && aof_wpt.initialize();
  if(ok) {
    OF_Reflector reflector(&aof_wpt);
    reflector.create(600, 500);
    ivp_function = reflector.extractIvPFunction();
  }

  return(ivp_function);
}
//-----------------------------------------------------------
// Procedure: buildTripwireWithZAIC

IvPFunction *BHV_InitialSweep::buildTripwireWithZAIC() 
{
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
  IvPFunction *crs_ipf = crs_zaic.extractIvPFunction();
  return(crs_ipf);
}
//-----------------------------------------------------------
// Procedure: postViewablePolygon()
//      Note: Even if the polygon is posted on each iteration, the
//            helm will filter out unnecessary duplicate posts.

void BHV_InitialSweep::postViewablePolygon()
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

void BHV_InitialSweep::postErasablePolygon()
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

void BHV_InitialSweep::handleVisualHint(string hint)
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
